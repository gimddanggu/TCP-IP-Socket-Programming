#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(){
	int fds[2];
	char str[] = "Who are you?";
	char str2[] = "Thank you for your message.";
	char buf[50]; // 데이터를 주고 받기 위한 메모리공간
	pid_t pid;

	pipe(fds);
	pid = fork();

	if(pid == 0){
		write(fds[1], str, sizeof(str));
		// sleep(2) 주석 처리 시 부모는 파이프에 접근 못함
		sleep(2);
		read(fds[0], buf, sizeof(buf));
		printf("child proc output: %s\n", buf);
	}
	else {
		read(fds[0], buf, sizeof(buf));
		printf("parent proc output: %s\n", buf);
		write(fds[1], str2, sizeof(str2));
		sleep(3);
	}

	return 0;
}
