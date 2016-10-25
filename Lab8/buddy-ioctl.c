#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "buddy-dev.h"

int get_mem(int mem, int size){
	struct get_mem_struct params = {
		.mem = mem,
		.size = size	
	};
	ioctl(mem, IOCTL_GET_MEM, (void *)(&params));
	
	return params.return_val;
}

int free_mem(int mem, int ref){
	strut free_mem_struct params = {
		.mem= mem,
		.ref = ref
	};

	ioctl(mem, IOCTL_FREE_MEM, (void *)(&params));
	
	return params.return_val;
}
int write_mem(int mem, int ref, char *buf){
	strut free_mem_struct params = {
		.mem= mem,
		.ref = ref,
		.buf = buf
	};
	ioctl(mem, IOCTL_WRITE_MEM, (void *)(&params));

	return params.return_val;
}

int read_mem(int mem, int ref, char *buf, int size){
	struct read_mem_struct params ={
		.mem=mem,
		.ref=ref,
		.buf=buf,
		.size = size	
	};
	
	ioctl(mem, IOCTL_READ_MEM, (void *)(&params));

	return params.return_val;

}
