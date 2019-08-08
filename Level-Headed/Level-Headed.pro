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


SOURCES += \
    ../../C_Common_Code/Qt/Git_Update_Checker/Git_Update_Checker.cpp \
    ../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.cpp \
    ../Common_Files/Random.cpp \
    Main_Window.cpp \
    Main.cpp \
    Plugin_Handler.cpp \
    CLI_Passthrough.cpp \
    Update_Dialog.cpp \
    Update_Thread.cpp

HEADERS  += \
    Main_Window.h \
    ../../C_Common_Code/Qt/Git_Update_Checker/Git_Update_Checker.h \
    ../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.h \
    ../Common_Files/Version.h \
    ../Common_Files/Random.h \
    Plugin_Handler.h \
    Common_Strings.h \
    Interpreter_Interface.h \
    CLI_Passthrough.h \
    Update_Dialog.h \
    Update_Thread.h

FORMS    += Main_Window.ui \
    Update_Dialog.ui

RC_FILE = Level-Headed.rc

OTHER_FILES +=

