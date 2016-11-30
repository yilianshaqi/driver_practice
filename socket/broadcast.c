/*************************************************************************
    > File Name: broadcast.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年11月30日 星期三 18时15分02秒
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<strings.h>
 #include <arpa/inet.h>
#include <unistd.h>
#define  PORT  9999
#define BUF_SIZE 30
const char ip[]="192.168.1.255";
int main()
{
	int fd = socket(AF_INET,SOCK_DGRAM,0);
	if(fd<0)
	{
		perror("socket error:\n");
		return -1;
	}
	int on = 1;
	int ret = setsockopt(fd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on));
	if(ret<0)
	{
		perror("setsockopt error\n");
		return -1;
	}
	struct sockaddr_in bcaddr;
	bzero(&bcaddr,sizeof(bcaddr));
	bcaddr.sin_family=AF_INET;
	bcaddr.sin_port=htons(PORT);
	inet_pton(AF_INET,ip,&bcaddr.sin_addr);
//	bcaddr.sin_addr.s_addr=htonl(INADDR_BROADCAST);
	/*
	ret = bind(fd,(struct sockaddr *)&bcaddr,sizeof(struct sockaddr));
	if(ret<0)
	{
		perror("bind error\n");
		return -1;
	}*/
	char buf[BUF_SIZE]="HELLO WORLD!";
	while(1)
	{
		sleep(1);
		printf("before send\n");
		ret = sendto(fd,buf,BUF_SIZE,0,(struct sockaddr *)&bcaddr,sizeof(struct sockaddr));

	}
	
}
