#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <arpa/inet.h>

class Socket {
 private:
  int fileDescriptor;
  sockaddr_in address;

 public:

  Socket(short port, std::string address);
  Socket(int fileDescriptor, sockaddr_in address);
  ~Socket();

  int bind();
  int listen(int requests);
  Socket accept();
  int send(std::string message);
  std::string receive(Socket sender);
  int connect(sockaddr_in address);

  int getFileDescriptor();
  sockaddr_in getSocketAddress();
  std::string getIPAddress();
};

#endif
