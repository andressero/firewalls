#include "clientsocket.h"

ClientSocket::ClientSocket(short port, std::string address) {
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

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(3000);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    this->connect(serverAddress);

}

int ClientSocket::connect(sockaddr_in& address) {
    return ::connect(this->fileDescriptor, (struct sockaddr*)&address, sizeof(address));
}

int ClientSocket::send(std::string message) {
    return ::send(this->fileDescriptor, message.c_str(), message.size(), 0);
}

std::string ClientSocket::receive() {
    char* buffer = (char*) calloc(1024, sizeof(char));
    ::recv(this->fileDescriptor, buffer, 1024, 0);
    return std::string(buffer);
}
