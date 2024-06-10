// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolaños, Andres
// Serrano>"
// Adapted from <https://www.geeksforgeeks.org/socket-programming-in-cpp/>

#include "Database.hpp"
#include "Socket.hpp"
#include "dbServerUtils.hpp"
#include <iostream>
#include <sstream>

Socket &server = Socket::getInstance();
Database &database = Database::getInstance("./src/medical_data.db");

std::string userDataRequest(const std::string &userId) {
  LOG("User data request ID: " + userId)
  std::string userData;

  // std::ostringstream oss;
  // Data obtained successfully
  std::string sqlStatement = "SELECT * FROM users WHERE patientID = '" + userId + "'";
  if (database.executeQuery(sqlStatement, Database::defaultCallback, (void*)&userData)) {
    // userData = oss.str();
  }
  return userData;
}

std::string insuranceStatusRequest(const std::string &userId) {
  std::string insuranceStatus = "";

  // Data obtained successfully
  if (database.executeQuery("SELECT isInsured FROM users WHERE patientID = '" +
                                userId + "'",
                            Database::defaultCallback, (void*)&insuranceStatus)) {
    // insuranceStatus = oss.str();
  }
  return insuranceStatus;
}

std::string labListRequest(const std::string &userId) {
  std::string labList = "";

  // Data obtained successfully
  if (database.executeQuery(
          "SELECT labDate, isPending FROM lab_results WHERE patientID = '" +
              userId + "'",
          Database::defaultCallback, (void*)&labList)) {
    // labList = oss.str();
  }
  return labList;
}

std::string labResultRequest(const std::string &labDate,
                             const std::string &userId) {
  std::string labResult = "";

  // Results from a certain lab exam
  if (database.executeQuery(
          "SELECT forms FROM lab_results WHERE patientID = '" + userId + "'" +
              " AND labDate = '" + labDate + "'",
          Database::defaultCallback, (void*)&labResult)) {
    // labResult = oss.str();
  }
  return labResult;
}

std::string patientsListRequest(const std::string &specialistId) {
  std::string patientsList = "";

  // Data obtained successfully
  if (database.executeQuery(
          "SELECT patientsList FROM 'assignations' WHERE specialistName = '" +
              specialistId + "'",
          Database::defaultCallback, (void*)&patientsList)) {
    // patientsList = oss.str();
  }
  return patientsList;
}

bool labResultInsertionRequest(const std::vector<std::string> &data) {
  // std::string userID = extractUserID(data[2]);
  std::cout << "Insertion Request data: " << data[2] << std::endl;
  LOG("INSERTION REQUEST: Inserting: " + data[2])
  // if (!checkInsertionFormat(data[2]) || userID.empty()) {
  //   LOG("INSERTION REQUEST: Regex failed")
  //   return false;
  // }
  const std::string firstQuery = "SELECT patientID FROM 'users' WHERE patientID = '" + data[2] + "'";
  
  if (!database.executeQuery(firstQuery, Database::defaultCallback, (void*)&data[2])) {
    LOG("INSERTION REQUEST: User is not in the database")
    return false;
  }
  LOG("First Query: " + firstQuery)

  const std::string query = "INSERT INTO lab_results(patientID, labDate, \
      isPending, forms) VALUES('" + data[2] + "', '" + data[3] + "', '" + data[4] + "', '" + data[5] +"')";

  bool result = database.executeQuery("BEGIN TRANSACTION", Database::defaultCallback, (void*)&data[2]);
  if(result) {
    LOG("TRANSACTION began successfully")
  } else {
    ERROR("TRANSACTION didn't began successfully")
  }

  result = database.executeQuery(query, Database::defaultCallback, (void*)&data[2]);
  LOG("Second Query: " + query)

  result = database.executeQuery("COMMIT", Database::defaultCallback, (void*)&data[2]);

  if(result) {
    LOG("COMMIT successfully execute")
  } else {
    ERROR("COMMIT failed")
  }
  return result;
}

// * Data insertion is only available for laboratorian as well as
// * insurance status is only available for patients.
const std::unordered_map<std::string, int> requestTypeMap = {
    {"USER_DATA", 1},     {"INSURANCE_STATUS", 2}, {"LAB_LIST", 3},
    {"LAB_RESULT", 4},      {"PATIENT_LIST", 5},     {"PATIENT_DATA", 6},
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
  case 1: { // c: REQUEST USER_DATA <userID>
    LOG("Request Type: USER_DATA")
    FILELOG("Request Type: USER_DATA");
    const std::string userData = userDataRequest(/*userID*/ command[2]);
    LOG("Requested user data:" + userData)
    FILELOG("Requested user data:" + userData);
    response = "OK\n" + userData + "\n";
    break;
  }
  case 2: { // c: REQUEST INSURANCE_STATUS <userID>
    LOG("Request Type: INSURANCE_STATUS");
    FILELOG("Request Type: INSURANCE_STATUS");
    const std::string insuranceStatus =
        insuranceStatusRequest(/*userID*/ command[2]);
    LOG("insuranceStatus is " + insuranceStatus);
    FILELOG("insuranceStatus is " + insuranceStatus);
    response = "OK\n" + insuranceStatus + "\n";
    break;
  }
  case 3: { // c: REQUEST LAB_LIST <userID>
    LOG("Request Type: LAB_LIST");
    FILELOG("Request Type: LAB_LIST");
    const std::string labList = labListRequest(/*userID*/ command[2]);
    response = "OK\n" + labList + "\n";
    LOG("Lab List is " << labList);
    FILELOG("Lab List is " << labList);
    break;
  }
  case 4: { // c: LAB_RESULT <labDate> <userID>
    LOG("Request Type: LAB_RESULT");
    FILELOG("Request Type: LAB_RESULT");
    const std::string labResult =
        labResultRequest(command[2] /*labDate*/, command[3] /*userID*/);
    response = "OK\n" + labResult + "\n";
    break;
  }
  case 5: { // c: REQUEST PATIENT_LIST <specialistID>
    LOG("Request Type: PATIENT_LIST");
    FILELOG("Request Type: PATIENT_LIST");
    const std::string patientsList =
        patientsListRequest(/*specialistID*/ command[2]);
    response = "OK\n" + patientsList + "\n";

    LOG("Patient List is : " + patientsList);
    FILELOG("Patient List is : " + patientsList);
    break;
  }
  case 6: { // c: REQUEST PATIENT_DATA <patientID>
    LOG("Request Type: PATIENT_DATA")
    FILELOG("Request Type: PATIENT_DATA");
    const std::string patientData = userDataRequest(/*patientID*/ command[2]);
    LOG("Requested patient data:" + patientData)
    FILELOG("Requested patient data:" + patientData);
    response = "OK\n" + patientData + "\n";
    break;
  }
  case 7: { // c: REQUEST DATA_INSERTION <dataToInsert>
    // data to insert MUST have this format: '777','hoy','No','BlahBlah'.
    // WITH THE SINGLE QUOTES INCLUDED
    LOG("Request type: DATA_INSERTION");
    FILELOG("Request type: DATA_INSERTION");
    if (labResultInsertionRequest(command)) {
      response = "OK\n";
    } else {
      response = "NOT_OK\n”";
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
  Database &database = Database::getInstance("./src/medical_data.db");
  database.signalHandler(signal, "./src/medical_data.db");
  exit(0);
}

int main() {
  std::signal(SIGINT, signalr);
  if (!server.create()) {
    ERROR("Unable to create server socket");
  } else if (!server.bind(5000, "127.0.0.1")) {
    ERROR("Unable to bind")
  } else if (!server.listen()) {
    ERROR("Couldn't listen")
  }
  while (true) {
    int clientSocket;
    if (server.accept(clientSocket)) {
      std::string clientRequest;
      server.receive(clientSocket, clientRequest);
      if (!clientRequest.empty()) {
        std::string response = protocolGarrobo(clientRequest);
        server.send(clientSocket, response);
        ::close(clientSocket);
      }
    }
  }
  return 0;
}
