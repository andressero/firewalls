#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>

class Socket {
public:
  Socket(const std::string &ip, int port);
  bool connect();
  void send(const std::string &message);
  std::string receive();
  void close();

private:
  std::string ip;
  int port;
  int sockfd;
};

#endif // SOCKET_HPP
