#-------------------------------------------------
#
# Project created by QtCreator 2014-09-14T12:02:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SMB1_Compliance_To_SMB1
TEMPLATE = lib
CONFIG += plugin c++11 pendantic Wall

SOURCES += SMB1_Compliance_To_SMB1.cpp \
    Enemy_Handler.cpp \
    Object_Handler.cpp \
    Item_Handler.cpp \
    SMB1_Compliance_Parser.cpp \
    ../SMB1_Compliance_Generator/SMB1_Compliance_Map.cpp \
    Header_Handler.cpp \
    Level_Generator.cpp \
    Hacks_Handler.cpp \
    Difficulty_Level_Configurations.cpp \
    Difficulty_Level_Form.cpp \
    Configure_Settings_Form.cpp

HEADERS += SMB1_Compliance_To_SMB1.h \
    ../../Level-Headed/Interpreter_Interface.h \
    ../../Level-Headed/Common_Strings.h \
    ../../Common_Files/Random.h \
    ../SMB1_Compliance_Generator/SMB1_Compliance_Generator_Interface.h \
    ../SMB1_Writer/SMB1_Writer_Interface.h \
    SMB1_Compliance_Parser.h \
    Enemy_Handler.h \
    Object_Handler.h \
    Item_Handler.h \
    ../SMB1_Compliance_Generator/SMB1_Compliance_Map.h \
    Header_Handler.h \
    Plugin_Settings.h \
    Level_Generator.h \
    Hacks_Handler.h \
    SMB1_Compliance_To_SMB1_Strings.h \
    Difficulty_Level_Configurations.h \
    Difficulty_Level_Settings.h \
    Difficulty_Level_Form.h \
    Configure_Settings_Form.h

OTHER_FILES +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    Difficulty_Level_Form.ui \
    Configure_Settings_Form.ui
