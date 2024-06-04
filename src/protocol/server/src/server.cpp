// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"
// Taken from <https://www.geeksforgeeks.org/socket-programming-in-cpp/>

#include "Socket.hpp"
#include "Logger.hpp"

int protocolGarrobo(char* command);

int main() {
  Socket server(8080, "0.0.0.0");
  server.bind();
  server.listen(5);
  server.accept();

  Logger logger;

  logger.log("Server accepted a client (1/5)", "server_log.txt");

  std::string mensaje = server.receive(server.getClientFileDescriptor());
  logger.log("Server received:\n" + mensaje, "server_log.txt");

  int answer = protocolGarrobo(&mensaje[0]);
  std::string response(std::to_string(answer));
  logger.log("Server responded:\n" + response, "server_log.txt");
  server.send(server.getClientFileDescriptor(), response);

  return 0;
}

// TODO(all): Use log to output messages 
// or faulty states.
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
