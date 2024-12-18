include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

INCLUDEPATH += $$PWD/googletest/googletest/include
INCLUDEPATH += $$PWD/googletest/googlemock/include

# Ajouter les fichiers sources de Google Test
SOURCES += $$PWD/googletest/googletest/src/gtest-all.cc
SOURCES += $$PWD/googletest/googlemock/src/gmock-all.cc

SOURCES += \
        main.cpp \
        tst_loadtest.cpp
