#-------------------------------------------------
#
# Project created by QtCreator 2017-03-02T01:08:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = practice
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp

HEADERS  += \
    mainwindow.h

FORMS    += \
    mainwindow.ui

INCLUDEPATH += /home/dan/.opencv/include

LIBS += -L/home/dan/.opencv/lib/ -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
LIBS += -lHalf -lIlmImf
