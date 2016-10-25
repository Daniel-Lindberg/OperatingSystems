//Daniel Lindberg
//Lab8: Buddy Allocator
// 10/28/2015

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/string.h>

#include "buddy-dev.h"
#define DEVICE_NAME "memory_device"

MODULE_LICENSE("GPL");
static int Device_open = 0;

static char *memory; 
enum block_state {PARENT, ALLOCATED, FREE};
struct block_node{
	enum block_state state;
	struct block_node *parent;
	struct block_node *left_child;
	struct block_node *right_child;
};

static struct block_node *buddy_root;

static void free_tree(struct block_node *block){
	if(block->left_child) {
		free_tree(block->left_child);
	}
	if(block->right_child){
		free_tree(block->right_child);	
	}
	kfree(block);
}

static void split_block(struct block_node *block){
	block->state = PARENT;
	
	block->left_child = kmalloc(sizeof(struct block_node), GFP_KERNEL);
	block->left_child->state = FREE;
	block->left_child->parent = block;
	block->left_child->left_child = NULL;
	block->left_child->right_child = NULL;

	
	block->right_child = kmalloc(sizeof(struct block_node), GFP_KERNEL);
	block->right_child->state = FREE;
	block->right_child->parent = block;
	block->right_child->left_child = NULL;
	block->right_child->right_child = NULL;

}

static void free_and_merge(struct block_node *block){
	struct block_node *parent;
	
	block->state = FREE;
	parent = block->parent;

	if(parent && parent->left_child->state == FREE && parent->right_child->state == FREE){
	kfree(parent->left_child);
	kfree(parent->right_child);
	parent->left_child = NULL;
	parent->right_child = NULL;
	free_and_merge(parent);
	}
}

struct block_node *get_block_from_address(int reference){
	int block_idx;
	int nth_bit;
	int n;
	struct block_node *current_node;

	if(reference <0 || MEM_SIZE == reference){
		return NULL;		
	}

	block_idx= reference/ BUDDY_BLOCK_SIZE;
	current_node = buddy_root;
	for(n = BUDDY_BLOCK_DEPTH-1; n>=0; n--){
		if(current_node->state != PARENT){
			break;		
		}	
		nth_bit = (block_idx >> n) & 1;
		if(nth_bit){
			current_node = current_node->right_child;		
		}
		else{
			current_node = current_node->left_child;		
		}
	}

	return current_node;
}

int get_mem(int size, struct block_node *block, int available){
	int reference;
	reference =0;
	
	if(available < size){
		return -1;	
	}
	
	if(block->state == FREE){
		if(size <= (available>>1) && BUDDY_BLOCK_SIZE <= (available>>1)){
			split_block(block);
			return get_mem(size, block->left_child, available >>1);		
		}	
		block->state = ALLOCATED;
		return 0;
	}
	
	if(block->state == ALLOCATED){
		return -1;
	}
	reference = get_mem(size, block->left_child, available>>1);
	if(reference >= 0){
		return reference;	
	}
	
	reference = get_mem(size, block->right_child, available>>1);

	if(reference < 0){
		return -1;	
	}
	reference += (available>>1);

	return reference;
	// kmalloc(size, GFP_KERNEL);
}

int free_mem(int reference){
	struct block_node *block;
	block = get_block_from_address(reference);
	if(block == NULL || block->state != ALLOCATED){
		return -1;
	}
	
	free_and_merge(block);
	return 0;
}

static int open(struct inode *inode, struct file *file){
	if(Device_open){
		return -EBUSY;	
	}
	else{
		Device_open++;
	}
	return 0;
}
static int release(struct inode *inode, struct file *file){

	Device_open--;
	return 0;
}

static ssize_t read(struct file *file, char *buffer, size_t length, loff_t *offset){
	copy_to_user(buffer, memory + (long)offset, length);
	return length;
}
static ssize_t write(struct file *file, const char *buffer, size_t length, loff_t *offset){
	copy_from_user(memory+ (long)offset, buffer, length);
	return length;
}
int write_mem(struct file *file, int ref, char *buf){
	struct block_node *block1;
	struct block_node *block2;

	int size;
	long rf;

	size = strlen(buf);

	block1=get_block_from_address(ref);
	block2=get_block_from_address(ref+size-1);

	rf = ref;

	if(block1 && block1 == block2){
		return (int)write(file,buf,size, (loff_t *)rf);	
	}
	
	return -1;
}
int read_mem(struct file *file, int ref, char *buf, int size){
	struct block_node *block1;
	struct block_node *block2;

	long rf;

	block1=get_block_from_address(ref);
	block2=get_block_from_address(ref+size-1);

	rf=ref;

	if(block1 && block1==block2){
		return (int)read(file,buf,size,(loff_t *)rf);	
	}

	return -1;
}
long ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param){
	switch(ioctl_num){
	case IOCTL_GET_MEM:
		((struct get_mem_struct *) ioctl_param)->return_val = get_mem(
			((struct get_mem_struct *) ioctl_param)->size,
			buddy_root,
			MEM_SIZE
		);
		break;
	case IOCTL_FREE_MEM:
		((struct free_mem_struct *)ioctl_param)->return_val = free_mem(
			((struct free_mem_struct *)ioctl_param)->ref
		);
		break;
	case IOCTL_WRITE_MEM:
		((struct write_mem_struct *)ioctl_param)->return_val = write_mem(
			file,
			((struct write_mem_struct *)ioctl_param)->ref,
			((struct write_mem_struct *)ioctl_param)->buf
		);
		break;
	case IOCTL_READ_MEM:
		((struct read_mem_struct *)ioctl_param)->return_val = read_mem(
			file,
			((struct read_mem_struct *)ioctl_param)->ref,
			((struct read_mem_struct *)ioctl_param)->buf,
			((struct read_mem_struct *)ioctl_param)->size
		);
		break;
	default:
		break;
	}
	return 0;
}
struct file_operations Fops = {
	.read=read,
	.write=write,
	.unlocked_ioctl=ioctl,
	.open=open,
	.release = release
};


int init_module(){
	int ret_val;

	ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &Fops);
	if(ret_val < 0) {
		return ret_val;	
	}

	memory= kmalloc(MEM_SIZE, GFP_KERNEL);
	memset(memory, 0, MEM_SIZE);

	buddy_root = kmalloc(sizeof(struct block_node), GFP_KERNEL);
	buddy_root->state = FREE;
	buddy_root->parent = NULL;
	buddy_root->left_child = NULL;
	buddy_root->right_child = NULL;

	return 0;
}

void cleanup_module(){
	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
	kfree(memory);
	free_tree(buddy_root);
}
