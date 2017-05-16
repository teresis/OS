#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

struct msgbuf
{

	long msgtype;
	char mtext[256];
	char myname[16];
	int seq;
};

int main()
{
	key_t key_id;
	int i;
	struct msgbuf mybuf;
	
	key_id = msgget((key_t)1234, IPC_CREAT | 0666);

	if( key_id == -1)
	{
		perror("msgget error : ");
		exit(0);
	}

	printf("key is %d \n" , key_id);

	memset(mybuf.mtext, 0x00, 256);
	memset(mybuf.myname, 0x00, 16);
	mybuf.seq = 0;
	i =0;

	while(1)
	{
		if(i%2 ==0)
		{
			memcpy(mybuf.mtext, "hello", 5);
			memcpy(mybuf.myname , "CDW", 3);
			mybuf.msgtype = 4;
		}
		else
		{
			memcpy(mybuf.mtext, "world" , 5);
			memcpy(mybuf.myname,"sys", 3);
			mybuf.msgtype =4;

		}

		mybuf.seq = i;

		if(msgsnd(key_id, (void *)&mybuf, sizeof(struct msgbuf), IPC_NOWAIT)== -1)
		{
			perror("msgsnd error :");
			exit(0);
		}

		printf("send %d\n", i);
		i++;
		sleep(1);
	}

	exit(0);
}
