#-------------------------------------------------
#
# Project created by QtCreator 2014-09-07T12:47:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SMB1_Compliance_Generator
TEMPLATE = lib
CONFIG += plugin c++11 pendantic Wall

SOURCES += SMB1_Compliance_Generator.cpp \
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
    Midpoint_Handler.cpp \
    Bridge_Generator.cpp \
    First_Page_Handler.cpp \
    Island_Generator.cpp

HEADERS += SMB1_Compliance_Generator.h \
    Object_Writer.h \
    Enemy_Writer.h \
    Header_Writer.h \
    Item_Writer.h \
    Item_Type.h \
    ../Common SMB1 Files/Level_Attribute.h \
    ../Common SMB1 Files/Background.h \
    ../Common SMB1 Files/Brick.h \
    ../Common SMB1 Files/Scenery.h \
    ../Common SMB1 Files/Enemy_Item.h \
    ../Common SMB1 Files/Enemy_Item_String.h \
    ../Common SMB1 Files/Object_Item.h \
    ../Common SMB1 Files/Object_Item_String.h \
    ../Common SMB1 Files/Level_Compliment.h \
    ../Common SMB1 Files/Level_Type.h \
    ../Common SMB1 Files/Level_Type_String.h \
    ../Common SMB1 Files/Level_Compliment_String.h \
    ../Common SMB1 Files/Level_Attribute_String.h \
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
    Midpoint_Handler.h \
    Bridge_Generator.h \
    First_Page_Handler.h \
    SMB1_Compliance_Generator_Arguments.h \
    End_Pattern.h \
    Island_Generator.h
OTHER_FILES +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}
