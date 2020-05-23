#include <iostream>
#include <boost/thread/thread.hpp>
#include "server.h"

#define THREAD_COUNT 4

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 2) {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        using namespace std; // For atoi.
        Server s(io_service, atoi(argv[1]));

        boost::thread_group threads;
        for (int i = 0; i < THREAD_COUNT; ++i)
        {
            threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
        }
        threads.join_all();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
