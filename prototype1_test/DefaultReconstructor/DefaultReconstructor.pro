#-------------------------------------------------
#
# Project created by QtCreator 2015-01-21T17:50:46
#
#-------------------------------------------------

include(../prototype1_test.pri)

QT       += testlib

QT       -= gui

TARGET = tst_defaultreconstructortest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_defaultreconstructortest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
