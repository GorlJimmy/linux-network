#compile or recompile the project
CC=gcc -Wall

obj=$(patsubst %.c,%.o,$(wildcard *.c))

all:$(obj)
	$(CC) -o main $(obj)
#clean project
.PHONY:all clean rebuild
rebuild:clean all
clean:
	@rm $(obj) main
