#if !defined(_GNU_SOURCE)
	#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <execinfo.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "daemon_body.h"
#include "monitor_proc.h"
#include "log_writer.h"

int main(int argc, char** argv)
{
	int status;
	int pid;

	pid = fork();

	if (pid == -1)
	{
		printf("Start Daemon Error: %s\n", strerror(errno));
		return -1;
	}
	else if (!pid)
	{
		umask(0);
		setsid();
		chdir("/");

		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		MonitorProc monitor("/Users/levkargalov/Documents/Projects/Programming/C++/TCP-File-Transfer/build/my_daemon.pid", [](){
			while(1) 
			{
				WriteLog("[TASK]", "Hello"); 
				std::this_thread::sleep_for(std::chrono::seconds(5));
			}
			});
		status = monitor.run();

		return status;
	}
	else 
	{
		return 0;
	}
}

