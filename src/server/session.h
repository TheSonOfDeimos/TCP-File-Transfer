#ifndef SESSION
#define SESSION

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

class Session
{
public:
    Session(boost::asio::io_service &io_service);
    boost::asio::ip::tcp::socket &socket();
    void start();

private:
    void readHandle(const boost::system::error_code &error, size_t bytes_transferred);
    void writeHandle(const boost::system::error_code &error);

    boost::asio::ip::tcp::socket m_socket;
    boost::asio::streambuf m_buff;
};

#endif