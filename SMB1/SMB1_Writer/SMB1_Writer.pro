#-------------------------------------------------
#
# Project created by QtCreator 2014-08-31T20:08:10
#
#-------------------------------------------------

QT       -= gui

TARGET = SMB1_Writer
TEMPLATE = lib
CONFIG += staticlib c++11

SOURCES += SMB1_Writer.cpp \
    Level_Offsets.cpp \
    Item_Writer.cpp \
    Binary_Manipulator.cpp \
    Object_Writer.cpp \
    Header_Writer.cpp

HEADERS += SMB1_Writer.h \
    ROM_Type.h \
    Item_Writer.h \
    Level.h \
    Binary_Manipulator.h \
    Object_Writer.h \
    Scenery.h \
    Brick.h \
    Level_Offset.h \
    Background.h \
    ROM_Checksum.h \
    Level_Compliment.h \
    Header_Writer.h \
    Level_Type.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
