#ifndef USERDATA_H
#define USERDATA_H

#include <QString>
#include <QStringList>

class UserData {
 public:
    QString userID;
    QString passwordHash;
    QString salt;
    int rightState;
    UserData();

    UserData(const QString& userID, const QString& passwordHash,
             const QString& salt, const int& rightState);

 private:

};

#endif // USERDATA_H
