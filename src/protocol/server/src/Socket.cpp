// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"

#include "Socket.hpp"

Socket::Socket() {
  this->socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

  // specifying the address
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8080);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  // binding socket.
  bind(this->socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

  timeval val;
  val.tv_sec = 60;
  setsockopt(this->socketDescriptor, SOL_SOCKET, SO_RCVTIMEO, &val, sizeof(val));
}

Socket::~Socket() {}

int Socket::listen() {
  ::listen(this->socketDescriptor, 5); // the 5 means the amount of request it'll allow
}

int Socket::accept() {
  this->connectionDescriptor = ::accept(this->socketDescriptor, nullptr, nullptr);
}

int Socket::send(std::string message) {
  ::send(this->connectionDescriptor, message.c_str(), message.size(), 0);
}

std::string Socket::receive() {
  char* buffer = new char[1024];
  recv(this->connectionDescriptor, buffer, sizeof(buffer), 0);
  std::string message(buffer);
  return message;
}

int Socket::close() {
  ::close(this->socketDescriptor);
}