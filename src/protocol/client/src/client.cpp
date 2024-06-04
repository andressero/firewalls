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
#include "Logger.hpp"


int main() {
  Socket client(8080, "127.0.0.1");
  Socket server(8080, "0.0.0.0");

  Logger logger;

  client.connect(server.getServerAddress());
  logger.log("Client connecting to: 127.0.0.1, port: 8080", "client_log.txt");

  client.send(client.getServerFileDescriptor(), "INICIO");
  logger.log("Client sent: INICIO", "client_log.txt");

  std::string answer = client.receive(client.getServerFileDescriptor());
  logger.log("Client received: " + answer, "client_log.txt")

}
