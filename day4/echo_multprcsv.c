#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100

void read_childproc(int sig);  // 자식 프로세스 종료 시 처리할 함수 선언

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int fds[2];  // 파이프 파일 디스크립터
    pid_t pid;
    struct sigaction act;
    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // SIGCHLD 신호에 대한 핸들러 설정
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);  // 자식 프로세스 종료 시 SIGCHLD 처리

    // 서버 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);  // IP 주소 설정
    serv_adr.sin_port = htons(atoi(argv[1]));  // 포트 설정

    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        perror("bind() error");
        close(serv_sock);
        exit(1);
    }

    if (listen(serv_sock, 5) == -1) {
        perror("listen() error");
        close(serv_sock);
        exit(1);
    }

    // 파이프 생성
    pipe(fds);
    pid = fork();  // 자식 프로세스 생성
    if (pid == 0) {
        FILE *fp = fopen("echomsg.txt", "wt");  // 자식 프로세스는 데이터를 파일에 저장
        char msgbuf[BUF_SIZE];
        int i, len;

        for (i = 0; i < 10; i++) {
            len = read(fds[0], msgbuf, BUF_SIZE);  // 파이프에서 데이터 읽기
            fwrite((void*)msgbuf, 1, len, fp);  // 읽은 데이터를 파일에 저장
        }
        fclose(fp);
        return 0;
    }

    while (1) {
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);  // 클라이언트 연결 수락
        if (clnt_sock == -1)
            continue;
        else
            puts("new client connected...");

        pid = fork();  // 자식 프로세스 생성
        if (pid == 0) {
            close(serv_sock);  // 자식 프로세스는 서버 소켓을 닫고 클라이언트와 통신
            while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0) {
                write(clnt_sock, buf, str_len);  // 클라이언트로 데이터 보내기
                write(fds[1], buf, str_len);  // 파이프에 데이터 쓰기
            }
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        } else {
            close(clnt_sock);  // 부모 프로세스는 클라이언트 소켓을 닫음
        }
    }

    close(serv_sock);
    return 0;
}

void read_childproc(int sig) {
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);  // 종료된 자식 프로세스의 상태 확인
    printf("removed proc id: %d\n", pid);  // 종료된 자식 프로세스의 PID 출력
}
