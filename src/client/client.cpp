#include "client.hpp"

#include <iostream>
#include <sys/mman.h>

Client::Client(boost::asio::io_service &t_io_service, const std::string t_host, const std::size_t t_port, const boost::asio::ip::tcp& t_protocol_type)
    : m_io_service(t_io_service),
    m_socket(t_io_service)
{
    using boost::asio::ip::tcp;

    tcp::resolver resolver(m_io_service);
    tcp::resolver::query query(t_protocol_type, t_host, std::to_string(t_port));
    tcp::resolver::iterator iterator = resolver.resolve(query);

    boost::system::error_code error = boost::asio::error::host_not_found;
    m_socket.connect(*iterator, error);

    if (error) {
        throw boost::system::system_error(error);
    }

    std::clog << "[INFO] Connection success\n";
}

void Client::send(const std::string &t_file_name)
{
    int file_d;
    void* file_data_ptr;
    struct stat statbuf;

    if ((file_d = open(t_file_name.c_str(), O_RDWR)) < 0) {
        std::cerr << "[ERROR] Can't open file for sending\n";
    }

    if (fstat(file_d, &statbuf) < 0) {
        std::cerr << "[ERROR] Can't get fstat of file for sending\n";
    }

    if ((file_data_ptr = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_d, 0)) == MAP_FAILED) {
        std::cerr << "[ERROR] Can't map file for sending\n";
    }

    send_file_size(statbuf.st_size);
    send_file_content(std::string_view(static_cast<char*>(file_data_ptr)));
}

void Client::send_file_content(std::string_view t_data_to_send)
{
    boost::system::error_code error;
    m_socket.write_some(boost::asio::buffer(t_data_to_send.data(), t_data_to_send.size()), error);
    if (error) {
        std::cerr << "[ERROR] Error ocurred while sending file data\n";
    }

    std::clog << "[INFO] Sending file data success\n";
}

void Client::send_file_size(const std::size_t t_file_size)
{
    boost::system::error_code error;

    auto file_size_str = std::to_string(t_file_size);
    m_socket.write_some(boost::asio::buffer(file_size_str.c_str(), file_size_str.size()), error);
    if (error) {
        std::cerr << "[ERROR] Error ocurred while sending file size\n";
    }

    std::clog << "[INFO] Sending file size success\n";
}