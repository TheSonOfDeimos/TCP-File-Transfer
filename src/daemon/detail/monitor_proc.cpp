#include "monitor_proc.h"

#include <sys/wait.h>

#include "config.h"
#include "daemon_body.h"
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

    sigaddset(&m_sigset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &m_sigset, NULL);

    SetPidFile(m_pid_file_path.c_str());

    LOG("[MONITOR] Started");
}

MonitorProc::~MonitorProc()
{
    LOG("[MONITOR] Stopped");
    unlink(m_pid_file_path.c_str());
}

int MonitorProc::processSignal()
{
    if (m_siginfo.si_signo == SIGCHLD)
    {
        wait(&m_status);
        m_status = WEXITSTATUS(m_status);
        if (m_status == CHILD_NEED_TERMINATE)
        {
            LOG("[MONITOR] Childer stopped");
            return -1;
        }
        else if (m_status == CHILD_NEED_WORK)
        {
            LOG("[MONITOR] Childer restart");
            sleep(RESTART_TIMEOUT);
        }
    }

    else if (m_siginfo.si_signo == SIGUSR1)
    {
        kill(m_pid, SIGUSR1);
        m_need_start = 0;
    }
    else
    {
        LOG("[MONITOR] Signal %s", strsignal(m_siginfo.si_signo));
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
            LOG("[MONITOR] Fork failed (%s)", strerror(errno));
        }
        else if (!m_pid)
        {
            WorkProc newProc(m_task);
            m_status = newProc.run();
            exit(m_status);
        }
        else
        {
            sigwaitinfo(&m_sigset, &m_siginfo);
            if (processSignal() == -1) {
                break;
            }
        }
    }

    return m_status;
}



