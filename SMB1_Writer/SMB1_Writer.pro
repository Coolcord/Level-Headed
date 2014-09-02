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
    Object_Writer.cpp

HEADERS += SMB1_Writer.h \
    Level_Offsets.h \
    ROM_Checksums.h \
    ROM_Type.h \
    Item_Writer.h \
    Level.h \
    Binary_Manipulator.h \
    Object_Writer.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
