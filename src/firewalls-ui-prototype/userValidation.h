#ifndef USERVALIDATION_H
#define USERVALIDATION_H

#include <argon2.h>
#include <QString>
#include <QStringList>
#include "filesystem.h"

bool validateLogin(const FileSystem& fs, const QString& username, const QString& password) {
    fs.open("Users");
    fs.change2ReadMode("Users");
    QString file = fs.read("Users", 1593);
    QStringList lines = file.split("\n");

    for (QString& line: lines) {
        QStringList userData = line.split(",");
        if (userData[0] == username) {
            QString salt = userData[2];
            unsigned char hashArray[64];
            if (argon2id_hash_raw(20, (1 << 16), 2, password, password.size(), salt, salt.size(), hash, sizeof(hash)) == ARGON2_OK) {
                QString hash(hashArray);
                if (hash == userData[1]) {
                    return true;
                }
            }
        }
    }
    return false;
}

// if (argon2i_hash_raw(t_cost, m_cost, parallelism, password, password_len, salt, salt_len, hash, sizeof(hash)) == ARGON2_OK) {
//     // Convert hash to QString
//     QByteArray byteArray(reinterpret_cast<const char*>(hash), sizeof(hash));
//     QString hashedPassword = QString::fromLatin1(byteArray.toHex());

//     std::cout << "Hashed password: " << hashedPassword.toStdString() << std::endl;
// }

#endif // USERVALIDATION_H

