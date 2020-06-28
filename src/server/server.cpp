#include "server.hpp"

#include <iostream>
#include <boost/bind.hpp>
#include <sys/mman.h>

Session::Session(boost::asio::io_service &t_io_service, const std::string& t_file_dir_path)
    : m_socket(t_io_service),
    m_file_dir_path(t_file_dir_path)
{
}

boost::asio::ip::tcp::socket &Session::socket()
{
    return m_socket;
}

void Session::start()
{
    std::clog << "[INFO] Client session started\n";

    m_socket.async_read_some(boost::asio::buffer(m_file_size, sizeof(std::size_t)), 
                            boost::bind(&Session::handleReadFileSize, 
                            this, 
                            boost::asio::placeholders::error, 
                            boost::asio::placeholders::bytes_transferred));
}

void Session::handleReadFileSize(const boost::system::error_code &t_error, const size_t t_bytes_transferred)
{
    if (!t_error)
    {
        std::clog << "[INFO] Got new file size: " << m_file_size << "\n";
        mapNewFile(std::atoi(m_file_size));

        m_socket.async_read_some(boost::asio::buffer(m_mapped_file_ptr, std::atoi(m_file_size)), 
                            boost::bind(&Session::handleReadFileData, 
                            this, 
                            boost::asio::placeholders::error, 
                            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        std::cerr << "[ERROR] Reading message error\n";
        delete this;
    }
}

void Session::handleReadFileData(const boost::system::error_code &t_error, const size_t t_bytes_transferred)
{
    if (!t_error)
    {
        std::clog << "[INFO] Got file data\n";
        delete this;
    }
    else
    {
        std::cerr << "[ERROR] Reading message error\n";
        delete this;
    }
}

void Session::mapNewFile(const std::size_t t_file_size)
{
    std::string file_name_template = m_file_dir_path + "/XXXXXX";
    std::string file_name (mktemp(const_cast<char*>(file_name_template.data())));

    int file_out;
    if ((file_out = open(file_name.c_str(), O_RDWR | O_CREAT)) < 0) {
        std::cerr << "[ERROR] Can't create file\n";
    }

    if (lseek(file_out, t_file_size - 1, SEEK_SET) == -1) {
        std::cerr << "[ERROR] Can't lseek file\n";
    }

    if (write(file_out, "", 1) != 1) {
        std::cerr << "[ERROR] Can't write to file\n";
    }

    void* file_mapping_ptr;
    if ((file_mapping_ptr = mmap(0, t_file_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_out, 0)) == MAP_FAILED) {
        std::cerr << "[ERROR] Can't map file\n";
    }

    m_mapped_file_ptr = static_cast<char*>(file_mapping_ptr);

    std::clog << "[INFO] New file " << file_name << " successfully mapped\n";
}

Server::Server(boost::asio::io_service &t_io_service, const std::string& t_file_dir_path, std::size_t t_port, const boost::asio::ip::tcp &t_protocol_type)
    : m_io_service(t_io_service),
      m_acceptor(t_io_service, boost::asio::ip::tcp::endpoint(t_protocol_type, t_port)),
      m_file_dir_path(t_file_dir_path)
{
    startAccept();
    std::clog << "[INFO] Server started on port " << t_port << "\n";
}

void Server::startAccept()
{
    Session *new_session = new Session(m_io_service, m_file_dir_path);
    m_acceptor.async_accept(new_session->socket(), boost::bind(&Server::handleAccept, this, new_session, boost::asio::placeholders::error));
}

void Server::handleAccept(Session *t_new_session, const boost::system::error_code &t_error)
{
    if (!t_error) {   
        std::clog << "[INFO] New connection on " << t_new_session->socket().remote_endpoint().address().to_string() << ":" 
                << t_new_session->socket().remote_endpoint().port() << "\n";

        t_new_session->start();
    }
    else {
        std::cerr << "[ERROR] New connection error\n";
        delete t_new_session;
    }

    startAccept();
}
