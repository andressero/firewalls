// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>

#ifndef AUDITOR_HPP
#define AUDITOR_HPP

#include "ClientSocket.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <limits>

typedef struct ConfigData {
 public:
  const std::string ip;
  unsigned short port;
  ConfigData(std::string ip, unsigned short port) : ip(ip), port(port) {}
} ConfigData;

inline ConfigData getServerData(const std::string& fileName, const std::string& serverName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        ERROR("Couldn't open file")
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

            // if(!validIP(ip)){}
            // if(!validPort(port)){}
            infoFound = true;
            LOG("Port: " + std::to_string(port) + "\nIP: " + ip)
        }
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    file.close();

    return ConfigData(ip, port);
}

#endif // AUDITOR_HPP