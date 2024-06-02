#include "socket.h"

Socket::~Socket() {
    ::close(this->fileDescriptor);
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
