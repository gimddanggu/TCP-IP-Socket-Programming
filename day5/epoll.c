// epoll 인스턴스생성 -> 감시할소켓(등록/수정/삭제)-> 이벤트발생대기)
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>

int main() 
{
	int epfd = epoll_create1(0);				// 인스턴스 생성(저장공간)
	struct epol_event event, evetns[10];	// epoll event 구조체 선언
	event.evetns = EPOLLIN;						// 감시할 이벤트(수신)지정
	event.data.fd = STDIN_FILENO;				// 감시할 FD 저장
	// STDIN_FILENO fed를 epfd에 등록하여 EPOLLIN이벤트를 감시한다.
	epoll_ctl(epfd,EPOLL_CTL_ADD, STDIN_FILENO, &event);

	printf("키보드 입력 대기 중...\n");
	//epfd안의 fd에 이벤트가 발생하면 그 이벤트 정보를 events 배열에 넣는다
	//한번에 10개까지 
	int n = epoll_wait(epfd, events, 10, 5000); // 5초 대기. 이벤트 발생한 수 반환
	if (n == 0)
	{
		printf("Timeout\n");
	}
	else if(n > 0)
	{
		printf("n: %d\n", n);
		char buf[100];
		fgets(buf, sizeof(buf), stdin);
		printf("입력됨: %s", buf);
	}

	close(epfd);
	return 0;
}
