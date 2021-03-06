#-------------------------------------------------
#
# Project created by QtCreator 2014-09-14T12:02:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SMB1_Compliance_To_SMB1
TEMPLATE = lib
CONFIG += plugin c++11 pendantic Weverything

SOURCES += SMB1_Compliance_To_SMB1.cpp \
    ../../../C_Common_Code/Qt/Text_Insertion_Buffer/Text_Insertion_Buffer.cpp \
    ../SMB1_Compliance_Generator/Enemy_Buffer.cpp \
    ../SMB1_Compliance_Generator/Item_Buffer.cpp \
    ../SMB1_Compliance_Generator/Object_Buffer.cpp \
    Config_File_Handler.cpp \
    Level_Generator.cpp \
    ../SMB1_Compliance_Generator/SMB1_Compliance_Map.cpp \
    Hacks_Handler.cpp \
    Difficulty_Level_Configurations.cpp \
    Configure_Settings_Form.cpp \
    Tab_Base_Game.cpp \
    Tab_Level_Generator.cpp \
    Tab_Difficulty.cpp \
    CLI_Parser.cpp \
    ../../../C_Common_Code/Qt/Random/Random.cpp \
    ../../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.cpp

HEADERS += SMB1_Compliance_To_SMB1.h \
    ../../../C_Common_Code/Qt/Text_Insertion_Buffer/Text_Insertion_Buffer.h \
    ../Common_SMB1_Files/Background.h \
    ../Common_SMB1_Files/Background_String.h \
    ../Common_SMB1_Files/Brick.h \
    ../Common_SMB1_Files/Brick_String.h \
    ../Common_SMB1_Files/Castle.h \
    ../Common_SMB1_Files/Enemy_Item.h \
    ../Common_SMB1_Files/Enemy_Item_String.h \
    ../Common_SMB1_Files/Header_String.h \
    ../Common_SMB1_Files/Level.h \
    ../Common_SMB1_Files/Level_Attribute.h \
    ../Common_SMB1_Files/Level_Attribute_String.h \
    ../Common_SMB1_Files/Level_Compliment.h \
    ../Common_SMB1_Files/Level_Compliment_String.h \
    ../Common_SMB1_Files/Level_String.h \
    ../Common_SMB1_Files/Level_Type.h \
    ../Common_SMB1_Files/Level_Type_String.h \
    ../Common_SMB1_Files/Object_Item.h \
    ../Common_SMB1_Files/Object_Item_String.h \
    ../Common_SMB1_Files/Scenery.h \
    ../Common_SMB1_Files/Scenery_String.h \
    ../SMB1_Compliance_Generator/Buffer_Data.h \
    ../SMB1_Compliance_Generator/Enemy_Buffer.h \
    ../SMB1_Compliance_Generator/Item_Buffer.h \
    ../SMB1_Compliance_Generator/Object_Buffer.h \
    Config_File_Handler.h \
    Level_Generator.h \
    Plugin_Settings.h \
    Hacks_Handler.h \
    SMB1_Compliance_To_SMB1_Strings.h \
    Difficulty_Level_Configurations.h \
    Difficulty_Level_Settings.h \
    Configure_Settings_Form.h \
    Tab_Base_Game.h \
    Tab_Level_Generator.h \
    Tab_Difficulty.h \
    Tab_Interface.h \
    CLI_Parser.h \
    ../../../C_Common_Code/Qt/Random/Random.h \
    ../../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.h

OTHER_FILES +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    Configure_Settings_Form.ui
