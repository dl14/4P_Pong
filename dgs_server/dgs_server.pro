#-------------------------------------------------
#
# Project created by QtCreator 2016-11-09T10:23:20
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dgs_server
TEMPLATE = app


SOURCES += main.cpp \
    dataparser.cpp \
    dialog.cpp \
    serverthread.cpp \
    server.cpp \
    objectrepo.cpp \
    msghandler.cpp \
    logger.cpp

HEADERS  += \
    dataparser.h \
    dialog.h \
    serverthread.h \
    server.h \
    globalvalues.h \
    objectrepo.h \
    msghandler.h \
    logger.h
