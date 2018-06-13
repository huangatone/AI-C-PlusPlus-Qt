#-------------------------------------------------
#
# Project created by QtCreator 2018-03-12T13:52:43
#
#-------------------------------------------------

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageApp
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
    pictureview.cpp \
    tourist.cpp \
    cvpictureview.cpp \
    dealview.cpp \
    rgbeditor.cpp \
    fileselectwidget.cpp \
    cvpanel.cpp \
    dfsplitter.cpp


HEADERS += \ 
    cvfunction.h \
    mlfunction.h \
    textareadetect.h \ 
    iconbutton.h \
    stylewindow.h \
    tessractfunction.h \
    pictureview.h \
    tourist.h \
    cvpictureview.h \
    dealview.h \
    rgbeditor.h \
    fileselectwidget.h \
    cvpanel.h \
    dfsplitter.h


FORMS += \
    iconbutton.ui \
    stylewindow.ui \
    pictureview.ui \
    cvpictureview.ui \
    dealview.ui \
    rgbeditor.ui \
    fileselectwidget.ui \
    cvpanel.ui



INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include
INCLUDEPATH += /rong/lib/include

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include

DISTFILES +=

RESOURCES += \
    ai.qrc

mac {
 RC_FILE = open.icns
}

nossl {
  DEFINES += NO_SSL
}

win32: {

}
else: mac: {

INCLUDEPATH += /rong/include/opencv
DEPENDPATH += /rong/include/opencv
INCLUDEPATH += /rong/include/opencv

INCLUDEPATH += /rong/include
DEPENDPATH += /rong/include

LIBS += -L/rong//lib/ -lopencv_aruco
LIBS += -L/rong//lib/ -lopencv_bgsegm
LIBS += -L/rong//lib/ -lopencv_bioinspired
LIBS += -L/rong//lib/ -lopencv_calib3d
LIBS += -L/rong//lib/ -lopencv_ccalib
LIBS += -L/rong//lib/ -lopencv_core
LIBS += -L/rong//lib/ -lopencv_datasets
LIBS += -L/rong//lib/ -lopencv_dnn
LIBS += -L/rong//lib/ -lopencv_dpm
LIBS += -L/rong//lib/ -lopencv_face
LIBS += -L/rong//lib/ -lopencv_features2d
LIBS += -L/rong//lib/ -lopencv_flann
LIBS += -L/rong//lib/ -lopencv_fuzzy
LIBS += -L/rong//lib/ -lopencv_highgui
LIBS += -L/rong//lib/ -lopencv_img_hash
LIBS += -L/rong//lib/ -lopencv_imgcodecs
LIBS += -L/rong//lib/ -lopencv_imgproc
LIBS += -L/rong//lib/ -lopencv_line_descriptor
LIBS += -L/rong//lib/ -lopencv_ml
LIBS += -L/rong//lib/ -lopencv_objdetect
LIBS += -L/rong//lib/ -lopencv_optflow
LIBS += -L/rong//lib/ -lopencv_phase_unwrapping
LIBS += -L/rong//lib/ -lopencv_photo
LIBS += -L/rong//lib/ -lopencv_plot
LIBS += -L/rong//lib/ -lopencv_reg
LIBS += -L/rong//lib/ -lopencv_rgbd
LIBS += -L/rong//lib/ -lopencv_saliency
LIBS += -L/rong//lib/ -lopencv_shape
LIBS += -L/rong//lib/ -lopencv_stereo
LIBS += -L/rong//lib/ -lopencv_stitching
LIBS += -L/rong//lib/ -lopencv_structured_light
LIBS += -L/rong//lib/ -lopencv_superres
LIBS += -L/rong//lib/ -lopencv_surface_matching
LIBS += -L/rong//lib/ -lopencv_text
LIBS += -L/rong//lib/ -lopencv_tracking
LIBS += -L/rong//lib/ -lopencv_video
LIBS += -L/rong//lib/ -lopencv_videoio
LIBS += -L/rong//lib/ -lopencv_videostab
LIBS += -L/rong//lib/ -lopencv_xfeatures2d
LIBS += -L/rong//lib/ -lopencv_ximgproc
LIBS += -L/rong//lib/ -lopencv_xobjdetect
LIBS += -L/rong//lib/ -ltbb

LIBS += -L/rong//lib/ -ltesseract -llept
}
else: unix: {

    INCLUDEPATH += $$(opencv)/include

    INCLUDEPATH += /usr/local/include
    DEPENDPATH += /usr/local/include
    INCLUDEPATH += /rong/lib/include

    INCLUDEPATH += /usr/local/include
    DEPENDPATH += /usr/local/include


    LIBS += -L/usr/local/lib/ -lopencv_aruco
    LIBS += -L/usr/local/lib/ -lopencv_bgsegm
    LIBS += -L/usr/local/lib/ -lopencv_bioinspired
    LIBS += -L/usr/local/lib/ -lopencv_calib3d
    LIBS += -L/usr/local/lib/ -lopencv_ccalib
    LIBS += -L/usr/local/lib/ -lopencv_core
    LIBS += -L/usr/local/lib/ -lopencv_datasets
    LIBS += -L/usr/local/lib/ -lopencv_dnn
    LIBS += -L/usr/local/lib/ -lopencv_dpm
    LIBS += -L/usr/local/lib/ -lopencv_face
    LIBS += -L/usr/local/lib/ -lopencv_features2d
    LIBS += -L/usr/local/lib/ -lopencv_flann
    LIBS += -L/usr/local/lib/ -lopencv_fuzzy
    LIBS += -L/usr/local/lib/ -lopencv_highgui
    LIBS += -L/usr/local/lib/ -lopencv_img_hash
    LIBS += -L/usr/local/lib/ -lopencv_imgcodecs
    LIBS += -L/usr/local/lib/ -lopencv_imgproc
    LIBS += -L/usr/local/lib/ -lopencv_line_descriptor
    LIBS += -L/usr/local/lib/ -lopencv_ml
    LIBS += -L/usr/local/lib/ -lopencv_objdetect
    LIBS += -L/usr/local/lib/ -lopencv_optflow
    LIBS += -L/usr/local/lib/ -lopencv_phase_unwrapping
    LIBS += -L/usr/local/lib/ -lopencv_photo
    LIBS += -L/usr/local/lib/ -lopencv_plot
    LIBS += -L/usr/local/lib/ -lopencv_reg
    LIBS += -L/usr/local/lib/ -lopencv_rgbd
    LIBS += -L/usr/local/lib/ -lopencv_saliency
    LIBS += -L/usr/local/lib/ -lopencv_shape
    LIBS += -L/usr/local/lib/ -lopencv_stereo
    LIBS += -L/usr/local/lib/ -lopencv_stitching
    LIBS += -L/usr/local/lib/ -lopencv_structured_light
    LIBS += -L/usr/local/lib/ -lopencv_superres
    LIBS += -L/usr/local/lib/ -lopencv_surface_matching
    LIBS += -L/usr/local/lib/ -lopencv_text
    LIBS += -L/usr/local/lib/ -lopencv_tracking
    LIBS += -L/usr/local/lib/ -lopencv_video
    LIBS += -L/usr/local/lib/ -lopencv_videoio
    LIBS += -L/usr/local/lib/ -lopencv_videostab
    LIBS += -L/usr/local/lib/ -lopencv_xfeatures2d
    LIBS += -L/usr/local/lib/ -lopencv_ximgproc
    LIBS += -L/usr/local/lib/ -lopencv_xobjdetect
    LIBS += -L/usr/local/lib/ -lopencv_xphoto
    LIBS += -L/usr/lib/ -ltesseract -llept
}

