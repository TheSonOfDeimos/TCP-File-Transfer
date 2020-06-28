#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>

class Client
{
public:
    Client(boost::asio::io_service& t_io_service, const std::string t_host, const std::size_t t_port, const boost::asio::ip::tcp& t_protocol_type = boost::asio::ip::tcp::v4());

    void send(const std::string& t_file_name);

private:
    void send_file_content(std::string_view t_data_to_send);
    void send_file_size(const std::size_t t_file_size);

    boost::asio::ip::tcp::socket m_socket;
    boost::asio::io_service& m_io_service;
};

#endif