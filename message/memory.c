/*************************************************************************
    > File Name: memory.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年12月06日 星期二 14时51分04秒
 ************************************************************************/

#include<stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
int main(int argc,char *argv[])
{
	if(argc !=2 )
	{
		printf("entry error\n");
		return -1;
	}
#define SIZE 64
	key_t key = 1010;
	//创建共享内存
	int shmid = shmget(key,SIZE,0666);
	printf("shmid = %d\n",shmid);
	if(shmid == -1){
		perror("shmget error:\n");
		return -1;
	}
	//映射共享内存
	int *shmaddr = shmat(shmid,NULL,0);
	if(shmaddr == (int *)-1){
		perror("shmat error\n");
		return -1;
	}
	system("ipcs -m");
	int data = 10;
	printf("pid = %d\n",atoi(argv[1]));
	while(data--)
	{
		*shmaddr = data;
		printf("send signal\n");
		kill(atoi(argv[1]),10);
		sleep(1);
	}
	//分离映射
	if(shmdt(shmaddr))
	{
		perror("shmdt error\n");
		return -1;
	}
	//关闭内存共享
	if(shmctl(shmid,IPC_RMID,NULL)==-1)
	{
		perror("shmctl remove error\n");
		return -1;
	}


#undef SIZE
}
