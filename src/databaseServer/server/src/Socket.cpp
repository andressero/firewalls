#include "Socket.hpp"
#include "dbServerUtils.hpp"
#include <chrono>
#include <fcntl.h>
#include <thread>

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

bool Socket::bind(const int port, const std::string &address) {
  if (!is_valid()) {
    return false;
  }

  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = inet_addr(address.c_str());
  m_addr.sin_port = htons(port);

  while (::bind(m_sock, (struct sockaddr *)&m_addr, sizeof(m_addr)) == -1) {
    ERROR("Attempting to bind in 5 seconds")
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }
  LOG("Binding successful. IP: " << address << ":" << port);
  return true;
}

bool Socket::listen() const { return ::listen(m_sock, SOMAXCONN) != -1; }

bool Socket::accept(int &new_sock) const {
  int addr_length = sizeof(m_addr);
  new_sock = ::accept(m_sock, (sockaddr *)&m_addr, (socklen_t *)&addr_length);
  return new_sock > 0;
}

bool Socket::connect(const std::string &host, const int port) {
  if (!is_valid())
    return false;

  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = inet_addr(host.c_str());
  m_addr.sin_port = htons(port);

  int status = ::connect(m_sock, (sockaddr *)&m_addr, sizeof(m_addr));
  return status == 0;
}

bool Socket::send(const int sock, const std::string &message) const {
  return ::send(sock, message.c_str(), message.size(), MSG_NOSIGNAL) != -1;
}

int Socket::receive(const int sock, std::string &message) const {
  char buf[1024];
  message.clear();
  memset(buf, 0, sizeof(buf));
  int status = ::recv(sock, buf, sizeof(buf), 0);
  if (status > 0) {
    message = std::string(buf, status);
  }
  return status;
}

void Socket::set_non_blocking(const bool b) {
  int opts = fcntl(m_sock, F_GETFL);
  if (opts < 0) {
    return;
  }
  opts = b ? (opts | O_NONBLOCK) : (opts & ~O_NONBLOCK);
  fcntl(m_sock, F_SETFL, opts);
}

bool Socket::is_valid() const { return m_sock != -1; }

// Signal handler function
void Socket::signalHandler(int signal) {
  std::cout << "Socket received signal " << signal << ". Releasing resources..."
            << std::endl;
  Socket &instance = Socket::getInstance();
  ::close(instance.m_sock);
}
