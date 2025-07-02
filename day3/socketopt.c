#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

int main()
{
	int sock;
	int sndbuf, rcvbuf;
	socklen_t optlen;

	sock = socket(PF_INET, SOCK_STREAM, 0);

	optlen = sizeof(sndbuf);
	if(getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&sndbuf, &optlen) <  0)
		perror("getsockpt failed");

	printf("sendbuf size: %d\n", sndbuf);

	optlen = sizeof(rcvbuf);
	if(getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcvbuf, &optlen) < 0)
		perror("getsockopt failed");
	printf("revbuf size: %d\n", rcvbuf);

	/* setsockopt */
	rcvbuf = 8192;
	if(setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf)) < 0)
		perror("setsockopt failed");
		
	printf("revbuf size: %d\n", rcvbuf);
	

	return 0;
}
