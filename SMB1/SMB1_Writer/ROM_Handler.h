#ifndef ROM_HANDLER_H
#define ROM_HANDLER_H

#include "ROM_Type.h"
#include <QStringList>
#include <QFile>
#include <QString>
#include <QWidget>

class ROM_Checksum;
class Sequential_Archive_Handler;

class ROM_Handler {
public:
    ROM_Handler(QWidget *parent, const QString &applicationLocation);
    ~ROM_Handler();
    void Set_Sequential_Archive_Handler(Sequential_Archive_Handler *sequentialArchiveHandler);
    QString Install_ROM();
    bool Clean_ROM_Directory();
    QString Get_Installed_ROM_Folder_Location();
    QStringList Get_Installed_ROMs();
    QFile *Load_Local_ROM(const QString &fileName, bool &cancel);
    QFile *Load_First_Local_ROM(bool &cancel);
    ROM_Type::ROM_Type Get_ROM_Type() const;
    QString Get_Ouput_ROM_Location();
    void Set_Output_ROM_Location(const QString &location);

private:
    void Show_Error(const QString &error);

    ROM_Type::ROM_Type romType;
    ROM_Checksum *romChecksum;
    Sequential_Archive_Handler *sequentialArchiveHandler;
    QFile *file;
    QString romFolderLocation;
    QString applicationLocation;
    QString outputROMLocation;
    QWidget *parent;
};

#endif // ROM_HANDLER_H
