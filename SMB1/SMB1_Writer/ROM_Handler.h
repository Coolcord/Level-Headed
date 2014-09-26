#ifndef ROM_HANDLER_H
#define ROM_HANDLER_H

#include "ROM_Type.h"
#include <QFile>
#include <QString>
#include <QWidget>

class ROM_Checksum;

class ROM_Handler
{
public:
    ROM_Handler(QWidget *parent, QString romFolderLocation);
    ~ROM_Handler();
    bool Install_ROM();
    bool Clean_ROM_Directory();
    QFile *Load_Local_ROM(const QString &fileName);
    QFile *Load_First_Local_ROM();
    ROM_Type::ROM_Type Get_ROM_Type();

private:
    bool Check_ROM_Header();
    bool Check_NES_ROM_Header(QByteArray *buffer);
    bool Check_FDS_ROM_Header(QByteArray *buffer);
    ROM_Type::ROM_Type Determine_ROM_Type();
    ROM_Type::ROM_Type romType;
    ROM_Checksum *romChecksum;
    QFile *file;
    QString romFolderLocation;
    QWidget *parent;
};

#endif // ROM_HANDLER_H
