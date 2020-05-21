#include "daemon_body.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <execinfo.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>

#include "error_worker.h"
#include "log_writer.h"
#include "config.h"

WorkProc::WorkProc(std::function<void()> task)
	: m_task(task)
{
	sigact.sa_flags = SA_SIGINFO;
	sigact.sa_sigaction = signal_error;

	sigemptyset(&sigact.sa_mask);
	sigaction(SIGFPE, &sigact, 0); // ошибка FPU
	sigaction(SIGILL, &sigact, 0); // ошибочная инструкция
	sigaction(SIGSEGV, &sigact, 0); // ошибка доступа к памяти
	sigaction(SIGBUS, &sigact, 0); // ошибка шины, при обращении к физической памяти

	sigemptyset(&sigset);
	sigaddset(&sigset, SIGQUIT);
	sigaddset(&sigset, SIGINT);
	sigaddset(&sigset, SIGTERM);
	sigaddset(&sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigset, NULL);

	WriteLog("[DAEMON] Started");
}

WorkProc::~WorkProc()
{
	
	WriteLog("[DAEMON] Stopped");
}

int WorkProc::run()
{
	m_task_worker = std::thread(m_task);
	m_task_worker.detach();
	sigwait(&sigset, &signo);
	WriteLog("[DAEMON] Got Signak");
	return CHILD_NEED_TERMINATE;
}