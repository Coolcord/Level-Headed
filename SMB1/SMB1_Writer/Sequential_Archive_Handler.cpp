#include "Sequential_Archive_Handler.h"
#include "../../Common_Files/Random.h"
#include "../../Common_Files/Version.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../Common_SMB1_Files/Fix_Strings.h"
#include "../../../Hexagon/Hexagon/Patch_Strings.h"
#include "SMB1_Writer_Strings.h"
#include <assert.h>
#include <QDebug>
#include <QFileInfo>
#include <QTextStream>

Sequential_Archive_Handler::Sequential_Archive_Handler(const QString &applicationLocation, const QString &romFolderLocation) {
    this->combineGraphicsPacks = false;
    this->combineMusicPacks = false;
    this->file = nullptr;
    this->hexagonPlugin = nullptr;
    this->sequentialArchivePlugin = nullptr;
    this->hexagonLoader = nullptr;
    this->sequentialArchiveLoader = nullptr;
    this->lastAppliedGraphicsPack = QString();
    this->graphicsPackStrings = QStringList();
    this->musicPackStrings = QStringList();
    this->bonusGraphicsPacks = QStringList();
    this->bonusMusicPacks = QStringList();
    this->invalidTones = new QSet<int>();
    this->allowPalettes = true;
    this->allowOnlyCoinPalettes = false;
    this->romFolderLocation = romFolderLocation;
    this->pluginLocation = applicationLocation + "/" + Common_Strings::STRING_PLUGINS + "/";
    this->romsArchiveLocation = applicationLocation+"/"+Common_Strings::STRING_DATA+"/"+Common_Strings::STRING_GAME_NAME+"/ROMs.sa";
    this->graphicsPacksArchiveLocation = applicationLocation+"/"+Common_Strings::STRING_DATA+"/"+Common_Strings::STRING_GAME_NAME+"/Graphics.sa";
    this->musicPacksArchiveLocation = applicationLocation+"/"+Common_Strings::STRING_DATA+"/"+Common_Strings::STRING_GAME_NAME+"/Music.sa";
}

Sequential_Archive_Handler::~Sequential_Archive_Handler() {
    if (this->hexagonLoader) this->hexagonLoader->unload();
    if (this->sequentialArchiveLoader) this->sequentialArchiveLoader->unload();
    this->hexagonLoader = nullptr;
    this->sequentialArchiveLoader = nullptr;
    this->hexagonPlugin = nullptr;
    this->sequentialArchivePlugin = nullptr;
    delete this->invalidTones;
    this->invalidTones = nullptr;
}

bool Sequential_Archive_Handler::Get_Combine_Graphics_Packs() {
    return this->combineGraphicsPacks;
}

bool Sequential_Archive_Handler::Get_Combine_Music_Packs() {
    return this->combineMusicPacks;
}

void Sequential_Archive_Handler::Set_Combine_Graphics_Packs(bool combineGraphicsPacks) {
    this->combineGraphicsPacks = combineGraphicsPacks;
}

void Sequential_Archive_Handler::Set_Combine_Music_Packs(bool combineMusicPacks) {
    this->combineMusicPacks = combineMusicPacks;
}

void Sequential_Archive_Handler::Set_File(QFile *file) {
    this->file = file;
}

bool Sequential_Archive_Handler::Apply_Graphics_Fix(const QString &fixName, const QString &fixType) {
    QByteArray patchBytes = this->Read_Graphics_Fix(fixName, fixType);
    if (patchBytes.isEmpty()) return true; //nothing to apply
    return this->Apply_Hexagon_Patch(patchBytes);
}

bool Sequential_Archive_Handler::Apply_Graphics_Pack_At_Index(int index) {
    if (this->graphicsPackStrings.isEmpty()) this->Get_Graphics_Packs();
    assert(index < this->graphicsPackStrings.size()+this->bonusGraphicsPacks.size());
    if (!this->file) return false;
    if (!this->Load_Plugins_If_Necessary()) return false;
    if (!this->sequentialArchivePlugin->Open(this->graphicsPacksArchiveLocation)) return false;

    //Get the correct pack from either bonuses or regular packs
    QString graphicsPack;
    if (index < this->graphicsPackStrings.size()) graphicsPack = this->graphicsPackStrings.at(index);
    else graphicsPack = this->bonusGraphicsPacks.at(index-this->graphicsPackStrings.size());

    QByteArray patchBytes = this->sequentialArchivePlugin->Read_File("/"+graphicsPack);
    if (!this->Get_Palettes_Allowed(patchBytes)) return false; //read if palettes are allowed

    qDebug() << "Using graphics pack " << graphicsPack;
    this->sequentialArchivePlugin->Close();
    if (patchBytes.isEmpty()) return false;
    int lineNum = 0;
    bool success = this->hexagonPlugin->Apply_Hexagon_Patch(patchBytes, this->file, false, lineNum) == Hexagon_Error_Codes::OK;
    if (success) this->lastAppliedGraphicsPack = graphicsPack;
    return success;
}

bool Sequential_Archive_Handler::Apply_Graphics_Sprite(const QString &spriteName, const QString &patchName) {
    QByteArray patchBytes = this->Read_Graphics_Sprite(spriteName, patchName);
    if (patchBytes.isEmpty()) return true; //nothing to apply
    return this->Apply_Hexagon_Patch(patchBytes);
}

bool Sequential_Archive_Handler::Apply_Random_Graphics_Sprite(const QString &spriteName) {
    QString patchName;
    QByteArray patchBytes = this->Read_Random_Graphics_Sprite(spriteName, patchName);
    if (patchBytes.isEmpty()) return true; //nothing to apply
    return this->Apply_Hexagon_Patch(patchBytes);
}

bool Sequential_Archive_Handler::Apply_Random_Graphics_Sprite(const QString &spriteName, QString &patchName) {
    QByteArray patchBytes = this->Read_Random_Graphics_Sprite(spriteName, patchName);
    if (patchBytes.isEmpty()) return true; //nothing to apply
    return this->Apply_Hexagon_Patch(patchBytes);
}

bool Sequential_Archive_Handler::Apply_Hexagon_Patch(const QByteArray &patchBytes) {
    int lineNum = 0;
    return this->hexagonPlugin->Apply_Hexagon_Patch(patchBytes, this->file, false, lineNum) == Hexagon_Error_Codes::OK;
}

bool Sequential_Archive_Handler::Apply_Music_Pack_At_Index(int index) {
    return this->Apply_Music_Pack_At_Index(index, false);
}

QStringList Sequential_Archive_Handler::Get_Bonus_Graphics_Packs() {
    if (this->graphicsPackStrings.isEmpty()) this->Get_Graphics_Packs();
    return this->bonusGraphicsPacks;
}

QStringList Sequential_Archive_Handler::Get_Bonus_Music_Packs() {
    if (this->musicPackStrings.isEmpty()) this->Get_Music_Packs();
    return this->bonusMusicPacks;
}

QStringList Sequential_Archive_Handler::Get_Graphics_Packs() {
    if (!this->graphicsPackStrings.isEmpty()) return this->graphicsPackStrings;
    if (!this->Load_Plugins_If_Necessary()) return this->graphicsPackStrings;
    if (!this->sequentialArchivePlugin->Open(this->graphicsPacksArchiveLocation)) return this->graphicsPackStrings;
    this->Get_HEXP_Files_From_File_List(this->graphicsPackStrings, this->bonusGraphicsPacks);
    this->sequentialArchivePlugin->Close();
    return this->graphicsPackStrings;
}

QString Sequential_Archive_Handler::Get_Graphics_Pack_At_Index(int index) {
    if (this->graphicsPackStrings.isEmpty()) this->Get_Graphics_Packs();
    assert(index < this->graphicsPackStrings.size());
    return this->graphicsPackStrings.at(index);
}

QString Sequential_Archive_Handler::Get_Last_Applied_Graphics_Pack() {
    return this->lastAppliedGraphicsPack;
}

QStringList Sequential_Archive_Handler::Get_Music_Packs() {
    if (!this->musicPackStrings.isEmpty()) return this->musicPackStrings;
    if (!this->Load_Plugins_If_Necessary()) return this->musicPackStrings;
    if (!this->sequentialArchivePlugin->Open(this->musicPacksArchiveLocation)) return this->musicPackStrings;
    this->Get_HEXP_Files_From_File_List(this->musicPackStrings, this->bonusMusicPacks);
    this->sequentialArchivePlugin->Close();
    return this->musicPackStrings;
}

QString Sequential_Archive_Handler::Get_Music_Pack_At_Index(int index) {
    if (this->musicPackStrings.isEmpty()) this->Get_Music_Packs();
    assert(index < this->musicPackStrings.size());
    return this->musicPackStrings.at(index);
}

int Sequential_Archive_Handler::Get_Number_Of_Bonus_Graphics_Packs() {
    if (this->graphicsPackStrings.isEmpty()) this->Get_Graphics_Packs();
    return this->bonusGraphicsPacks.size();
}

int Sequential_Archive_Handler::Get_Number_Of_Bonus_Music_Packs() {
    if (this->musicPackStrings.isEmpty()) this->Get_Music_Packs();
    return this->bonusMusicPacks.size();
}

int Sequential_Archive_Handler::Get_Number_Of_Graphics_Packs() {
    if (this->graphicsPackStrings.isEmpty()) this->Get_Graphics_Packs();
    return this->graphicsPackStrings.size();
}

int Sequential_Archive_Handler::Get_Number_Of_Music_Packs() {
    if (this->musicPackStrings.isEmpty()) this->Get_Music_Packs();
    return this->musicPackStrings.size();
}

bool Sequential_Archive_Handler::Is_Tone_Invalid(int tone) {
    return this->invalidTones->contains(tone);
}

bool Sequential_Archive_Handler::Are_Color_Palettes_Allowed() {
    return this->allowPalettes;
}

bool Sequential_Archive_Handler::Are_Only_Coin_Palettes_Allowed() {
    return this->allowOnlyCoinPalettes;
}

QByteArray Sequential_Archive_Handler::Read_Graphics_Fix(const QString &fixName, const QString &fixType) {
    return this->Read_Graphics_Fix(fixName, fixType, this->lastAppliedGraphicsPack);
}

QByteArray Sequential_Archive_Handler::Read_Graphics_Fix(const QString &fixName, const QString &fixType, QString graphicsPack) {
    if (!this->file || !this->Load_Plugins_If_Necessary()) return QByteArray();
    if (!this->sequentialArchivePlugin->Open(this->graphicsPacksArchiveLocation)) return QByteArray();
    if (graphicsPack.isEmpty()) graphicsPack = this->lastAppliedGraphicsPack;
    QByteArray patchBytes = this->sequentialArchivePlugin->Read_File("/"+Fix_Strings::STRING_FIXES+"/"+fixType+"/"+fixName+"/"+graphicsPack);
    this->sequentialArchivePlugin->Close();
    return patchBytes;
}

QByteArray Sequential_Archive_Handler::Read_Graphics_Sprite(const QString &patchName, const QString &spriteName) {
    if (!this->file || !this->Load_Plugins_If_Necessary()) return QByteArray();
    if (!this->sequentialArchivePlugin->Open(this->graphicsPacksArchiveLocation)) return QByteArray();
    QByteArray patchBytes = this->sequentialArchivePlugin->Read_File("/"+Fix_Strings::STRING_SPRITES+"/"+spriteName+"/"+patchName);
    this->sequentialArchivePlugin->Close();
    return patchBytes;
}

QByteArray Sequential_Archive_Handler::Read_Random_Graphics_Sprite(const QString &spriteName, QString &patchName) {
    if (!this->file || !this->Load_Plugins_If_Necessary()) return QByteArray();
    if (!this->sequentialArchivePlugin->Open(this->graphicsPacksArchiveLocation)) return QByteArray();
    if (!this->sequentialArchivePlugin->Change_Directory("/"+Fix_Strings::STRING_SPRITES+"/"+spriteName)) { this->sequentialArchivePlugin->Close(); return QByteArray(); }
    QStringList files = this->sequentialArchivePlugin->Get_Files();
    int num = Random::Get_Instance().Get_Num((files.size()*2)-1); //use random sprite half of the time
    //int num = Random::Get_Instance().Get_Num(files.size());
    if (num >= files.size()) {
        this->sequentialArchivePlugin->Close();
        return QByteArray(); //don't apply anything
    }
    patchName = files.at(num);
    qDebug().noquote() << spriteName+": \""+patchName+"\"";
    QByteArray patchBytes = this->sequentialArchivePlugin->Read_File("/"+Fix_Strings::STRING_SPRITES+"/"+spriteName+"/"+patchName);
    this->sequentialArchivePlugin->Close();
    return patchBytes;
}

bool Sequential_Archive_Handler::Is_Hexagon_Line_End_Of_Header(const QString &line) {
    if (!this->Load_Plugins_If_Necessary()) return false;
    return this->hexagonPlugin->Is_Line_End_Of_Header(line);
}

bool Sequential_Archive_Handler::Install_ROM_Patches(const QString &romName) {
    if (!this->Load_Plugins_If_Necessary()) return true; //no plugins, so just skip this
    if (!this->sequentialArchivePlugin->Open(this->romsArchiveLocation)) return true; //no ROMs.sa exists, so don't install anything
    if (!this->sequentialArchivePlugin->Change_Directory(romName)) {
        this->sequentialArchivePlugin->Close();
        return true;
    }

    //Check if any patches are available for the file
    QString extension = QFileInfo(romName).suffix();
    QStringList patches = this->sequentialArchivePlugin->Get_Files();
    if (patches.isEmpty()) {
        this->sequentialArchivePlugin->Close();
        return true;
    }

    //Read the original file into a buffer
    QFile originalFile(this->romFolderLocation + "/" + romName);
    if (!originalFile.open(QIODevice::ReadOnly)) return false;
    QByteArray originalBytes = originalFile.readAll();
    originalFile.close();
    if (originalBytes.isEmpty()) return false;

    //Apply every patch available to the file
    for (QString patch : patches) {
        //Make a copy of the original file using the patch file's name
        QString patchFileBaseName = QFileInfo(patch).fileName(); patchFileBaseName.chop(QFileInfo(patch).suffix().size()+1);
        QFile patchFile(this->romFolderLocation+"/"+patchFileBaseName+"."+extension);
        if (!patchFile.open(QIODevice::ReadWrite)) return this->Removed_Installed_ROM_Patches_On_Failure(patches, extension);
        if (patchFile.write(originalBytes) != originalBytes.size() || !patchFile.reset()) {
            patchFile.close();
            return this->Removed_Installed_ROM_Patches_On_Failure(patches, extension);
        }

        //Apply the Hexagon patch to the new file
        QByteArray patchBytes = this->sequentialArchivePlugin->Read_File(patch);
        int lineNum = 0;
        if (this->hexagonPlugin->Apply_Hexagon_Patch(patchBytes, &patchFile, false, lineNum) != Hexagon_Error_Codes::OK) {
            patchFile.close();
            return this->Removed_Installed_ROM_Patches_On_Failure(patches, extension);
        }
        patchFile.close();
    }
    this->sequentialArchivePlugin->Close();
    return true;
}

bool Sequential_Archive_Handler::Apply_Music_Pack(const QString &musicPack, bool isSecondaryPatch, QStringList &previouscompatiblePacks) {
    if (!this->file) return false;
    QByteArray patchBytes = this->Read_Music_Pack(musicPack);
    qDebug() << "Using music pack " << musicPack;
    if (patchBytes.isEmpty()) return false;
    int lineNum = 0;

    //Read the invalid tones
    if (!this->Get_Invalid_Tones(patchBytes, isSecondaryPatch)) return false;

    //Apply the base patch
    if (this->hexagonPlugin->Apply_Hexagon_Patch(patchBytes, this->file, false, lineNum) != Hexagon_Error_Codes::OK) return false;

    //Possibly apply additional patches
    if (!this->combineMusicPacks) return true;
    QStringList currentCompatiblePacks = this->Get_Compatible_Music_Packs(patchBytes);
    QStringList compatiblePacks;
    if (isSecondaryPatch) { //only consider what's compatible with this pack as well as all previous packs
        for (int i = 0; i < previouscompatiblePacks.size(); ++i) {
            QString packName = previouscompatiblePacks.at(i);
            if (currentCompatiblePacks.contains(packName)) {
                compatiblePacks.push_back(packName);
            }
        }
    } else { //consider all compatible patches for the primary patch
        compatiblePacks = currentCompatiblePacks;
    }
    int secondaryIndex = Random::Get_Instance().Get_Num(compatiblePacks.size());
    if (secondaryIndex == compatiblePacks.size()) return true; //don't apply anything
    QString packName = compatiblePacks.at(secondaryIndex);
    compatiblePacks.removeAt(secondaryIndex); //remove the pack that is going to be used
    return this->Apply_Music_Pack(packName, true, compatiblePacks);
}

bool Sequential_Archive_Handler::Apply_Music_Pack_At_Index(int index, bool isSecondaryPatch) {
    if (this->musicPackStrings.isEmpty()) this->Get_Music_Packs();
    assert(index < this->musicPackStrings.size()+this->bonusMusicPacks.size());
    QStringList compatiblePacks;
    if (index < this->musicPackStrings.size()) return this->Apply_Music_Pack(this->musicPackStrings.at(index), isSecondaryPatch, compatiblePacks);
    else return this->Apply_Music_Pack(this->bonusMusicPacks.at(index-this->musicPackStrings.size()), isSecondaryPatch, compatiblePacks);
}

QStringList Sequential_Archive_Handler::Get_Compatible_Music_Packs(const QByteArray &patchBytes) {
    if (patchBytes.isEmpty()) return QStringList();

    //Read the patch file to get the compatible patch files
    QTextStream stream(patchBytes);
    QStringList compatibleMusicPacks;
    bool compatibleSection = false;
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (this->hexagonPlugin->Is_Line_End_Of_Header(line)) return compatibleMusicPacks;
        if (compatibleSection) { //read the compatible music packs
            if (!line.startsWith(Patch_Strings::STRING_COMMENT)) continue;
            line = line.remove(0, Patch_Strings::STRING_COMMENT.size()).trimmed();
            compatibleMusicPacks.append(line);
        } else { //find the compatible section
            if (line == Fix_Strings::STRING_COMPATIBLE_SECTION) compatibleSection = true;
        }
    }
    return compatibleMusicPacks;
}

bool Sequential_Archive_Handler::Get_Invalid_Tones(const QByteArray &patchBytes, bool isSecondaryPatch) {
    if (!isSecondaryPatch) this->invalidTones->clear();
    if (patchBytes.isEmpty()) return true; //nothing to do

    //Read the patch file to get the invalid tones
    QTextStream stream(patchBytes);
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (this->hexagonPlugin->Is_Line_End_Of_Header(line)) return true;
        if (line.startsWith(Fix_Strings::STRING_COMPATIBLE_SECTION)) return true;
        if (line.startsWith(Fix_Strings::STRING_INVALID_TONES)) {
            line = line.remove(0, Fix_Strings::STRING_INVALID_TONES.size()).trimmed();
            QStringList tones = line.split(',');
            for (int i = 0; i < tones.size(); ++i) {
                bool valid = false;
                int tone = tones.at(i).toInt(&valid);
                if (!valid) return false;
                this->invalidTones->insert(tone);
            }
        }
    }
    return true;
}

bool Sequential_Archive_Handler::Get_Palettes_Allowed(const QByteArray &patchBytes) {
    if (patchBytes.isEmpty()) return true; //nothing to do

    //Read the patch file to get the invalid tones
    QTextStream stream(patchBytes);
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (this->hexagonPlugin->Is_Line_End_Of_Header(line)) return true;
        if (line.startsWith(Fix_Strings::STRING_ALLOW_PALETTES)) this->allowPalettes = false;
        if (line.startsWith(Fix_Strings::STRING_ALLOW_ONLY_COIN_PALETTES)) this->allowOnlyCoinPalettes = true;
    }
    return true;
}

void Sequential_Archive_Handler::Get_HEXP_Files_From_File_List(QStringList &normalFiles, QStringList &bonusFiles) {
    QStringList fileList = this->sequentialArchivePlugin->Get_Files();
    normalFiles = QStringList();
    bonusFiles = QStringList();
    for (QString file : fileList) {
        if (file.startsWith(".")) {
             if (Version::VERSION_NUMBER.contains("dev")) bonusFiles.append(file);
        } else if (file.startsWith("+")) {
            bonusFiles.append(file);
        } else if (file.toLower().endsWith(".hexp")) {
            normalFiles.append(file);
        }
    }
}

bool Sequential_Archive_Handler::Load_Plugins_If_Necessary() {
    bool success = true;
    if (!this->hexagonPlugin && !this->Load_Hexagon_Plugin()) success = false;
    if (!this->sequentialArchivePlugin && !this->Load_Sequential_Archive_Plugin()) success = false;
    return success;
}

bool Sequential_Archive_Handler::Load_Hexagon_Plugin() {
    QString hexagonPluginLocation = this->pluginLocation + "Hexagon" + Common_Strings::STRING_PLUGIN_EXTENSION;
    if (!QFile(hexagonPluginLocation).exists()) return false;

    //Load the Hexagon Plugin
    this->hexagonLoader = new QPluginLoader(hexagonPluginLocation);
    QObject *validPlugin = this->hexagonLoader->instance();
    if (!validPlugin) return false;
    this->hexagonPlugin = qobject_cast<Hexagon_Interface*>(validPlugin);
    return this->hexagonPlugin;
}

bool Sequential_Archive_Handler::Load_Sequential_Archive_Plugin() {
    QString sequentialArchivePluginLocation = this->pluginLocation + "Sequential_Archive" + Common_Strings::STRING_PLUGIN_EXTENSION;
    if (!QFile(sequentialArchivePluginLocation).exists()) return false;

    //Load the Sequential Archive Plugin
    this->sequentialArchiveLoader = new QPluginLoader(sequentialArchivePluginLocation);
    QObject *validPlugin = this->sequentialArchiveLoader->instance();
    if (!validPlugin) return false;
    this->sequentialArchivePlugin = qobject_cast<Sequential_Archive_Interface*>(validPlugin);
    return this->sequentialArchivePlugin;
}

QByteArray Sequential_Archive_Handler::Read_Music_Pack(const QString &musicPackString) {
    if (!this->Load_Plugins_If_Necessary()) return QByteArray();
    if (!this->sequentialArchivePlugin->Open(this->musicPacksArchiveLocation)) return QByteArray();
    QByteArray patchBytes = this->sequentialArchivePlugin->Read_File("/"+musicPackString);
    this->sequentialArchivePlugin->Close();
    return patchBytes;
}

bool Sequential_Archive_Handler::Removed_Installed_ROM_Patches_On_Failure(const QStringList &patches, const QString &extension) {
    this->sequentialArchivePlugin->Close();
    for (QString patch : patches) {
        QString patchFileBaseName = QFileInfo(patch).fileName(); patchFileBaseName.chop(QFileInfo(patch).suffix().size()+1);
        QFile::remove(this->romFolderLocation+"/"+patchFileBaseName+"."+extension); //ignore output
    }
    return false; //always return false. An error had to occur to get here
}
