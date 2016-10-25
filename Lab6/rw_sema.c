//Daniel Lindberg
//Lab6: Read-Write Semaphore
// 10/28/2015

#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/rwsem.h>
#include <linux/module.h>
#include <linux/semaphore.h>

MODULE_LICENSE("GPL");

struct rw_semaphore sem; 

wait_queue_head_t queue;

static int Major;
dev_t dev_no;
struct cdev *kernel_cdev;
int bytes_written, bytes_read;

struct device{
	char array[100];
	struct rw_semaphore rwsem;
} char_arr;


ssize_t read(struct file *filp, char *buff, size_t count, loff_t *offp){
	/*printk(KERN_INFO "Inside read \n");
	char write_buff[100];
	write(fd, write_buf, sizeof(write_buf));
	return write_buff;*/
	unsigned long returnedValue;	
	if(count > 100){
		count = 100;	
	}
	down_read(&char_arr.rwsem);
	wait_event_timeout(queue,0,20*HZ);
	returnedValue=copy_to_user(buff, char_arr.array, count);
	up_read(&char_arr.rwsem);
	return count;
}
ssize_t write(struct file *filp, const char *buff, size_t count, loff_t *offp){
	/*char buffer[128];
	cnt = (cnt > 64) ? 64 : cnt;
	strncpy(buffer, b, cnt);
	buffer[cnt-1]=0;
	printk(KERN_INFO "Inside write, buffer=%s, size=%d\n", buffer, (int)cnt);
	return cnt;*/
	unsigned long returnedValue;
	if(count > 100){
		count = 100;	
	}
	down_write(&char_arr.rwsem);
	wait_event_timeout(queue,0,20*HZ);
	returnedValue= copy_from_user(char_arr.array,buff,count);
	bytes_written = bytes_written + count;
	up_write(&char_arr.rwsem);
	return count;
}
int open(struct inode *inode, struct file *filp){
	/*printk(KERN_INFO "Inside open \n");
	char read_buf[100];
	read(fd, read_buf, sizeof(read_buff));*/
	bytes_read = bytes_written;
	return 0;
}
int release(struct inode *inode, struct file *filp){
	//printk(KERN_INFO "Inside close \n");
	return 0;
}

struct file_operations fops = {
	.read = read,
	.write = write,
	.open = open,
	.release = release
};



int init_module(){
	int ret;
	int dev;
	printk("Loading read write sema");

	kernel_cdev=cdev_alloc();
	kernel_cdev->ops = &fops;
	kernel_cdev->owner = THIS_MODULE;
	ret = alloc_chrdev_region(&dev_no, 0, 1, "interface");

	Major = MAJOR(dev_no);
	dev = MKDEV(Major, 0);
	ret = cdev_add(kernel_cdev, dev, 1);	

	init_rwsem(&sem);
	init_waitqueue_head(&queue);
	
	return 0;
}

void cleanup_module(){
	printk(KERN_INFO "Cleanup module");
	cdev_del(kernel_cdev);
	unregister_chrdev_region(dev_no,1);
	unregister_chrdev(Major, "interface");
}
