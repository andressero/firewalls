// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>"

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class Socket {
public:
  Socket();
  ~Socket();

  // Create a socket
  bool create();

  // Bind the socket to a port
  bool bind(const int port, const std::string address);

  // Listen for incoming connections
  bool listen() const;

  // Accept a new connection
  bool accept(Socket &newSocket) const;

  // Connect to a remote server
  bool connect(const std::string &host, const int port);

  // Send a message
  bool send(const std::string &message) const;

  // Receive a message
  int recv(std::string &message) const;

  // Set socket to non-blocking mode
  void set_non_blocking(const bool);

private:
  int m_sock;
  sockaddr_in m_addr;

  // Check if socket is valid
  bool is_valid() const;
};

#endif
