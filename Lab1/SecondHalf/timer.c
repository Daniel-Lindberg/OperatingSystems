#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/slab.h>

//Daniel Lindberg
//September 9th, 2015

MODULE_LICENSE("GPL");

static int count=0;
struct workqueue_struct *queue;
struct delayed_work *dwork;

void function(struct work_struct *work){
	if(count<10){      
		printk(KERN_EMERG "DING\n");count++;     
		queue_delayed_work(queue,dwork,HZ);
	}
	else{
		return;	
	} 
}

int init_module( void ) {
  printk("Timer module installing\n");

  queue = create_workqueue("queue");
  dwork = (struct delayed_work*)kmalloc(sizeof(struct delayed_work), GFP_KERNEL); 
  INIT_DELAYED_WORK((struct delayed_work *)dwork, function);
  queue_delayed_work(queue,dwork,HZ);
  return 0;
}

void cleanup_module( void ) {
	if (dwork && delayed_work_pending(dwork)){
		cancel_delayed_work(dwork);	
	}
	flush_workqueue(queue);
	destroy_workqueue(queue);

  printk("Timer module uninstalling\n");

  return;
}
