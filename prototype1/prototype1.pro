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
    src/defaultreconstructor.cpp \
    src/deviceconfiguration.cpp \
    src/laser.cpp \
    src/line.cpp \
    src/main.cpp \
    src/reconstructor.cpp \
    src/serial.cpp \
    src/servo.cpp \
    src/lightbardetector.cpp \
    src/difflightbardetector.cpp \
    src/reconstruction.cpp

HEADERS += \ 
    include/camera.h \
    include/configuration.h \
    include/defaultreconstructor.h \
    include/deviceconfiguration.h \
    include/laser.h \
    include/line.h \
    include/reconstructor.h \
    include/serial.h \
    include/servo.h \
    include/singleton.h \
    include/lightbardetector.h \
    include/difflightbardetector.h \
    include/reconstruction.h