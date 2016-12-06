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

int *shmaddr =NULL;
void handler(int signum)
{
	printf("recv sigusr1:%d\n",signum);
	printf(" data =%d\t",*shmaddr);
}
int main()
{
#define SIZE 64
	key_t key = 1010;
	printf("%d\n",getpid());
//	sleep(2);
	int shmid = shmget (key,SIZE,0666|IPC_CREAT);
	printf("shmid = %d\n",shmid);
	if(shmid ==-1)
	{
		perror("shmget error\n");
		return -1;
	}
	 shmaddr = shmat(shmid,NULL,0);
	if(shmaddr == (int *)-1)
	{
		perror("shmat error\n");
		return -1;
	}
	signal(SIGUSR1,handler);
	printf("wait message\n");
	while(1){
	sleep(100);
	}
	if(shmdt(shmaddr))
	{
		perror("shmdt error\n");
		return -1;
	}

#undef SIZE 
	return 0;
}
