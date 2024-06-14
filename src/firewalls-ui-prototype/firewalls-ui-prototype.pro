QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clientsocket.cpp \
    insurancenegative.cpp \
    insurancepositive.cpp \
    lablistwindow.cpp \
    labresultinsertion.cpp \
    main.cpp \
    loginwindow.cpp \
    mainmenudoctor.cpp \
    mainmenulaboratory.cpp \
    mainmenupatient.cpp \
    menupersonaldata.cpp \
    patientinsurancemenu.cpp \
    patientlabmenu.cpp \
    patientselectionmenu.cpp \
    request.cpp \
    sha256.cpp \
    userdata.cpp

HEADERS += \
    clientsocket.h \
    insurancenegative.h \
    insurancepositive.h \
    lablistwindow.h \
    labresultinsertion.h \
    loginwindow.h \
    mainmenudoctor.h \
    mainmenulaboratory.h \
    mainmenupatient.h \
    menupersonaldata.h \
    patientinsurancemenu.h \
    patientlabmenu.h \
    patientselectionmenu.h \
    request.h \
    sha256.h \
    userdata.h \
    uservalidation.h

FORMS += \
    insurancenegative.ui \
    insurancepositive.ui \
    lablistwindow.ui \
    labresultinsertion.ui \
    loginwindow.ui \
    mainmenudoctor.ui \
    mainmenulaboratory.ui \
    mainmenupatient.ui \
    menupersonaldata.ui \
    patientinsurancemenu.ui \
    patientlabmenu.ui \
    patientselectionmenu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

contains(ANDROID_TARGET_ARCH,x86) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

contains(ANDROID_TARGET_ARCH,x86_64) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

DISTFILES += \
    users.txt
    data.txt


OTHER_FILES += \
    data.txt
    users.txt

RESOURCES += \
    application.qrc

