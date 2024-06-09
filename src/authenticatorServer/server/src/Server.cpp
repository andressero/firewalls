// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>"
// Adapted from <https://www.geeksforgeeks.org/socket-programming-in-cpp/>

#include "FileSystem.hpp"
#include "Socket.hpp"
#include "authUtils.hpp"

Socket &server = Socket::getInstance();
FileSystem &fs = FileSystem::getInstance();

/**
 * @brief Obtains the correct users hash and compares it with the processed
 * received hash.
 *
 * @param user User that's authenticating
 * @param hash Received(rcvd) Hash.
 * @return true Authentication was successful
 * @return false otherwise
 */
bool tryAuth(const std::string &user, const std::string &hash) {
  bool authSuccessful = false;
  // Get Auth data from File System
  if (!fs.open(user)) {
    LOG("\tFileSystem failed at open");
    FILELOG("\tFileSystem failed at open");
    return authSuccessful;
  }
  fs.change2ReadMode(user);
  const std::string userData = fs.read(user, 200);
  if (userData == "") {
    LOG("\tFileSystem failed at read");
    FILELOG("\tFileSystem failed at read");
    return authSuccessful;
  }
  if (!fs.close(user)) {
    LOG("\tFileSystem failed at close");
    FILELOG("\tFileSystem failed at close");
    return authSuccessful;
  }

  const std::vector<std::string> fields = splitString(userData, ",");

  const std::string storedHash = fields[1];
  LOG("STORED HASH SIZE " + std::to_string(storedHash.size()))
  LOG("RECEIVED HASH SIZE " + std::to_string(hash.size()))

  // updated input is hash + salt
  std::string saltedRcvdHash = hash + fields[2];
  const std::string updatedRcvdHash = sha256Hash(saltedRcvdHash);

  authSuccessful = storedHash == updatedRcvdHash;
  LOG("Successful Authentication: " + std::to_string(authSuccessful))

  return authSuccessful;
}

/**
 * @brief Servers protocol to handle user auth.
 *
 * @details It just allows one of the instructions of the protocol
 * specification. AUTH
 *
 * @param input Clients input.
 * @return The servers response
 */
std::string protocolGarrobo(const std::string &input) {
  std::string response = "NOT_OK\n";

  std::vector<std::string> command = splitString(input, " ");

  if (command[0] != "AUTH") {
    LOG("Server is not able to handle " + command[0]);
    FILELOG("Server is not able to handle " + command[0]);
    return response;
  }
  LOG("AUTH command");
  FILELOG("AUTH command");

  std::string user = command[1];
  std::string hash = command[2];
  if (tryAuth(user, hash)) {
    LOG("Authentication successful");
    FILELOG("Authentication successful");
    response = "OK\n";
  } else {
    LOG("Authentication unsuccessful");
    FILELOG("Authentication unsuccessful");
  }

  return response;
}

/**
 * @brief Handles Ctrl + C signal.
 */
static void signalr(int signal) {
  std::cout << "Received signal " << signal << ". Releasing resources..."
            << std::endl;
  Socket &server = Socket::getInstance();
  server.signalHandler(signal);
  FileSystem &fs = FileSystem::getInstance();
  fs.signalHandler(signal);
  exit(0);
}

int main() {
  std::signal(SIGINT, signalr);
  server.bind();
  server.listen(5);
  while (true) {
    server.accept();
    std::string request = server.receive(server.getClientFileDescriptor());
    if (!request.empty()) {
      std::string response = protocolGarrobo(request);
      server.send(server.getClientFileDescriptor(), response);
    }
  }
  return 0;
}
