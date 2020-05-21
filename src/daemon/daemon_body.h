#ifndef DAEMON_BODY
#define DAEMON_BODY

#include <thread>

class WorkProc
{
public:

	WorkProc(std::function<void()> task);
	~WorkProc();
	int run();

private:
	

	struct sigaction sigact;
	sigset_t sigset;
	int signo;

	std::thread m_task_worker;
	std::function<void()> m_task;

};

#endif