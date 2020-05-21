#ifndef DAEMON_BODY
#define DAEMON_BODY

#include <thread>
#include <functional>
#include <signal.h>

class WorkProc
{
public:

	WorkProc(std::function<void()> task);
	~WorkProc();
	int run();

private:

	struct sigaction m_sigact;
	sigset_t m_sigset;
	int m_signo;
	std::thread m_task_worker;
	std::function<void()> m_task;
};

#endif