#ifndef SEQUENTIAL_ARCHIVE_HANDLER_H
#define SEQUENTIAL_ARCHIVE_HANDLER_H

#include "../../../Hexagon/Hexagon/Hexagon_Interface.h"
#include "../../../Sequential_Archive/Sequential_Archive/Sequential_Archive_Interface.h"
#include <QPluginLoader>
#include <QString>

class Sequential_Archive_Handler {
public:
    Sequential_Archive_Handler(const QString &applicationLocation);
    ~Sequential_Archive_Handler();
    void Set_Combine_Music_Packs(bool combineMusicPacks);
    void Set_File(QFile *file);
    bool Apply_Graphics_Pack_At_Index(int index);
    bool Apply_Music_Pack_At_Index(int index);
    QStringList Get_Graphics_Packs();
    QString Get_Graphics_Pack_At_Index(int index);
    QStringList Get_Music_Packs();
    QString Get_Music_Pack_At_Index(int index);
    int Get_Number_Of_Graphics_Packs();
    int Get_Number_Of_Music_Packs();

private:
    bool Apply_Music_Pack(const QString &musicPack, bool isSecondaryPatch);
    bool Apply_Music_Pack_At_Index(int index, bool isSecondaryPatch);
    bool Apply_Secondary_Music_Patches(const QString &patchList);
    QStringList Get_Compatible_Music_Packs(const QByteArray &patchBytes);
    bool Load_Plugins_If_Necessary();
    bool Load_Hexagon_Plugin();
    bool Load_Sequential_Archive_Plugin();
    QByteArray Read_Music_Pack(const QString &musicPackString);

    QFile *file;
    bool combineMusicPacks;
    Hexagon_Interface *hexagonPlugin;
    Sequential_Archive_Interface *sequentialArchivePlugin;
    QPluginLoader *hexagonLoader;
    QPluginLoader *sequentialArchiveLoader;
    QStringList graphicsPackStrings;
    QStringList musicPackStrings;
    QString pluginLocation;
    QString graphicsPacksArchiveLocation;
    QString musicPacksArchiveLocation;
};

#endif // SEQUENTIAL_ARCHIVE_HANDLER_H
