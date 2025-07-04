#include <stdio.h>
#include <unistd.h>
#include <poll.h>

int main()
{
	struct pollfd pfd; // 감시할FD와 이벤트를 담을 구조체
	
	pfd.fd = STDIN_FILENO;	// 표준입력 등록
	pfd.events = POLLIN;

	printf("Wait 5sec....\n");

	int ret = poll(&pfd, 1, 5000);

	if (ret == -1) perror("poll error\n");
	else if (ret == 0) printf("timeout\n");
	else 
	{
		if (pfd.revents& POLLIN)
	{
			char buffer[1000];
			fgets(buffer, sizeof(buffer), stdin);
			printf("Entered: %s\n", buffer);
			
		}
	}
	return 0;
}
