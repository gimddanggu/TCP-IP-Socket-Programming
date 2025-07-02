#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr; //IPv4관련 구조체
	socklen_t client_addr_size;
	char buffer[BUFFER_SIZE];
	ssize_t byte_read;

	if(argc != 2) {
		printf("%s  <port>\n", argv[0]);
		exit(1);
	}

	server_fd = socket(PF_INET, SOCK_STREAM, 0);
	if(server_fd == 1)
	{
		perror("socket failed");
		exit(1);
	}
	/*구조체 초기화*/
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;				// IPv4 주소체계
	server_addr.sin_addr.s_addr = INADDR_ANY;		// IP
	server_addr.sin_port = htons(atoi(argv[1]));	// PORT

	/*서버소켓 바인딩*/
	if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind faild");
		close(server_fd);
		exit(1); 
	}

	/*서버소켓바인딩 - 연결 대기*/
	// 대기 큐 5개
	if(listen(server_fd, 5)  == -1) 
	{
		perror("listen failed");
		close(server_fd);
		exit(1);
	}
	/*클라이언트 소켓 생성- 클라이언트 연결 수락*/
	client_addr_size = sizeof(client_addr);
	if((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_size)) == -1)
	{
		perror("accept failed");
		close(server_fd);
		exit(1);
	}

	while((byte_read = recv(client_fd, buffer, sizeof(buffer)-1, 0)) != 0)
	{
		buffer[byte_read] = '\0';	
		printf("Message from client: %s", buffer);
		send(client_fd, buffer, byte_read, 0);
	}

	close(client_fd);
	close(server_fd);

	return 0;
}	
