#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main(void)
{
	int fd;
	char str[] = "Hello socket!!\n";

	fd = open("hello.txt", O_CREAT | O_WRONLY);
	if (fd < 0)
	{
		perror("could not open hello.txt");
		exit(1);
	}
	
	if (write(fd, str, strlen(str) + 1) < 0)
	{
		perror("Faild to write");
		exit(1);
	} 

	close(fd);

	return 0;

}
