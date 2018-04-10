#-------------------------------------------------
#
# Project created by QtCreator 2018-01-15T11:48:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TTTu
TEMPLATE = app


Release:DESTDIR = /rong/project/MyDev/bin/release
Debug:DESTDIR = /rong/project/MyDev/bin/debug

OBJECTS_DIR = /rong/project/MyDev/bin/obj
MOC_DIR = /rong/project/MyDev/bin/moc
RCC_DIR =/rong/project/MyDev/bin/qrc
UI_DIR =/rong/project/MyDev/bin/u

CONFIG += debug_and_release



CONFIG(debug, debug|release) {
    DESTDIR = /rong/project/MyDev/bin/debug
} else {
    DESTDIR = /rong/project/MyDev/bin/release
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


SOURCES +=         main.cpp \
    colordetector.cpp \
    cvfunction.cpp \
    dfhtmltable.cpp \
    dfpixmapwdiget.cpp \
    dfsplitter.cpp \   
    mainwindow.cpp \   
    opencvwindow.cpp \
    rgbeditor.cpp

HEADERS +=     opencvwindow.h \
    colordetector.h \
    colorhistogram.h \
    cvfunction.h \
    dfhtmltable.h \
    dfpixmapwdiget.h \
    dfsplitter.h \
    imageComparator.h \
    laplacianZC.h \
    mainwindow.h \
    morphoFeatures.h \
    objectFinder.h \
    opencvwindow.h \
    rgbeditor.h

FORMS += \
        mainwindow.ui \
    dfpixmapwdiget.ui \
    opencvwindow.ui \
    rgbeditor.ui

RESOURCES +=   cvrc.qrc

INCLUDEPATH += $$(opencv)/include

INCLUDEPATH += /rong/lib/opencv/3.4.0/include
DEPENDPATH += /rong/lib/opencv/3.4.0/include
INCLUDEPATH += /rong/lib/include

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include


LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_aruco
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_bgsegm
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_bioinspired
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_calib3d
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_ccalib
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_core
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_datasets
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_dnn
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_dpm
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_face
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_features2d
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_flann
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_fuzzy
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_highgui
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_img_hash
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_imgcodecs
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_imgproc
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_line_descriptor
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_ml
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_objdetect
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_optflow
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_phase_unwrapping
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_photo
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_plot
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_reg
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_rgbd
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_saliency
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_shape
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_stereo
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_stitching
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_structured_light
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_superres
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_surface_matching
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_text
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_tracking
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_video
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_videoio
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_videostab
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_xfeatures2d
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_ximgproc
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_xobjdetect
LIBS += -L/rong/lib/opencv/3.4.0/lib/ -lopencv_xphoto


mac {
 RC_FILE = aa.icns
}

win32 {
 RC_FILE = aa.rc
}
