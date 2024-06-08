#include "Socket.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

Socket::Socket(const std::string &ip, int port)
    : ip(ip), port(port), sockfd(-1) {}

bool Socket::connect() {
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cerr << "Error creating socket\n";
    return false;
  }

  struct sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
    std::cerr << "Invalid address or address not supported\n";
    return false;
  }

  if (::connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) <
      0) {
    std::cerr << "Connection Failed\n";
    return false;
  }

  return true;
}

void Socket::send(const std::string &message) {
  ::send(sockfd, message.c_str(), message.size(), 0);
}

std::string Socket::receive() {
  char buffer[1024] = {0};
  int valread = read(sockfd, buffer, 1024);
  return std::string(buffer, valread);
}

void Socket::close() { ::close(sockfd); }
