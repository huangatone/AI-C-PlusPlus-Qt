#-------------------------------------------------
#
# Project created by QtCreator 2018-03-12T13:52:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenDetect
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
        main.cpp \     
    cvfunction.cpp \
    mlfunction.cpp \
    textareadetect.cpp \ 
    iconbutton.cpp \
    stylewindow.cpp \
    tessractfunction.cpp \
    pictureview.cpp

HEADERS += \ 
    cvfunction.h \
    mlfunction.h \
    textareadetect.h \ 
    iconbutton.h \
    stylewindow.h \
    tessractfunction.h \
    pictureview.h

FORMS += \
    iconbutton.ui \
    stylewindow.ui \
    pictureview.ui

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include
INCLUDEPATH += /rong/lib/include

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include




INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include

INCLUDEPATH += /rong/lib_github/brew/include
DEPENDPATH += /rong/lib_github/brew/include
INCLUDEPATH += /rong/lib_github/brew/include
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_aruco
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_bgsegm
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_bioinspired
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_calib3d
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_ccalib
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_core
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_datasets
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_dnn
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_dpm
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_face
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_features2d
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_flann
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_fuzzy
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_highgui
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_img_hash
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_imgcodecs
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_imgproc
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_line_descriptor
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_ml
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_objdetect
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_optflow
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_phase_unwrapping
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_photo
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_plot
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_reg
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_rgbd
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_saliency
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_shape
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_stereo
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_stitching
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_structured_light
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_superres
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_surface_matching
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_text
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_tracking
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_video
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_videoio
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_videostab
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_xfeatures2d
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_ximgproc
LIBS += -L/rong/lib_github/brew/lib/ -lopencv_xobjdetect
LIBS += -L/rong/lib_github/brew/lib/ -ltbb

LIBS += -L/rong/lib_github/brew/lib/ -ltesseract -llept


