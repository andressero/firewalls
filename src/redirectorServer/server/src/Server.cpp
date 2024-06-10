#include "Socket.hpp"
#include "redirectorUtils.hpp"

// Handle client connection and protocol logic
std::string protocolGarrobo(Socket client_socket, Socket &auth_server_socket,
                            Socket &db_server_socket) {
  std::string clientRequest;
  client_socket.recv(clientRequest);
  LOG("Received client request: " + clientRequest);
  FILELOG("Received client request: " + clientRequest);
  std::string auth_response;
  std::string response;

  const std::vector<std::string> lines = splitString(clientRequest, "\n");

  for (const std::string &line : lines) {
    const std::vector<std::string> command = splitString(line, " ");

    if (command[0] == "AUTH") {
      auth_server_socket.create();
      auth_server_socket.connect("127.0.0.1", 4000);
      // auth_server_socket.listen();
      auth_server_socket.send(line);
      LOG("AUTH line sent to auth_server");
      FILELOG("AUTH line sent to auth_server");
      auth_server_socket.recv(auth_response);
      LOG("Response from auth_server: " + auth_response);
      FILELOG("Response from auth_server: " + auth_response);
      response = auth_response;
    }
    // Once the auth response it's been received it determines whether or not it
    // can handle a subsequent request
    else if (command[0] == "REQUEST" && auth_response == "OK\n") {
      db_server_socket.create();
      db_server_socket.connect("127.0.0.1", 5000);
      // db_server_socket.listen();
      db_server_socket.send(line);
      LOG("REQUEST line sent to db_server");
      FILELOG("REQUEST line sent to db_server");
      std::string db_response;
      db_server_socket.recv(db_response);
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
  return response;
}

// TODO(me): Ctrl + C
int main() {
  Socket server_socket, auth_server_socket, db_server_socket;

  // Set up the server socket
  if (!server_socket.create() || !server_socket.bind(3000, "127.0.0.1") ||
      !server_socket.listen()) {
    ERROR("Failed to setup server");
    FILELOG("Failed to setup server");
    return 1;
  }

  // // Connect to the auth server
  // if (!auth_server_socket.create() /*|| !auth_server_socket.connect("127.0.0.1", 4000)*/) {
  //   ERROR("Failed to connect to auth server");
  //   FILELOG("Failed to connect to auth server");
  //   return 1;
  // }

  // // Connect to the db server
  // if (!db_server_socket.create() /*|| !db_server_socket.connect("127.0.0.1", 5000)*/) {
  //   ERROR("Failed to connect to db server");
  //   FILELOG("Failed to connect to db server");
  //   return 1;
  // }

  // Main server loop
  while (true) {
    Socket client_socket;
    std::string response;
    if (server_socket.accept(client_socket)) {
      LOG("Accepted connection from client");
      FILELOG("Accepted connection from client");
      response =
          protocolGarrobo(client_socket, auth_server_socket, db_server_socket);
    }
    client_socket.send(response);
    LOG("Response sent to client");
    FILELOG("Response sent to client");
    client_socket.~Socket();
  }

  return 0;
}
