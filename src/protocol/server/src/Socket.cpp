// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"

#include "Socket.hpp"
#include <chrono>    // For std::chrono::seconds
#include <thread>    // For std::this_thread::sleep_for

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


  struct timeval timeout;      
  timeout.tv_sec = 2;
  timeout.tv_usec = 0;

  if (setsockopt (this->serverFileDescriptor, SOL_SOCKET, SO_RCVTIMEO, &timeout,
              sizeof(timeout)) < 0) {
    ERROR("setsockopt failed\n");
  }
  if (setsockopt (this->serverFileDescriptor, SOL_SOCKET, SO_SNDTIMEO, &timeout,
              sizeof(timeout)) < 0) {
    ERROR("setsockopt failed\n");
  }
}

Socket::~Socket() {
  ::close(this->serverFileDescriptor);
}

int Socket::bind() {
  int bindStatus = 0;
  while (true) {
    bindStatus = ::bind(this->serverFileDescriptor, (struct sockaddr*)&this->serverAddress, sizeof(this->serverAddress));
    if (bindStatus == -1) {
      ERROR("Error binding socket");
    } else {
      break;
    }
    std::cout << "Waiting for 5 seconds..." << std::endl;

    // Wait for 5 seconds
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }
  return bindStatus;
}

int Socket::listen(int requests) {
  int listenStatus = ::listen(this->serverFileDescriptor, requests); // the 5 means the amount of request it'll allow
  if (listenStatus == -1) {
    ERROR("Error listening on socket");
  }
  return listenStatus;
}

int Socket::accept() {
  this->clientFileDescriptor = ::accept(this->serverFileDescriptor, nullptr, nullptr);
  if (this->clientFileDescriptor == -1) {
    ERROR("Error accepting connection");
  }
  return this->clientFileDescriptor;
}

int Socket::connect(sockaddr_in address) {
  int connectStatus = ::connect(this->serverFileDescriptor, (struct sockaddr*)&address, sizeof(address));
  if (connectStatus == -1) {
    ERROR("Error connecting to server");
  }
  return connectStatus;
}

int Socket::send(int toFileDescriptor, std::string message) {
  int sendStatus = ::send(toFileDescriptor, message.c_str(), message.size(), 0);
  if (sendStatus == -1) {
    ERROR("Error sending message");
  }
  return sendStatus;
}

std::string Socket::receive(int fromFileDescriptor) {
  const int MAX_BUFFER_SIZE = 1024;
  char* buffer = new char[MAX_BUFFER_SIZE];
  memset(buffer, 0, MAX_BUFFER_SIZE); // fill buffer with 0s (null-terminator
  recv(fromFileDescriptor, buffer, MAX_BUFFER_SIZE, 0);
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