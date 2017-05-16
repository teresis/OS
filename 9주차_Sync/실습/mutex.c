#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int ncount;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *do_ops1(void *data)
{
	int i;
	for(i=0;i<10;i++)
	{
		pthread_mutex_lock(&mutex);
		printf("do_ops1 : %d \n", ncount);
		ncount++;
		if(i==10)
		{
			return 0;
		}
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

void *do_ops2(void *data)
{
	int i;
	for(i=0;i<10;i++)
	{
		pthread_mutex_lock(&mutex);
		printf("do_ops2 : %d\n", ncount);
		ncount++;
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}
	
}

int main(void)
{
	int rtmp;
	pthread_t thread[2];
	int status;
	int a = 1;

	ncount = 0;
	rtmp =pthread_create(&thread[0], NULL, do_ops1, (void *) &a);
	sleep(1);
	rtmp =pthread_create(&thread[0], NULL, do_ops2, (void *) &a);

	pthread_join(thread[0], (void *)&status);
	pthread_join(thread[1], (void *)&status);
	status = pthread_mutex_destroy(&mutex);
	printf("status : %d \n", status);
	printf("End\n");
	return 0;
}
