#-------------------------------------------------
#
# Project created by QtCreator 2014-09-14T12:02:15
#
#-------------------------------------------------

QT       += core gui

TARGET = SMB1_Compliance_To_SMB1
TEMPLATE = lib
CONFIG += plugin

SOURCES += SMB1_Compliance_To_SMB1.cpp

HEADERS += SMB1_Compliance_To_SMB1.h \
    ../../Level-Headed/Interpreter_Interface.h

OTHER_FILES += SMB1_Compliance_To_SMB1.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
