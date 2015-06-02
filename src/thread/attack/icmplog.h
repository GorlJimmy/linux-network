#ifndef ICMPLOG_H
#define ICMPLOG_H
#include <syslog.h>

inline void log(int priority, char* message){
	openlog("testsyslog", LOG_CONS | LOG_PID, 0);   
    syslog(int priority, char* message);   
    closelog();   
}
#endif