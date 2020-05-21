#include "log_writer.h"

#include <execinfo.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <time.h>

void WriteLog(std::string file_path, const char* Msg, ...)
{
    FILE* f_ptr = fopen(file_path.c_str(), "a");
    if (f_ptr != NULL) {

        char buff[20];
        struct tm *sTm;
        time_t now = time (0);
        sTm = gmtime (&now);
        strftime (buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", sTm);
        fprintf(f_ptr, "%s  ", buff);

        va_list args;
        va_start(args, Msg);
        vfprintf(f_ptr, Msg, args);
        va_end(args);
        fprintf(f_ptr, "\n");
        fflush(f_ptr);
    }
    fclose(f_ptr); 
}