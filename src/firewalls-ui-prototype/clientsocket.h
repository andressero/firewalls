// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos,
// Andres Serrano>"

#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QTcpSocket>
#include <string>

class ClientSocket {
private:
    QTcpSocket socket;

public:
    ClientSocket(unsigned int port, const std::string& address);
    ~ClientSocket();

    bool connect();
    qint64 send(const std::string& message);
    std::string receive();
};

#endif
