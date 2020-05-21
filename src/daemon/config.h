#ifndef CONFIG
#define CONFIG

#define PORT_NAME                       "/dev/pts/2"
#define PORT_BAUDRATE                   75
#define PID_FILE                        "/var/run/my_daemon.pid"
#define LOG_FILE			            "/Users/levkargalov/Documents/Projects/Programming/C++/TCP-File-Transfer/build/Daemon.log"
#define FD_LIMIT			            1024*10 // лимит для установки максимально кол-во открытых дискрипторов
#define THREAD_COUNT                    4

#define CHILD_NEED_WORK	                1
#define CHILD_NEED_TERMINATE	        2

#define LIGHTER_DURATION                100 // in seconds
#define NUMBER_OF_LIGHTERS              3

#define LIGHTER_ADDRESS_T1              2
#define LIGHTER_ADDRESS_T2              3
#define LIGHTER_FIRE_COUNT              5

#define RESTART_TIMEOUT                 1 // in secods

#endif // CONFIG
