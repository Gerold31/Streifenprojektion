QT += core serialport
LIBS += -lopencv_core -lopencv_highgui
QMAKE_CXXFLAGS += -std=c++14

INCLUDEPATH += $$PWD/../prototype1/include

SOURCES += $$PWD/../prototype1/src/*.cpp
HEADERS += $$PWD/../prototype1/include/*.h

DEFINES += MAINLESS
