#include <stdio.h>
#include <pthread.h>

void* thread_inc(void* arg);
void* thread_des(void* arg);

long long num = 0;

// 스레드 사용시 주의 사항
// 공통자원 접근시 
int main()
{
	// 스레드 100개 생성
	pthread_t thread_id[100];

	for(int i = 0; i < 100; i++)
	{
		if (i % 2)
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else
			pthread_create(&(thread_id[i]), NULL, thread_des ,NULL);
	}

	for (int i = 0; i < 100; i++)
		pthread_join(thread_id[i], NULL);

	printf("result: %lld\n", num);
	
	return 0;
}

void* thread_inc(void* arg)
{
	int i;
	for (int i = 0; i < 100000; i++)
	{
		num += 1;
	}
	return NULL;
}

void * thread_des(void* arg)
{
	int i = 0;
	for (int i = 0; i < 100000; i++)
	{
		num -= 1;
	}
	return NULL;
}

// 기대결과: 0
// 출력결과: 실행할 때 마다 값이 달라짐
// 스레드가 동시에 같은 변수에 접근할 경우
// thread1이 변수 접근하여 연산하는  도중 thread2가변수 값을 읽게되어
// 이상한 값에 접근하게 됨
// 해결방안: 동기화 기법
