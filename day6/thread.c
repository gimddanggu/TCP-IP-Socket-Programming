#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void * thread_main(void*);

int main()
{
	pthread_t t_id;			// thread id를 저장할 변수
	int thread_arg = 5;
	// 스레드 id, 기본스레드, 스레드함수, 스레드함수의 입력
	
	if (pthread_create(&t_id, NULL, thread_main, &thread_arg) != 0)
	{
		puts("pthread_create() error");
		return -1;
	}
	sleep(10);		// 다른 스레드가 종료될 때까지 기다림
	puts("end of main");

	return 0;
}
// void* arg 인 이유? 
// 실행될 때 어떤 타입의 매개변수인지 모름
// 런타임에서 결정되기 때문에 void 타입의 인자를 줌
void* thread_main(void* arg)
{
	int i;
	int cnt = *((int* )arg);
	for (i = 0; i < cnt; i++)
	{
		sleep(1);
		puts("running thread");
	}
	return NULL;
}
