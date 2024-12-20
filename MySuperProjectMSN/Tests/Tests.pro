QT += testlib
QT += network
CONFIG += qt warn_on depend_includepath testcase testlib

DEPENDPATH += ../Server

TEMPLATE = app

SOURCES +=  tst_testload.cpp

INCLUDEPATH += ../Server

DISTFILES += \
    tst_servertest.qml

SOURCES += ../Server/server.cpp
HEADERS += ../Server/server.h
