#ifndef SIGNAL_ERROR_WORKER
#define SIGNAL_ERROR_WORKER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <execinfo.h>
#include <unistd.h>
#include <errno.h>

#include "config.h"

static void signal_error(int sig, siginfo_t *si, void *ptr)
{
	void* ErrorAddr;
	void* Trace[16];
	int x;
	int TraceSize;
	char** Messages;

	LOG("[DAEMON] Signal: %s, Addr: 0x%0.16X", strsignal(sig), si->si_addr);

	#if __WORDSIZE == 64
		ErrorAddr = (void*)((ucontext_t*)ptr)->uc_mcontext.gregs[REG_RIP];
	#else
		ErrorAddr = (void*)((ucontext_t*)ptr)->uc_mcontext.gregs[REG_EIP];
	#endif

	TraceSize = backtrace(Trace, 16);
	Trace[1] = ErrorAddr;

	Messages = backtrace_symbols(Trace, TraceSize);
	if (Messages)
	{
		LOG("== Backtrace ==");
		for (x = 1; x < TraceSize; x++)
		{
			LOG("%s\n", Messages[x]);
		}

		LOG("== End Backtrace ==");
		free(Messages);
	}

	LOG("[DAEMON] Stopped");

	exit(CHILD_NEED_WORK);
}

#endif // SIGNAL_ERROR_WORKER