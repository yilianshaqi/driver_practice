/*************************************************************************
    > File Name: non_block.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年11月28日 星期一 16时46分44秒
 ************************************************************************/
#include<string.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<errno.h>
#define BUF_SIZE 20
int main(){
	char buf[BUF_SIZE]="";
	int ret = 0;
	int fd = open("./test_data.txt",O_RDWR | O_CREAT,0664);
	if(fd <0 )
	{
		perror("open file :\n");
		return -1;
	}
	ret = read(fd,buf,BUF_SIZE);
	printf("read after ret= %d\n",ret);
	if(ret < 0 )
	{
		printf("read return :%s\n",strerror(errno));
		return -1;
	}
	printf("ret =%d\tread:%s\n",ret,buf);
	return 0;

}
