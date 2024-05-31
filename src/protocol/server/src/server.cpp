// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"
// Taken from <https://www.geeksforgeeks.org/socket-programming-in-cpp/>

#include "Socket.hpp"

int protocolGarrobo(char* command);

int main() {
  Socket socket;

  socket.listen();

  socket.accept();

  std::string mensaje = socket.receive();
  std::cout << mensaje << std::endl;

  int answer = protocolGarrobo(&mensaje[0]);
  std::string response(std::to_string(answer));
  socket.send(response);

  socket.close();
  /*
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  // specifying the address
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8080);
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  
  // binding socket.
  bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

  // listening to the assigned socket
  listen(serverSocket, 5);

  // accepting connection request
  int clientSocket = accept(serverSocket, nullptr, nullptr);

  timeval val;
  val.tv_sec = 60;
  setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &val, sizeof(val));

  while(true) {
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer << std::endl;

    int answer = protocolGarrobo(buffer);
    char respuesta[128] = {0};
    sprintf(respuesta, "%d", answer);
    send(clientSocket, respuesta, sizeof(answer), 0);
  }
  // recieving data
  // closing the socket.
  close(serverSocket);
  */

  return 0;
}

int protocolGarrobo(char* command) {
  char* commandToken = strtok(command, " \n");
  // while( commandToken != NULL ) {
  //   printf( " %s\n", commandToken ); //printing each commandToken
  //   commandToken = strtok(NULL, " \n");
  // }

  if (strncmp(commandToken, "INICIO", 7) == 0) {
    std::cout << "OK\n";
    return 1;
  } else if (strncmp(commandToken, "LOGIN", 6) == 0) {
    char* userToken = strtok(NULL, " \n");
    char* hashToken = strtok(NULL, " \n");
    // return handleLogin(userToken, hashToken);
    //TODO(Any): Change into an object (session) so you can access the requests for requests
    //TODO(Any): Add flag to give the user logged permissions
  } else if (strncmp(commandToken, "REQUEST", 8) == 0) {
    //! This must go inside the object session to make sure the user can only see themselves
    //TODO(Any): If user is verified allow the following
    char* requestToken = strtok(NULL, " \n");
    if (strncmp(requestToken, "USER_DATA", 10) == 0) {
      //TODO(Any): There must be a way to allowed the user to only see themselves
    } else if (strncmp(requestToken, "INSURANCE_STATUS", 17) == 0) {
      char* idToken = strtok(NULL, " \n");
      // return insuranceStatusRequest();
    } else if (strncmp(requestToken, "LAB_LIST", 9) == 0) {
      //TODO(Any): There must be a way to allowed the user to only see themselves

    } else if (strncmp(requestToken, "LAB_RESULT", 11) == 0) {
      char* labIdToken = strtok(NULL, " \n");
      //TODO(Any): ---

    } else {
      std::cerr << "Error: "<< requestToken << " is an unknown request\n";
    }
  } else if (strncmp(commandToken, "LOGOUT", 7) == 0) {
    //TODO(Any): Change flag to remove logged permissions from user
  } else if (strncmp(commandToken, "QUIT", 5) == 0) {

  } else {
    std::cerr << "Error: "<< commandToken << " is an unknown command\n";
  }
  return -1;
}

int handleLogin(char* userToken, char* hashToken) {
  //TODO(Any): implement user verification the same way as was done previously
  // return 2 : if Login Succesful
  // return -2 : if login Unsuccessful
}
