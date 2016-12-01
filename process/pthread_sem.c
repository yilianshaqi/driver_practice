/*************************************************************************
    > File Name: pthread_sem.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年12月01日 星期四 18时21分48秒
 ************************************************************************/

#include <pthread.h>
#include<stdio.h>
#include <semaphore.h>
#include <unistd.h>
	//定义进程信号量
	sem_t ftos;
	sem_t stot;
void *show(void *);
void *print(void *);
int main()
{
	//初始化
	if(sem_init(&ftos,0,0))
	{
		perror("sem_init error\n");
		return -1;
	}
	if(sem_init(&stot,0,0))
	{
		perror("sem_init error\n");
		return -1;
	}
	//创建线程
	pthread_t first;
	int num=2;
	if(pthread_create(&first,NULL,show,(void *)&num))
	{
		perror("first pthread create error\n");
		return -1;
	}

//	pthread_detach(first);
	pthread_t second;
	int other=3;
	if(pthread_create(&second ,NULL,print ,(void *)&other))
	{
		perror("second pthread_create error\n");
		return -1;
	}
//	pthread_detach(second);
	printf("create ok \n");
	printf("1 pthead over and release sem  ftos\n");
	//释放信号量让show可以执行
	sem_post(&ftos);
//	sleep(5);
	pthread_join(second,NULL);
	printf("main over\n");
	return -1;

	
}
//等主线程释放信号量后获得信号量再执行
void *show(void *num)
{
	if(sem_wait(&ftos))
	{
		perror("show pthread sem_wait\n");
		return NULL;
	}
	int *count =(int *)num;
	printf("this is %d\n",*count);
		sleep(2);
	sem_post(&stot);
	return NULL;
}
//等show函数释放了信号量后再申请信号量后执行
void *print(void *num)
{
	int *count =(int *)num;
	if(sem_wait(&stot))
	{
		perror("print pthread sem_wait\n");
		return NULL;
	}
	printf("this is %d\n",*count);
	return NULL;
}
