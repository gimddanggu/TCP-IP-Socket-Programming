#include <stdio.h>
#include <unistd.h>

int main(){
	int fds[2];
	char str[] = "Who are you?";
	char buf[30];
	pid_t pid;

	pipe(fds);
	pid = fork();
	// 자식 프로세스 파이프에 Who are you를 넣었고 부모가 출력
	if(pid == 0) write(fds[1], str, sizeof(str));
	else {
		read(fds[0], buf, sizeof(buf));
		puts(buf);
	}

	return 0;
}
