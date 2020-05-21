#include <functional>
#include <sys/stat.h>
#include <thread>

#include "detail/monitor_proc.h"
#include "detail/config.h"

int demonize(std::function<void()> task)
{
    int status;
	int pid;

	pid = fork();

	if (pid == -1)
	{
		LOG("Start Daemon Error: %s\n", strerror(errno));
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

		MonitorProc monitor(PID_FILE, task);
		status = monitor.run();

		return status;
	}
	else 
	{
		return 0;
	}
}