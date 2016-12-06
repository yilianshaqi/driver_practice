/*************************************************************************
    > File Name: mem_o.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年12月06日 星期二 16时14分25秒
 ************************************************************************/
#include<stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
typedef struct  
{
	              
	sem_t mes;    
	sem_t sem; 
		int data;  
}Data;         
/*
int *shmaddr =NULL;
void handler(int signum)
{
	printf("recv sigusr1:%d\n",signum);
	printf(" data =%d\t",*shmaddr);
}   */
int main()
{
#define SIZE 64
	key_t key = 1010;
	printf("%d\n",getpid());
//	sleep(2);
	int shmid = shmget (key,SIZE,0666);
	printf("shmid = %d\n",shmid);
	if(shmid ==-1)
	{
		perror("shmget error\n");
		return -1;
	}
	 int *shmaddr = shmat(shmid,NULL,0);
	if(shmaddr == (int *)-1)
	{
		perror("shmat error\n");
		return -1;
	}
//	signal(SIGUSR1,handler);
	Data * semph =(Data*)shmaddr;
	while(1){
		printf("wait message\n");
		sem_wait(&semph->mes);
		printf("data= %d\t",semph->data);
		sem_post(&semph->sem);
	}
	if(shmdt(shmaddr))
	{
		perror("shmdt error\n");
		return -1;
	}

#undef SIZE 
	return 0;
}
