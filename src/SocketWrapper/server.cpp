#include "Socket.hpp"

// TODO(any): Figure out why this server can't send messages
// TODO(any): Make the server receive more than one socket.

int main() {
  Socket server(8080, "0.0.0.0");
	server.bind();
	server.listen(5);
	Socket client = server.accept();


	const char* mensaje = "Hello client!!";
	char* respuesta[1024] = {0};

	::recv(client.getFileDescriptor(), respuesta, sizeof(respuesta), 0);
	::send(client.getFileDescriptor(), mensaje, strlen(mensaje), 0);

	std::cout << "Mensaje del cliente: " << mensaje << std::endl;

	#if 0
	std::string message = server.receive(client);
	std::string answer = "Hello client";
	::send(client.getFileDescriptor(), answer.c_str(), answer.size(), 0);
	std::cout << "Message from client: " << message << std::endl;
	#endif
	

	// server.send("Hello client");

return 0;
}
