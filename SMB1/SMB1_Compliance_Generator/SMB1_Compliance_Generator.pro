#-------------------------------------------------
#
# Project created by QtCreator 2014-09-07T12:47:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SMB1_Compliance_Generator
TEMPLATE = lib
CONFIG += plugin c++11 pendantic Weverything

SOURCES += SMB1_Compliance_Generator.cpp \
    Midpoint_Handler.cpp \
    Object_Writer.cpp \
    Enemy_Writer.cpp \
    Header_Writer.cpp \
    Item_Writer.cpp \
    Level_Generator.cpp \
    Standard_Overworld_Generator.cpp \
    Pipe_Pointer_Writer.cpp \
    Simple_Object_Spawner.cpp \
    End_Spawner.cpp \
    Common_Pattern_Spawner.cpp \
    Object_Spawner.cpp \
    Level_Crawler.cpp \
    SMB1_Compliance_Map.cpp \
    Enemy_Spawner.cpp \
    Bridge_Generator.cpp \
    First_Page_Handler.cpp \
    Island_Generator.cpp \
    Underwater_Generator.cpp \
    Underground_Generator.cpp \
    Item_Spawner.cpp \
    Required_Enemy_Spawns.cpp \
    Castle_Generator.cpp \
    Pipe_Exit_Generator.cpp \
    Continuous_Enemies_Spawner.cpp \
    ../../Common_Files/Random.cpp

HEADERS += SMB1_Compliance_Generator.h \
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
    Midpoint_Handler.h \
    Object_Writer.h \
    Enemy_Writer.h \
    Header_Writer.h \
    Item_Writer.h \
    Level_Generator.h \
    Standard_Overworld_Generator.h \
    Pipe_Pointer_Writer.h \
    Simple_Object_Spawner.h \
    Physics.h \
    SMB1_Compliance_Generator_Interface.h \
    End_Spawner.h \
    Common_Pattern_Spawner.h \
    Object_Spawner.h \
    Level_Crawler.h \
    SMB1_Compliance_Map.h \
    Enemy_Spawner.h \
    Bridge_Generator.h \
    First_Page_Handler.h \
    SMB1_Compliance_Generator_Arguments.h \
    End_Pattern.h \
    Island_Generator.h \
    Underwater_Generator.h \
    Underground_Generator.h \
    Item_Spawner.h \
    Required_Enemy_Spawns.h \
    Extra_Enemy_Args.h \
    Castle_Generator.h \
    Pipe_Exit_Generator.h \
    Difficulty.h \
    Continuous_Enemies_Spawner.h
OTHER_FILES +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}
