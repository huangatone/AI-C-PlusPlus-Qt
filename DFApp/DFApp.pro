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
    testdialog.cpp \
    testwidget.cpp \
    mycv.cpp \
    osevent.cpp \
    testthread.cpp \
    testtmpwidget.cpp


HEADERS += \
        mainwindow.h \
    serverwidget.h \
    clientwidget.h \
    autotestwidget.h \
    testdialog.h \
    testwidget.h \
    mycv.h \
    osevent.h \
    testthread.h \
    testtmpwidget.h


FORMS += \
        mainwindow.ui \
    serverwidget.ui \
    clientwidget.ui \
    autotestwidget.ui \
    testdialog.ui \
    testwidget.ui \
    testtmpwidget.ui

 mac: {

INCLUDEPATH += /rong/include/opencv
DEPENDPATH += /rong/include/opencv
INCLUDEPATH += /rong/include/opencv
LIBS += -L/rong/lib/ -lopencv_aruco
LIBS += -L/rong/lib/ -lopencv_bgsegm
LIBS += -L/rong/lib/ -lopencv_bioinspired
LIBS += -L/rong/lib/ -lopencv_calib3d
LIBS += -L/rong/lib/ -lopencv_ccalib
LIBS += -L/rong/lib/ -lopencv_core
LIBS += -L/rong/lib/ -lopencv_datasets
LIBS += -L/rong/lib/ -lopencv_dnn
LIBS += -L/rong/lib/ -lopencv_dpm
LIBS += -L/rong/lib/ -lopencv_face
LIBS += -L/rong/lib/ -lopencv_features2d
LIBS += -L/rong/lib/ -lopencv_flann
LIBS += -L/rong/lib/ -lopencv_fuzzy
LIBS += -L/rong/lib/ -lopencv_highgui
LIBS += -L/rong/lib/ -lopencv_img_hash
LIBS += -L/rong/lib/ -lopencv_imgcodecs
LIBS += -L/rong/lib/ -lopencv_imgproc
LIBS += -L/rong/lib/ -lopencv_line_descriptor
LIBS += -L/rong/lib/ -lopencv_ml
LIBS += -L/rong/lib/ -lopencv_objdetect
LIBS += -L/rong/lib/ -lopencv_optflow
LIBS += -L/rong/lib/ -lopencv_phase_unwrapping
LIBS += -L/rong/lib/ -lopencv_photo
LIBS += -L/rong/lib/ -lopencv_plot
LIBS += -L/rong/lib/ -lopencv_reg
LIBS += -L/rong/lib/ -lopencv_rgbd
LIBS += -L/rong/lib/ -lopencv_saliency
LIBS += -L/rong/lib/ -lopencv_shape
LIBS += -L/rong/lib/ -lopencv_stereo
LIBS += -L/rong/lib/ -lopencv_stitching
LIBS += -L/rong/lib/ -lopencv_structured_light
LIBS += -L/rong/lib/ -lopencv_superres
LIBS += -L/rong/lib/ -lopencv_surface_matching
LIBS += -L/rong/lib/ -lopencv_text
LIBS += -L/rong/lib/ -lopencv_tracking
LIBS += -L/rong/lib/ -lopencv_video
LIBS += -L/rong/lib/ -lopencv_videoio
LIBS += -L/rong/lib/ -lopencv_videostab
LIBS += -L/rong/lib/ -lopencv_xfeatures2d
LIBS += -L/rong/lib/ -lopencv_ximgproc
LIBS += -L/rong/lib/ -lopencv_xobjdetect


}

mac {
 RC_FILE = dfcollect.icns
}


macx: LIBS += -L/rong/lib/shark/ -lSharUI.1.0.0

INCLUDEPATH += /rong/include/shark
DEPENDPATH += /rong/include/shark

RESOURCES += \
    ai.qrc
