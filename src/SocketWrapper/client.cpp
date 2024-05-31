#include "Socket.hpp"

// TODO(any): Figure out why this client can't receive messages
int main() {
  Socket client(8080, "127.0.0.1");

  Socket server(8080, "0.0.0.0");
  client.connect(server.getSocketAddress());

  const char* mensaje = "Hello, server!!";
  char* respuesta[1024] = {0};


  ::send(client.getFileDescriptor(), mensaje, strlen(mensaje), 0);
  ::recv(client.getFileDescriptor(), respuesta, sizeof(respuesta), 0);

  std::cout << "Mensaje del servidor: " << mensaje << std::endl;

  #if 0
  client.send("Hello, server!");
  std::string message = client.receive(client);
  std::cout << "Message from server: " << message << std::endl;
  #endif
  return 0;
}