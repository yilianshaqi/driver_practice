/*************************************************************************
    > File Name: udpc.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年11月30日 星期三 15时58分57秒
 ************************************************************************/

#include<stdio.h>
#include<strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
  #include <unistd.h>
#define BUF_SIZE 30
int main()
{
	int fd = socket(AF_INET,SOCK_DGRAM,0);
	if(fd<0)
	{
		perror("create socket :\n");
		return -1;
	}
	struct sockaddr_in seraddr;
	bzero(&seraddr,sizeof(seraddr));
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(9999);
	inet_pton(AF_INET,"127.0.0.8",&seraddr.sin_addr);
	int ret = 0;
	char buf[BUF_SIZE]="";
	while(1)
	{
		sprintf(buf,"test ok !");
		sleep(2);
		ret = sendto(fd,buf,BUF_SIZE,0,(struct sockaddr *)&seraddr,sizeof(seraddr));
		if(ret <0)
		{
			perror("sendto :\n");
			break;
		}
		printf("sendto ok!\n");
		bzero(buf,BUF_SIZE);
	}
	close(fd);
	return 0;
}
