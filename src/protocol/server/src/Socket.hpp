// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <cstring>
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

class Socket {
 private:
  int socketDescriptor;
  int connectionDescriptor;

 public:
  Socket();
  ~Socket();
  int listen();
  int accept();
  int send(std::string message);
  std::string receive();
  int close();
};

#endif