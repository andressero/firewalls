#ifndef REQUEST_H
#define REQUEST_H

#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <QDebug>
#include "blowfish.h"

class ClientSocket;

inline bool validIP(const std::string& ip) {
    std::stringstream ipStream(ip);
    std::string ipValue;
    bool valid = true;
    int ipValueCount = 0;

    while (std::getline(ipStream, ipValue, '.') && valid) {
        std::stringstream ipValueStream(ipValue);
        int ipNumber = -10;
        ipValueStream >> ipNumber;
        if (ipNumber < 0 || ipNumber > 255) {
            valid = false;
        }
        ++ipValueCount;
    }

    if (ipValueCount != 4) {
        valid = false;
    }

    return valid;
}

typedef struct ConfigData {
public:
    const std::string ip;
    unsigned short port;
    ConfigData(std::string ip, unsigned short port) : ip(ip), port(port) {}
} ConfigData;

inline ConfigData getServerData(const std::string& fileName, const std::string& serverName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        qInfo() << "Couldn't open file\n";
        return ConfigData("", 0);
    }

    std::string name;
    bool infoFound = false;
    unsigned short port = 0;
    std::string ip;

    while (!infoFound && file.peek() != EOF) {
        file >> name;
        if (name == serverName) {
            file >> port >> ip;

            if (!validIP(ip)) {
                file.close();
                return ConfigData("", 0);
            }

            infoFound = true;
        }
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    file.close();

    return ConfigData(ip, port);
}


inline std::string getKey(const std::string& fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        qInfo() << "Couldn't open key file\n";
        return "";
    }

    std::string key;
    file >> key;

    if (key.empty()) {
        qInfo() << "Key file is empty\n";
        return "";
    }

    return key;
}

// Use sockets
// In charge of parsing data
class Request {
private:
  std::string username;
  std::string hash;
  std::string redirectorServerIP;
  int redirectorServerPort;
  Blowfish cipher;
  bool ready;

  std::string parse(std::string message);
  void setup();

  Request() {
      this->setup();
  }

  Request(const Request &) = delete;
  Request &operator=(const Request &) = delete;

public:
  static Request &getInstance() {
    static Request instance;
    return instance;
  }
  bool isReady();
  std::string
  removeServerConfirmationResponse(const std::string &receivedResponse);
  std::string requestLogin();
  std::string requestUserData();
  std::string requestPatientData(const std::string &patientID);
  std::string requestInsuranceStatus(const std::string &id);
  std::string requestLabList();
  std::string requestLabResult(const std::string &labDate);
  std::string requestPatientList();
  bool requestDataInsertion(const std::string &data);

  void setUsername(const std::string &username);
  void setHash(const std::string &hash);
  void setRedirectorServerIDAndPort(const std::string &redirectorServerIP,
                                    const int redirectorServerPort);
  std::vector<std::string> splitString(const std::string &input,
                                       const std::string &delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;

    while (std::getline(iss, token, delimiter[0])) {
      tokens.push_back(token);
    }

    return tokens;
  }
};
#endif // REQUEST_H
