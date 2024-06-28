// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>"

#include "ClientSocket.hpp"

#define BUFFER_SIZE 1024

ClientSocket::ClientSocket(const std::string &ip, int port) {
  // Create socket
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    perror("Failed to create socket");
    exit(EXIT_FAILURE);
  }

  // Configure server address
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  if (inet_pton(AF_INET, ip.c_str(), &server_address.sin_addr) <= 0) {
    perror("Invalid address/Address not supported");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }
}

ClientSocket::~ClientSocket() { close(socket_fd); }

void ClientSocket::connectToServer() {
  if (connect(socket_fd, (struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    ERROR("Unable to connect to server redirector.");
    FILELOG("Error: Unable to connect to server redirector");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }
  LOG("Connection to server redirector successful.");
  FILELOG("Connection to server redirector successful.");
}

int ClientSocket::sendData(const std::string &data) {
  return send(socket_fd, data.c_str(), data.length(), 0);
}

std::string ClientSocket::receiveData() {
  char buffer[BUFFER_SIZE];
  int bytes_received = recv(socket_fd, buffer, BUFFER_SIZE, 0);
  if (bytes_received < 0) {
    ERROR("Failed to receive.");
    FILELOG("Error: Failed to receive.");
    return "";
  } else {
    buffer[bytes_received] = '\0';
    return std::string(buffer);
  }
}
