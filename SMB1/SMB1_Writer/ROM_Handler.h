#ifndef ROM_HANDLER_H
#define ROM_HANDLER_H

#include "ROM_Type.h"
#include <QStringList>
#include <QFile>
#include <QString>
#include <QWidget>

class ROM_Checksum;

class ROM_Handler
{
public:
    ROM_Handler(QWidget *parent, QString romFolderLocation);
    ~ROM_Handler();
    QString Install_ROM();
    bool Clean_ROM_Directory();
    QStringList Get_Installed_ROMs();
    QFile *Load_Local_ROM(const QString &fileName, bool &cancel);
    QFile *Load_First_Local_ROM(bool &cancel);
    ROM_Type::ROM_Type Get_ROM_Type() const;

private:
    void Show_Error(const QString &error);

    ROM_Type::ROM_Type romType;
    ROM_Checksum *romChecksum;
    QFile *file;
    QString romFolderLocation;
    QWidget *parent;
};

#endif // ROM_HANDLER_H
