// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"
#ifndef SESSION_HPP
#define SESSION_HPP

#include <cstring>
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

class Session {
 private:
  std::string user; 
  std::string hash;
  bool loggedIn;
 public:
  Session(): loggedIn(true) {};
  ~Session() {};

  int userDataRequest();
  int insuranceStatusRequest();
  int labListRequest();
  int labResultRequest();
  void logout();

};

#endif#