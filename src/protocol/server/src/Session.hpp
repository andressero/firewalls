// Copyright [2024] <Andres Quesada, Pablo Cascante, Diego Bolanos, Andres
// Serrano>"
#ifndef SESSION_HPP
#define SESSION_HPP

#include "FileSystem.hpp"
#include "Sqlite.hpp"
#include "SHA256.hpp"

// Sha256 stuff.
// TODO[any]: Put sha256Hash and sha256ToString elsewhere?
std::string sha256ToString(const BYTE* hash) {
    std::string answer(2*SHA256_BLOCK_SIZE, '0');
    for (size_t i = 0; i < SHA256_BLOCK_SIZE; ++i) {
        char temp[3] = {0};
        snprintf(temp, 3, "%02x", hash[i]);
        answer[2*i] = temp[0];
        answer[2*i + 1] = temp[1];
    }
    return answer;
}

std::string sha256Hash(std::string& strInput) {
    BYTE *input =  new BYTE[SHA256_BLOCK_SIZE];
    input = reinterpret_cast<BYTE*>(&strInput[0]);

    BYTE hash[SHA256_BLOCK_SIZE];
    SHA256_CTX context;
    sha256_init(&context);
    sha256_update(&context, input, strInput.size());
    sha256_final(&context, hash);
    // delete[] input;
    return sha256ToString(hash);
}

class LabResult {
public:
  std::string labResultID;
  std::string labResultTestName;
  std::string labResultDate;
  std::string labResultContent;
  LabResult() = default;

  int operator=(const LabResult &other) {
    this->labResultID = other.labResultID;
    this->labResultTestName = other.labResultTestName;
    this->labResultDate = other.labResultDate;
    this->labResultContent = other.labResultContent;
    return 0;
  }

  LabResult(const LabResult &other) { *this = other; }

  LabResult(char *labResultID, char *labResultTestName, char *labResultDate,
            char *labResultContentFilePath) {
    this->labResultID = std::string(labResultID);
    this->labResultTestName = std::string(labResultTestName);
    this->labResultDate = std::string(labResultDate);
    std::ifstream file(labResultContentFilePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    this->labResultContent = buffer.str();
  }

  bool empty() const {
    const bool a = this->labResultID.empty();
    return a;
  }

  std::string toString() const {
    std::stringstream file;
    file << "LabResultID: " << this->labResultID
         << " LabResultTestName: " << this->labResultTestName
         << " LabResultDate: " << this->labResultDate << " LabResultContent: "
         << "[" << this->labResultContent << "]";
    return file.str();
  }
};

class Session {
private:
  std::string user;
  std::string hash;
  bool loggedIn;

public:
  Session() : loggedIn(false){};
  Session(std::string user, std::string hash)
      : user(user), hash(hash), loggedIn(false){};
  ~Session() = default;

  void setUser(std::string user) { this->user = user; }
  void setHash(std::string hash) { this->hash = hash; }
  void setLogStatus(bool loggedIn) { this->loggedIn = loggedIn; }
  bool getLogStatus() { return this->loggedIn; }
  bool tryLogin() {
    FileSystem &fs4 = FileSystem::getInstance();
    fs4.open(user);
    fs4.change2ReadMode(user);
    const std::string userData = fs4.read(user, 200);
    fs4.close(user);

    const std::vector<std::string> fields = splitString(userData, ",");

    const std::string storedHash = fields[1];
    LOG("STORED HASH SIZE " + std::to_string(storedHash.size()))
    // ASSERT(storedHash.size() == 124);
    LOG("RECEIVED HASH SIZE " + std::to_string(this->hash.size()))
    // ASSERT(storedHash.size() == 124);

    // updated input is hash + salt
    std::string updatedHashInput = this->hash + fields[2];
    const std::string updatedHash = sha256Hash(updatedHashInput);

    const bool validLogin = storedHash == updatedHash;

    LOG("Valid login: " + std::to_string(validLogin))

    if (validLogin) {
      this->loggedIn = true;
    }
    return validLogin;
  }

  static int getUserData(void *data, int argc, char **argv, char **azColName) {
    (void)azColName;
    std::string *isInsured = (std::string *)data;
    for (int i = 0; i < argc; i++) {
      *isInsured += std::string(argv[i]) + ",";
    }
    return 0;
  }

  std::string userDataRequest() {
    Sqlite &sqlite = Sqlite::getInstance();
    std::string sql_statement =
        "SELECT * FROM 'userData'  WHERE patientID = " + this->user;
    std::string userData;
    sqlite.exec(sql_statement, getUserData, (void *)&userData);
    return userData;
  }

  std::string insuranceStatusRequest() {
    const std::string insuranceStatus =
        splitString(this->userDataRequest(), ",")[11];
    return insuranceStatus;
  }

  static int getLabReportList(void *data, int argc, char **argv,
                              char **azColName) {
    (void)argc;
    (void)azColName;
    std::vector<LabResult> *labResults = (std::vector<LabResult> *)data;
    LabResult labResult(argv[0], argv[2], argv[4], argv[3]);
    labResults->push_back(labResult);
    // LOG("LabResultID: " << argv[0] << " LabResultTestName: " << argv[1] << "
    // LabResultDate: " << argv[3] << " LabResultContent: " << argv[2])
    return 0;
  }

  std::vector<LabResult> labListRequest() {
    Sqlite &sqlite = Sqlite::getInstance();
    std::string sql_statement = "SELECT * FROM 'LabResults' LIMIT 0,30";
    std::vector<LabResult> labResults;
    sqlite.exec(sql_statement, getLabReportList, (void *)&labResults);
    return labResults;
  }

  static int getLabReport(void *data, int argc, char **argv, char **azColName) {
    (void)argc;
    (void)azColName;
    LabResult *labResult = (LabResult *)data;
    LabResult labResult2(argv[0], argv[2], argv[4], argv[3]);
    *labResult = labResult2;
    return 0;
  }

  LabResult labResultRequest(std::string labResultID) {
    Sqlite &sqlite = Sqlite::getInstance();
    std::string sql_statement =
        "SELECT * FROM 'LabResults'  WHERE labResultID = " + labResultID +
        " AND patientID = " + this->user;
    LabResult labResult;
    sqlite.exec(sql_statement, getLabReport, (void *)&labResult);
    return labResult;
  }
};

#endif