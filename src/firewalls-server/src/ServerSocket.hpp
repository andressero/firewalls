#include "Socket.hpp"

class ServerSocket: public Socket {
 private:
  int clientFileDescriptor;
  int bind();
 public:
  ServerSocket(int port, std::string address);

  int listen(int& requests);
  int accept();
  virtual int send(std::string message);
  virtual std::string receive();

};