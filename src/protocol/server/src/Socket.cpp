// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"

#include "Socket.hpp"

Socket::Socket(short port = 8080, std::string address = "0.0.0.0") {
  this->serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

  // specifying the address
  // Server address as attribute
  /*
  */
  this->serverAddress.sin_family = AF_INET;
  this->serverAddress.sin_port = htons(port);
  this->serverAddress.sin_addr.s_addr = INADDR_ANY;

  if (address == "0.0.0.0") {
    this->serverAddress.sin_addr.s_addr = INADDR_ANY;
  } else {
    this->serverAddress.sin_addr.s_addr = inet_addr(address.c_str());
  }

  // Server address as local variable
  /*
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8080);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  if (address == "0.0.0.0") {
    serverAddress.sin_addr.s_addr = INADDR_ANY;
  } else {
    serverAddress.sin_addr.s_addr = inet_addr(address.c_str());
  }
  */
}

Socket::Socket(int fileDescriptor/*, sockaddr_in address*/) : serverFileDescriptor(fileDescriptor) {
  // this->serverAddress = address;
}

Socket::~Socket() {
  ::close(this->serverFileDescriptor);
}

int Socket::bind() {
  return ::bind(this->serverFileDescriptor, (struct sockaddr *)&this->serverAddress, sizeof(this->serverAddress));
}

int Socket::listen(int requests) {
  return ::listen(this->serverFileDescriptor, requests); // the 5 means the amount of request it'll allow
}

int Socket::accept() {
  this->clientFileDescriptor = ::accept(this->serverFileDescriptor, nullptr, nullptr);
  return 0;
}

int Socket::connect(sockaddr_in address) {
  return ::connect(this->serverFileDescriptor, (struct sockaddr*)&address, sizeof(address));
}

int Socket::send(int toFileDescriptor, std::string message) {
  return ::send(toFileDescriptor, message.c_str(), message.size(), 0);
}

std::string Socket::receive(int fromFileDescriptor) {
  char* buffer = new char[1024];
  recv(fromFileDescriptor, buffer, sizeof(buffer), 0);
  std::string message(buffer);
  return message;
}

int Socket::close() {
  return ::close(this->serverFileDescriptor);
}

int Socket::getServerFileDescriptor() {
  return this->serverFileDescriptor;
}

int Socket::getClientFileDescriptor() {
  return this->clientFileDescriptor;
}

std::string Socket::getIPAddress() {
  return inet_ntoa(this->serverAddress.sin_addr);
}

sockaddr_in Socket::getServerAddress() {
  return this->serverAddress;
}