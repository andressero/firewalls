// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>"
// Adapted from <https://www.geeksforgeeks.org/socket-programming-in-cpp/>

#include "Database.hpp"
#include "Socket.hpp"
#include "dbServerUtils.hpp"
#include <iostream>
#include <sstream>

Socket &server = Socket::getInstance();
Database &database = Database::getInstance("../../medical_data.db");

std::string userDataRequest(const std::string &userId) {
  std::string userData = "";

  std::ostringstream oss;
  // Data obtained successfully
  if (database.executeQuery("SELECT * FROM users WHERE patientID = '" + userId +
                                "'",
                            Database::defaultCallback, &oss)) {
    userData = oss.str();
  }
  return userData;
}

std::string insuranceStatusRequest(const std::string &userId) {
  std::string insuranceStatus = "";

  std::ostringstream oss;
  // Data obtained successfully
  if (database.executeQuery("SELECT isInsured FROM users WHERE patientID = '" +
                                userId + "'",
                            Database::defaultCallback, &oss)) {
    insuranceStatus = oss.str();
  }
  return insuranceStatus;
}

std::string labListRequest(const std::string &userId) {
  std::string labList = "";

  std::ostringstream oss;
  // Data obtained successfully
  if (database.executeQuery(
          "SELECT labDate, isPending FROM lab_results WHERE patientID = '" +
              userId + "'",
          Database::defaultCallback, &oss)) {
    labList = oss.str();
  }
  return labList;
}

std::string labResultRequest(const std::string &labDate,
                             const std::string &userId) {
  std::string labResult = "";

  std::ostringstream oss;
  // Results from a certain lab exam
  if (database.executeQuery(
          "SELECT forms FROM lab_results WHERE patientID = '" + userId + "'" +
              " AND labDate = '" + labDate + "'",
          Database::defaultCallback, &oss)) {
    labResult = oss.str();
  }
  return labResult;
}

std::string patientsListRequest(const std::string &specialistId) {
  std::string patientsList = "";

  std::ostringstream oss;
  // Data obtained successfully
  if (database.executeQuery(
          "SELECT patientsList FROM 'assignations' WHERE specialistName = '" +
              specialistId + "'",
          Database::defaultCallback, &oss)) {
    patientsList = oss.str();
  }
  return patientsList;
}

bool labResultInsertionRequest(const std::string &data) {
  return database.executeQuery("INSERT INTO lab_results(patientID, labDate, "
                               "isPending, forms) VALUES('" +
                                   data + ")",
                               Database::defaultCallback);
}

// * Data insertion is only available for laboratorian as well as
// * insurance status is only available for patients.
const std::unordered_map<std::string, int> requestTypeMap = {
    {"USER_DATA", 1},     {"INSURANCE_STATUS", 2}, {"LAB_RESULT", 3},
    {"LAB_LIST", 4},      {"PATIENT_DATA", 5},     {"PATIENT_LIST", 6},
    {"DATA_INSERTION", 7}};

void getRequestResponse(const std::vector<std::string> &command,
                        std::string &response) {
  const std::string requestType_ = command[1];
  if (requestTypeMap.find(requestType_) == requestTypeMap.end()) {
    ERROR(requestType_ + " is an unknown request type")
    response = "ERROR\n";
    return;
  }

  switch (requestTypeMap.find(requestType_)->second) {
  case 1: { // USER_DATA
    LOG("Request Type: USER_DATA")
    // SELECT * FROM users WHERE patientID = '123456789'
    const std::string userData = userDataRequest(/*userID*/ command[2]);
    LOG("userData is" + userData)
    response += "OK\n" + userData + "\n";
    break;
  }
  case 2: { // INSURANCE_STATUS
    LOG("Request Type: INSURANCE_STATUS")
    // SELECT isInsured FROM users WHERE patientID = '123456789'
    const std::string insuranceStatus =
        insuranceStatusRequest(/*userID*/ command[2]);
    LOG("insuranceStatus is " + insuranceStatus)
    response += "OK\n" + insuranceStatus + "\n";
    break;
  }
  case 3: { // LAB_LIST
    // SELECT labDate, isPending FROM lab_results WHERE patientID = '123456789'
    LOG("Request Type: LAB_LIST")
    const std::string labList = labListRequest(/*userID*/ command[2]);
    break;
  }
  case 4: { // LAB_RESULT
    // SELECT forms FROM lab_results WHERE patientID = '123456789' AND labDate =
    // '31-07-2025'
    LOG("Request Type: LAB_RESULT")
    const std::string labResultDate = command[2];
    const std::string labResult =
        labResultRequest(labResultDate, command[3] /*userID*/);
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
    // SELECT patientsList FROM 'assignations' WHERE specialistName = 'L12345'
    // TODO(any): Implement this request
    break;
  }
  case 6: { // PATIENT_DATA
    /*c(L): REQUEST PATIENT_DATA <patientID>*/
    // SELECT * FROM users WHERE patientID = '123456789'
    // TODO(any): Implement this request
    break;
  }
  case 7: { // DATA_INSERTION
    /*c(L): REQUEST DATA_INSERTION <dataToInsert>*/
    // INSERT INTO lab_results(patientID, labDate, isPending, forms)
    // VALUES('666', 'hoy', 'No', 'BlahBlah')
    //  TODO(any): Implement this request
    // data to insert MUST have this format: '666', 'hoy', 'No', 'BlahBlah'
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
  std::string response = "NOT_OK\n";
  const std::vector<std::string> command = splitString(input, " ");
  if (command[0] != "REQUEST") {
    LOG("Received unknown command");
    FILELOG("Received unknown command");
    return response;
  }
  getRequestResponse(command, response);
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
