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
 * @return true Login was successful
 * @return false otherwise
 */
bool tryLogin(const std::string &user, const std::string &hash) {
  bool isLoginValid = false;
  // Get login data from File System
  if (!fs.open(user)) {
    return isLoginValid;
  }
  fs.change2ReadMode(user);
  const std::string userData = fs.read(user, 200);
  if (userData == "") {
    return isLoginValid;
  }
  if (!fs.close(user)) {
    return isLoginValid;
  }

  const std::vector<std::string> fields = splitString(userData, ",");

  const std::string storedHash = fields[1];
  LOG("STORED HASH SIZE " + std::to_string(storedHash.size()))
  LOG("RECEIVED HASH SIZE " + std::to_string(hash.size()))

  // updated input is hash + salt
  std::string saltedRcvdHash = hash + fields[2];
  const std::string updatedRcvdHash = sha256Hash(saltedRcvdHash);

  isLoginValid = storedHash == updatedRcvdHash;
  LOG("Valid login: " + std::to_string(isLoginValid))

  return isLoginValid;
}

/**
 * @brief Servers protocol to handle user auth.
 *
 * @details It just allows one of the instructions of the protocol
 * specification. LOGIN
 *
 * @param input Clients input.
 * @return The servers response
 */
std::string protocolGarrobo(const std::string &input) {
  std::string response = "NOT_OK\n";

  std::vector<std::string> command = splitString(input, " ");

  if (command[0] != "LOGIN") {
    LOG("Server is not able to handle " + command[0]);
    FILELOG("Server is not able to handle " + command[0]);
    return response;
  }
  LOG("LOGIN command");
  FILELOG("LOGIN command");

  std::string user = command[1];
  std::string hash = command[2];
  if (tryLogin(user, hash)) {
    LOG("Login successful");
    FILELOG("Login successful");
    response = "OK\n";
  } else {
    LOG("Login unsuccessful");
    FILELOG("Login unsuccessful");
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
    std::string response = protocolGarrobo(request);
    server.send(server.getClientFileDescriptor(), response);
  }
  return 0;
}
