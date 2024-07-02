// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>"

#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>


inline std::string getCurrentDateTime() {
    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    // Use a char array to format the date and time manually
    char buffer[100];
    std::sprintf(buffer, " %02d/%02d/%04d at %02d:%02d",
                 local_time->tm_mday,
                 local_time->tm_mon + 1,
                 local_time->tm_year + 1900,
                 local_time->tm_hour,
                 local_time->tm_min);

    return std::string(buffer);
}

#define DEBUG 1
/**
 * @brief General output format to convey errors to stderr
 *
 */
#define ERROR(message)                                                         \
    std::cerr << "\033[1;31m"                                                    \
    << "ERROR: " << message << getCurrentDateTime() << "\033[0m" << std::endl;

#define LOG(message)                                                           \
if (DEBUG)                                                                   \
        std::cerr << "\033[1;33m"                                                  \
        << "LOG: " << message << getCurrentDateTime() << "\033[0m" << std::endl;

#define FILELOG(message)                                                       \
  do {                                                                         \
    std::ofstream file("auditor-log.txt", std::ios_base::app);                   \
    if (file.is_open()) {                                                      \
      file << message << getCurrentDateTime() << std::endl;                                            \
      file.close();                                                            \
    } else {                                                                   \
      std::cerr << "Unable to open file "                                      \
                << "auditor-log.txt" << std::endl;                               \
    }                                                                          \
  } while (0)


class ClientSocket {
public:
  ClientSocket(const std::string &ip, int port);
  ~ClientSocket();

  void connectToServer();
  int sendData(const std::string &data);
  std::string receiveData();

private:
  int socket_fd;
  struct sockaddr_in server_address;
};

#endif // CLIENTSOCKET_HPP
