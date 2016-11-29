/*************************************************************************
    > File Name: client.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年11月28日 星期一 19时49分18秒
 ************************************************************************/

#include<stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include<unistd.h>
#include <sys/socket.h>
#include<strings.h>
#define BUF_SIZE  30
int main(){
	int fs=0;
	int ret =0;
	char buf[BUF_SIZE]="";
	struct sockaddr_in my_addr;
	bzero(&my_addr,sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(9000);
	ret = inet_pton(AF_INET,"127.0.0.10",&my_addr.sin_addr);
	if(ret <0)
	{
		perror("inet_pton:\n");
		return -1;
	}
	fs = socket(AF_INET,SOCK_STREAM,0);
	if(fs<=0)
	{
		perror("socket :\n");
		return -1;
	}
	ret = connect(fs,(struct sockaddr *)&my_addr,sizeof(my_addr));
	if(ret<0)
	{
		perror("connect :\n");
		return -1;
	}
	while(1){
			sleep(1);
		ret = recv(fs,buf,BUF_SIZE,MSG_DONTWAIT);
		if(ret<0)
		{
			perror("recv : \n");
			break;
		}
		printf("ret =%d\t,recv : %s\n",ret,buf);

	}
	close(fs);
	return 0;


}
