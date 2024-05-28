// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"
// Taken from <https://www.geeksforgeeks.org/socket-programming-in-cpp/>

#include <cstring>
#include <iostream>
#include <string>
#include <map>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

void protocolGarrobo(char* command);

int main() {
  // std::map<std::string, int> traductor= {};
  // traductor["INICIO"] = 1;
  // traductor["LOGIN"] = 2;
  // traductor["REQUEST"] = 3;
  // traductor["QUIT"] = 4;
  // creating socket
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  // specifying the address
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8080);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  // binding socket.
  bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

  // listening to the assigned socket
  listen(serverSocket, 5);

  // accepting connection request
  int clientSocket = accept(serverSocket, nullptr, nullptr);

  // recieving data
  char buffer[1024] = {0};
  recv(clientSocket, buffer, sizeof(buffer), 0);
  protocolGarrobo(buffer);
  std::cout << "Message from client: " << buffer << std::endl;

  // closing the socket.
  close(serverSocket);

  return 0;
}

void protocolGarrobo(char* command) {
  char* token = strtok(command, " \n");
  // while( token != NULL ) {
  //   printf( " %s\n", token ); //printing each token
  //   token = strtok(NULL, " \n");
  // }

  if (strncmp(token, "INICIO", 7) == 0) {
    std::cout << "OK\n";
  } else if (strncmp(token, "LOGIN", 6) == 0) {

  } else if (strncmp(token, "REQUEST", 8) == 0) {

  } else if (strncmp(token, "QUIT", 5) == 0) {

  } else {
    std::cerr << "Error: "<< token << " is an unknown command\n";
  }
}
