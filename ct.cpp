#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5002
#define BUFFER_SIZE 1024

void error_handling(char *message) {
  perror(message);
  exit(1);
}

int main() {
  int sock;
  struct sockaddr_in server_addr;
  char message[BUFFER_SIZE];
  int str_len;

  // Crear socket
  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1)
    error_handling("socket() error");

  // Configurar dirección del servidor
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  server_addr.sin_port = htons(SERVER_PORT);

  // Conectar al servidor
  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    error_handling("connect() error");

  // Leer mensaje del usuario
  printf("Ingrese el mensaje al servidor: ");
  fgets(message, BUFFER_SIZE, stdin);

  // Enviar mensaje al servidor
  if (write(sock, message, strlen(message)) == -1)
    error_handling("write() error");

  // Leer respuesta del servidor
  str_len = read(sock, message, BUFFER_SIZE - 1);
  if (str_len == -1)
    error_handling("read() error");

  message[str_len] = 0;
  printf("Mensaje del servidor: %s\n", message);

  // Cerrar el socket
  close(sock);
  return 0;
}
