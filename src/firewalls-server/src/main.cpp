// Copyright [2024] Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano
#include "Definitions.hpp"
#include "FileSystem.hpp"

#define ALL 0
#define TEST_FILE_NOT_FOUND 0
#define TEST_FILE_WITH_NAME 0
#define TEST_READ_EMPTY_FILE 0
#define TEST_WRITE_READ_FILE 1

#define TEST_APPEND_FILE 0
#define TEST_ERASE_FILE 0
// TODO(ALL) PRINT TESTNAME BEFORE TEST.
// DONE TODO(ALL) REMOVE TWO BREAKS FROM READ METHOD.
int main() {
#if TEST_FILE_NOT_FOUND == 1 || ALL == 1
  FileSystem fs;
  FileProperties prueba;
  fs.create(prueba);
  fs.open(prueba);

  fs.close(prueba);
#endif

#if TEST_FILE_WITH_NAME == 1 || ALL == 1
  FileSystem fs2;
  FileProperties prueba2("Apuntes", "22-12-22", "Pablo");
  fs2.create(prueba2);
  fs2.open(prueba2);

  fs2.close(prueba2);
#endif

#if TEST_READ_EMPTY_FILE == 1 || ALL == 1
  FileSystem fs3;
  FileProperties prueba3("Vacio", "01-4-15", "Andres");
  fs3.create(prueba3);
  fs3.open(prueba3);
  fs3.read(prueba3, 100);

  fs3.close(prueba3);
#endif

#if TEST_WRITE_READ_FILE == 1 || ALL == 1
  FileSystem fs4;
  FileProperties prueba4("Escritura", "02-9-30", "Diego");
  fs4.create(prueba4);
  fs4.open(prueba4);
  string buffer = "Hola mundo, esto es una prueba de escritura";
  fs4.write(prueba4, buffer, buffer.size());
  
  string data = fs4.read(prueba4, buffer.size());
  LOG(data);

  fs4.close(prueba4);
#endif

  return EXIT_SUCCESS;
}