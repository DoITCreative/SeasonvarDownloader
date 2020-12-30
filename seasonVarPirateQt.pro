#-------------------------------------------------
#
# Project created by QtCreator 2018-04-13T10:57:04
#
#-------------------------------------------------

QT += core gui network
CONFIG+=sdk_no_version_check c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = seasonVarPirateQt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/cpp/controllers/proxysettings_controller.cpp \
    src/cpp/main.cpp \
    src/cpp/mainwindow.cpp \
    src/cpp/download_screen.cpp \
    src/cpp/download_module.cpp \
    src/cpp/download_manager.cpp \
    src/cpp/network_requester.cpp \
    src/cpp/proxysettings.cpp \
    src/cpp/script_screen.cpp \
    src/cpp/utils.cpp \
    src/cpp/controllers/mainwindow_controller.cpp

HEADERS += \
    src/headers/mainwindow.h \
    src/headers/mainwindow_controller.h \
    src/headers/download_screen.h \
    src/headers/download_module.h \
    src/headers/download_manager.h \
    src/headers/mainwindow_interface.h \
    src/headers/mainwindow_model.h \
    src/headers/network_requester.h \
    src/headers/proxysettings.h \
    src/headers/proxysettings_controller.h \
    src/headers/proxysettings_model.h \
    src/headers/script_screen.h \
    src/headers/utils.h

FORMS += \
    src/ui/mainwindow.ui \
    src/ui/download_screen.ui \
    src/ui/download_module.ui \
    src/ui/proxysettings.ui \
    src/ui/script_screen.ui

RESOURCES += \
    src/resources.qrc
