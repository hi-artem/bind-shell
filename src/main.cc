#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <unistd.h>

using namespace boost::process;
using boost::asio::ip::tcp;

const int max_length = 1024;

void session(tcp::socket sock)
{
  try
  {
    dup2(sock.native_handle(), 0);
    dup2(sock.native_handle(), 1);
    dup2(sock.native_handle(), 2);
    
    ipstream pipe_stream;
    child c("/bin/bash", std_out > pipe_stream);

    std::string line;

    while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
        std::cerr << line << std::endl;

    c.wait();

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_context& io_context, unsigned short port)
{
  tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    std::thread(session, a.accept()).detach();
  }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    server(io_context, std::atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
