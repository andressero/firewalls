#ifndef USERVALIDATION_H
#define USERVALIDATION_H

#include <QString>
#include <QStringList>
#include <string>
#include "clientsocket.h"
#include "sha256.h"

#include <QDebug>

QString sha256Hash(const QString& password);
std::string sha256ToString(BYTE* hash);

bool validateLogin(const QString& username, const QString& password) {
    ClientSocket socket(8080, "127.0.0.1");

    // Connect to server

    QString hash = sha256Hash(password);

    std::string message = "INICIO\nLOGIN " + username.toStdString() + " " + hash.toStdString();
    message.append("\nREQUEST USER_DATA\nREQUEST INSURANCE_STATUS");
    message.append("\nREQUEST LAB_LIST");


    socket.send(message);
    std::string answer = socket.receive();
    QString finalAnswer(answer.c_str());

    qInfo() << answer << "\n";

    if(answer == "ERROR\n") {
        return false;
    }

    return true;

    #if 0
    qInfo() << "ValidateLogin: " << answer << "\n";
    if (answer == "NOT_OK" || answer == "ERROR") {
        // Tell user login is not available right now?
        return false;
    }

    socket.send(std::string("LOGIN " + username.toStdString() + " " + hash.toStdString()));

    QString loginStatus(socket.receive().c_str());
    answer.removeLast();
    qInfo() << "ValidateLogin: " << answer << "\n";
    if (loginStatus == "NOT_OK") {
        // Tell user wrong username or password
        return false;
    }
    #endif

    return true;

}

QString sha256Hash(const QString& password) {
    std::string passwordString = password.toStdString();

    BYTE *input =  new BYTE[SHA256_BLOCK_SIZE];
    input = reinterpret_cast<BYTE*>(&passwordString[0]);

    BYTE hash[SHA256_BLOCK_SIZE];
    SHA256_CTX context;
    sha256_init(&context);
    sha256_update(&context, input, passwordString.size());
    sha256_final(&context, hash);
    // delete[] input;
    return QString(sha256ToString(hash).c_str());
}

// Maybe send to another .h file?
std::string sha256ToString(BYTE* hash) {
    std::string answer(2*SHA256_BLOCK_SIZE, '0');
    for (size_t i = 0; i < SHA256_BLOCK_SIZE; ++i) {
        char temp[3] = {0};
        snprintf(temp, 3, "%02x", hash[i]);
        answer[2*i] = temp[0];
        answer[2*i + 1] = temp[1];
    }
    return answer;
}

#endif // USERVALIDATION_H
