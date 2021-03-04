#include <cstdlib>
#include <iostream>
#include <thread>
#include <string>
#include <utility>

#include <boost/asio.hpp>
#include <boost/process.hpp>

using boost::asio::ip::tcp;
using namespace boost::process;

const int max_length = 1024;

void session(tcp::socket sock)
{
  try
  {
    for (;;)
    {
      char data[max_length];

      boost::system::error_code error;
      size_t length = sock.read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.
     
      std::string cmd (data);
      cmd.pop_back();

      ipstream pipe_stream;
      child c(cmd, std_out > pipe_stream);

      std::string out = "";
      std::string line;

      while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
          out += line;
          out += "\n";

      c.wait();

      boost::asio::write(sock, boost::asio::buffer(out, out.size()));
    }
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
