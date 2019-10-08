#-------------------------------------------------
#
# Project created by QtCreator 2014-08-31T20:08:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SMB1_Writer
TEMPLATE = lib
CONFIG += plugin c++11 pendantic Weverything

SOURCES += SMB1_Writer.cpp \
    Bowser_Bridge_Destroyer.cpp \
    Colors.cpp \
    Graphics_Combiner.cpp \
    Item_Writer.cpp \
    Binary_Manipulator.cpp \
    Object_Writer.cpp \
    Header_Writer.cpp \
    Enemy_Writer.cpp \
    Palettes.cpp \
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
    Graphics.cpp \
    Text.cpp \
    Sequential_Archive_Handler.cpp \
    Powerups.cpp \
    Sound.cpp \
    ../../Common_Files/Random.cpp

HEADERS += SMB1_Writer.h \
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
    Bowser_Bridge_Destroyer.h \
    Color.h \
    Colors.h \
    Graphics_Combiner.h \
    Palettes.h \
    ROM_Type.h \
    Item_Writer.h \
    Binary_Manipulator.h \
    Object_Writer.h \
    Level_Offset.h \
    ROM_Checksum.h \
    Header_Writer.h \
    Enemy_Writer.h \
    SMB1_Writer_Interface.h \
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
    Graphics.h \
    Text.h \
    Sequential_Archive_Handler.h \
    Powerups.h \
    Unfixed_ROM_Type.h \
    Sound.h \
    ../../Common_Files/Random.h \
    ../../Common_Files/Version.h \
    ../Common_SMB1_Files/Fix_Strings.h
OTHER_FILES +=

unix {
    target.path = /usr/lib
    INSTALLS += target
}
