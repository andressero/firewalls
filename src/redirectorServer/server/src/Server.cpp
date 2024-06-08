// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>"
// Adapted from <https://www.geeksforgeeks.org/socket-programming-in-cpp/>

#include "Socket.hpp"
#include "redirectorUtils.hpp"

Socket &server = Socket::getInstance();

const std::unordered_map<std::string, int> relation = {{"AUTH", 1},
                                                       {"REQUEST", 2}};

// * Data insertion is only available for laboratorian as well as
// * insurance status is only available for patients.
const std::unordered_map<std::string, int> requestTypeMap = {
    {"USER_DATA", 1},     {"INSURANCE_STATUS", 2}, {"LAB_LIST", 3},
    {"LAB_RESULT", 4},    {"PATIENT_LIST", 5},     {"PATIENT_DATA", 6},
    {"DATA_INSERTION", 7}};

std::string protocolGarrobo(const std::string &input) {
  const std::vector<std::string> lines = splitString(input, "\n");
  std::string response = "NOT_OK\n";
  /*Each client message got:
  <always>AUTH ...
  <whenever clients wants if logged in(auth successful)>REQUEST ...
  */
  for (const std::string &line : lines) {

    LOG("Line: "
        << "|" << line << "|")
    const std::vector<std::string> command = splitString(line, " ");
    const std::string operation = command[0];

    if (relation.find(operation) == relation.end()) {
      ERROR(operation + " is an unknown operation!");
      return "ERROR\n";
    }

    switch (relation.find(operation)->second) {
    case 1: { // AUTH
      LOG("Operation: AUTH\n")
      // TODO(any): send it to auth_server
      // response = auth_servers response
      break;
    }
    case 2: // REQUEST
      LOG("Operation: REQUEST")
      // TODO(any): send it to db_server
      // response = db_server response
      break;
    default:
      response = "ERROR\n";
      ERROR(operation + " is an unknown operation!");
      break;
    }
  }

  return response;
}

static void signalr(int signal) {
  std::cout << "Received signal " << signal << ". Releasing resources..."
            << std::endl;
  Socket &server = Socket::getInstance();
  server.signalHandler(signal);
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
