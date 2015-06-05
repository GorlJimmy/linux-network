#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define BACKLOG 4



int main(int argc, char const *argv[])
{
	register int s, c;
	int b;
	struct sockaddr_in sa;
	time_t t;
	struct tm *tm;
	FILE *client;
 if ((s=socket(PF_INET,SOCK_STREAM,0))<0){
	perror("socket error!\n");
	exit(1);
}
memset(&sa,0, sizeof(sa));
sa.sin_family=AF_INET;
sa.sin_port=htons(13);
if(INADDR_ANY){
	sa.sin_addr.s_addr=htons(INADDR_ANY);
}
if(bind(s,(struct sockaddr *)&sa, sizeof(sa))){
	perror("bind\n");
	exit(1);
}

switch (fork()) {
        case -1:
            perror("fork");
            return 3;
            break;
        default:
            close(s);
            return 0;
            break;
        case 0:
            break;
    }

    listen(s, BACKLOG);

    for (;;) {
        b = sizeof sa;
        if ((c = accept(s, (struct sockaddr *)&sa, &b)) < 0) {
            perror("daytimed accept");
            return 4;
        }

        if ((client = fdopen(c, "w")) == NULL) {
            perror("daytimed fdopen");
            return 5;
        }

        if ((t = time(NULL)) < 0) {
            perror("daytimed time");

            return 6;
        }

        tm = gmtime(&t);
        fprintf(client, "%.4i-%.2i-%.2iT%.2i:%.2i:%.2iZ\n",
            tm->tm_year + 1900,
            tm->tm_mon + 1,
            tm->tm_mday,
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec);

        fclose(client);
    }

	return 0;
}