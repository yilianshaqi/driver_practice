/*************************************************************************
    > File Name: interrupter.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年11月23日 星期三 16时46分25秒
 ************************************************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
//定义等待结构体队列
struct work_struct my_queue;
//定义一个下半部函数
void my_queue_func(struct work_struct *my_wq){
	printk("welcome to work queue \n");
	ssleep(10);
	printk("thank you!\n");
}
//在上半部函数中调用下半部函数
struct resource *psource=NULL;
irqreturn_t my_handler(int dev, void *name){
	printk("come to interrupte\n");
	schedule_work(&my_queue);
	printk("psource->start=%d,psource->name=%s,psource->flags=%lu\n",psource->start,psource->name,psource->flags);
	return IRQ_HANDLED;
}
int my_probe(struct platform_device *dev){
	//申请资源
	int ret;
	psource=platform_get_resource(dev,IORESOURCE_IRQ,0);
	if(NULL==psource){
		printk("get source error\n");
		return -1;
	}
	//注册中断

	ret=request_irq(psource->start, my_handler, psource->flags & IRQF_TRIGGER_MASK,
			    "key_interrupte", NULL);
	if(ret<0)
	{
		printk("request error\n");
		return -1;
	}
	//初始化等待队列和把绑定函数
	INIT_WORK(&my_queue,my_queue_func);

	return 0;
}

int my_remove(struct platform_device *dev){
	free_irq(psource->start,NULL);
	return 0;
}





struct of_device_id  my_fw[]={
	{.compatible="fs4412,key2"},
	{},
};

struct platform_driver my_driver={
	.probe=my_probe,
	.remove=my_remove,
	.driver={
		.name="interrupt",
		.of_match_table=my_fw,
		},
};
//建立模块
int my_module_init(void){
	platform_driver_register(&my_driver);
	return 0;
}
void my_module_exit(void){
	platform_driver_unregister(&my_driver);

}
MODULE_LICENSE("Dual BSD/GPL");
module_init(my_module_init);
module_exit(my_module_exit);
