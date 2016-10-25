#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>

#define ARRAY_SIZE 1000000

MODULE_LICENSE("GPL");

int arr[ARRAY_SIZE] = {0};	
int stat[ARRAY_SIZE] = {0};	
int index = 0;			
int cs1 = 0;			
int cs2 = 0;		
struct semaphore lock;		

struct task_struct* t1;		
struct task_struct* t2;		

int incrementer(void* data) {

	while (index < ARRAY_SIZE && !kthread_should_stop()) {

		if((int)data >1) {
			cs1++; 
		}
		else{
			 cs2++;
		}
	
		if(!down_interruptible(&lock)) {
			if(index >= ARRAY_SIZE) {
				up(&lock);
				break;
			}
			arr[index++]++;	
			up(&lock);		
		} 
	}
	return 0;
}

int init_module(void) {	
	int id1 = 1;
	int id2 = 2;

	sema_init(&lock, 1);
	t1 = kthread_create(incrementer, (void*)id1, "inc1");
	t2 = kthread_create(incrementer, (void*)id2, "inc2");

	if(t1&&t2) {
		printk(KERN_INFO "Starting...\n");
		wake_up_process(t1);
		wake_up_process(t2);
	} else {
		printk(KERN_EMERG "Error\n");
	}
	return 0;
}

void cleanup_module(void) {
	int i = 0;

	if(index < ARRAY_SIZE) {
		kthread_stop(t1);
		kthread_stop(t2);
		printk(KERN_EMERG "Module removed before finished!\n");
	}

	for(i = 0; i < ARRAY_SIZE; i++) stat[arr[i]]++;

	for(i = 0 ; i < ARRAY_SIZE; i++) {
		if(stat[i] > 0) { 
			printk(KERN_INFO "stat[%d] = %d\n",i, stat[i]);
		}
	}
	printk(KERN_INFO "total: %d\tcs1: %d\tcs2: %d\n",index, cs1, cs2);
	return;
}
