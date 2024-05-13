#if 1
#include "loginwindow.h"
#include <QApplication>
#include "filesystem.h"

void loadUsers(FileSystem& fs, const std::string& usersFilename,
               const std::string& filePath);

int main(int argc, char *argv[])
{
    FileSystem fs;
    fs.create("Users", "12/05/2024", "server");
    fs.open("Users");
    fs.chang2WriteMode("Users");

    loadUsers(fs, "Users", "data.txt");
    fs.close("Users");

    QApplication a(argc, argv);
    LoginWindow w(nullptr, &fs);
    w.setWindowTitle("CCSS App");
    w.show();
    return a.exec();
}
#endif

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
