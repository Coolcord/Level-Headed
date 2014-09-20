#-------------------------------------------------
#
# Project created by QtCreator 2014-09-14T12:02:15
#
#-------------------------------------------------

QT       += core gui

TARGET = SMB1_Compliance_To_SMB1
TEMPLATE = lib
CONFIG += plugin

SOURCES += SMB1_Compliance_To_SMB1.cpp \
    SMB1_Compliance_Parser.cpp \
    Enemy_Handler.cpp \
    Object_Handler.cpp \
    Item_Handler.cpp

HEADERS += SMB1_Compliance_To_SMB1.h \
    ../../Level-Headed/Interpreter_Interface.h \
    ../../Level-Headed/Common_Strings.h \
    ../SMB1_Compliance_Generator/SMB1_Compliance_Generator_Interface.h \
    ../SMB1_Writer/SMB1_Writer_Interface.h \
    SMB1_Compliance_Parser.h \
    Enemy_Handler.h \
    Object_Handler.h \
    Item_Handler.h

OTHER_FILES +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}
