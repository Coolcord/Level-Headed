#ifndef SMB1_WRITER_INTERFACE_H
#define SMB1_WRITER_INTERFACE_H

#include <QObject>
#include <QtPlugin>
#include <QString>

class SMB1_Writer_Interface : public QObject {
public:
    virtual void Shutdown()=0;
    virtual bool Load_ROM(const QString &romLocation)=0;
    virtual bool New_Level(const int objectOffset, const int enemyOffset)=0;
    virtual bool Write_Level()=0;
};

Q_DECLARE_INTERFACE(SMB1_Writer_Interface, "SMB1_Writer_Interface")

#endif // SMB1_WRITER_INTERFACE_H
