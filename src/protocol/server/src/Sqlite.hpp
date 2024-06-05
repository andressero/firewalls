#ifndef SQLITE_HPP
#define SQLITE_HPP

#include <sqlite3.h>
#include <string>
#include <cstdio>

class Sqlite {
private:
    sqlite3* db;
    Sqlite(const Sqlite&) = delete;
    Sqlite& operator=(const Sqlite&) = delete;
  public:
  
    static Sqlite& getInstance() {
        static Sqlite instance; // Guaranteed to be initialized only once
        return instance;
    }

  Sqlite() : db(0) {
    this->open(std::string("../data/data.db"));
  }

  ~Sqlite() {
    this->close();
  }

  void open(std::string filepath) {
    sqlite3_open(filepath.c_str(), &this->db);
  }
  
  void close() {
    sqlite3_close(db);
  }

  void exec(std::string sql_statement, int (*callback)(void*, int, char**, char**), void* data) {
    char* zErrMsg = 0;
    int rc = sqlite3_exec(db, sql_statement.c_str(), callback, data, &zErrMsg);
    if( rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Operation done successfully\n");
    }
  }

  // Signal handler function
  static void signalHandler(int signal) {
    std::cout << "SQLITE Received signal " << signal << ". Releasing resources..." << std::endl;
    Sqlite& instance = Sqlite::getInstance();
    instance.close();
  }

};

#endif