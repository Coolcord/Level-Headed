#-------------------------------------------------
#
# Project created by QtCreator 2014-09-07T12:47:33
#
#-------------------------------------------------

QT       -= gui

TARGET = SMB1_Compliance_Generator
TEMPLATE = lib
CONFIG += plugin c++11

SOURCES += SMB1_Compliance_Generator.cpp \
    Object_Writer.cpp \
    Enemy_Writer.cpp \
    Header_Writer.cpp \
    Item_Writer.cpp \
    Level_Generator.cpp \
    Standard_Overworld_Generator.cpp \
    Pipe_Pointer_Writer.cpp \
    Simple_Object_Spawner.cpp \
    Spawner.cpp

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
    Spawner.h \
    Physics.h
OTHER_FILES += SMB1_Compliance_Generator.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
