// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "dbServerUtils.hpp"

class Socket {
private:
  int serverFileDescriptor;
  int clientFileDescriptor;

  sockaddr_in serverAddress;
  sockaddr_in clientAddress;
  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

public:
  Socket(short port, std::string address);
  Socket(int fileDescriptor /*, sockaddr_in address*/);
  ~Socket();

  static Socket &getInstance() {
    static Socket instance(8080,
                           "0.0.0.0"); // Guaranteed to be initialized only once
    return instance;
  }

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

  // Signal handler function
  static void signalHandler(int signal) {
    std::cout << "Socket received signal " << signal
              << ". Releasing resources..." << std::endl;
    Socket &instance = Socket::getInstance();
    instance.close();
  }
};

#endif