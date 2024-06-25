// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>

#include "ClientSocket.hpp"

#define SERVER_PORT 3000
#define SERVER_IP "192.168.100.68"

int main() {
  ClientSocket client(SERVER_IP, SERVER_PORT);

  client.connectToServer();
  // TODO(any): Create admin user, create server health status request.
  std::string message = "AUTH admin hash\nREQUEST USER_DATA admin";
  if(client.sendData(message) < 0) {
    ERROR("Unable to send.");
    FILELOG("Error: Unable to send.");
    return 1;
  }
  LOG("Sent successful.");
  FILELOG("Sent successful.");

  std::string response = client.receiveData();
  if(response == "") {
    LOG("");
    FILELOG("");
  }

  return 0;
}
