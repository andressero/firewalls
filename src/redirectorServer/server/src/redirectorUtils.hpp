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

#define LOG_TO_FILE(message, filename)                                         \
  do {                                                                         \
    std::ofstream file(filename, std::ios_base::app);                          \
    if (file.is_open()) {                                                      \
      file << message << std::endl;                                            \
      file.close();                                                            \
    } else {                                                                   \
      std::cerr << "Unable to open file " << filename << std::endl;            \
    }                                                                          \
  } while (0)

#define LOG_TO_DEFAULT_FILE(message) LOG _TO_FILE(message, "log.txt")

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

std::string sendToServer(const std::string &serverIp, int serverPort,
                         const std::string &message) {
  Socket socket(serverIp, serverPort);
  if (!socket.connect()) {
    return "ERROR";
  }
  socket.send(message);
  std::string response = socket.receive();
  socket.close();
  return response;
}
#endif // REDIRECTORUTILS_HPP