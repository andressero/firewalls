#if 1
#include "loginwindow.h"
#include <QApplication>
#include "filesystem.h"
#include <QDir>
#include <QDebug>

void loadUsers(FileSystem& fs, const std::string& usersFilename,
               const std::string& filePath);

int main(int argc, char *argv[])
{
    FileSystem fs;
    fs.create("Users", "12/05/2024", "server");
    fs.open("Users");
    fs.chang2WriteMode("Users");
    std::string data = "123456789,12386531238785373283138664801578873895511966296585825349207,jPKHYbUsShwdAzTC,P,1\n";
    data += "L12345,18391225044809765264184193313980912568774903191066989168016,iANa0ngbTtzLd8vX,L,0\n";
    data += "234567890,398874093416949372945413150052185037341547968653894361291,cA6Dm8PBxnjAo12J,P,1\n";

    fs.write("Users", data, data.size());
    qInfo() << "Successfully written data\n";
    // loadUsers(fs, "Users", "/sdk_gphone_x86_64/Documents/users.txt");
    fs.close("Users");

    QApplication a(argc, argv);
    LoginWindow w(nullptr, &fs);
    w.setWindowTitle("CCSS App");
    w.show();
    return a.exec();
}

void loadUsers(FileSystem& fs, const std::string& usersFilename,
               const std::string& filePath) {

    FILE* file = fopen(filePath.c_str(), "r");
    if (file) {
        std::ifstream file(filePath, std::ifstream::in);
        std::string line;
        std::cout << line;
        // std::getline(file, line);
        while(std::getline(file, line)) {
            line.append("\n");
            fs.write(usersFilename, line, line.size());
        }
    }
}

#endif

void loadUsers(FileSystem& fs, const std::string& usersFilename,
               const std::string& filePath) {


    std::ifstream file(filePath, std::ifstream::in);
    QString currentDir = QDir::currentPath();
    qInfo() << "Current dir: " << currentDir << "\n";
    if (!file.fail()) {
        qInfo() << "============== File open ==============";
        std::string line;
        std::cout << line;
        // std::getline(file, line);
        while(std::getline(file, line)) {
            line.append("\n");
            fs.write(usersFilename, line, line.size());
        }
    } else {
        qInfo() << "File" << filePath << "didn't open\n";
    }
}

#if 0
#include "loginwindow.h"

#include <QApplication>
#include "patientlabmenu.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    patientLabMenu l;
    l.show();
    return a.exec();
}
#endif
