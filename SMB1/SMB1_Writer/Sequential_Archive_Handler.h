#ifndef SEQUENTIAL_ARCHIVE_HANDLER_H
#define SEQUENTIAL_ARCHIVE_HANDLER_H

#include "../../../Hexagon/Hexagon/Hexagon_Interface.h"
#include "../../../Sequential_Archive/Sequential_Archive/Sequential_Archive_Interface.h"
#include <QPluginLoader>
#include <QString>
#include <QSet>

class Sequential_Archive_Handler {
public:
    Sequential_Archive_Handler(const QString &applicationLocation, const QString &romFolderLocation);
    ~Sequential_Archive_Handler();
    void Set_Combine_Graphics_Packs(bool combineGraphicsPacks);
    void Set_Combine_Music_Packs(bool combineMusicPacks);
    void Set_File(QFile *file);
    bool Apply_Graphics_Fix(const QString &fixName, const QString &fixType);
    bool Apply_Graphics_Fix(const QByteArray &patchBytes);
    bool Apply_Graphics_Pack_At_Index(int index);
    bool Apply_Music_Pack_At_Index(int index);
    QStringList Get_Bonus_Graphics_Packs();
    QStringList Get_Bonus_Music_Packs();
    QStringList Get_Graphics_Packs();
    QString Get_Graphics_Pack_At_Index(int index);
    QString Get_Last_Applied_Graphics_Pack();
    QStringList Get_Music_Packs();
    QString Get_Music_Pack_At_Index(int index);
    int Get_Number_Of_Bonus_Graphics_Packs();
    int Get_Number_Of_Bonus_Music_Packs();
    int Get_Number_Of_Graphics_Packs();
    int Get_Number_Of_Music_Packs();
    bool Is_Tone_Invalid(int tone);
    bool Are_Color_Palettes_Allowed();
    bool Are_Only_Coin_Palettes_Allowed();
    QByteArray Read_Graphics_Fix(const QString &fixName, const QString &fixType);
    bool Is_Hexagon_Line_End_Of_Header(const QString &line);
    bool Install_ROM_Patches(const QString &romName);

private:
    bool Apply_Music_Pack(const QString &musicPack, bool isSecondaryPatch, QStringList &previouscompatiblePacks);
    bool Apply_Music_Pack_At_Index(int index, bool isSecondaryPatch);
    bool Apply_Secondary_Music_Patches(const QString &patchList);
    QStringList Get_Compatible_Music_Packs(const QByteArray &patchBytes);
    bool Get_Invalid_Tones(const QByteArray &patchBytes, bool isSecondaryPatch);
    bool Get_Palettes_Allowed(const QByteArray &patchBytes);
    void Get_HEXP_Files_From_File_List(QStringList &normalFiles, QStringList &bonusFiles);
    bool Load_Plugins_If_Necessary();
    bool Load_Hexagon_Plugin();
    bool Load_Sequential_Archive_Plugin();
    QByteArray Read_Music_Pack(const QString &musicPackString);
    bool Removed_Installed_ROM_Patches_On_Failure(const QStringList &patches, const QString &extension);

    QFile *file;
    QSet<int> *invalidTones;
    bool allowPalettes;
    bool allowOnlyCoinPalettes;
    bool combineGraphicsPacks;
    bool combineMusicPacks;
    Hexagon_Interface *hexagonPlugin;
    Sequential_Archive_Interface *sequentialArchivePlugin;
    QPluginLoader *hexagonLoader;
    QPluginLoader *sequentialArchiveLoader;
    QStringList bonusGraphicsPacks;
    QStringList bonusMusicPacks;
    QStringList graphicsPackStrings;
    QStringList musicPackStrings;
    QString romFolderLocation;
    QString pluginLocation;
    QString romsArchiveLocation;
    QString graphicsPacksArchiveLocation;
    QString musicPacksArchiveLocation;
    QString lastAppliedGraphicsPack;
};

#endif // SEQUENTIAL_ARCHIVE_HANDLER_H
