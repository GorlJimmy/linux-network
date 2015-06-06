#include "operator.h"

int create_file(char *file_name){


}

int open_file(char *file_name){

	int fd=-1;
    if((fd=open(file_path,O_RDWR) < 0){
		perror("open failed");
        exit(1);
    }
    return fd;
}


int read_file(int fd,void *buffer,size_t count){
	
	if (fd < 0){
		perror("read file error!\n");
		exit(1);
	}
	while(true){
		int size = read(fd,buffer,count);
		if(size < 0 ) break;

	}

	printf("Read done!\n");
	return 0;
}



int write_file(int fd,void *buffer,size_t count){
	if (fd < 0){
		perror("write file error\n");
		exit(1);
	}
	while(true){
		int size = write(fd, buffer, sizeof(buffer));
		if( size < 0 )break;
	}
	printf("write done\n");

}


int seek_file(int fd,unsigned int u_offset ){

}


int close_file(int fd){
	close(fd);
}

