/*************************************************************************
    > File Name: socket.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年11月28日 星期一 18时41分14秒
 ************************************************************************/
#include <sys/types.h>      
#include <sys/socket.h>
#include <strings.h>
 #include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include<stdio.h>
#define BUF_SIZE 30
#define COUNT 5
#include <signal.h>
void handler(int signum)
{
	printf("signal is :sigpipe,num=%d\n",signum);

}
int main()
{
	struct sockaddr  recvaddr;
	int length = sizeof(recvaddr);
	bzero(&recvaddr,length);
	//创建socket文件
	int fs =0;
	int rfs = 0;
	int ret = 0;
	int i=100;
	char buf[BUF_SIZE]="";
	fs = socket(AF_INET,SOCK_STREAM,0);
	if(fs <0)
	{
		perror("socket error:\n");
		return -1;
	}

	//信号处理
	signal(SIGPIPE,handler);
	//设置协议族和端口
	struct sockaddr_in my_add;
	bzero(&my_add,sizeof(my_add));
	my_add.sin_family = AF_INET;
	my_add.sin_port = htons(9000);
	ret = inet_pton(AF_INET,"127.0.0.10",&my_add.sin_addr);
	if(ret <=0 )
	{
		if (ret == 0)
			fprintf(stderr, "Not in presentation format");
		else
			perror("inet_pton");
		return -1;
	}
 

	//绑定socket
	ret = bind(fs,(struct sockaddr *)&my_add,sizeof(struct sockaddr));
	if(ret < 0)
	{
		perror("bind error:");
		return -1;
	}
	//监听socket连接
	ret = listen(fs,COUNT);
	if(ret<0)
	{
		perror("listen error:\n");
		return -1;
	}
	
	//接收socket连接
	printf("accept before\n");
	rfs = accept(fs,&recvaddr,&length);
	printf("accept after\n");
	if(rfs<0)
	{
		perror("accept error\n");
		return -1;
	}
	//读写操作
	while(i--){

		sprintf(buf,"socket test ok!%d",i);
		ret = send(rfs,buf,BUF_SIZE,0);
		bzero(buf,BUF_SIZE);
		printf("ret =%d\n",ret);
		sleep(3);
	}
	//断开连接
	close(fs);
	close(rfs);
}
