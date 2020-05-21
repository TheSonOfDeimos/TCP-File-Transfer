#ifndef MONITOR_PROC
#define MONITOR_PROC

#include <string.h>
#include <unistd.h>
#include <string>
#include <signal.h>
#include <functional>

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
    siginfo_t m_siginfo;
	std::string m_pid_file_path;
    std::function<void()> m_task;
};

#endif
