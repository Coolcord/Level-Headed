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
    Item_Writer.cpp

HEADERS += SMB1_Compliance_Generator.h \
    ../Common SMB1 Files/Level_Attribute.h \
    Level_Type.h \
    Object_Writer.h \
    Enemy_Writer.h \
    Header_Writer.h \
    Item_Writer.h \
    ../Common SMB1 Files/Background.h \
    ../Common SMB1 Files/Brick.h \
    ../Common SMB1 Files/Scenery.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
