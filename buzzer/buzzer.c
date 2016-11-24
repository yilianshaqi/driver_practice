/*************************************************************************
    > File Name: buzzer.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年11月22日 20时43分20秒 CST
 ************************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>

#include <asm/io.h>
#include <asm/uaccess.h>


#define OFF   1
#define ON    0
#define MAJORBU 211
#define MINORBU 99
#define DEV_COUNT 1
struct cdev mydev;
#define BUZZER_CON  0X114000A0
#define BUZZER_DAT  0X114000A4
static unsigned int *gpd0con=NULL;
static unsigned int *gpd0dat=NULL;

static int my_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
	return 0;

}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
		case OFF:
				writel(readl(gpd0dat) | 1,gpd0dat);
			break;
		case ON:
				writel(readl(gpd0dat) & ~1,gpd0dat);
			break;
		default :
			break;
	}
	return 0;
}
struct file_operations  operate={
	.owner=THIS_MODULE,
	.open=my_open,
	.release=my_close,
	.unlocked_ioctl=my_ioctl,
};
int my_probe(struct platform_device *dev)
{
	//申请设备号
	dev_t  dev_num=MKDEV(MAJORBU,MINORBU);
	int ret=register_chrdev_region(dev_num,DEV_COUNT,"buzzer");
	if(ret<0)
	{
		printk("register error\n");
		return -1;
	}
	//初始化设备
	mydev.owner = THIS_MODULE;
	cdev_init(&mydev,&operate);
	//注册设备
	ret=cdev_add(&mydev,dev_num,DEV_COUNT);
	if(ret<0)
	{
		printk("add device error\n");
		goto error1;
	}
	
	//buzzer地址映射
	gpd0con=ioremap(BUZZER_CON,4);
	gpd0dat=ioremap(BUZZER_DAT,4);
	if(gpd0con==NULL)	{
		printk("ioremap error");
		goto error2;
	}
	if(gpd0dat==NULL){
		printk("ioremap error");
		goto error3;
	}


	//buzzer初始化
	writel((readl(gpd0con) & ~(0xf)) | (0x1),gpd0con);
	writel(readl(gpd0dat) & ~1,gpd0dat);
	return 0;

error3:
	iounmap(gpd0con);
error2:
	cdev_del(&mydev);
error1:
	unregister_chrdev_region(dev_num,DEV_COUNT);
	return 0;
}


int my_remove(struct platform_device *dev)
{
	
	dev_t  dev_num=MKDEV(MAJORBU,MINORBU);
	//物理地址映射映射释放
	iounmap(gpd0con);
	iounmap(gpd0dat);
	//设备删除
	cdev_del(&mydev);
	//设备号释放
	unregister_chrdev_region(dev_num,DEV_COUNT);
	return 0;
}


struct of_device_id table[]={
{ .compatible = "buzzer" },               //和设备数中设备名相匹配
	{},
};

struct platform_driver   my_driver={
	.probe = my_probe,
	.remove=my_remove,
	.driver={
		.name="buzzer",
		.of_match_table=table,
	}
};

int buzzer_init(void)
{
	platform_driver_register(&my_driver);
	return 0;
}
void buzzer_exit(void)
{
	platform_driver_unregister(&my_driver);

}
MODULE_LICENSE("GPL");
module_init(buzzer_init);
module_exit(buzzer_exit);
