// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>

#include "auditor.hpp"

// #define SERVER_PORT 3000
// #define SERVER_IP "192.168.100.68"

int main() {
  ConfigData data = getServerData("../../serverCommon/IP-addresses.txt", "Redirector");
  // ClientSocket client(SERVER_IP, SERVER_PORT);
  ClientSocket client(data.ip, data.port);

  while (true) {
    client.connectToServer(); //! indeciso si dentro o fuera del while
    // TODO(any):create server health status request. admin has user 999999999, password admin
    // TODO(any):add the admin user to the database so it can be tested
    std::string message = "AUTH 999999999 8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918\nREQUEST USER_DATA admin";
    if(client.sendData(message) < 0) {
      ERROR("Unable to send.");
      FILELOG("Error: Unable to send.");
      return 1;
    }
    LOG("Sent successful.");
    FILELOG("Sent successful.");

    // La intención es que no nos importe lo que diga, si no que diga algo
    //Pero si solo mediante mensajes podemos saber si uno esta mal que asi sea 
    std::string response = client.receiveData();
    if(client.receiveData() == "") {
      LOG("");
      FILELOG("");
    }

    sleep(30); // Sleep for 30 seconds then 
  }

  return 0;
}
