// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>"
// Adapted from <https://www.geeksforgeeks.org/socket-programming-in-cpp/>

#include "FileSystem.hpp"
#include "Session.hpp"
#include "Socket.hpp"
#include "Sqlite.hpp"

Socket &server = Socket::getInstance();
FileSystem &fs4 = FileSystem::getInstance();
Sqlite &database = Sqlite::getInstance();

const std::unordered_map<std::string, int> relation = {
    {"INICIO", 1}, {"LOGIN", 2}, {"REQUEST", 3}, {"LOGOUT", 4}, {"QUIT", 5}};

const std::unordered_map<std::string, int> requestTypeMap = {
    {"USER_DATA", 1},
    {"INSURANCE_STATUS", 2},
    {"LAB_LIST", 3},
    {"LAB_RESULT", 4}};

void getRequestResponse(std::string &response,
                        const std::vector<std::string> &command,
                        Session *session) {
  if (!session) {
    LOG("No session object, NOT LOGGED IN YET")
    response = "ERROR\n";
    return;
  }
  if (!session->getLogStatus()) {
    LOG("NOT LOGGED IN YET")
    response = "ERROR\n";
    return;
  }
  const std::string requestType_ = command[1];
  if (requestTypeMap.find(requestType_) == requestTypeMap.end()) {
    ERROR(requestType_ + " is an unknown request type")
    response = "ERROR\n";
    return;
  }
  switch (requestTypeMap.find(requestType_)->second) {
  case 1: { // USER_DATA
    LOG("Request Type: USER_DATA")
    const std::string userData = session->userDataRequest();
    LOG("userData is" + userData)
    response += "OK\n" + userData + "\n";
    break;
  }
  case 2: { // INSURANCE_STATUS
    LOG("Request Type: INSURANCE_STATUS")
    const std::string insuranceStatus = session->insuranceStatusRequest();
    LOG("insuranceStatus is " + insuranceStatus)
    response += "OK\n" + insuranceStatus + "\n";
    break;
  }
  case 3: { // LAB_LIST
    LOG("Request Type: LAB_LIST")
    const std::vector<LabResult> labList = session->labListRequest();
    response = "";
    for (const LabResult &i : labList) {
      LOG(i.toString())
      response += i.toString() + "\t";
    }
    response += "\n";
    break;
  }
  case 4: { // LAB_RESULT
    LOG("Request Type: LAB_RESULT")
    const std::string labResultID = command[2];
    const LabResult labResult = session->labResultRequest(labResultID);
    LOG("labResult is " << labResult.toString())
    if (labResult.empty()) {
      response = "ERROR\n";
    } else {
      response += "OK\n" + labResult.toString() + "\n";
    }
    break;
  }
  default:
    response = "ERROR\n";
    ERROR(requestType_ + " is an unknown request type")
    break;
  }
  return;
}

std::string protocolGarrobo(const std::string &input) {
  const std::vector<std::string> lines = splitString(input, "\n");
  std::string response = "ERROR\n";
  Session *session = 0;

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
    case 1: // INICIO
      LOG("Operation: INICIO")
      response = "OK\n";
      break;
    case 2: { // LOGIN
      LOG("Operation: LOGIN\n")
      if (session) {
        LOG("Already existing session object, Already LOGGED IN!")
        return "ERROR\n";
      }
      const std::string user = command[1];
      const std::string hashToken = command[2];
      ASSERT(hashToken.size() == 64);

      session = new Session(user, hashToken);
      const bool loginStatus = session->tryLogin();
      if (loginStatus) {
        LOG("Login Succesful")
        response = "OK\n";
      } else {
        LOG("Login Failed")
        response = "ERROR\n";
        delete session;
        session = 0;
      }
      break;
    }
    case 3: // REQUEST
      LOG("Operation: REQUEST")
      getRequestResponse(response, command, session);
      break;
    case 4: // LOGOUT FALLTHROUGH
      LOG("Operation LOGOUT")
      [[fallthrough]];
    case 5: // QUIT
      LOG("QUIT")
      if (session) {
        delete session;
        session = 0;
      }
      break;
    default:
      response = "ERROR\n";
      ERROR(operation + " is an unknown operation!");
      break;
    }
  }
  if (session) {
    delete session;
    session = 0;
  }
  return response;
}

static void signalr(int signal) {
  std::cout << "Received signal " << signal << ". Releasing resources..."
            << std::endl;
  Socket &server = Socket::getInstance();
  server.signalHandler(signal);
  Sqlite &database = Sqlite::getInstance();
  database.signalHandler(signal);
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
