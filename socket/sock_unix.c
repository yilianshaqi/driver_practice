/*************************************************************************
    > File Name: sock_unix.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年12月01日 星期四 15时17分45秒
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<strings.h>
#include <arpa/inet.h>
#include<string.h>
#include <unistd.h>
 #include <sys/un.h>
#define COUNT 5
#define BUF_SIZE 30
const char socket_name[]="/home/hy/practice/unix";
int main()
{
	struct sockaddr_un saddr;
	bzero(&saddr,sizeof(saddr));
	saddr.sun_family = PF_LOCAL;
	strcpy(saddr.sun_path,socket_name);
	//创建UNIX socket
	int fd = socket(PF_LOCAL,SOCK_DGRAM,0);
	if(fd<0)
	{
		perror("create socket error\n");
		return -1;
	}
	//绑定socket
	if(bind(fd,(struct sockaddr *)&saddr,sizeof(saddr)))
	{
		perror("bind error\n");
		return -1;
	}
/*	//listen
	if(listen(fd,COUNT))
	{
		perror("listen error\n");
		return -1;
	}     
	//accept
	int rfd=accept(fd,NULL,NULL);
	if(rfd<0)
	{
		perror("accept error\n");
		return -1;
	}      */
	//recv
	char buf[BUF_SIZE]="";
	int ret =0;
	int i = 10;
	while(i--){

		ret = recvfrom(fd,buf,BUF_SIZE,0,NULL,NULL);
	//	ret = recv(rfd,buf,BUF_SIZE,0);
		if(ret <=0)
		{
			perror("recv error:\n");
			return -1;
		}
		printf("ret =%d,recv : %s\n",ret,buf);
	}
	//close
//	close(rfd);
	close(fd);
	return 0;
}

