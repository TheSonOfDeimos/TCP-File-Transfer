#include "session.h"

Session::Session(boost::asio::io_service &io_service)
    : m_socket(io_service)
{
}

boost::asio::ip::tcp::socket& Session::socket()
{
    return m_socket;
}

void Session::start()
{
    boost::asio::async_read_until(socket(), m_buff, '\n',
                            boost::bind(&Session::readHandle, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void Session::readHandle(const boost::system::error_code &error, size_t bytes_transferred)
{
    if (!error)
    {
        std::string str((std::istreambuf_iterator<char>(&m_buff)), std::istreambuf_iterator<char>());
        std::cout << "[SERVER " << boost::this_thread::get_id() << "] Got message:" << str << "\n";
        boost::asio::async_write(m_socket,
                                 boost::asio::buffer(str),
                                 boost::bind(&Session::writeHandle, this,
                                             boost::asio::placeholders::error));
    }
    else
    {
        delete this;
    }
}

void Session::writeHandle(const boost::system::error_code &error)
{
    if (!error)
    {
        delete this;
        // socket_.async_read_some(boost::asio::buffer(data_, max_length),
        //                         boost::bind(&session::handle_read, this,
        //                                     boost::asio::placeholders::error,
        //                                     boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        delete this;
    }
}
