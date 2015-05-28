#include <stdio.h>
#include <time.h>

void setDaemon(void);

main() 
{ 
FILE *fp; 
time_t t; 
setDaemon();

while(1)
{ 
sleep(60);
if((fp=fopen("test.log","a")) >=0) 
{ 
t=time(0); 
fprintf(fp,"Im here at %s/n",asctime(localtime(&t)) ); 
fclose(fp); 
} 
} 
} 
