#-------------------------------------------------
#
# Project created by QtCreator 2016-01-09T15:18:52
#
#-------------------------------------------------

QT       += core gui
QT       += network sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    options.cpp \
    chat_menu.cpp \
    adding_friend.cpp \
    login.cpp \
    registration.cpp \
    change_info.cpp \
    removing_friend.cpp

HEADERS  += widget.h \
    options.h \
    chat_menu.h \
    adding_friend.h \
    login.h \
    registration.h \
    change_info.h \
    removing_friend.h


#CONFIG += mobility
#MOBILITY =

