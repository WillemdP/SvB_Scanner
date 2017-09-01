#-------------------------------------------------
#
# Project created by QtCreator 2017-08-23T21:55:49
#
#-------------------------------------------------

QT       += core gui widgets sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SvB_Scanner
TEMPLATE = app

LIBS += -L/usr/local/lib\
     -lopencv_calib3d\
     -lopencv_contrib\
     -lopencv_core\
     -lopencv_features2d \
     -lopencv_flann\
     -lopencv_gpu\
     -lopencv_highgui\
     -lopencv_imgproc\
     -lopencv_legacy\
     -lopencv_ml\
     -lopencv_objdetect\
     -lopencv_ocl\
     -lopencv_photo\
     -lopencv_stitching\
     -lopencv_superres\
     -lopencv_ts\
     -lopencv_video\
     -lopencv_videostab\
    -lzbar

SOURCES += main.cpp\
        svbwindow.cpp

HEADERS  += svbwindow.h

FORMS    += svbwindow.ui
