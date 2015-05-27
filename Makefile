#compile or recompile the project
obj=main.o edit.o kbd.o core.o file.o

all:$(obj)
	cc -o main $(obj)
$(obj):common.h
edit.o:edit.h
kdb.o core.o file.o:def.h

include ./server/Makefile.mk


#clean project
.PHONY:clean
clean:
	rm $(obj)
