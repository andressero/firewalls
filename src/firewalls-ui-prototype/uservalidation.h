#ifndef USERVALIDATION_H
#define USERVALIDATION_H

#include <QString>
#include <QStringList>
#include <string>
#include "clientsocket.h"
#include "sha256.h"
#include "request.h"

#include <QDebug>

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

std::string sha256Hash(std::string& password) {

    BYTE *input =  new BYTE[SHA256_BLOCK_SIZE];
    input = reinterpret_cast<BYTE*>(&password[0]);

    BYTE hash[SHA256_BLOCK_SIZE];
    SHA256_CTX context;
    sha256_init(&context);
    sha256_update(&context, input, password.size());
    sha256_final(&context, hash);
    // delete[] input;
    return sha256ToString(hash);
}

bool validateLogin(std::string username, std::string password) {
    std::string hash = sha256Hash(password);

    Request& request = Request::getInstance();
    request.setUsername(username);
    request.setHash(hash);

    std::string answer = request.requestLogin();

    if(answer == "ERROR\n") {
        return false;
    }
    return true;
}

#endif // USERVALIDATION_H
