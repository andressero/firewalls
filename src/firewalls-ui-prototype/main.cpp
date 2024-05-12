#if 1
#include "loginwindow.h"
#include <bits/wordsize.h>
#include <QApplication>

#include "filesystem.h"

int main(int argc, char *argv[])
{
    FileSystem fileSystem;
    fileSystem.create("Users", "11/05/2024", "Server");

    fileSystem.open("Users");
    fileSystem.chang2WriteMode("Users");

    loadUsers(fileSystem, "Users", "data.txt");
    fileSystem.close("Users");

    QApplication a(argc, argv);
    LoginWindow w();
    // LoginWindow w(fileSystem);
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
