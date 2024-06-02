#include "socket.h"

class ClientSocket: public Socket {
public:

    ClientSocket(short port = 8080, std::string address = std::string("0.0.0.0"));
    int connect(sockaddr_in& address);
    virtual int send(std::string message);
    virtual std::string receive();
};
