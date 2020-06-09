#ifndef SEQUENTIAL_ARCHIVE_HANDLER_H
#define SEQUENTIAL_ARCHIVE_HANDLER_H

#include "../../../Hexagon/Hexagon/Hexagon_Interface.h"
#include "../../../Sequential_Archive/Sequential_Archive/Sequential_Archive_Interface.h"
#include <QPluginLoader>
#include <QString>
#include <QSet>

class Text;

class Sequential_Archive_Handler {
public:
    Sequential_Archive_Handler(const QString &applicationLocation, const QString &romFolderLocation);
    ~Sequential_Archive_Handler();
    bool Was_Mario_Sprite_A_Bonus_Sprite();
    bool Get_Combine_Graphics_Packs();
    bool Get_Combine_Music_Packs();
    void Set_Combine_Graphics_Packs(bool combineGraphicsPacks);
    void Set_Combine_Music_Packs(bool combineMusicPacks);
    void Set_File(QFile *file);
    void Set_Text(Text *text);
    bool Apply_Graphics_Fix(const QString &fixName);
    bool Apply_Mario_Sprite_And_Graphics_Fixes(const QString &fixName);
    bool Apply_Graphics_Pack_At_Index(int index);
    bool Apply_Graphics_Sprite(const QString &spriteName, const QString &patchName);
    bool Apply_Mario_Sprite_Fix(const QString &fixName);
    bool Apply_Mario_Sprite_At_Index(int index);
    bool Apply_Random_Graphics_Sprite(const QString &spriteName);
    bool Apply_Random_Graphics_Sprite(const QString &spriteName, QString &patchName);
    bool Apply_Random_SMB1_Mario_Sprite();
    bool Apply_Hexagon_Patch(const QByteArray &patchBytes);
    bool Apply_Music_Pack_At_Index(int index);
    QStringList Get_Bonus_Graphics_Packs();
    QStringList Get_Bonus_Mario_Sprites();
    QStringList Get_Bonus_Music_Packs();
    QStringList Get_Graphics_Packs();
    QStringList Get_Mario_Sprites();
    QString Get_Graphics_Pack_At_Index(int index);
    QString Get_Last_Applied_Graphics_Pack();
    QString Get_Last_Applied_Mario_Sprite();
    QStringList Get_Music_Packs();
    QString Get_Music_Pack_At_Index(int index);
    int Get_Number_Of_Bonus_Graphics_Packs();
    int Get_Number_Of_Bonus_Mario_Sprites();
    int Get_Number_Of_Bonus_Music_Packs();
    int Get_Number_Of_Graphics_Packs();
    int Get_Number_Of_Mario_Sprites();
    int Get_Number_Of_Music_Packs();
    bool Random_But_Our_Princess_Is_In_Another_Castle_Text();
    bool Random_Thank_You_Text();
    bool Is_Tone_Invalid(int tone);
    bool Are_Color_Palettes_Allowed();
    bool Are_Only_Coin_Palettes_Allowed();
    QByteArray Read_Graphics_Fix(const QString &fixName);
    QByteArray Read_Graphics_Fix(const QString &fixName, QString graphicsPack);
    QByteArray Read_Graphics_Sprite(const QString &spriteName, const QString &patchName);
    QByteArray Read_Mario_Sprite_Fix(const QString &fixName);
    QByteArray Read_Mario_Sprite_Fix(const QString &fixName, QString marioSprite);
    QByteArray Read_Random_Graphics_Sprite(const QString &spriteName, QString &patchName);
    bool Is_Hexagon_Line_End_Of_Header(const QString &line);
    bool Install_ROM_Patches(const QString &romName);

private:
    bool Apply_Music_Pack(const QString &musicPack, bool isSecondaryPatch, QStringList &previouscompatiblePacks);
    bool Apply_Music_Pack_At_Index(int index, bool isSecondaryPatch);
    bool Apply_Secondary_Music_Patches(const QString &patchList);
    QStringList Get_Compatible_Music_Packs(const QByteArray &patchBytes);
    bool Get_Invalid_Tones(const QByteArray &patchBytes, bool isSecondaryPatch);
    QString Get_Replacement_Text(const QString &fileNameWithoutExtension);
    bool Get_Palettes_Allowed(const QByteArray &patchBytes);
    void Get_HEXP_Files_From_File_List(QStringList &normalFiles, QStringList &bonusFiles);
    QString Get_Replacement_Text_For_Castle();
    QString Get_Replacement_Text_For_Princess();
    QString Get_Replacement_Text_For_Thank_You();
    bool Load_Plugins_If_Necessary();
    bool Load_Hexagon_Plugin();
    bool Load_Sequential_Archive_Plugin();
    QByteArray Read_Music_Pack(const QString &musicPackString);
    QString Read_Attribute_From_Patch_Header(const QByteArray &patchBytes, const QString &attribute);
    bool Removed_Installed_ROM_Patches_On_Failure(const QStringList &patches, const QString &extension);

    QFile *file;
    QSet<int> *invalidTones;
    bool allowPalettes;
    bool allowOnlyCoinPalettes;
    bool combineGraphicsPacks;
    bool combineMusicPacks;
    bool wasMarioSpriteABonusSprite;
    Hexagon_Interface *hexagonPlugin;
    Sequential_Archive_Interface *sequentialArchivePlugin;
    QPluginLoader *hexagonLoader;
    QPluginLoader *sequentialArchiveLoader;
    Text *text;
    QStringList bonusGraphicsPacks;
    QStringList bonusMarioSprites;
    QStringList bonusMusicPacks;
    QStringList graphicsPackStrings;
    QStringList marioSpriteStrings;
    QStringList musicPackStrings;
    QString romFolderLocation;
    QString pluginLocation;
    QString romsArchiveLocation;
    QString graphicsPacksArchiveLocation;
    QString musicPacksArchiveLocation;
    QString textArchiveLocation;
    QString lastAppliedGraphicsPack;
    QString lastAppliedMarioSprite;
    QVector<int> *smb1MarioSpritesIndexes;
};

#endif // SEQUENTIAL_ARCHIVE_HANDLER_H
