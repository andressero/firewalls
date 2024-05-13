#include "userdata.h"

UserData::UserData(const QString& userID, const QString& passwordHash,
                   const QString& salt, const int& rightState):
                    userID(userID),
                    passwordHash(passwordHash),
                    salt(salt),
                    rightState(rightState) {

}

UserData::UserData(): userID(""), passwordHash(""), salt(""), rightState(0) {

}


