#-------------------------------------------------
#
# Project created by QtCreator 2013-05-04T11:29:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtCdr
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    ccdrdefine.cpp \
    ccdrdata.cpp \
    ccdrbaseinfo.cpp \
    ccdrtableviewext.cpp

HEADERS  += \
    ccdrdefine.h \
    ccdrdata.h \
    ccdrbaseinfo.h \
    dialog.h \
    ccdrtableviewext.h

FORMS    += dialog.ui

RESOURCES +=

QMAKE_CXXFLAGS += -std=c++11

RC_FILE = myapp.rc

#QMAKE_CXX = g++-dw2
#QMAKE_CC = gcc-dw2
#QMAKE_LINK = g++-dw2
