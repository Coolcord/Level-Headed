#-------------------------------------------------
#
# Project created by QtCreator 2014-08-31T20:08:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SMB1_Writer
TEMPLATE = lib
CONFIG += plugin c++11 pendantic Wall

SOURCES += SMB1_Writer.cpp \
    Item_Writer.cpp \
    Binary_Manipulator.cpp \
    Object_Writer.cpp \
    Header_Writer.cpp \
    Enemy_Writer.cpp \
    SMB1_Writer_Accessors.cpp \
    ROM_Handler.cpp \
    ROM_Checksum.cpp \
    Midpoint_Writer.cpp \
    Room_Order_Writer.cpp \
    Room_ID_Handler.cpp \
    Room_Address_Writer.cpp \
    Level_Offset.cpp \
    Enemy_Bytes_Tracker.cpp \
    Hacks.cpp \
    Music.cpp \
    Byte_Writer.cpp \
    Graphics.cpp

HEADERS += SMB1_Writer.h \
    ROM_Type.h \
    Item_Writer.h \
    Binary_Manipulator.h \
    Object_Writer.h \
    Scenery.h \
    Level_Offset.h \
    Background.h \
    ROM_Checksum.h \
    Level_Compliment.h \
    Header_Writer.h \
    Enemy_Writer.h \
    ../Common_SMB1_Files/Level_Attribute.h \
    ../Common_SMB1_Files/Level_Compliment.h \
    ../Common_SMB1_Files/Background.h \
    ../Common_SMB1_Files/Brick.h \
    ../Common_SMB1_Files/Scenery.h \
    ../../Common_Files/Version.h \
    SMB1_Writer_Interface.h \
    ../Common_SMB1_Files/Level.h \
    ROM_Handler.h \
    ROM_Filename.h \
    Midpoint_Writer.h \
    Room_Order_Writer.h \
    Room_ID_Handler.h \
    Room_Address_Writer.h \
    Enemy_Bytes_Tracker.h \
    Hacks.h \
    Music.h \
    Byte_Writer.h \
    Graphics.h
OTHER_FILES +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}
