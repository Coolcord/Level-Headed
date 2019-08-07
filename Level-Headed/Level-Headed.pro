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
    ../../C_Common_Code/Qt/Git_Update_Checker/Git_Update_Checker.cpp \
    Main_Window.cpp \
    Main.cpp \
    Plugin_Handler.cpp \
    CLI_Passthrough.cpp \
    ../Common_Files/Random.cpp

HEADERS  += Main_Window.h \
    ../../C_Common_Code/Qt/Git_Update_Checker/Git_Update_Checker.h \
    Plugin_Handler.h \
    Common_Strings.h \
    Interpreter_Interface.h \
    CLI_Passthrough.h

FORMS    += Main_Window.ui

RC_FILE = Level-Headed.rc

OTHER_FILES +=

