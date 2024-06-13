#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#define SERVER_PORT 3000
#define SERVER_IP "192.168.100.68"
#define BUFFER_SIZE 1024

class ClientSocket {
public:
  ClientSocket(const std::string &ip, int port);
  ~ClientSocket();

  void connectToServer();
  void sendData(const std::string &data);
  std::string receiveData();

private:
  int socket_fd;
  struct sockaddr_in server_address;
};

ClientSocket::ClientSocket(const std::string &ip, int port) {
  // Create socket
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    perror("Failed to create socket");
    exit(EXIT_FAILURE);
  }

  // Configure server address
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  if (inet_pton(AF_INET, ip.c_str(), &server_address.sin_addr) <= 0) {
    perror("Invalid address/Address not supported");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }
}

ClientSocket::~ClientSocket() { close(socket_fd); }

void ClientSocket::connectToServer() {
  if (connect(socket_fd, (struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    perror("Connection failed");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }
  std::cout << "Connected to the server" << std::endl;
}

void ClientSocket::sendData(const std::string &data) {
  send(socket_fd, data.c_str(), data.length(), 0);
}

std::string ClientSocket::receiveData() {
  char buffer[BUFFER_SIZE];
  int bytes_received = recv(socket_fd, buffer, BUFFER_SIZE, 0);
  if (bytes_received < 0) {
    perror("Failed to receive data");
    return "";
  } else {
    buffer[bytes_received] = '\0';
    return std::string(buffer);
  }
}

int main() {
  ClientSocket client(SERVER_IP, SERVER_PORT);

  client.connectToServer();

  std::string message = "AUTH 123456789 18d404b76462a6b04b4e413c977734ae4923e2796a34cd1dd82b09f92d340bac\nREQUEST USER_DATA 123456789\n";
  client.sendData(message);

  std::string response = client.receiveData();
  std::cout << "Received from server: " << response << std::endl;

  return 0;
}
