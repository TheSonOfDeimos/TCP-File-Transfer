#include "server.h"

Server::Server(boost::asio::io_service &io_service, short port)
    : m_io_service(io_service),
      m_acceptor(io_service, tcp::endpoint(tcp::v4(), port))
{
    startAccept();
}

void Server::startAccept()
{
    Session *new_session = new Session(m_io_service);
    m_acceptor.async_accept(new_session->socket(),
                           boost::bind(&Server::acceptHandle, this, new_session,
                                       boost::asio::placeholders::error));
}

void Server::acceptHandle(Session *new_session, const boost::system::error_code &error)
{
    if (!error)
    {
        new_session->start();
    }
    else
    {
        delete new_session;
    }

    std::cout << "[SERVER " << boost::this_thread::get_id() << "] New acceptor \n";
    startAccept();
}