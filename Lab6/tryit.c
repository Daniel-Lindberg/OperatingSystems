//Daniel Lindberg
// 10/30/2015
//try it function for lab6

#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
	int fd;
	fd = open("/dev/interface",O_RDWR);
	if(fd== -1){
		exit(-1);	
	}
	if(argc == 3) len = strlen(argv[2]); else len=0;
	
	if(strcmp(argv[1],"read")==0){
		read(fd, read_buf, sizeof(read_buf));
		printf("%s",read_buf);
	}
	if(strcmp(argv[1],"write")==0){
		strcpy(write_buf,argv[2], len);
		write(fd, write_buf, len);
		printf("%s",write_buf);				
	}	
	close(fd);
return 0;
}
