#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char** argv){
    int sock_fd;
    struct sockaddr_in server_addr;
    char buf[1024];
    int recv_len;

    if(argc != 3){
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    
    if(inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0){
        perror("inet_pton failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    if(connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        perror("connect failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // 메시지 수신
    recv_len = read(sock_fd, buf, sizeof(buf) - 1);
    if(recv_len == -1){
        perror("read failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    buf[recv_len] = '\0';
    printf("Received from server: %s", buf);

    close(sock_fd);
    return 0;
}
