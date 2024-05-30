#include "Socket.hpp"

// TODO(any): Figure out why this server can't send messages
// TODO(any): Make the server receive more than one socket.

int main() {
  Socket server(8080, "0.0.0.0");
	server.bind();
	server.listen(5);

	Socket client = server.accept();
	std::string message = server.receive(client);
	std::cout << "Message from client: " << message << std::endl;

return 0;
}
