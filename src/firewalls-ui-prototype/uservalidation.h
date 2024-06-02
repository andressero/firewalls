#ifndef USERVALIDATION_H
#define USERVALIDATION_H

#include <QString>
#include <QStringList>
#include <string>
#include <functional>
#include "filesystem.h"
#include "userdata.h"
#include "clientsocket.h"
#include "sha256.h"

#include <QDebug>


QString myHash(const std::string& password, const std::string& salt);
QString sha256Hash(const QString& password, const QString& salt);
std::string sha256ToString(BYTE* hash);

UserData* tokenizer(QString usersFile, int* size) {
    QStringList lines = usersFile.split("\n");
    UserData* users = new UserData[lines.size()];
    int i = 0;
    for (QString& line: lines) {
        qInfo() << "Creating user: " << line << "\n";
        QStringList userData = line.split(",");
        qInfo() << "Line size: " << line.size() << "\n";
        if (userData.size() == 5) {
            int state = (int) userData[3].toStdString()[0] - '0';

            users[i].userID = userData[0];
            users[i].passwordHash = userData[1];
            users[i].salt = userData[2];
            users[i].rightState = state;
            qInfo() << "User " << i << " hash: " << users[i].passwordHash << "\n";
            ++i;
        }
    }
    *size = lines.size();
    return users;
}


bool validateLogin(FileSystem* fs, const QString& username, const QString& password) {
    fs->open("Users");
    fs->change2ReadMode("Users");
    QString file(fs->read("Users", 279).c_str());
    fs->close("Users");

    qInfo() << "Read from file: "<< file << "\n";

    int usersSize = -1;
    UserData* users = tokenizer(file, &usersSize);

    for (int i = 0; i < usersSize; ++i) {
        qInfo() << "Users: " << users[i].userID << " == " << username << "?\n";
        if (users[i].userID == username) {
            QString hash = myHash(password.toStdString(), users[i].salt.toStdString());
            qInfo() << "password: " << password << "\t salt: " << users[i].salt << "\n";
            qInfo() << "Hash: " << hash << " == " << users[i].passwordHash << "?\n";
            if (users[i].passwordHash == hash) {
                return true;
            }
        }
    }

    delete[] users;
    // for (UserData& user: users) {
    //     if (user.userID == username) {
    //         if (user.passwordHash == myHash(password.toStdString(), salt.toStdString())) {
    //             return true;
    //         }
    //     }
    // }

    return false;

    // for (QString& line: lines) {
    //     QStringList userData = line.split(",");
    //     if (userData[0] == username) {
    //         QString salt = userData[2];
    //         unsigned char hashArray[64];
    //         if (myHash(password.toStdString(), salt.toStdString())) {
    //             QString hash(hashArray);
    //             if (hash == userData[1]) {
    //                 return true;
    //             }
    //         }
    //     }
    // }
    // return false;
}

bool validateLogin(const QString& username, const QString& password) {
    ClientSocket socket(8080, "127.0.0.1");

    // Connect to server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    socket.connect(serverAddress);

    socket.send(std::string("INICIO"));
    QString answer(socket.receive().c_str());
    qInfo() << "ValidateLogin: " << answer << "\n";
    if (answer == "NOT_OK") {
        // Tell user login is not available right now?
        return false;
    }

    socket.send(std::string("REQUEST SALT " + username.toStdString()));

    QString salt(socket.receive().c_str());
    qInfo() << "ValidateLogin: " << answer << "\n";
    if (salt.size() == 0 || salt == "NOT_OK") {
        // Tell user something went wrong with the login procedure
        return false;
    }



    QString hash = sha256Hash(password, salt);

    socket.send(std::string("LOGIN " + username.toStdString() + " " + hash.toStdString()));

    QString loginStatus(socket.receive().c_str());
    qInfo() << "ValidateLogin: " << answer << "\n";
    if (loginStatus == "NOT_OK") {
        // Tell user wrong username or password
        return false;
    }

    return true;

}

QString sha256Hash(const QString& password, const QString& salt) {
    std::string passwordAndSalt = password.toStdString() + salt.toStdString();

    BYTE *input =  new BYTE[SHA256_BLOCK_SIZE];
    input = reinterpret_cast<BYTE*>(&passwordAndSalt[0]);

    BYTE hash[SHA256_BLOCK_SIZE];
    SHA256_CTX context;
    sha256_init(&context);
    sha256_update(&context, input, passwordAndSalt.size());
    sha256_final(&context, hash);
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

QString myHash(const std::string& password, const std::string& salt) {
    std::size_t h1 = std::hash<std::string>{}(password);
    std::size_t h2 = std::hash<std::string>{}(salt);
    std::size_t hash1 = h1 ^ (h2 << 1);
    std::size_t hash2 = h2 ^ (h1 << 1);
    std::string final = std::to_string(hash1);
    final = final.append(std::to_string(hash2));
    h2 = std::hash<std::string>{}(final);
    std::size_t hash3 = h1 ^ (h2 << 1);
    return QString(final.append(std::to_string(hash3)).c_str());
}

#endif // USERVALIDATION_H
