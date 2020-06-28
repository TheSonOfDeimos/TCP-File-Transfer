#include <iostream>
#include <boost/thread/thread.hpp>
#include "server.hpp"

int main(int argc, char *argv[])
{
    try
    {
        std::string file_dir =  "/Users/levkargalov/Documents/Projects/Programming/C++/TCP-File-Transfer/test_files";

        boost::asio::io_service io_service;
        Server server(io_service, file_dir, 8888);
        io_service.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
