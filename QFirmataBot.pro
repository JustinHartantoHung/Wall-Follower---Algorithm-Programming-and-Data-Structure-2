#-------------------------------------------------
#
# Project created by QtCreator 2016-03-02T08:13:14
#
#-------------------------------------------------

QT       += core gui
CONFIG += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
win32::INCLUDEPATH += C:\AlgorithmProgramSem2\WallFollowerGroup3\src

TARGET = QFirmataBot
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp\
    analogpin.cpp \
    digitalpin.cpp \
    pin.cpp \
    qcustomplot.cpp


HEADERS  += mainwindow.h \
    analogpin.h \
    digitalpin.h \
    pin.h \
    qcustomplot.h

FORMS    += mainwindow.ui

win32::include(C:\AlgorithmProgramSem2\WallFollowerGroup3\QFirmata.pri)
linux::include($$PWD/../QFirmata.pri)
