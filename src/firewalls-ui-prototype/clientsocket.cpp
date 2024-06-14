// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos,
// Andres Serrano>"

#include "clientsocket.h"
#include <QHostAddress>
#include <QByteArray>

ClientSocket::ClientSocket(unsigned int port, const std::string& address) {
    socket.connectToHost(QHostAddress(QString::fromStdString(address)), port);
}

ClientSocket::~ClientSocket() {
    socket.close();
}

bool ClientSocket::connect() {
    return socket.waitForConnected(3000); // Wait 3 secs to connect
}

qint64 ClientSocket::send(const std::string& message) {
    QByteArray data = QByteArray::fromStdString(message);
    return socket.write(data);
}

std::string ClientSocket::receive() {
    if (socket.waitForReadyRead(3000)) { // Wait 3 secs to receive data
        QByteArray data = socket.readAll();
        return std::string(data.constData(), data.size());
    }
    return std::string();
}
