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

std::string Request::requestLogin() {
    ClientSocket socket(8080, "127.0.0.1");
    std::string request = "INICIO\nLOGIN " + this->username + " " + this->hash;
    socket.send(request);
    std::string answer = socket.receive();
    std::string parsedAnswer = this->parse(answer);
    return parsedAnswer;
}

std::string Request::requestUserData() {
    ClientSocket socket(8080, "127.0.0.1");
    std::string request = "INICIO\nLOGIN " + this->username + " " + this->hash;
    request.append("\nREQUEST USER_DATA");

    socket.send(request);
    std::string answer = socket.receive();
    std::string parsedAnswer = this->parse(answer);

    return parsedAnswer;
}

std::string Request::requestInsuranceStatus(/*std::string id*/) {
    ClientSocket socket(8080, "127.0.0.1");
    std::string request = "INICIO\nLOGIN " + this->username + " " + this->hash;
    request.append("\nREQUEST INSURANCE_STATUS "/* + id*/);

    socket.send(request);
    std::string answer = socket.receive();
    std::string parsedAnswer = this->parse(answer);

    return parsedAnswer;
}

std::string Request::requestLabList() {
    ClientSocket socket(8080, "127.0.0.1");
    std::string request = "INICIO\nLOGIN " + this->username + " " + this->hash;
    request.append("\nREQUEST LAB_LIST");

    socket.send(request);
    std::string answer = socket.receive();
    std::string parsedAnswer = this->parse(answer);

    return parsedAnswer;
}

std::string Request::requestLabResult(int resultNumber) {
    ClientSocket socket(8080, "127.0.0.1");
    std::string request = "INICIO\nLOGIN " + this->username + " " + this->hash;
    request.append("\nREQUEST LAB_RESULT " + std::to_string(resultNumber));

    socket.send(request);
    std::string answer = socket.receive();
    std::string parsedAnswer = this->parse(answer);

    return parsedAnswer;
}

void Request::setUsername(const std::string& username) {
    this->username = username;
}

void Request::setHash(const std::string& hash) {
    this->hash = hash;
}
