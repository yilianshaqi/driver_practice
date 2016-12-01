/*************************************************************************
    > File Name: pipe.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年12月01日 星期四 19时20分25秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUF_SIZE 30
void handler(int signum)
{
	printf("send signal%d\n",signum);

}
int main()
{
	int fd[2]={0};
	//创建pipe
	if(pipe(fd))
	{
		perror("create pipe error:\n");
		return -1;
	}
	signal(SIGPIPE,handler);
	//创建子线程
	pid_t pid;
	if((pid=fork())==0)
	{
		//向父进程发送数据
		char buf[BUF_SIZE]="hello father";
		sleep(3);
		write(fd[1],buf,strlen(buf));
		bzero(buf,BUF_SIZE);
	//	read(fd[0],buf,BUF_SIZE);
	//	printf("pid =%d,read:%s\n",getpid(),buf);
		printf("chlid over\n");
	//	close(fd[1]);
		sleep(6);
		return 0;
	}
	else if(pid == -1)
	{
		perror("fork error\n");
		return -1;
	}
	char buf[BUF_SIZE]="";
//	sleep(1);
//	close(fd[0]);
	printf("before read\n");
	close(fd[0]);
//	read(fd[0],buf,BUF_SIZE);
//	printf("after read\n");
//	bzero(buf,BUF_SIZE);
//	sprintf(buf,"ok");
//	write(fd[1],buf,BUF_SIZE);
	printf("read:%s\n",buf);
	wait(NULL);
	printf("all over\n");
	return 0;
}
