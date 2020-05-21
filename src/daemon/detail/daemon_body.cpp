#include "daemon_body.h"
#include "error_worker.h"
#include "config.h"

WorkProc::WorkProc(std::function<void()> task)
	: m_task(task)
{
	m_sigact.sa_flags = SA_SIGINFO;
	m_sigact.sa_sigaction = signal_error;

	sigemptyset(&m_sigact.sa_mask);
	sigaction(SIGFPE, &m_sigact, 0);
	sigaction(SIGILL, &m_sigact, 0);
	sigaction(SIGSEGV, &m_sigact, 0);
	sigaction(SIGBUS, &m_sigact, 0);

	sigemptyset(&m_sigset);
	sigaddset(&m_sigset, SIGQUIT);
	sigaddset(&m_sigset, SIGINT);
	sigaddset(&m_sigset, SIGTERM);
	sigaddset(&m_sigset, SIGHUP);
	sigaddset(&m_sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &m_sigset, NULL);

	LOG("[DAEMON] Started");
}

WorkProc::~WorkProc()
{	
	LOG("[DAEMON] Stopped");
}

int WorkProc::run()
{
	m_task_worker = std::thread(m_task);
	m_task_worker.detach();
	sigwait(&m_sigset, &m_signo);
	return CHILD_NEED_TERMINATE;
}