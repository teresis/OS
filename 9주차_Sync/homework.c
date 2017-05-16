#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include<time.h>

#define NUMBER_OF_PHIL 6
#define EXEC_TIME 60

enum PHIL_STATE
{
	HUNGRY= 0,
	EATING= 1,
	THINKING =2
};

typedef struct philosopher{
	unsigned short num_eat;
	enum PHIL_STATE state;
} philosopher;

philosopher phil[NUMBER_OF_PHIL];
sem_t chopstick[NUMBER_OF_PHIL];

int idlewait()
{
	int sleepTimeMS = (rand() % 91 +10);
	usleep(sleepTimeMS *1000);
	return sleepTimeMS;
}

unsigned int tick()
{
	struct timeval tv;
	gettimeofday(&tv, (void *)0);
	return tv.tv_sec * (unsigned int)1000 + tv.tv_usec / 1000;
}

void initPhil(void)
{
	unsigned short i;
	for(i = 0; i<NUMBER_OF_PHIL; i++)
	{
		phil[i].num_eat = 0;
		phil[i].state =THINKING;
		sem_init(&chopstick[i], 0 , 1);
	}
}

void *dining(void* arg)
{
	int t=EXEC_TIME;
	unsigned short first, second;
	unsigned int start_time;
	unsigned short phil_i= (int)(intptr_t)arg;
	philosopher* curphil = &phil[phil_i];
	first = (phil_i+(phil_i%2)) % NUMBER_OF_PHIL;
	second = (phil_i+((phil_i+1)%2)) %NUMBER_OF_PHIL;

	while(t > 0)
	{
		start_time =tick();
		idlewait();
		curphil->state = HUNGRY;
		sem_wait(&chopstick[first]);
		sem_wait(&chopstick[second]);
		printf("[Phil%d] : EATING\n",phil_i);
		curphil->state = EATING;
		curphil->num_eat++;
		idlewait();

		sem_post(&chopstick[second]);
		sem_post(&chopstick[first]);
		printf("[Phil%d] : THINKING\n",phil_i);


		curphil->state = THINKING;

		t= t - ( tick()/1000- start_time/1000);
	}

}

int main(void){
	pthread_t t[NUMBER_OF_PHIL];
	unsigned short i , args[NUMBER_OF_PHIL], minCount =USHRT_MAX, maxCount = 0;
	long long start =0 ;
	long long end =0;
	void* thread_result;
	srand(time(NULL));

	initPhil();
	start = tick();
	for(i=0; i<NUMBER_OF_PHIL;i++){
		args[i] = i;
		pthread_create(&t[i], NULL, dining, (void *)(intptr_t)args[i]);
	}

	for (i = 0; i<NUMBER_OF_PHIL; i++){
		pthread_join(t[i], &thread_result);
	}
	
	end = tick();

	printf("\n\n");

	for(i=0; i<NUMBER_OF_PHIL; i++)
	{
		printf("Philosopher %d eating count : %d \n\n", i, phil[i].num_eat);
	}

	printf("Total Exec Time : %lld.%lld sec\n\n",(end-start)/1000, (end-start) % 1000);
}
