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
	ret = inet_pton(AF_INET,"127.0.0.9",&my_add.sin_addr);
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
	fd_set set;
	FD_ZERO(&set);
	printf("FD_SET SIZE:%d\n",sizeof(fd_set));
	FD_SET(fs,&set);
	int max=fs;
	fd_set myset;
	FD_ZERO(&myset);
	while(1)
	{
		myset=set;
		ret = select(max+1,&myset,NULL,NULL,NULL);
		if(ret<0)
		{
			perror("select error:\n");
			return 0;
		}
		if(ret>0)
		{
			for(int i=0;i<max+1;i++)
			{
				if(FD_ISSET(i,&myset))
				{
					if(i==fs)
					{
						rfs = accept(fs,&recvaddr,&length);
						if(rfs<0)
						{
							perror("accept error\n");
							return -1;
						}
						FD_SET(rfs,&set);
						if(max<rfs)
						{
							max=rfs;
						}
					}
					else{
						ret = read(i,buf,BUF_SIZE);
						if(ret <=0)
						{
							close(i);
							FD_CLR(i,&set);
						}
						printf("ret = %d\t,read:%s\n",ret,buf);
						bzero(buf,BUF_SIZE);
					}
				}
			}

		}
	}
	//断开连接
	for(int i=0;i<=max;i++)
	{
		close(i);
	}
	return 0;
}
