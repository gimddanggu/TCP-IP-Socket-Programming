#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

#define BUFSIZE 1024

void err_display(const char*);
void err_quit(const char*);
char request[] = "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n";

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <IP> <PORT>\n", argv[0]);
        exit(1);
    }
    int retval;
    // socket()
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) err_quit("socket()");
    
    // connect()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &serveraddr.sin_addr);
    serveraddr.sin_port = htons(atoi(argv[2]));
    retval = connect(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)  err_quit("connect()");

    char buf[BUFSIZE + 1];
	int len;

    // 데이터 보내기
    retval = send(sock, request, strlen(request), 0);
    if (retval == -1) {
        err_display("send()");
        close(sock);
        exit(1);
    }
    // 서버와 데이터 통신
	while ((retval = recv(sock, buf, BUFSIZE, 0)) > 0) 
    {
        buf[retval] = '\0';
        printf("%s", buf);
    }   

	// 소켓 닫기
	close(sock);
	return 0;
}

void err_display(const char *msg) {
    perror(msg);
}

void err_quit(const char *msg) {
    perror(msg);
    exit(1);
}