#ifndef CONFIG
#define CONFIG

#include "../../common/log_writer.h"

#define PID_FILE                        "/var/run/my_daemon.pid"
#define LOG_FILE			            "/home/deimos/projects/TCP-File-Transfer/build/Daemon.log"
#define CHILD_NEED_WORK	                1
#define CHILD_NEED_TERMINATE	        2
#define RESTART_TIMEOUT                 1 // in secods

#define LOG(LABEL, ...)                 WriteLog(LOG_FILE, LABEL __VA_OPT__(,) __VA_ARGS__)
//#define LOG(LABEL)                      WriteLog(LOG_FILE, LABEL)

#endif // CONFIG
