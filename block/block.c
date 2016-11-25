/*************************************************************************
    > File Name: block.c
    > Author: huang yang
    > Mail: huangyangwork@sina.com 
    > Created Time: 2016年11月24日 星期四 20时26分31秒
 ************************************************************************/
#include <linux/kernel.h>

#include <linux/module.h>

#include <linux/fs.h>

#include <linux/uaccess.h>

#include <linux/blkdev.h>

#include <linux/hdreg.h>

#include <linux/vmalloc.h>
#define BYTES_PER_SECTOR   512
#define CYLINDERS  32
#define HEADS      64
#define SECTOR_PER_TRACK  32
#define TOTAL_SECTORS  (CYLINDERS*HEADS*SECTOR_PER_TRACK)
#define TOTAL_SIZE (TOTAL_SECTORS*BYTES_PER_SECTOR)


#define BLKCOUNT 5
char *data=0;
const char devname[15]="test_block";
int major=0;
struct gendisk *my_block=NULL;
spinlock_t lock;
struct request_queue *requeue=NULL;

static int my_open(struct block_device *bdev, fmode_t mode)
{
	return 0;
}
static void my_close(struct gendisk *gd, fmode_t mode)
{

}
static int my_ioctl(struct block_device *bdev,fmode_t mode,unsigned cmd, unsigned long arge){
	return 0;
}
static int my_getgeo(struct block_device *bdev,struct hd_geometry *geo){
	geo->heads=HEADS;
	geo->sectors =SECTOR_PER_TRACK;
	geo->cylinders=CYLINDERS;
	geo->start =0;
	return 0;
}
struct block_device_operations block_operate={
	.owner=THIS_MODULE,
	.open=my_open,
	.release=my_close,
	.ioctl=my_ioctl,
	.getgeo=my_getgeo,
};

//队列的成员处理方法
void my_request_handler (struct request_queue *q){
	struct request *perq=blk_fetch_request(q); //从队列中提取成员
	while(perq)									//成员不为空进行读写
	{
		unsigned long pos=blk_rq_pos(perq)*BYTES_PER_SECTOR;  //获取请求结构要访问的数据扇区
		int nbytes=blk_rq_cur_bytes(perq);    //获取请求结构体要获得的字节数
		unsigned long cur_position=pos+nbytes;  
		if(cur_position>TOTAL_SIZE)          //不超出界限才可以访问
		{
			printk("out of rang\n");
			
		}
		if(rq_data_dir(perq)==READ)           //判断请求结构体的请求是读还是写
		{
			memcpy(perq->buffer,data+pos,nbytes);
		}
		else
		{
			memcpy(data+pos,perq->buffer,nbytes);
		}
		if(!__blk_end_request_cur(perq,0))  //判断请求结构体后是否为空（请求队列是否处理完毕）
		{
			perq=NULL;
		}
	}
}
//初始化模块
int block_init(void ){

	int ret=0;
	//申请块设备号
	major=register_blkdev(0,devname);
	if(major<0)
	{
		printk("register error\n");
		return major;
	}
	
	//初始化块设备
	data = vmalloc(TOTAL_SIZE);  //申请内存为了虚拟磁盘，给队列成员处理函数用了
	if(NULL==data)
	{
		printk(" vmalloc error\n");
		ret = -ENOMEM;
		goto VFREE;
	}
		//建立快设备结构体
	my_block=alloc_disk(BLKCOUNT);
	if(NULL==my_block)
	{
		printk("alloc error\n");
		ret = -ENOMEM;
		goto UNREQUEST;
	}
		//准备请求队列
		spin_lock_init(&lock);
		requeue=blk_init_queue(my_request_handler,&lock);
		if(NULL==requeue)
		{
			printk("init_queue error\n");
			ret = -ENOMEM;
			goto RELEASE_DISK; 
		}
		//设备结构体初始化，全是赋值
		my_block->major=major;
		my_block->first_minor=0;
		my_block->fops=&block_operate;
		my_block->queue=requeue;
		sprintf(my_block->disk_name,"%s",devname);
		set_capacity(my_block,TOTAL_SECTORS);
	//注册块设备
	add_disk(my_block);
	printk("init block ok\n");
	return 0;
RELEASE_DISK:
	del_gendisk(my_block);
VFREE:
	vfree(data);
UNREQUEST:
	unregister_blkdev(major,devname);
	return ret;
}
void block_exit(void){
	//释放申请的内存（为了虚拟设备而申请的内存）
	vfree(data);
	//释放队列	
	blk_cleanup_queue(requeue);
	//删除设备结构体
	del_gendisk(my_block);
	//释放设备号
	unregister_blkdev(major,devname);
}
//卸载模块
MODULE_LICENSE("GPL");
module_init(block_init);
module_exit(block_exit);

