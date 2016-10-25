#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>

MODULE_LICENSE("GPL");

int arr[1000000];
int index=0;

int cs1=0, cs2=0;

int incrementer(){
	printk(KERN_INFO "Consumer TID %d\n", (int)ptr);
	while(index < 1000000){
		if((int)ptr==1){
			cs1++;		
		}
		else{
			cs2++;		
		}
		arr[index++] +=1;
		//schedule();	
	}

	return 0;
} 
int init_module( void ) {
   int id=1, id=2;
   t1=kthread_create(incrementer, (void*)id1, "inc1");
   t2=kthread_create(incrementer, (void*)id2, "inc2");
   if(t1&&t2){
		printk(KERN_INFO "Starting..\n");
		wake_up_process(t1);
		wake_up_process(t2);   
	} else{
		printk(KERN_EMERG "Error \n");	
	}
   return 0;
}

void cleanup_module( void ) {
	for(int i =0; i< 1000000; i++){
		if(arr[i] ==x){
		
		stat[x]+=1;	
		//In cleanup module write a loop that adds 1 to stat[x] for each i from 
		//0 to to 999999 where arr[index] has value x.
		}
		if(stat[i]>0){
			printk(KERN_INFO stat[i]);
			printk("CS1:"+cs1 + " CS2:"+cs2+" CS1+CS2:"+cs1+cs2);
		}
	}  
   printk("Kthread module uninstalling\n");
   return;
}
