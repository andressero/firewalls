#ifndef USERVALIDATION_H
#define USERVALIDATION_H

#include <QString>
#include <QStringList>
#include <string>
#include <functional>
#include "filesystem.h"
#include "userdata.h"


QString myHash(const std::string& password, const std::string& salt);

UserData* tokenizer(QString usersFile, int* size) {
    QStringList lines = usersFile.split("\n");
    UserData* users = new UserData[lines.size()];
    int i = 0;
    for (QString& line: lines) {
        QStringList userData = line.split(",");
        if (line.size() == 4) {
            int state = (int) userData[3].toStdString()[0] - '0';

            users[i].userID = userData[0];
            users[i].passwordHash = userData[1];
            users[i].salt = userData[2];
            users[i].rightState = state;

            ++i;
        }
    }
    *size = lines.size();
    return users;
}


bool validateLogin(FileSystem* fs, const QString& username, const QString& password) {
    fs->open("Users");
    fs->change2ReadMode("Users");
    QString file(fs->read("Users", 1593).c_str());

    int usersSize = -1;
    UserData* users = tokenizer(file, &usersSize);

    for (int i = 0; i < usersSize; ++i) {
        if (users[i].userID == username) {
            if (users[i].passwordHash == myHash(password.toStdString(), users[i].salt.toStdString())) {
                return true;
            }
        }
    }

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
