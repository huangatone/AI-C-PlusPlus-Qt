#-------------------------------------------------
#
# Project created by QtCreator 2019-09-04T10:09:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UITest
TEMPLATE = app

CONFIG(debug, debug|release) {
    DESTDIR = /work/debug/UITest
    OBJECTS_DIR = /work/debug/UITest
    MOC_DIR = /work/debug/UITest
    RCC_DIR = /work/debug/UITest
    UI_DIR = /work/debug/UITest
} else {
    DESTDIR = /work/release/UITest
    OBJECTS_DIR = /work/release/UITest
    MOC_DIR = /work/release/UITest
    RCC_DIR = /work/release/UITest
    UI_DIR = /work/release/UITest
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        baseknow.cpp \
        build.cpp \
        dftoolbtn.cpp \
        factory.cpp \
        main.cpp \
        modeltest.cpp \
        product.cpp \
        testwindow.cpp

HEADERS += \
        baseknow.h \
        build.h \
        dftoolbtn.h \
        factory.h \
        modeltest.h \
        product.h \
        testwindow.h

FORMS += \
        testwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
