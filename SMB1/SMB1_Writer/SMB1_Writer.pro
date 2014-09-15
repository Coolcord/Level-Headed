#-------------------------------------------------
#
# Project created by QtCreator 2014-08-31T20:08:10
#
#-------------------------------------------------

QT       += core gui

TARGET = SMB1_Writer
TEMPLATE = lib
CONFIG += plugin c++11

SOURCES += SMB1_Writer.cpp \
    Level_Offsets.cpp \
    Item_Writer.cpp \
    Binary_Manipulator.cpp \
    Object_Writer.cpp \
    Header_Writer.cpp \
    Enemy_Writer.cpp \
    Level_Pointers_Writer.cpp

HEADERS += SMB1_Writer.h \
    ROM_Type.h \
    Item_Writer.h \
    Level.h \
    Binary_Manipulator.h \
    Object_Writer.h \
    Scenery.h \
    Level_Offset.h \
    Background.h \
    ROM_Checksum.h \
    Level_Compliment.h \
    Header_Writer.h \
    Enemy_Writer.h \
    Level_Pointers_Writer.h \
    ../Common SMB1 Files/Level_Attribute.h \
    ../Common SMB1 Files/Level_Compliment.h \
    ../Common SMB1 Files/Background.h \
    ../Common SMB1 Files/Brick.h \
    ../Common SMB1 Files/Scenery.h \
    SMB1_Writer_Interface.h
OTHER_FILES +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}
