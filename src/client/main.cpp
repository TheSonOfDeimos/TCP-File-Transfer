#include <iostream>
#include "client.hpp"

int main(int argc, char *argv[])
{
    try
    {
        boost::asio::io_service io_service;
        Client client(io_service, "localhost", 8888);
        io_service.run();

        std::string file_to_send = "/Users/levkargalov/Documents/Projects/Programming/C++/TCP-File-Transfer/test_files/input.txt";
        client.send(file_to_send);
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}