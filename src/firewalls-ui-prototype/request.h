#ifndef REQUEST_H
#define REQUEST_H

#include <string>

// Use sockets
// In charge of parsing data
class Request
{
private:
    std::string username;
    std::string hash;

    std::string parse(std::string message);
    Request() {}
    Request(const Request&) = delete;
    Request& operator=(const Request&) = delete;
public:
    static Request& getInstance() {
        static Request instance;
        return instance;
    }

    std::string requestLogin();
    std::string requestUserData();
    std::string requestInsuranceStatus();
    std::string requestLabList();
    std::string requestLabResult(int resultNumber);

    void setUsername(const std::string& username);
    void setHash(const std::string& hash);

};

#endif // REQUEST_H
