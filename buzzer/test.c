/*************************************************************************
    > File Name: test.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年11月22日 星期二 23时59分53秒
 ************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include<stdio.h>

#define OFF   1
#define ON    0
int main()
{
	int fd=open("/dev/buzzer",O_RDWR);
	if(fd<0)
	{
		perror("open file error");
		return -1;
	}
	while(1){


		ioctl(fd,OFF);
		sleep(2);
		ioctl(fd,ON);
		sleep(2);
	}
		close(fd);
	return 0;
}
