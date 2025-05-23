#include "Blowfish.hpp"
#include "Socket.hpp"
#include "redirectorUtils.hpp"
#include <algorithm>

static const std::string authServerIP = "127.0.0.1", dbServerIP = "127.0.0.1";
static const std::string serverName = "Redirector";

// Taken from https://stackoverflow.com/a/217605
// trim from end (in place)
inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

// Handle client connection and protocol logic
std::string protocolGarrobo(Socket &client_socket, Socket &auth_server_socket,
                            Socket &db_server_socket, Blowfish &cipher) {
  std::string clientRequest;
  std::string auth_response;
  std::string response;

  client_socket.recv(clientRequest);
  LOG("Received client request(CIPHERED): " + clientRequest);
  cipher.decrypt(clientRequest, clientRequest);
  LOG("Received client request(DECIPHERED): " + clientRequest);
  FILELOG("Received client request: " + clientRequest);

  size_t zeroPosition = 0;
  while ((zeroPosition = clientRequest.find_first_of('\000', zeroPosition)) !=
         std::string::npos) {
    clientRequest.erase(zeroPosition, 1);
  }
  const std::vector<std::string> lines =
      splitString(cleanString(clientRequest), "\n");

  for (const std::string &line : lines) {
    const std::vector<std::string> command =
        splitString(cleanString(line), " ");
    if (command[0] == "AUTH") {
      if (!auth_server_socket.create()) {
        ERROR("Unable to create auth server socket");
        response = "";
        break;
      }
      if (!auth_server_socket.connect(authServerIP, 4000)) {
        ERROR("Unable to connect to auth server");
        response = "";
        if (command[1] == "999999999") {
          response =
              "auth server failed: unable to connect - port might be down";
        }
        break;
      }
      LOG("Connected successfully with Auth server");
      std::string cipheredLine = line;
      LOG("Send to auth server(DECIPHERED): " + line)
      cipher.encrypt(cipheredLine, cipheredLine);
      LOG("Send to auth server(CIPHERED): " + cipheredLine)
      if (!auth_server_socket.send(cipheredLine)) {
        ERROR("Failed to send AUTH request to Auth server");
        response = "";
        if (command[1] == "999999999") {
          response = "auth server failed: unreachable";
        }
        break;
      }
      LOG("AUTH line sent to auth_server");
      FILELOG("AUTH line sent to auth_server");
      if (!auth_server_socket.recv(auth_response)) {
        ERROR("Failed to receive auth response");
        response = "";
        if (command[1] == "999999999") {
          response = "auth server failed: unreachable";
        }
        break;
      }
      LOG("Response from auth_server(CIPHERED): " + auth_response);
      cipher.decrypt(auth_response, auth_response);
      LOG("Response from auth_server(DECIPHERED): " + auth_response);
      FILELOG("Response from auth_server: " + auth_response);
      response = auth_response;
    }
    // Once the auth response it's been received it determines whether or not it
    // can handle a subsequent request
    else if (command[0] == "REQUEST" && cleanString(auth_response) == "OK\n") {
      if (!db_server_socket.create()) {
        ERROR("Unable to create db server socket");
        response = "";
        break;
      }
      if (!db_server_socket.connect(dbServerIP, 5000)) {
        ERROR("Unable to connect to db server");
        response = "";
        if (command[2] == "999999999") {
          response = "db server failed: unable to connect - port might be down";
        }
        break;
      }
      std::string cipheredLine = line;
      cipher.encrypt(cipheredLine, cipheredLine);
      if (!db_server_socket.send(cipheredLine)) {
        ERROR("Failed to send REQUEST to DB server");
        response = "";
        if (command[2] == "999999999") {
          response = "db server failed: unreachable";
        }
        break;
      }
      LOG("REQUEST line sent to db_server");
      FILELOG("REQUEST line sent to db_server");
      std::string db_response;
      if (!db_server_socket.recv(db_response)) {
        ERROR("Failed to receive db response");
        response = "";
        if (command[2] == "999999999") {
          response = "db server failed: unreachable";
        }
        break;
      }
      cipher.decrypt(db_response, db_response);
      LOG("Response from db_server: " + db_response);
      FILELOG("Response from db_server: " + db_response);
      if (db_response != "NOT_OK\n") {
        response = db_response;
      }
    } else {
      LOG("Received unordered or unknown request type");
      FILELOG("Received unordered or unknown request type");
      response = "NOT_OK\n";
      break;
    }
  }
  ::rtrim(response);
  return response;
}

int main() {
  Socket server_socket, auth_server_socket, db_server_socket;

  std::string key = getKey("Key.txt");

  if (key.empty()) {
    ERROR("Couldn't get key from file")
    return -1;
  }

  LOG("Got key: " + key)
  Blowfish cipher;
  cipher.setKey(key);

  // Set up the server socket
  if (!server_socket.create()) {
    ERROR("Unable to create server socket");
    FILELOG("Unable to create server socket");
    return 1;
  }
  // It "can't" fail, cause it'll try again and again until it's able to bind.
  ConfigData data =
      getServerData("../../serverCommon/IP-addresses.txt", serverName);

  if (!server_socket.bind(data.port, data.ip)) {
    ERROR("Couldn't bind");
    FILELOG("Couldn't bind");
    return -1;
  }

  if (!server_socket.listen()) {
    ERROR("Couldn't listen");
    FILELOG("Couldn't listen");
    return 1;
  }

  // Main server loop
  while (true) {
    Socket client_socket;
    if (server_socket.accept(client_socket)) {
      LOG("Accepted connection from client");
      FILELOG("Accepted connection from client");
      const std::string response = protocolGarrobo(
          client_socket, auth_server_socket, db_server_socket, cipher);
      LOG("Response sent to client: "
          << "|" << response << "|");
      if (response.empty()) {
        ERROR("Nothing to send to client");
      } else {
        std::string cipheredResponse;
        cipher.encrypt(cipheredResponse, response);
        const bool send_return = client_socket.send(cipheredResponse);
        if (!send_return) {
          ERROR("Failed to send response to client");
          FILELOG("Failed to send response to client");
        } else {
          LOG("Response sent to client");
          FILELOG("Response sent to client");
        }
      }
    } else {
      ERROR("Failed to accept connection");
      FILELOG("Failed to accept connection");
    }
    client_socket.~Socket();
  }

  return 0;
}
