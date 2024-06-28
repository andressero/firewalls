// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>

#include "auditor.hpp"

int main() {
  ConfigData data = getServerData("../serverCommon/IP-addresses.txt", "Redirector");
  // ClientSocket client(SERVER_IP, SERVER_PORT);
  ClientSocket client(data.ip, data.port);
  
  while (true) {
    client.connectToServer();
    std::string message = "AUTH 123456789 18d404b76462a6b04b4e413c977734ae4923e2796a34cd1dd82b09f92d340bac\nREQUEST USER_DATA 123456789\n";
    if(client.sendData(message) < 0) {
      ERROR("Unable to send. Redirector possibly failed");
      FILELOG("Error: Unable to send. Redirector possibly failed");
      return 1;
    }
    LOG("Sent successful.");
    FILELOG("Sent successful.");

    std::string response = client.receiveData();
    if(response  == "failed: auth " || response == "failed: db ") {
      LOG(response);
      FILELOG(response);
    }  else if(response.empty()) {
      LOG("No response from server redirector.");
      FILELOG("No response from server redirector.");
    }else {
      LOG("Servers responding correctly");
      FILELOG("Servers responding correctly");
    }

    sleep(1); // Sleep for 30 seconds then 
  }

  return 0;
}
