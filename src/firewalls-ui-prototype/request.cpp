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
    ClientSocket socket(3000, "127.0.0.1");
    const std::string request = "AUTH " + this->username + " " + this->hash + "\n";
    socket.send(request);
    std::string answer = socket.receive();
    std::string parsedAnswer = this->parse(answer);
    return parsedAnswer;
}

std::string Request::requestUserData() {
    ClientSocket socket(3000, "127.0.0.1");
    const std::string request = "AUTH " + this->username + " " + this->hash + "\n" +\
                                "REQUEST USER_DATA " + this->username + "\n";

    socket.send(request);
    std::string answer = socket.receive();
    std::string parsedAnswer = this->parse(answer);

    return parsedAnswer;
}

std::string Request::requestInsuranceStatus(const std::string& id) {
    ClientSocket socket(3000, "127.0.0.1");
    const std::string request = "AUTH " + this->username + " " + this->hash + "\n" + \
    "REQUEST INSURANCE_STATUS " + id + "\n";

    socket.send(request);
    std::string answer = socket.receive();
    std::string parsedAnswer = this->parse(answer);

    return parsedAnswer;
}

std::string Request::requestLabList() {
    ClientSocket socket(3000, "127.0.0.1");
    const std::string request = "AUTH " + this->username + " " + this->hash + "\n" +\
    "REQUEST LAB_LIST " + this->username + "\n";

    socket.send(request);
    std::string answer = socket.receive();
    std::string parsedAnswer = this->parse(answer);

    return parsedAnswer;
}

std::string Request::requestLabResult(const std::string& labDate) {
    ClientSocket socket(3000, "127.0.0.1");
    const std::string request = "AUTH " + this->username + " " + this->hash + "\n" +\
    "REQUEST LAB_RESULT " + labDate + " " + this->username + "\n";

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
