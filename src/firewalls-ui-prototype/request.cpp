#include "request.h"
#include "clientsocket.h"

#include <QFile>
#include <QDir>

std::string Request::parse(std::string message) {
  size_t messageSize = message.size();
  for (size_t i = 0; i < messageSize; ++i) {
    if (message[i] == '\t') {
      message[i] = '\n';
    }
  }
  return message;
}

void Request::setup() {
    bool statusOK = true;
    std::string key = getKey("../../Key.txt");
    ConfigData data = getServerData("../../../serverCommon/IP-addresses.txt", "Redirector");

    this->cipher.setKey(key);
    this->setRedirectorServerIDAndPort(data.ip, data.port);

    qInfo() << "Read key: " + key + "\n";
    qInfo() << "Read ip " + data.ip + " \n";
    qInfo() << "Read port: " + std::to_string(data.port) + "\n";

    if (key.empty()) {
        qInfo() << "Key is empty or couldn't be read\n";
        statusOK = false;
    }

    if (!validIP(data.ip)) {
        qInfo() << "Invalid IP " + data.ip + "\n";
        statusOK = false;
    }

    if (data.port == 0 || data.port == 65535) {
        qInfo() << "Possibly invalid port " + std::to_string(data.port) + "\n";
    }

    this->ready = statusOK;
    return;
}

bool Request::isReady() {
    return this->ready;
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
  if(this->ready) {
    ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
    const std::string request =
        "AUTH " + this->username + " " + this->hash + "\n";
    std::string requestCopy = request;
    this->cipher.encrypt(requestCopy, requestCopy);
    socket.send(requestCopy);
    std::string answer = socket.receive();
    this->cipher.decrypt(answer, answer);
    std::string parsedAnswer = this->parse(answer);
    return parsedAnswer;
  } else {
    return "";
  }
}

std::string Request::requestUserData() {
  if(this->ready){
    ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
    const std::string request = "AUTH " + this->username + " " + this->hash +
                                "\n" + "REQUEST USER_DATA " + this->username +
                                "\n";
    std::string requestCopy = request;
    this->cipher.encrypt(requestCopy, requestCopy);
    socket.send(requestCopy);
    std::string answer = socket.receive();
    this->cipher.decrypt(answer, answer);
    std::string parsedAnswer = this->parse(answer);
    return parsedAnswer;
  } else {
    return "";
  }
}

std::string Request::requestPatientData(const std::string &patientID) {
  if (this-ready) {
    ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
    const std::string request = "AUTH " + this->username + " " + this->hash +
                                "\n" + "REQUEST USER_DATA " + patientID + "\n";

    std::string requestCopy = request;
    this->cipher.encrypt(requestCopy, requestCopy);
    socket.send(requestCopy);
    std::string answer = socket.receive();
    this->cipher.decrypt(answer, answer);
    std::string parsedAnswer = this->parse(answer);

    return parsedAnswer;
  } else {
    return "";
  }

}

std::string Request::requestInsuranceStatus(const std::string &id) {
  if (this->ready) {
    ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
    const std::string request = "AUTH " + this->username + " " + this->hash +
                                "\n" + "REQUEST INSURANCE_STATUS " + id + "\n";

    std::string requestCopy = request;
    this->cipher.encrypt(requestCopy, requestCopy);
    socket.send(requestCopy);
    std::string answer = socket.receive();
    this->cipher.decrypt(answer, answer);
    std::string parsedAnswer = this->parse(answer);

    return parsedAnswer;
  } else {
    return "";
  }

}

std::string Request::requestLabList() {
  if(this->ready) {
    ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
    const std::string request = "AUTH " + this->username + " " + this->hash +
                                "\n" + "REQUEST LAB_LIST " + this->username +
                                "\n";

    std::string requestCopy = request;
    this->cipher.encrypt(requestCopy, requestCopy);
    socket.send(requestCopy);
    std::string answer = socket.receive();
    this->cipher.decrypt(answer, answer);
    std::string parsedAnswer = this->parse(answer);

    return parsedAnswer;
  } else {
    return "";
  }

}

std::string Request::requestLabResult(const std::string &labDate) {
  if(this->ready) {
    ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
    const std::string request = "AUTH " + this->username + " " + this->hash +
                                "\n" + "REQUEST LAB_RESULT " + labDate + " " +
                                this->username + "\n";

    std::string requestCopy = request;
    this->cipher.encrypt(requestCopy, requestCopy);
    socket.send(requestCopy);
    std::string answer = socket.receive();
    this->cipher.decrypt(answer, answer);
    std::string processedAnswer = removeServerConfirmationResponse(answer);
    std::string parsedAnswer = this->parse(processedAnswer);

    return parsedAnswer;
  } else {
    return "";
  }
}

std::string Request::requestPatientList() {
  if (this->ready) {
    ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
    const std::string request = "AUTH " + this->username + " " + this->hash +
                                "\n" + "REQUEST PATIENT_LIST " + this->username +
                                "\n";

    std::string requestCopy = request;
    this->cipher.encrypt(requestCopy, requestCopy);
    socket.send(requestCopy);
    std::string answer = socket.receive();
    this->cipher.decrypt(answer, answer);
    std::string parsedAnswer = this->parse(answer);

    return parsedAnswer;
  } else {
    return "";
  }
}

bool Request::requestDataInsertion(const std::string &data) {
  if (this->ready) {
    ClientSocket socket(this->redirectorServerPort, this->redirectorServerIP);
    const std::string request = "AUTH " + this->username + " " + this->hash +
                                "\n" + "REQUEST DATA_INSERTION " + data + "\n";

    std::string requestCopy = request;
    this->cipher.encrypt(requestCopy, requestCopy);
    socket.send(requestCopy);
    std::string answer = socket.receive();
    this->cipher.decrypt(answer, answer);
    std::string parsedAnswer = this->parse(answer);

    return true;
  } else {
    return "";
  }

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


