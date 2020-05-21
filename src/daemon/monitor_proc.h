#ifndef MONITOR_PROC
#define MONITOR_PROC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <execinfo.h>
#include <unistd.h>
#include <errno.h>
#include <string>

class MonitorProc
{
public:
	MonitorProc(std::string pid_file_path, std::function<void()> task);
	~MonitorProc();
    int run();
private:

	int processSignal();
	

    int m_pid;
    int m_status;
    int m_need_start;
    sigset_t m_sigset;
    int m_siginfo;
	std::string m_pid_file_path;

    std::function<void()> m_task;
};

#endif
