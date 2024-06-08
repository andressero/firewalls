// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>"
// Adapted from <https://www.geeksforgeeks.org/socket-programming-in-cpp/>

#include "Session.hpp"
#include "Socket.hpp"
#include "Sqlite.hpp"

Socket &server = Socket::getInstance();
Sqlite &database = Sqlite::getInstance();

// TODO(any): Remove INICIO and QUIT as both are unnecessary.
const std::unordered_map<std::string, int> relation = {
    {"INICIO", 1}, {"LOGIN", 2}, {"REQUEST", 3}, {"LOGOUT", 4}, {"QUIT", 5}};

// * Data insertion is only available for laboratorian as well as
// * insurance status is only available for patients.
const std::unordered_map<std::string, int> requestTypeMap = {
    {"USER_DATA", 1},     {"INSURANCE_STATUS", 2}, {"LAB_RESULT", 3},
    {"LAB_LIST", 4},    {"PATIENT_DATA", 5},     {"PATIENT_LIST", 6},
    {"DATA_INSERTION", 7}};

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
  case 5: { // PATIENT_LIST
    /*c(L): REQUEST PATIENT_LIST*/
    // TODO(any): Implement this request
    break;
  }
  case 6: { // PATIENT_DATA
    /*c(L): REQUEST PATIENT_DATA <patientID>*/
    // TODO(any): Implement this request
    break;
  }
  case 7: { // DATA_INSERTION
    /*c(L): REQUEST DATA_INSERTION <dataToInsert>*/
    // TODO(any): Implement this request
    break;
  }
  default:
    response = "ERROR\n";
    ERROR(requestType_ + " is an unknown request type")
    break;
  }
  return;
}

// TODO(any): Remove INICIO and QUIT as both are unnecessary.
/*
std::string protocolGarrobo(const std::string &input) {
  const std::vector<std::string> lines = splitString(input, "\n");
  std::string response = "ERROR\n";
  Session *session = 0;

  // TODO(any): Separate requests.
  for (const std::string &line : lines) {

    LOG("Line: " << "|" << line << "|")
    const std::vector<std::string> command = splitString(line, " ");
    const std::string operation = command[0];

    if (relation.find(operation) == relation.end()) {
      ERROR(operation + " is an unknown operation!");
      return "ERROR\n";
    }

    switch (relation.find(operation)->second) {
      case 1: // REQUEST
        LOG("Operation: REQUEST")
        getRequestResponse(response, command, session);
        break;
      case 2: // INSERT
        LOG("Operation: INSERT")
        getInsertResponse();
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
*/

Session* createSession(const std::string& request) {
  const std::vector<std::string> command = splitString(request, " ");

  if (command[0] == "CONNECTION") {
    return new Session(command[1], ""/*command[2]*/);
  }
  return nullptr;
}

std::string protocolGarrobo(const std::string& input) {
  const std::vector<std::string> requests = splitString(input, "\n");
  std::string response = "";
  Session* session = nullptr;
  if (requests.size() > 0) {
    session = createSession(requests[0]);
  } else {
    return "ERROR\n";
  }

  if (session == nullptr) {
    return "ERROR\n";
  }

  for (size_t i = 1; i < requests.size(); ++i) {
    const std::string& request = requests[i];
    const std::vector<std::string> command = splitString(request, " ");

    // command[0] == REQUEST always
    const std::string operation = command[1];

    if (requestTypeMap.find(operation) == relation.end()) {
      ERROR(operation + " is an unknown operation!")
      return response;
    }

    int requestNumber = requestTypeMap.find(operation)->second;

    switch (requestNumber) {
      case 1:{ // USER_DATA
        LOG("Operation: REQUEST USER_DATA")
        std::string userData = session->userDataRequest();
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
      case 3: { // LAB_RESULT
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
      case 4: { // LAB_LIST
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
      case 5: { // PATIENT_DATA
      // TODO(any): finish patient data request
      /*
        LOG("Request Type: PATIENT_DATA")
        const std::string patientID = command[2];
        const std::string patientData = session->patientDataRequest(patientID);
        LOG("patientData is " + patientData)
        response += "OK\n" + patientData + "\n";
      */
        break;
      }
      case 6: { // PATIENT_LIST
      // TODO(any): finish patient list request
        /*
        LOG("Request Type: PATIENT_LIST")
        const std::vector<std::string> patientList = session->patientListRequest();
        */
        break;
      }
      case 7: { // DATA_INSERTION
      // TODO(any): finish data insertion request
      /*
        LOG("Request Type: DATA_INSERTION")
        if (session->dataInsertion() == OK) {
          response += "OK\n";
        } else {
          response = "ERROR\n";
        }
      */
        break;
      }
    default:
      response = "ERROR\n";
      ERROR(operation + " is an unknown request type")
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
  Sqlite &database = Sqlite::getInstance();
  database.signalHandler(signal);
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
