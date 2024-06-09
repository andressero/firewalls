// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos,
// Andres Serrano>"

#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <iostream>
#include <sqlite3.h>
#include <sstream>
#include <string>

class Database {
public:
  static Database &getInstance(const std::string &dbName);

  bool executeQuery(const std::string &query,
                    int (*callback)(void *, int, char **,
                                    char **) = defaultCallback,
                    void *data = nullptr);
  void close();
  static void signalHandler(int signal, const std::string dbName);

  ~Database();
  static int defaultCallback(void *data, int argc, char **argv,
                             char **azColName);

private:
  sqlite3 *db;

  Database(const std::string &dbName);

  Database(const Database &) = delete;
  Database &operator=(const Database &) = delete;
};

Database &Database::getInstance(const std::string &dbName) {
  static Database instance(dbName);
  return instance;
}

bool Database::executeQuery(const std::string &query,
                            int (*callback)(void *, int, char **, char **),
                            void *data) {
  char *zErrMsg = 0;
  int rc = sqlite3_exec(db, query.c_str(), callback, data, &zErrMsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << zErrMsg << std::endl;
    sqlite3_free(zErrMsg);
    return false;
  } else {
    std::cout << "Query executed successfully" << std::endl;
    return true;
  }
}

Database::~Database() { sqlite3_close(db); }

Database::Database(const std::string &dbName) {
  int rc = sqlite3_open(dbName.c_str(), &db);
  if (rc) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
  } else {
    std::cout << "Opened database successfully" << std::endl;
  }
}

int Database::defaultCallback(void *data, int argc, char **argv,
                              char **azColName) {
  std::ostringstream *oss = static_cast<std::ostringstream *>(data);
  for (int i = 0; i < argc; i++) {
    *oss << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
  }
  *oss << "\n";
  return 0;
}

void Database::close() { sqlite3_close(db); }

// Signal handler function
void Database::signalHandler(int signal, const std::string dbName) {
  std::cout << "Database received signal " << signal
            << ". Releasing resources..." << std::endl;
  Database &instance = Database::getInstance(dbName);
  instance.close();
}

#endif // DATABASE_HPP
