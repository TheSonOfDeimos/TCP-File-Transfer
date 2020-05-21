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

#include "log_writer.h"

static void signal_error(int sig, siginfo_t *si, void *ptr)
{
	void* ErrorAddr;
	void* Trace[16];
	int x;
	int TraceSize;
	char** Messages;

	// запишем в лог что за сигнал пришел
	WriteLog("[DAEMON] Signal: %s, Addr: 0x%0.16X", strsignal(sig), si->si_addr);

	// #if __WORDSIZE == 64 // если дело имеем с 64 битной ОС
	// 	// получим адрес инструкции которая вызвала ошибку
	// 	ErrorAddr = (void*)((ucontext_t*)ptr)->uc_mcontext.gregs[REG_RIP];
	// #else
	// 	// получим адрес инструкции которая вызвала ошибку
	// 	ErrorAddr = (void*)((ucontext_t*)ptr)->uc_mcontext.gregs[REG_EIP];
	// #endif

	// // произведем backtrace чтобы получить весь стек вызовов
	// TraceSize = backtrace(Trace, 16);
	// Trace[1] = ErrorAddr;

	// // получим расшифровку трасировки
	// Messages = backtrace_symbols(Trace, TraceSize);
	// if (Messages)
	// {
	// 	WriteLog("== Backtrace ==");

	// 	// запишем в лог
	// 	for (x = 1; x < TraceSize; x++)
	// 	{
	// 		WriteLog("%s\n", Messages[x]);
	// 	}

	// 	WriteLog("== End Backtrace ==");
	// 	free(Messages);
	// }

	// WriteLog("[DAEMON] Stopped");

	// остановим все рабочие потоки и корректно закроем всё что надо

	// завершим процесс с кодом требующим перезапуска
	exit(CHILD_NEED_WORK);
}

#endif // SIGNAL_ERROR_WORKER