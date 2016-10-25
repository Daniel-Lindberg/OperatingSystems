#ifndef BUDDYDEV_H
#define BUDDYDEV_H

#include <linux/ioctl.h>

#define MAJOR_NUM 150

#define BUDDY_BLOCK_DEPTH 4
#define BUDDY_BLOCK_SIZE (1<<BUDDY_BLOCK_DEPTH)
#define BUDDY_NUM_BLOCKS (1<<BUDDY_BLOCK_DEPTH)
#define MEM_SIZE (BUDDY_NUM_BLOCKS * BUDDY_BLOCK_SIZE)

struct get_mem_struct{
	int mem;
	int size;
	
	int return_val;
};

struct free_mem_struct{
	int mem;
	int ref;
	
	int return_val;
};
struct write_mem_struct{
	int mem;
	int ref;
	char*buf;
	
	int return_val;
};
struct read_mem_struct{
	int mem;
	int ref;
	char *buf;
	int size;
	int return_val;
};

#define IOCTL_GET_MEM _IOR(MAJOR_NUM, 0, void*)

#define IOCTL_FREE_MEM _IOR(MAJOR_NUM, 1, void*)

#define IOCTL_WRITE_MEM _IOR(MAJOR_NUM, 2, void*)

#define IOCTL_READ_MEM _IOR(MAJOR_NUM, 3, void*)


#endif
