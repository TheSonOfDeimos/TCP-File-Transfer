#ifndef SERVER
#define SERVER


#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "session.h"

using boost::asio::ip::tcp;

class Server
{
public:
  Server(boost::asio::io_service& io_service, short port);

private:
  void startAccept();
  void acceptHandle(Session* new_session,const boost::system::error_code& error);

  boost::asio::io_service& m_io_service;
  boost::asio::ip::tcp::acceptor m_acceptor;
};

#endif