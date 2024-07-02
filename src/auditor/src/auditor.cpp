// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>

#include "auditor.hpp"
#include "Blowfish.hpp"

// TODO(any): Encrypt client messages.

inline std::string getKey(const std::string& fileName) {
  std::ifstream file(fileName);

  if (!file.is_open()) {
    ERROR("Couldn't open key file")
    return "";
  }

  std::string key;
  file >> key;

  if (key.empty()) {
    ERROR("Key file is empty")
    return "";
  }

  return key;
}

inline std::string cleanString(const std::string& input) {
    std::string result;
    for (char ch : input) {
        if (std::isprint(static_cast<unsigned char>(ch)) || ch == '\n') {
            result += ch;
        }
    }
    return result;
}

int main() {
  ConfigData data =
      getServerData("../serverCommon/IP-addresses.txt", "Redirector");

  std::string key = getKey("Key.txt");

  if (key.empty()) {
    ERROR("Couldn't get key from file")
    return -1;
  }

  Blowfish cipher;
  cipher.setKey(key);

  while (true) {
    ClientSocket client(data.ip, data.port);
    client.connectToServer();
    std::string message = "AUTH 999999999 "
                          "8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f"
                          "6f2ab448a918\nREQUEST USER_DATA 999999999\n";
    cipher.encrypt(message, message);
    if (client.sendData(message) < 0) {
      ERROR("Unable to send. Redirector possibly failed");
      FILELOG("Error: Unable to send. Redirector possibly failed");
      return 1;
    }
    LOG("Sent successful.");
    FILELOG("Sent successful.");

    std::string response = client.receiveData();
    cipher.decrypt(response, response);
    response = cleanString(response);
    if (response.find("auth server failed") == std::string::npos ||
        response.find("db server failed") == std::string::npos) {
      LOG(response);
      FILELOG(response);
    } else if (response.empty()) {
      LOG("No response from server redirector.");
      FILELOG("No response from server redirector.");
    } else {
      LOG("Servers responding correctly");
      FILELOG("Servers responding correctly");
    }

    sleep(10); // Sleep for 10 seconds then
  }

  return 0;
}
