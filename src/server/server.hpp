#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

class Session
{
public:
  Session(boost::asio::io_service& t_io_service, const std::string& t_file_dir_path);
  
  boost::asio::ip::tcp::socket& socket();
  void start();
  
private:
  void handleReadFileSize(const boost::system::error_code& t_error, const std::size_t t_bytes_transferred);
  void handleReadFileData(const boost::system::error_code& t_error, const std::size_t t_bytes_transferred);

  void mapNewFile(const std::size_t t_file_size);

  boost::asio::ip::tcp::socket m_socket;
  char* m_mapped_file_ptr;
  char m_file_size[sizeof(std::size_t)];

  std::string m_file_dir_path;

};

class Server
{
public:
  Server(boost::asio::io_service& t_io_service, const std::string& t_file_dir_path, std::size_t t_port, const boost::asio::ip::tcp& t_protocol_type = boost::asio::ip::tcp::v4());
 
private:
  void startAccept();
  void handleAccept(Session* t_new_session, const boost::system::error_code& t_error);

  boost::asio::io_service& m_io_service;
  boost::asio::ip::tcp::acceptor m_acceptor;

  std::string m_file_dir_path;
};

#endif