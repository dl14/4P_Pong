#-------------------------------------------------
#
# Project created by QtCreator 2016-10-28T10:36:04
#
#-------------------------------------------------

QT       += core gui
QT       += network widgets testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dgs_client
TEMPLATE = app


SOURCES += main.cpp \
    client.cpp \
    dataparser.cpp \
    datasender.cpp \
    graphicswidget.cpp \
    gsm.cpp \
    game.cpp \
    usergo.cpp \
    userwidget.cpp \
    shapego.cpp \
    paddlego.cpp \
    gamecontrollerwidget.cpp \
    abstractshapego.cpp \
    sendmsghandler.cpp \
    gamemodewidget.cpp \
    infogo.cpp \
    logger.cpp \
    legendwidget.cpp

HEADERS  += \
    client.h \
    dataparser.h \
    datasender.h \
    graphicswidget.h \
    gsm.h \
    game.h \
    usergo.h \
    userwidget.h \
    shapego.h \
    paddlego.h \
    gamecontrollerwidget.h \
    abstractshapego.h \
    globalvalues.h \
    sendmsghandler.h \
    gamemodewidget.h \
    infogo.h \
    logger.h \
    legendwidget.h

FORMS    +=
