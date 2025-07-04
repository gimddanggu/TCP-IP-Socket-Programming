#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(){
	fd_set reads, temp;
	int result, str_len;	
	char buf[BUF_SIZE];
	struct timeval timeout;

	FD_ZERO(&reads);
	FD_SET(0, &reads);	// 파일디스크립터0번을 fdset에 등록

	// fd에 등록하고 변화가 있는 파일디스크립터만 남고(reads)
	// 변화가없는 fd는0으로 초기화됨
	// select라는 함수를 호출하게되면 
	while(1){
		temp = reads; // 원본에 해당하는 FD를 temps에 저장
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		result = select(1, &temp, 0, 0, &timeout);
		if(result == -1)
		{
			puts("select() error");
			break;
		}
		else if(result == 0)
		{	
			puts("time-out");
		}
		else{
			if(FD_ISSET(0, &temp))
			{
				str_len = read(0, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from console: %s", buf);
			}
		}
	}

	return 0;
}
