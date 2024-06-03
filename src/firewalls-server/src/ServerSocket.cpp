#include "ServerSocket.hpp"

ServerSocket::ServerSocket(int port = 8080, std::string address = std::string("0.0.0.0")) {
  this->fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (this->fileDescriptor == -1) {
    std::cerr << "Error creating socket" << std::endl;
    return;
  }

  this->address.sin_family = AF_INET;
  this->address.sin_port = htons(port);
  if (address == "0.0.0.0") {
    this->address.sin_addr.s_addr = INADDR_ANY;
  } else {
    this->address.sin_addr.s_addr = inet_addr(address.c_str());
  }

  this->bind();
}

int ServerSocket::bind() {
	return ::bind(this->fileDescriptor, (struct sockaddr*)&this->address, sizeof(this->address));
}

int ServerSocket::listen(int& requests) {
	return ::listen(this->fileDescriptor, requests);
}

int ServerSocket::accept() {
	this->clientFileDescriptor = ::accept(this->fileDescriptor, nullptr, nullptr);
	return 0;
}

int ServerSocket::send(std::string message) {
	return ::send(this->clientFileDescriptor, message.c_str(), message.size(), 0);
}

std::string ServerSocket::receive() {
  char* buffer = new char[1024];
	::recv(this->clientFileDescriptor,buffer, 1024, 0);
	return std::string(buffer);
}