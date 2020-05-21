#ifndef DETAIL
#define DETAIL

#include <stdio.h>

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

#endif // DEFINE