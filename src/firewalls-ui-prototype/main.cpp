#if 1
#include "loginwindow.h"
#include <QApplication>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow w(nullptr);
    w.setWindowTitle("CCSS App");
    w.show();
    return a.exec();
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
