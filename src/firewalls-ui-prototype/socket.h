#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

class Socket {
protected:
    int fileDescriptor;
    sockaddr_in address;

public:

    ~Socket();

    virtual int send(std::string message) = 0;
    virtual std::string receive() = 0;

    int getFileDescriptor();
    sockaddr_in getSocketAddress();
    std::string getIPAddress();
};

#endif
