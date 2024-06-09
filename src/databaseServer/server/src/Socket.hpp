#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include <csignal>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class Socket {
public:
  static Socket &getInstance() {
    static Socket instance;
    return instance;
  }

  // Delete copy constructor and assignment operator to ensure singleton
  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

  // Create a socket
  bool create();

  // Bind the socket to a port
  bool bind(const int port, const std::string &address);

  // Listen for incoming connections
  bool listen() const;

  // Accept a new connection and return a reference to the Singleton instance
  bool accept(int &new_sock) const;

  // Connect to a remote server
  bool connect(const std::string &host, const int port);

  // Send a message
  bool send(const int sock, const std::string &message) const;

  // Receive a message
  int receive(const int sock, std::string &message) const;

  // Set socket to non-blocking mode
  void set_non_blocking(const bool);

  // Signal handler function
  static void signalHandler(int signal);

private:
  int m_sock;
  sockaddr_in m_addr;

  Socket();
  ~Socket();

  // Check if socket is valid
  bool is_valid() const;
};

#endif
