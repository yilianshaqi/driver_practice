/*************************************************************************
    > File Name: msq.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年12月06日 星期二 19时02分32秒
 ************************************************************************/
 #include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<string.h>
#include<stdio.h>
#include <unistd.h>
int main()
{
#define SIZE 64
	typedef struct {
	long type;
	char buf[SIZE];
	}Data;

//创建消息队列
	key_t key=1021;
	int msgid =msgget(key,0666);
	if(msgid==-1)
	{
		perror("msgget error\n");
		return -1;
	}

//发送消息
	Data message;
	bzero(&message,sizeof(message));
	message.type=119;
	strcpy(message.buf,"return");
	if(msgsnd(msgid,&message,SIZE,0)==-1)
	{
		perror("msgsnd error\n");
		return -1;
	}
	printf("first ok\n");
/*	bzero(&message,sizeof(message));
	message.type=110;
	strcpy(message.buf,"hello");
	if(msgsnd(msgid,&message,sizeof(message),0)==-1)
	{
		perror("msgsnd error\n");
		return -1;
	}
	printf("second ok\n");  */
	
//接收消息
//	bzero(&message,sizeof(message));
	Data rcvmsg;
	bzero(&rcvmsg,sizeof(rcvmsg));
	if(msgrcv(msgid,&rcvmsg,sizeof(rcvmsg),110,0)==-1)
	{
		perror("msgrcv error\n");
		return -1;
	}
	printf("recv :%s\n",rcvmsg.buf);
//删除消息队列
/*
	sleep(15);
	if(msgctl(msgid,IPC_RMID,NULL))
	{
		perror("msgctl error\n");
	}*/
#undef SIZE
	return 0;
}
