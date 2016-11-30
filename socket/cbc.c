/*************************************************************************
  > File Name: cbc.c
  > Author: huang yang
  > Mail: huangyangwork@sina.com 
  > Created Time: 2016年11月30日 星期三 19时25分12秒
 ************************************************************************/

#include<stdio.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<strings.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9999
#define BUF_SIZE 30
const char ip[]="0.0.0.0";
int main()
{
	int fd = socket(AF_INET,SOCK_DGRAM,0);
	if(fd<0)
	{
		perror("socket error\n");
		return -1;
	}
	struct sockaddr_in caddr;
	bzero(&caddr,sizeof(caddr));
	caddr.sin_family=AF_INET;
	caddr.sin_port=htons(PORT);
	inet_pton(AF_INET,ip,&caddr.sin_addr);
//	caddr.sin_addr.s_addr=INADDR_ANY;
	printf("INADDR_ANY=%d\t,%s\n",INADDR_ANY,inet_ntoa(caddr.sin_addr));
	int ret = bind(fd,(struct sockaddr *)&caddr,sizeof(struct sockaddr));
	if(ret<0)
	{
		perror("bind error\n");
		return -1;
	}
	int i=10;
	char buf[BUF_SIZE]="";
	while(i--)
	{

		printf("before recv\n");
		ret = recvfrom(fd,buf,BUF_SIZE,0,NULL,NULL);
		printf("ret=%d,recv: %s\n",ret,buf);
		bzero(buf,BUF_SIZE);
	}
}
