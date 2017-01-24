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
    logger.cpp

HEADERS += \
    mytcpsocket.h \
    myclientsocket.h \
    myserversocket.h \
    fileowner.h \
    client.h \
    filesystem.h \
    tcpconn.h \
    logger.h
LIBS += \
       -lboost_system\
       -lboost_filesystem\

