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
#include <arpa/inet.h>

class Socket {
 private:
  int serverFileDescriptor;
  int clientFileDescriptor;

  sockaddr_in serverAddress;
  sockaddr_in clientAddress;

 public:
  Socket(short port, std::string address);
  Socket(int fileDescriptor/*, sockaddr_in address*/);
  ~Socket();

  int bind();
  int listen(int requests);
  int accept();
  int connect(sockaddr_in address);
  int send(int fileDescriptor, std::string message);
  std::string receive(int fileDescriptor);
  int close();

  int getServerFileDescriptor();
  int getClientFileDescriptor();
  sockaddr_in getServerAddress();

  std::string getIPAddress();
};

#endif