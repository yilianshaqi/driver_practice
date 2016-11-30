/*************************************************************************
    > File Name: udps.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年11月30日 星期三 15时18分42秒
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<strings.h>
#include <unistd.h>
#define BUF_SIZE 30
int main()
{
	//socket创建
	int fd = 0;
	fd = socket(AF_INET,SOCK_DGRAM,0);
	if(fd<0)
	{
		perror("create socket\n");
		return -1;
	}
	int ret =0;
	//ip和端口配置
	struct sockaddr_in seraddr;
	bzero(&seraddr,sizeof(seraddr));
	seraddr.sin_family=AF_INET;
	seraddr.sin_port = htons(9999);
	inet_pton(AF_INET,"127.0.0.8",&seraddr.sin_addr);
	struct sockaddr cliaddr;
	int length=sizeof(cliaddr);
	bzero(&cliaddr,length);
	//socket 和地址绑定
	ret = bind(fd,(struct sockaddr *)&seraddr,sizeof(seraddr));
	if(ret<0)
	{
		perror("bind error\n");
		close(fd);
		return -1;
	}
	//直接通过socket文件描述符接收信息
	char buf[BUF_SIZE]="";
	int i=100;
	while(i--)
	{
		ret = recvfrom(fd,buf,BUF_SIZE,0,&cliaddr,&length);
		if(ret < 0)
		{
			perror("recvfrom error:\n");
			continue;
		}
		else{
			printf("ret = %d,recv:%s\n",ret,buf);
			bzero(buf,BUF_SIZE);
		}
	}
	return 0;

}
