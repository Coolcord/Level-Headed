#-------------------------------------------------
#
# Project created by QtCreator 2014-09-07T12:47:33
#
#-------------------------------------------------

QT       -= gui

TARGET = SMB1_Compliance_Generator
TEMPLATE = lib
CONFIG += staticlib c++11

SOURCES += SMB1_Compliance_Generator.cpp \
    Object_Writer.cpp \
    Enemy_Writer.cpp \
    Header_Writer.cpp \
    Item_Writer.cpp \
    Level_Generator.cpp \
    Standard_Overworld_Generator.cpp

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
    Standard_Overworld_Generator.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
