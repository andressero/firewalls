#ifndef REQUEST_H
#define REQUEST_H

#include <sstream>
#include <string>
#include <vector>

// Use sockets
// In charge of parsing data
class Request {
private:
  std::string username;
  std::string hash;
  std::string redirectorServerIP;
  int redirectorServerPort;

  std::string parse(std::string message);
  Request() : redirectorServerIP("127.0.0.1") {}
  Request(const Request &) = delete;
  Request &operator=(const Request &) = delete;

public:
  static Request &getInstance() {
    static Request instance;
    return instance;
  }
  std::string
  removeServerConfirmationResponse(const std::string &receivedResponse);
  std::string requestLogin();
  std::string requestUserData();
  std::string requestPatientData(const std::string &patientID);
  std::string requestInsuranceStatus(const std::string &id);
  std::string requestLabList();
  std::string requestLabResult(const std::string &labDate);
  std::string requestPatientList();
  bool requestDataInsertion(const std::string &data);

  void setUsername(const std::string &username);
  void setHash(const std::string &hash);
  void setRedirectorServerIDAndPort(const std::string &redirectorServerIP,
                                    const int redirectorServerPort);
  std::vector<std::string> splitString(const std::string &input,
                                       const std::string &delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;

    while (std::getline(iss, token, delimiter[0])) {
      tokens.push_back(token);
    }

    return tokens;
  }
};
#endif // REQUEST_H
