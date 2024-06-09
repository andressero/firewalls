#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 3000
#define BUFFER_SIZE 1024

int main() {
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char *hello =
      "AUTH 123456789 "
      "b50adfab92b3e54af123e99c980d4cea9221047e307ebc1e1f9cb9549790a219\n";
  char buffer[BUFFER_SIZE] = {0};

  // Crear socket
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convertir direcciones IPv4 e IPv6 de texto a binario
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  // Conectarse al servidor
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed \n");
    return -1;
  }

  // Enviar mensaje al servidor
  send(sock, hello, strlen(hello), 0);
  printf("Message sent\n");

  // Leer respuesta del servidor
  valread = read(sock, buffer, BUFFER_SIZE);
  printf("Server response: %s\n", buffer);

  // Cerrar el socket
  close(sock);

  return 0;
}
