// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"
// Taken from <https://www.geeksforgeeks.org/socket-programming-in-cpp/>

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
  // creating socket
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  // specifying address
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8080);
  serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Cambiar aqui la ip

  // sending connection request
  connect(clientSocket, (struct sockaddr*)&serverAddress,
          sizeof(serverAddress));

  // sending data
  const char* message = "INICIO\n";
  send(clientSocket, message, strlen(message), 0);

  // closing socket
  close(clientSocket);

  return 0; // Pablo sersi
}
