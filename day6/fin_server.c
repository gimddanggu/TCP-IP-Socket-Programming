#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>

#define SERVERPORT 9000
#define BUFSIZE 1024

void err_display(const char *msg) {
    perror(msg);
}

void err_quit(const char *msg) {
    perror(msg);
    exit(1);
}

// 전송할 문자열
char webpage[] = "HTTP/1.1 200 OK\r\n"
"Server:Linux Web Server\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>\r\n"
"<html><head><title> My Web Page </title>\r\n"
"<style>body {background-color: #FFFF00 }</style></head>\r\n"
"<body><center><h1>Hello wolrd!!</h1><br>\r\n"
"<img src=\"/terra.jpg\" width=\"400\"></center></body></html>\r\n";


void *ProcessClient(void *arg)
{
    int retval;
    int client_sock = (int)(long long)arg;
    struct sockaddr_in clientaddr;
    char addr[INET_ADDRSTRLEN];
    socklen_t addrlen = sizeof(clientaddr);

    getpeername(client_sock, (struct sockaddr *)&clientaddr, &addrlen);
    inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

    char buf[BUFSIZE + 1];
    retval = recv(client_sock, buf, BUFSIZE, 0);
    printf(">> 클라이언트 요청 전문:\n%s\n", buf);
    if (retval <= 0)
    {
        err_display("recv()");
        close(client_sock);
        return NULL;
    }


    buf[retval] = '\0';
    printf("[TCP/%s:%d] 요청 수신: %s\n", addr, ntohs(clientaddr.sin_port), buf);

    // 이미지 요청 처리
    if (strstr(buf, "GET /terra.jpg") != NULL)
    {
        FILE *fp = fopen("terra.jpg", "rb");
        if (!fp)
        {
            err_display("fopen(terra.jpg)");
            close(client_sock);
            return NULL;
        }

        // HTTP 응답 헤더 (Content-Type: image/jpeg)
        char header[] = "HTTP/1.1 200 OK\r\n"
                        "Content-Type: image/jpeg\r\n"
                        "Connection: close\r\n\r\n";
        send(client_sock, header, strlen(header), 0);

        // 이미지 파일 전송
        char imgbuf[BUFSIZE];
        int n;
        while ((n = fread(imgbuf, 1, BUFSIZE, fp)) > 0)
        {
            send(client_sock, imgbuf, n, 0);
        }

        fclose(fp);
    }
    else
    {
        // 기본 웹페이지 응답
        retval = send(client_sock, webpage, strlen(webpage), 0);
        if (retval == -1)
        {
            err_display("send(webpage)");
        }
    }
    
    close(client_sock);
    printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
           addr, ntohs(clientaddr.sin_port));
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <PORT>\n", argv[0]);
        exit(1);
    }

    int retval;
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == -1)
        err_quit("socket()");

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(argv[1])); // argv[1] = 포트

    retval = bind(listen_sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1)
        err_quit("bind()");

    retval = listen(listen_sock, SOMAXCONN);
    if (retval == -1)
        err_quit("listen()");

    int client_sock;
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    pthread_t tid;

    while (1)
    {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (struct sockaddr *)&clientaddr, &addrlen);
        if (client_sock == -1)
        {
            err_display("accept()");
            continue;
        }

        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
               addr, ntohs(clientaddr.sin_port));

        retval = pthread_create(&tid, NULL, ProcessClient, (void *)(long long)client_sock);
        if (retval != 0)
        {
            close(client_sock);
            continue;
        }

        pthread_detach(tid); // 자원 회수 자동화
    }

    close(listen_sock);
    return 0;
}
