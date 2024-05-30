#include "Socket.hpp"

//TODO(any): Figure out why this client can't receive messages
int main() {
  Socket client(8080, "127.0.0.1");

  Socket server(8080, "0.0.0.0");
  client.connect(server.getSocketAddress());

  client.send("Hello, server!");

  return 0;
}