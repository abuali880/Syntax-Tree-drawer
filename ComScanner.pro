#-------------------------------------------------
#
# Project created by QtCreator 2016-11-03T18:49:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ComScanner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    node.cpp \
    tree.cpp

HEADERS  += mainwindow.h \
    node.h \
    tree.h

FORMS    += mainwindow.ui
