/*************************************************************************
    > File Name: client.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年11月28日 星期一 19时49分18秒
 ************************************************************************/
#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include<strings.h>
#include <arpa/inet.h>
#include<string.h>
#include <unistd.h>
#include <sys/un.h>
#define BUF_SIZE  30
const char name[]="/home/hy/practice/unix";
int main(){
	int fs=0;
	int ret =0;
	char buf[BUF_SIZE]="";
	struct sockaddr_un my_addr;
	bzero(&my_addr,sizeof(my_addr));
	my_addr.sun_family = PF_LOCAL;
	strcpy(my_addr.sun_path,name);
//	fs = socket(PF_LOCAL,SOCK_STREAM,0);
	fs = socket(PF_LOCAL,SOCK_DGRAM,0);
	if(fs<=0)
	{
		perror("socket :\n");
		return -1;
	}
/*	ret = connect(fs,(struct sockaddr *)&my_addr,sizeof(my_addr));
	if(ret<0)
	{
		perror("connect :\n");
		return -1;
	}  */
	int i=10;
	while(i){
		sleep(1);
		strcpy(buf,"test ok !");
//		ret = send(fs,buf,strlen(buf),0);
		ret = sendto(fs,buf,strlen(buf),0,(struct sockaddr *)&my_addr,sizeof(my_addr));
		if(ret<0)
		{
			printf("ret =%d\n",ret);
			perror("recv : \n");
			break;
		}
		bzero(buf,BUF_SIZE);
	}
	close(fs);
	return 0;


}
