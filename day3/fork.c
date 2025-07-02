#include <stdio.h>
#include <unistd.h>

int globalval = 10;			// 전역
int main()
{
	pid_t pid;
	int localval = 20;		// 지역

	globalval++;
	localval += 5;

	pid = fork();
	if(pid == 0)
	{
		globalval += 2;
		localval += 2;
	} else {
		globalval -= 2;
		localval -=2;
	}

	if(pid == 0) printf("child proc:[%d, %d]\n", globalval, localval);
	else printf("Parent proc: [%d, %d]\n", globalval, localval);
	return 0;
}
