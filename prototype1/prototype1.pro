#-------------------------------------------------
#
# Project created by QtCreator 2014-12-22T17:22:42
#
#-------------------------------------------------

QT       += core serialport

QT       -= gui

TARGET = prototype1
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += include/

LIBS += -lopencv_core -lopencv_highgui

QMAKE_CXXFLAGS += -std=c++14

SOURCES += \ 
    src/camera.cpp \
    src/configuration.cpp \
    src/defaultlinedetector.cpp \
    src/defaultreconstructor.cpp \
    src/deviceconfiguration.cpp \
    src/laser.cpp \
    src/line.cpp \
    src/linedetector.cpp \
    src/main.cpp \
    src/pointcloud.cpp \
    src/reconstructor.cpp \
    src/serial.cpp \
    src/servo.cpp

HEADERS += \ 
    include/camera.h \
    include/configuration.h \
    include/defaultlinedetector.h \
    include/defaultreconstructor.h \
    include/deviceconfiguration.h \
    include/laser.h \
    include/line.h \
    include/linedetector.h \
    include/pointcloud.h \
    include/reconstructor.h \
    include/serial.h \
    include/servo.h \
    include/singleton.h
