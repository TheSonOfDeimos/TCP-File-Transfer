#include "monitor_proc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <execinfo.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include "config.h"
#include "daemon_body.h"
#include "log_writer.h"
#include "detail.h"

MonitorProc::MonitorProc(std::string pid_file_path, std::function<void()> task)
    : m_need_start(1),
    m_pid_file_path(pid_file_path),
    m_task(task)
{
    sigemptyset(&m_sigset);
    sigaddset(&m_sigset, SIGQUIT);
    sigaddset(&m_sigset, SIGINT);
    sigaddset(&m_sigset, SIGTERM);
    sigaddset(&m_sigset, SIGCHLD);

    // пользовательский сигнал который мы будем использовать для обновления конфига
    sigaddset(&m_sigset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &m_sigset, NULL);

    SetPidFile(m_pid_file_path.c_str());

    WriteLog("[MONITOR] Started");
}

MonitorProc::~MonitorProc()
{
    WriteLog("[MONITOR] Stopped");
    unlink(m_pid_file_path.c_str());
}

int MonitorProc::processSignal()
{
    if (m_siginfo == SIGCHLD)
    {
        wait(&m_status);

        // преобразуем статус в нормальный вид
        m_status = WEXITSTATUS(m_status);

        // если потомок завершил работу с кодом говорящем о том, что нет нужны дальше работать
        if (m_status == CHILD_NEED_TERMINATE)
        {
            WriteLog("[MONITOR] Childer stopped");
            return -1;
        }
        else if (m_status == CHILD_NEED_WORK) // если требуется перезапустить потомка
        {
            WriteLog("[MONITOR] Childer restart");
            sleep(RESTART_TIMEOUT);
        }
    }

    else if (m_siginfo == SIGUSR1)
    {
        kill(m_pid, SIGUSR1);
        m_need_start = 0;
    }
    else
    {
        WriteLog("[MONITOR] Signal %s", strsignal(m_siginfo));
        kill(m_pid, SIGTERM);
        m_status = 0;
        return -1;
    }

    return 0;
}

int MonitorProc::run()
{
    for (;;)
    {
        if (m_need_start)
        {
            m_pid = fork();
        }

        m_need_start = 1;

        if (m_pid == -1)
        {
            WriteLog("[MONITOR] Fork failed (%s)", strerror(errno));
        }
        else if (!m_pid)
        {
            //WorkProc newProc(m_task);
           // m_status = newProc.run();
            exit(CHILD_NEED_TERMINATE);
        }
        else
        {
            sigwait(&m_sigset, &m_siginfo);
            WriteLog("[MONITOR] Got Signal");
            if (processSignal() == -1) {
                break;
            }
        }
    }

    return m_status;
}



