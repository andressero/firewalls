// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"
#ifndef REDIRECTORUTILS_HPP
#define REDIRECTORUTILS_HPP

#include "Socket.hpp"
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <limits>
// #include "../../../serverCommon/utils.hpp"

/**
 * @brief asserts that a certain condition is true, else it gives an error
 * message to stderr
 *
 */
#define ASSERT(condition)                                                      \
  do {                                                                         \
    if (!(condition)) {                                                        \
      fprintf(stderr, "Assertion failed: %s", #condition);                     \
      fprintf(stderr, ", in line %d of file %s\n", __LINE__, __FILE__);        \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

#define DEBUG 1
/**
 * @brief General output format to convey errors to stderr
 *
 */
#define ERROR(message)                                                         \
  std::cerr << "\033[1;31m"                                                    \
            << "ERROR: " << message << "\033[0m" << std::endl;

#define LOG(message)                                                           \
  if (DEBUG)                                                                   \
    std::cerr << "\033[1;32m"                                                  \
              << "LOG: " << message << "\033[0m" << std::endl;

#define FILELOG(message)                                                       \
  do {                                                                         \
    std::ofstream file("log-redirector_server.txt", std::ios_base::app);       \
    if (file.is_open()) {                                                      \
      file << message << std::endl;                                            \
      file.close();                                                            \
    } else {                                                                   \
      std::cerr << "Unable to open file "                                      \
                << "log-redirector_server.txt" << std::endl;                   \
    }                                                                          \
  } while (0)

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

inline std::vector<std::string> splitString(const std::string &input,
                                            const std::string &delimiter) {
  std::vector<std::string> tokens;
  std::istringstream iss(input);
  std::string token;

  while (std::getline(iss, token, delimiter[0])) {
    tokens.push_back(token);
  }

  return tokens;
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

            if (!validIP(ip)) {
              file.close();
              return ConfigData("", 0);
            }

            infoFound = true;
            LOG("Port: " + std::to_string(port) + "\nIP: " + ip)
        }
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    file.close();

    return ConfigData(ip, port);
}

#endif // REDIRECTORUTILS_HPP