#include "Socket.hpp"

Socket::Socket(short port = 8080, std::string address = "0.0.0.0") {
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

}

Socket::Socket(int fileDescriptor, sockaddr_in address): 
  fileDescriptor(fileDescriptor),
	address(address) {
}

Socket::~Socket() {
  ::close(this->fileDescriptor);
}

int Socket::bind() {
	return ::bind(this->fileDescriptor, (struct sockaddr*)&this->address, sizeof(this->address));
}

int Socket::listen(int requests) {
	return ::listen(this->fileDescriptor, requests);
}

Socket Socket::accept() {
	sockaddr_in acceptedAddress;
	socklen_t acceptedAddressLength = sizeof(acceptedAddress);

	int acceptedFileDescriptor = ::accept(this->fileDescriptor, (struct sockaddr*)&acceptedAddress, &acceptedAddressLength);

	if (acceptedFileDescriptor == -1) {
		// throw error or empty socket??
	}
	return Socket(acceptedFileDescriptor, acceptedAddress);
}

int Socket::send(std::string message) {
	return ::send(this->fileDescriptor, message.c_str(), message.size(), 0);
}

std::string Socket::receive(Socket sender) {
	// char* buffer = new char[1024];
	std::string message(1024, '\0');
	::recv(sender.getFileDescriptor(), &message[0], 1024, 0);
	return message;
}

int Socket::connect(sockaddr_in address) {
	return ::connect(this->fileDescriptor, (struct sockaddr*)&address, sizeof(address));
}

int Socket::getFileDescriptor() {
	return this->fileDescriptor;
}

sockaddr_in Socket::getSocketAddress() {
	return this->address;
}

std::string Socket::getIPAddress() {
	return std::string(::inet_ntoa(this->address.sin_addr));
}