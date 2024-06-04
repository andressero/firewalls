// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"
// Taken from <https://www.geeksforgeeks.org/socket-programming-in-cpp/>

#include "Socket.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> splitString(const std::string& input, const std::string& delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;
 
    while (std::getline(iss, token, delimiter[0])) {
        tokens.push_back(token);
    }
 
    return tokens;
}

int protocolGarrobo(std::string command) {
    std::vector<std::string> tokens = splitString(command, " \n");

    if (tokens[0] == "INICIO") {
        std::cout << "OK\n";
        return 1;
    } else if (tokens[0] == "LOGIN") {
        std::string userToken = tokens[1];
        std::string hashToken = tokens[2];
        // return handleLogin(userToken, hashToken);
        //TODO(Any): Change into an object (session) so you can access the requests for requests
        //TODO(Any): Add flag to give the user logged permissions
    } else if (tokens[0] == "REQUEST") {
        //! This must go inside the object session to make sure the user can only see themselves
        //TODO(Any): If user is verified allow the following
        std::string requestToken = tokens[1];
        if (requestToken == "USER_DATA") {
            //TODO(Any): There must be a way to allowed the user to only see themselves
        } else if (requestToken == "INSURANCE_STATUS") {
            std::string idToken = tokens[2];
            // return insuranceStatusRequest();
        } else if (requestToken == "LAB_LIST") {
            //TODO(Any): There must be a way to allowed the user to only see themselves
        } else if (requestToken == "LAB_RESULT") {
            std::string labIdToken = tokens[2];
            //TODO(Any): ---
        } else {
            std::cerr << "Error: "<< requestToken << " is an unknown request\n";
        }
    } else if (tokens[0] == "LOGOUT") {
        //TODO(Any): Change flag to remove logged permissions from user
    } else if (tokens[0] == "QUIT") {
        //TODO(Any): ---
    } else {
        std::cerr << "Error: "<< tokens[0] << " is an unknown command\n";
    }
    return -1;
}

int handleLogin(char* userToken, char* hashToken) {
  //TODO(Any): implement user verification the same way as was done previously
  // return 2 : if Login Succesful
  // return -2 : if login Unsuccessful
}


int main() {
  Socket server(8080, "0.0.0.0");
  server.bind();
  server.listen(5);
  server.accept();

  std::string mensaje = server.receive(server.getClientFileDescriptor());
  std::cout << mensaje << std::endl;

  int answer = protocolGarrobo(mensaje);
  std::string response(std::to_string(answer));
  server.send(server.getClientFileDescriptor(), response);

  return 0;
}
