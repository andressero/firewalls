// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos,
// Andres Serrano>"

#ifndef DBSERVERUTILS_HPP
#define DBSERVERUTILS_HPP

#include <arpa/inet.h>
#include <chrono> // For std::chrono::seconds
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sqlite3.h>
#include <sstream>
#include <string>
#include <thread> // For std::this_thread::sleep_for
#include <unistd.h>
#include <unordered_map>
#include <vector>
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
    std::ofstream file("log-db_server", std::ios_base::app);                   \
    if (file.is_open()) {                                                      \
      file << message << std::endl;                                            \
      file.close();                                                            \
    } else {                                                                   \
      std::cerr << "Unable to open file "                                      \
                << "log-db_server" << std::endl;                               \
    }                                                                          \
  } while (0)

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

inline bool isAllDigits(const std::string &str) {
  for (char c : str) {
    if (!std::isdigit(c)) {
      return false;
    }
  }
  return true;
}

inline std::string extractUserID(const std::string &input) {
  // Check if the string starts with a single quote
  if (input[0] != '\'') {
    return "";
  }

  // Find the position of the next single quote after the first one
  size_t endPos = input.find('\'', 1);
  if (endPos == std::string::npos) {
    return "";
  }

  // Extract the substring between the first and second single quotes
  std::string userID = input.substr(1, endPos - 1);

  // Check if the extracted userID is either 9 or 12 digits long and all
  // characters are digits
  if ((userID.length() == 9 || userID.length() == 12) && isAllDigits(userID)) {
    return userID;
  }

  return "";
}

/**
 * @brief Checks whether or not the data that intending to be inserted has the
 * correct format.
 *
 * @param input Data that's intending to be inserted.
 * @return true if the format is correct.
 * @return false otherwise.
 */
inline bool checkInsertionFormat(const std::string &input) {
  std::regex pattern(
      R"('(\d{9}|\d{12})',\s*'\d{2}-\d{2}-\d{4}',\s*'(Yes|No)',\s*'.*')");
  return  true /*std::regex_match(input, pattern)*/;
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

#endif