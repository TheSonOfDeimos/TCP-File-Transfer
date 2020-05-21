#ifndef DETAIL
#define DETAIL

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

#include "config.h"

int LoadConfig(const char* FileName)
{
	return 1;
}

int ReloadConfig()
{
	return 1;
}

int SetFdLimit(int MaxFd)
{
	struct rlimit lim;
	int status;

	// зададим текущий лимит на кол-во открытых дискриптеров
	lim.rlim_cur = MaxFd;
	// зададим максимальный лимит на кол-во открытых дискриптеров
	lim.rlim_max = MaxFd;

	// установим указанное кол-во
	status = setrlimit(RLIMIT_NOFILE, &lim);

	return status;
}

void SetPidFile(const char* Filename)
{
	FILE* f;
	f = fopen(Filename, "w+");
	if (f)
	{
		fprintf(f, "%u", getpid());
		fclose(f);
	}
}

#ifdef __APPLE__

int sigwaitinfo(const sigset_t *set, siginfo_t *info)
{
    int sig = -1;

    if ( sigwait(set, &sig) < 0 )
        return -1;

    return sig;
}
#endif

#endif // DEFINE