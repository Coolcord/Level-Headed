#-------------------------------------------------
#
# Project created by QtCreator 2014-09-13T19:56:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Level-Headed
TEMPLATE = app
CONFIG += c++11 pendantic Weverything


SOURCES +=\
        Main_Window.cpp \
    Main.cpp \
    Plugin_Handler.cpp

HEADERS  += Main_Window.h \
    Plugin_Handler.h \
    Common_Strings.h \
    Interpreter_Interface.h \

FORMS    += Main_Window.ui

RC_FILE = Level-Headed.rc

OTHER_FILES +=

