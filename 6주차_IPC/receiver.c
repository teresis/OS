#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/stat.h>

struct msgbuf
{
	long msgtype;
	char mtext[256];
	char myname[16];
	int seq;
};

int main(int argc, char **argv)
{
	key_t key_id;
	struct msgbuf mybuf;
	int msgtype;
	
	if(argc ==2)
	{
		msgtype =3;
	}
	else
	{
		msgtype =4;
	}

	key_id = msgget(1234, IPC_CREAT |0666);

	if(key_id <0)
	{
		perror("msgget error :");
		exit(0);
	}

	while(1)
	{
		if(msgrcv(key_id, (void *)&mybuf, sizeof(struct msgbuf), 0, 0)==-1)
		{
			perror("msgrcv error :");
			exit(0);
		}
		printf("msgtype is  %d \n", mybuf.msgtype);
		printf("mtext is %s  \n", mybuf.mtext);
		printf("myname is %s \n", mybuf.myname);
		printf("seq is %d \n", mybuf.seq);
	}

	exit(0);
}
