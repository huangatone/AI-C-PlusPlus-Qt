#-------------------------------------------------
#
# Project created by QtCreator 2018-05-24T11:59:02
#
#-------------------------------------------------

QT       += core gui network testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DFApp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += /rong/include/shark



# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    serverwidget.cpp \
    clientwidget.cpp \
    autotestwidget.cpp \
    testdialog.cpp


HEADERS += \
        mainwindow.h \
    serverwidget.h \
    clientwidget.h \
    autotestwidget.h \
    testdialog.h


FORMS += \
        mainwindow.ui \
    serverwidget.ui \
    clientwidget.ui \
    autotestwidget.ui \
    testdialog.ui



mac {
 RC_FILE = dfcollect.icns
}


macx: LIBS += -L$$PWD/../../../lib/shark/ -lSharUI.1.0.0

INCLUDEPATH += $$PWD/../../../include/shark
DEPENDPATH += $$PWD/../../../include/shark
