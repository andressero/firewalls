// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"
// Taken from <https://www.geeksforgeeks.org/socket-programming-in-cpp/>

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../../server/src/Socket.hpp"


int main() {
  Socket client(8080, "127.0.0.1");
  Socket server(8080, "0.0.0.0");

  client.connect(server.getServerAddress());

  client.send(client.getServerFileDescriptor(), "INICIO");

  std::string answer = client.receive(client.getServerFileDescriptor());

  std::cout << "Message from server: " << answer << std::endl;

}
