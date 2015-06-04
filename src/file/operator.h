#ifndef OPERTOR_H

#define OPERTOR_H
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

int create_file(char *file_name);

int open_file(char *file_name,int style);

int read_file(int fd,void *buffer,size_t count);

int write_file(int fd,void *buffer,size_t count);

int seek_file(int fd);


int close(int fd);

#endif