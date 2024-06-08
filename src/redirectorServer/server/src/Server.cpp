#include "Socket.hpp"
#include "redirectorUtils.hpp"
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

void handleClient(Socket &clientSocket) {
  std::string message = clientSocket.receive();

  // TODO(any): Put db_server and auth_server ips and it's
  std::string server1_ip = "127.0.0.1";
  int server1_port = 12345;
  std::string server2_ip = "127.0.0.1";
  int server2_port = 54321;

  if (message.find("AUTH") == 0) {
    std::string response1 = sendToServer(server1_ip, server1_port, message);
    if (response1 == "OK\n") {
      std::string response2 = sendToServer(server2_ip, server2_port, message);
      clientSocket.send(response2);
    } else {
      clientSocket.send("NOT_OK\n");
    }
  } else if (message.find("REQUEST") == 0) {
    std::string response1 = sendToServer(server1_ip, server1_port, message);
    if (response1 == "OK\n") {
      std::string response2 = sendToServer(server2_ip, server2_port, message);
      clientSocket.send(response2);
    } else {
      clientSocket.send("NOT_OK\n");
    }
  } else {
    clientSocket.send("INVALID_COMMAND\n");
  }
}

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(8080);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  while (true) {
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    Socket clientSocket("127.0.0.1", new_socket);
    handleClient(clientSocket);
  }

  return 0;
}
