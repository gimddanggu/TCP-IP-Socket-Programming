#include <stdio.h>
#include <pthread.h>

void* thread_inc(void*);
void* thread_des(void*);

long long num = 0;
pthread_mutex_t mutex;

int main()
{
	pthread_t thread_id[100];
	int i;

	pthread_mutex_init(&mutex, NULL);	// 뮤텍스 생성
	for (int i = 0; i < 100; i++)
	{
		if(i % 2) pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
	}
	for (i = 0; i < 100; i++) pthread_join(thread_id[i], NULL);

	printf("result: %lld\n", num);

	pthread_mutex_destroy(&mutex);		// 뮤텍스 소멸(키)
	
	return 0;
}

void* thread_inc(void* arg)
{
	int i = 0;
	pthread_mutex_lock(&mutex);		// 뮤텍스(키) 획득
	for (i = 0; i < 1000000; i++)
	{
		num += 1;
	}
	pthread_mutex_unlock(&mutex); 	// 키 반납
	return NULL;
}

void *thread_des(void* arg)
{
	int i;
	pthread_mutex_lock(&mutex);		// 뮤텍스(키) 획득

	for (i = 0; i < 1000000; i++)
	{
		num -= 1;
	}
	pthread_mutex_unlock(&mutex); 	// 키 반납

	return NULL;
}
