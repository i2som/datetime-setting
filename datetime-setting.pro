#-------------------------------------------------
#
# Project created by QtCreator 2019-10-12T22:56:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = datetime-setting
TEMPLATE = app

contains(QMAKE_CC, arm-openstlinux_eglfs-linux-gnueabi-gcc) {
    DEFINES += PLATFORM_ARM

    CONFIG(debug, debug|release) {
        message("Configuring for ARM-Debug build ...")
        DEFINES += PLATFORM_DEBUG
    } else {
        message("Configuring for ARM-Release build ...")
        DEFINES += PLATFORM_RELEASE
    }
} else {
    DEFINES += PLATFORM_X86

    CONFIG(debug, debug|release) {
        message("Configuring for X86-Debug build ...")
        DEFINES += PLATFORM_DEBUG
    } else {
        message("Configuring for X86-Release build ...")
        DEFINES += PLATFORM_RELEASE
    }
}

target.path = /home/root
INSTALLS += target

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
