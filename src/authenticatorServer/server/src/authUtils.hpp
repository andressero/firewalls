// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"
#ifndef AUTHUTILS_HPP
#define AUTHUTILS_HPP

#include "FileSystem.hpp"
#include "SHA256.hpp"
// #include "../../../serverCommon/utils.hpp"

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

inline std::string sha256ToString(const BYTE *hash) {
  std::string answer(2 * SHA256_BLOCK_SIZE, '0');
  for (size_t i = 0; i < SHA256_BLOCK_SIZE; ++i) {
    char temp[3] = {0};
    snprintf(temp, 3, "%02x", hash[i]);
    answer[2 * i] = temp[0];
    answer[2 * i + 1] = temp[1];
  }
  return answer;
}

inline std::string sha256Hash(std::string &strInput) {
  const BYTE *input = reinterpret_cast<const BYTE *>(strInput.data());

  BYTE hash[SHA256_BLOCK_SIZE];
  SHA256_CTX context;
  sha256_init(&context);
  sha256_update(&context, input, strInput.size());
  sha256_final(&context, hash);
  return sha256ToString(hash);
}

#endif // AUTHUTILS_HPP