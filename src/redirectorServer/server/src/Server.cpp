#include "Socket.hpp"
#include "redirectorUtils.hpp"
#include <algorithm>

std::string authServerIP = "10.1.35.23", dbServerIP = "10.1.35.24";

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
                            Socket &db_server_socket) {
  std::string clientRequest;
  std::string auth_response;
  std::string response;

  client_socket.recv(clientRequest);
  LOG("Received client request: " + clientRequest);
  FILELOG("Received client request: " + clientRequest);

  const std::vector<std::string> lines = splitString(clientRequest, "\n");

  for (const std::string &line : lines) {
    const std::vector<std::string> command = splitString(line, " ");

    if (command[0] == "AUTH") {
      if (!auth_server_socket.create()) {
        ERROR("Unable to create auth server socket");
        return "";
      }
      if (!auth_server_socket.connect(authServerIP, 4000)) {
        ERROR("Unable to connect to auth server");
        return "";
      }
      LOG("Connected successfully with Auth server");
      // auth_server_socket.listen();
      if (!auth_server_socket.send(line)) {
        ERROR("Failed to send AUTH request to Auth server");
        return "";
      }
      LOG("AUTH line sent to auth_server");
      FILELOG("AUTH line sent to auth_server");
      if (!auth_server_socket.recv(auth_response)) {
        ERROR("Failed to receive auth response");
        return "";
      }
      LOG("Response from auth_server: " + auth_response);
      FILELOG("Response from auth_server: " + auth_response);
      response = auth_response;
    }
    // Once the auth response it's been received it determines whether or not it
    // can handle a subsequent request
    else if (command[0] == "REQUEST" && auth_response == "OK\n") {
      if (!db_server_socket.create()) {
        ERROR("Unable to create db server socket");
        return "";
      }
      if (!db_server_socket.connect(dbServerIP, 5000)) {
        ERROR("Unable to connect to db server");
        return "";
      }
      if (!db_server_socket.send(line)) {
        ERROR("Failed to send REQUEST to DB server");
        return "";
      }
      LOG("REQUEST line sent to db_server");
      FILELOG("REQUEST line sent to db_server");
      std::string db_response;
      if (!db_server_socket.recv(db_response)) {
        ERROR("Failed to receive db response");
        return "";
      }
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

// TODO(me): Ctrl + C
int main() {
  Socket server_socket, auth_server_socket, db_server_socket;

  // Set up the server socket
  if (!server_socket.create()) {
    ERROR("Unable to create server socket");
    FILELOG("Unable to create server socket");
    return 1;
  }
  // It "can't" fail, cause it'll try again and again until it's able to bind.
  server_socket.bind(3000, "0.0.0.0");
  if (!server_socket.listen()) {
    ERROR("Couldn't listen");
    FILELOG("Couldn't listen");
    return 1;
  }
  LOG("Listening...");
  FILELOG("Listening...");

  // Main server loop
  while (true) {
    Socket client_socket;
    if (server_socket.accept(client_socket)) {
      LOG("Accepted connection from client");
      FILELOG("Accepted connection from client");
      const std::string response =
          protocolGarrobo(client_socket, auth_server_socket, db_server_socket);
      LOG("Response sent to client: "
          << "|" << response << "|");
      if (response.empty()) {
        ERROR("Nothing to send to client");
      } else {
        const bool send_return = client_socket.send(response);
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
