#include "request.h"
#include "clientsocket.h"

std::string Request::parse(std::string message) {
  size_t messageSize = message.size();
  for (size_t i = 0; i < messageSize; ++i) {
    if (message[i] == '\t') {
      message[i] = '\n';
    }
  }
  return message;
}

std::string
Request::removeServerConfirmationResponse(const std::string &receivedResponse) {
  std::string result = receivedResponse;
  const std::string okStr = "OK\n";

  size_t pos = 0;
  while ((pos = result.find(okStr, pos)) != std::string::npos) {
    result.erase(pos, okStr.length());
  }

  return result;
}

std::string Request::requestLogin() {
  ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
  const std::string request =
      "AUTH " + this->username + " " + this->hash + "\n";
  socket.send(request);
  std::string answer = socket.receive();
  std::string parsedAnswer = this->parse(answer);
  return parsedAnswer;
}

std::string Request::requestUserData() {
  ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
  const std::string request = "AUTH " + this->username + " " + this->hash +
                              "\n" + "REQUEST USER_DATA " + this->username +
                              "\n";

  socket.send(request);
  std::string answer = socket.receive();
  std::string parsedAnswer = this->parse(answer);

  return parsedAnswer;
}

std::string Request::requestPatientData(const std::string &patientID) {
  ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
  const std::string request = "AUTH " + this->username + " " + this->hash +
                              "\n" + "REQUEST USER_DATA " + patientID + "\n";

  socket.send(request);
  std::string answer = socket.receive();
  std::string parsedAnswer = this->parse(answer);

  return parsedAnswer;
}

std::string Request::requestInsuranceStatus(const std::string &id) {
  ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
  const std::string request = "AUTH " + this->username + " " + this->hash +
                              "\n" + "REQUEST INSURANCE_STATUS " + id + "\n";

  socket.send(request);
  std::string answer = socket.receive();
  std::string parsedAnswer = this->parse(answer);

  return parsedAnswer;
}

std::string Request::requestLabList() {
  ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
  const std::string request = "AUTH " + this->username + " " + this->hash +
                              "\n" + "REQUEST LAB_LIST " + this->username +
                              "\n";

  socket.send(request);
  std::string answer = socket.receive();
  std::string parsedAnswer = this->parse(answer);

  return parsedAnswer;
}

std::string Request::requestLabResult(const std::string &labDate) {
  ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
  const std::string request = "AUTH " + this->username + " " + this->hash +
                              "\n" + "REQUEST LAB_RESULT " + labDate + " " +
                              this->username + "\n";

  socket.send(request);
  std::string answer = socket.receive();
  std::string processedAnswer = removeServerConfirmationResponse(answer);
  std::string parsedAnswer = this->parse(processedAnswer);

  return parsedAnswer;
}

void Request::setUsername(const std::string &username) {
  this->username = username;
}

void Request::setHash(const std::string &hash) { this->hash = hash; }

void Request::setRedirectorServerIDAndPort(
    const std::string &redirectorServerIP, const int redirectorServerPort) {
  this->redirectorServerIP = redirectorServerIP;
  this->redirectorServerPort = redirectorServerPort;
}

std::string Request::requestPatientList() {
  ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
  const std::string request = "AUTH " + this->username + " " + this->hash +
                              "\n" + "REQUEST PATIENT_LIST " + this->username +
                              "\n";

  socket.send(request);
  const std::string answer = socket.receive();
  const std::string parsedAnswer = this->parse(answer);
  return parsedAnswer;
}

bool Request::requestDataInsertion(const std::string &data) {
  ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
  const std::string request = "AUTH " + this->username + " " + this->hash +
                              "\n" + "REQUEST DATA_INSERTION " + data + "\n";

  socket.send(request);
  const std::string answer = socket.receive();
  const std::string parsedAnswer = this->parse(answer);
  return true;
}
