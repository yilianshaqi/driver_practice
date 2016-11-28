/*************************************************************************
    > File Name: block.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年11月28日 星期一 17时49分14秒
 ************************************************************************/

#include<stdio.h>
#include<errno.h>
#include <unistd.h>
#include<string.h>
#include <fcntl.h>
#include<stdlib.h>
#define BUF_SIZE 20
int main()
{
	char buf[BUF_SIZE]="";
	int ret = 0;
	int flags = 0 ;
	printf("stdin : %d\n",STDIN_FILENO);
	flags = fcntl(STDIN_FILENO,F_GETFL);
	fcntl(STDIN_FILENO,F_SETFL,flags | O_NONBLOCK);
	ret = read(STDIN_FILENO,buf,BUF_SIZE);
	printf("read after ret = %d\n",ret);
	if(ret <0){
		printf("read file : %s\n",strerror(errno));
		return -1;
	}
	printf("ret = %d\t,buf=%s\n",ret,buf);
	return 0;
}
