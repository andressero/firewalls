// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>"

#include "Socket.hpp"
#include "redirectorUtils.hpp"
#include <fcntl.h>

Socket::Socket() : m_sock(-1) { memset(&m_addr, 0, sizeof(m_addr)); }

Socket::~Socket() {
  if (is_valid()) {
    ::close(m_sock);
  }
}

bool Socket::create() {
  m_sock = socket(AF_INET, SOCK_STREAM, 0);
  return is_valid();
}

bool Socket::bind(const int port) {
  if (!is_valid()) {
    return false;
  }

  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons(port);

  int bind_return = ::bind(m_sock, (struct sockaddr *)&m_addr, sizeof(m_addr));
  return bind_return != -1;
}

bool Socket::listen() const {
  bool listenSuccessful = false;
  if (!is_valid()) {
    return listenSuccessful;
  }

  int listen_return = ::listen(m_sock, 5);
  listenSuccessful = listen_return != -1;
  if (!listenSuccessful) {
    ERROR("Unable to start listening");
    FILELOG("Unable to start listening");
  } else {
    LOG("Listening...");
    FILELOG("Listening...");
  }
  return listenSuccessful;
}

bool Socket::accept(Socket &newSocket) const {
  int addr_length = sizeof(m_addr);
  newSocket.m_sock =
      ::accept(m_sock, (sockaddr *)&m_addr, (socklen_t *)&addr_length);
  return newSocket.m_sock > 0;
}

bool Socket::connect(const std::string &host, const int port) {
  if (!is_valid()) {
    return false;
  }

  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons(port);

  int status = inet_pton(AF_INET, host.c_str(), &m_addr.sin_addr);
  if (status == 0) {
    return false;
  }

  status = ::connect(m_sock, (sockaddr *)&m_addr, sizeof(m_addr));
  return status == 0;
}

bool Socket::send(const std::string &message) const {
  int status = ::send(m_sock, message.c_str(), message.size(), MSG_NOSIGNAL);
  return status != -1;
}

int Socket::recv(std::string &message) const {
  char buffer[1024];
  message.clear();
  memset(buffer, 0, 1024);
  int bytes_received = ::recv(m_sock, buffer, 1024, 0);
  if (bytes_received > 0) {
    message = buffer;
  }
  return bytes_received;
}

/**
 * @brief Sets the socket to non-blocking or blocking mode.
 *
 * This method sets the file descriptor flags for the socket to enable or
 * disable non-blocking mode. In non-blocking mode, socket operations such as
 * `recv` and `send` will return immediately if they cannot be completed,
 * allowing the program to continue executing other code. In blocking mode,
 * these operations will wait until they can be completed.
 *
 * @param b If true, the socket is set to non-blocking mode. If false, the
 * socket is set to blocking mode.
 */
void Socket::set_non_blocking(const bool b) {
  int opts;
  // Get the current file descriptor flags
  opts = fcntl(m_sock, F_GETFL);

  if (opts < 0) {
    return;
  }
  // Set or clear the O_NONBLOCK flag based on the boolean parameter 'b'
  opts = (b) ? (opts | O_NONBLOCK) : (opts & ~O_NONBLOCK);
  // Set the modified file descriptor flags
  fcntl(m_sock, F_SETFL, opts);
}

bool Socket::is_valid() const { return m_sock != -1; }
