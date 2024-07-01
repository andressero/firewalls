// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>"
// Adapted from <https://www.geeksforgeeks.org/socket-programming-in-cpp/>

#include "FileSystem.hpp"
#include "Socket.hpp"
#include "authUtils.hpp"
#include "Blowfish.hpp"
#include <condition_variable>
#include <csignal>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// Singleton instances
Socket &server = Socket::getInstance();
FileSystem &fs = FileSystem::getInstance();
Blowfish &cipher = Blowfish::getInstance();
static const std::string serverName = "Authenticator";
// TODO(any): Better name for this mutex
std::mutex canAccessThreadStuff;
std::mutex canAccessFS;
std::condition_variable cv;
bool shutdownRequested = false;
std::vector<std::thread> threads;

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
  std::string userData;
  // Get Auth data from File System
  { // Protect FS from race condition (open/close/change2ReadMode)
    std::lock_guard<std::mutex> lock(canAccessFS);

    if (!fs.open(user)) {
      LOG("\tFileSystem failed at open");
      FILELOG("\tFileSystem failed at open");
      return authSuccessful;
    }
    fs.change2ReadMode(user);
    userData = fs.read(user, 200);
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
  }

  const std::vector<std::string> fields = splitString(userData, ",");

  const std::string storedHash = fields[1];
  LOG("STORED HASH SIZE " + std::to_string(storedHash.size()))
  LOG("RECEIVED HASH SIZE " + std::to_string(hash.size()))


  // updated input is hash + salt
  std::string saltedRcvdHash = hash + fields[2];
  const std::string updatedRcvdHash = sha256Hash(saltedRcvdHash);

  LOG(saltedRcvdHash + " = received hash + salt")
  LOG(updatedRcvdHash  + " = received salted hash")
  LOG(storedHash + " = stored hash")

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

void handleClient(int clientSocket) {
  std::string clientRequest;
  server.receive(clientSocket, clientRequest);
  LOG("Auth received(CIPHERED): " + clientRequest);
  cipher.decrypt(clientRequest, clientRequest);
  LOG("Auth received(DECIPHERED): " + clientRequest);
  if (!clientRequest.empty()) {
    std::string response = protocolGarrobo(clientRequest);
    LOG("Auth response(DECIPHERED): " + response)
    cipher.encrypt(response, response);
    LOG("Auth response(CIPHERED): " + response)
    server.send(clientSocket, response);
  }
  ::close(clientSocket);
}

void signalr(int signal) {
  std::cout << "Received signal " << signal << ". Releasing resources..."
            << std::endl;
  {
    std::lock_guard<std::mutex> lock(canAccessThreadStuff);
    shutdownRequested = true;
  }
  // All threads close by signal
  cv.notify_all();
  server.signalHandler(signal);
  FileSystem &fs = FileSystem::getInstance();
  fs.signalHandler(signal);
  for (std::thread &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }
  exit(0);
}

int main() {
  std::signal(SIGINT, signalr);
  ConfigData data =
      getServerData("../../serverCommon/IP-addresses.txt", serverName);

  std::string key = getKey("Key.txt");

  if (!server.create()) {
    ERROR("Unable to create server socket");
    return 1;
  } else if (!server.bind(data.port, data.ip)) {
    ERROR("Unable to Bind")
    return 1;
  } else if (!server.listen()) {
    ERROR("Couldn't listen")
    return 1;
  } else if (key.empty()) {
    ERROR("Couldn't get key from file")
    return 1;
  }

  LOG("Got key: " + key)
  cipher.setKey(key);

  while (true) {
    {
      std::unique_lock<std::mutex> lock(canAccessThreadStuff);
      if (shutdownRequested) {
        break;
      }
    }

    int clientSocket;
    if (server.accept(clientSocket)) {
      // Thread per connection accepted
      std::thread clientThread(handleClient, clientSocket);
      {
        std::lock_guard<std::mutex> lock(canAccessThreadStuff);
        threads.push_back(std::move(clientThread));
      }
    }
  }

  for (std::thread &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  return 0;
}
