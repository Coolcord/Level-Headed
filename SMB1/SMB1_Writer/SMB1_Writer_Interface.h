#ifndef SMB1_WRITER_INTERFACE_H
#define SMB1_WRITER_INTERFACE_H

#include "../Common SMB1 Files/Level.h"
#include <QObject>
#include <QtPlugin>
#include <QString>

class Object_Writer;
class Enemy_Writer;
class Header_Writer;

class SMB1_Writer_Interface : public QObject {
public:
    virtual void Shutdown()=0;
    virtual bool Load_ROM(const QString &romLocation)=0;
    virtual bool New_Level(Level::Level level)=0;
    virtual bool Write_Level()=0;
    virtual int Get_Num_Object_Bytes()=0;
    virtual int Get_Num_Enemy_Bytes()=0;
    virtual Object_Writer *Get_Object_Writer()=0;
    virtual Enemy_Writer *Get_Enemy_Writer()=0;
    virtual Header_Writer *Get_Header_Writer()=0;
};

Q_DECLARE_INTERFACE(SMB1_Writer_Interface, "SMB1_Writer_Interface")

#endif // SMB1_WRITER_INTERFACE_H
