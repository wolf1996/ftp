QT += core
QT -= gui

CONFIG += c++11

TARGET = untitled
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mytcpsocket.cpp \
    myclientsocket.cpp \
    myserversocket.cpp \
    fileowner.cpp \
    client.cpp \
    filesystem.cpp \
    tcpconn.cpp \
    logger.cpp \
    parser_arg.cpp

HEADERS += \
    mytcpsocket.h \
    myclientsocket.h \
    myserversocket.h \
    fileowner.h \
    client.h \
    filesystem.h \
    tcpconn.h \
    logger.h \
    win_include.h \
    parser_arg.h
unix {
LIBS +=-lboost_system\

LIBS += \
       -lboost_system\
       -lboost_filesystem\
}

win32 {
INCLUDEPATH += J:\QT\Tools\boost\boost_1_63_0
LIBS += -L"J:\QT\Tools\boost\boost_1_63_0\stage\lib" -lboost_system-mgw53-mt-1_63 -lboost_filesystem-mgw53-mt-1_63
LIBS += -L"J:\QT\Tools\mingw530_32\i686-w64-mingw32\lib" -lws2_32
}
