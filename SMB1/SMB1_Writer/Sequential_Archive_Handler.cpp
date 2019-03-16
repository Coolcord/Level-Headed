#include "Sequential_Archive_Handler.h"
#include "../../Common_Files/Random.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../../../Hexagon/Hexagon/Patch_Strings.h"
#include "SMB1_Writer_Strings.h"
#include <assert.h>
#include <QTextStream>

#include <QDebug>

const static QString STRING_INVALID_TONES = Patch_Strings::STRING_COMMENT+" Invalid Tones: ";
const static QString STRING_COMPATIBLE_SECTION = Patch_Strings::STRING_COMMENT+" Compatible:";

Sequential_Archive_Handler::Sequential_Archive_Handler(const QString &applicationLocation) {
    this->combineMusicPacks = false;
    this->file = nullptr;
    this->hexagonPlugin = nullptr;
    this->sequentialArchivePlugin = nullptr;
    this->hexagonLoader = nullptr;
    this->sequentialArchiveLoader = nullptr;
    this->lastAppliedGraphicsPack = QString();
    this->graphicsPackStrings = QStringList();
    this->musicPackStrings = QStringList();
    this->invalidTones = new QSet<int>();
    this->pluginLocation = applicationLocation + "/" + Common_Strings::STRING_PLUGINS + "/";
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

void Sequential_Archive_Handler::Set_Combine_Music_Packs(bool combineMusicPacks) {
    this->combineMusicPacks = combineMusicPacks;
}

void Sequential_Archive_Handler::Set_File(QFile *file) {
    this->file = file;
}

bool Sequential_Archive_Handler::Apply_Graphics_Fix(const QString &fixName) {
    QByteArray patchBytes = this->Read_Graphics_Fix(fixName);
    if (patchBytes.isEmpty()) return true; //nothing to apply
    return this->Apply_Graphics_Fix(patchBytes);
}

bool Sequential_Archive_Handler::Apply_Graphics_Fix(const QByteArray &patchBytes) {
    int lineNum = 0;
    return this->hexagonPlugin->Apply_Hexagon_Patch(patchBytes, this->file, false, lineNum) == Hexagon_Error_Codes::OK;
}

bool Sequential_Archive_Handler::Apply_Graphics_Pack_At_Index(int index) {
    if (this->graphicsPackStrings.isEmpty()) this->Get_Graphics_Packs();
    assert(index < this->graphicsPackStrings.size());
    if (!this->file) return false;
    if (!this->Load_Plugins_If_Necessary()) return false;
    if (!this->sequentialArchivePlugin->Open(this->graphicsPacksArchiveLocation)) return false;
    QByteArray patchBytes = this->sequentialArchivePlugin->Read_File("/"+this->graphicsPackStrings.at(index));
    qDebug() << "Using graphics pack " << this->graphicsPackStrings.at(index);
    this->sequentialArchivePlugin->Close();
    if (patchBytes.isEmpty()) return false;
    int lineNum = 0;
    bool success = this->hexagonPlugin->Apply_Hexagon_Patch(patchBytes, this->file, false, lineNum) == Hexagon_Error_Codes::OK;
    if (success) this->lastAppliedGraphicsPack = this->graphicsPackStrings.at(index);
    return success;
}

bool Sequential_Archive_Handler::Apply_Music_Pack_At_Index(int index) {
    return this->Apply_Music_Pack_At_Index(index, false);
}

QStringList Sequential_Archive_Handler::Get_Graphics_Packs() {
    if (!this->graphicsPackStrings.isEmpty()) return this->graphicsPackStrings;
    if (!this->Load_Plugins_If_Necessary()) return this->graphicsPackStrings;
    if (!this->sequentialArchivePlugin->Open(this->graphicsPacksArchiveLocation)) return this->graphicsPackStrings;
    this->graphicsPackStrings = this->Get_HEXP_Files_From_File_List(this->sequentialArchivePlugin->Get_Files());
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
    this->musicPackStrings = this->Get_HEXP_Files_From_File_List(this->sequentialArchivePlugin->Get_Files());
    this->sequentialArchivePlugin->Close();
    return this->musicPackStrings;
}

QString Sequential_Archive_Handler::Get_Music_Pack_At_Index(int index) {
    if (this->musicPackStrings.isEmpty()) this->Get_Music_Packs();
    assert(index < this->musicPackStrings.size());
    return this->musicPackStrings.at(index);
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

QByteArray Sequential_Archive_Handler::Read_Graphics_Fix(const QString &fixName) {
    if (!this->file || !this->Load_Plugins_If_Necessary()) return QByteArray();
    if (!this->sequentialArchivePlugin->Open(this->graphicsPacksArchiveLocation)) return QByteArray();
    QByteArray patchBytes = this->sequentialArchivePlugin->Read_File("/"+fixName+"/"+this->lastAppliedGraphicsPack);
    this->sequentialArchivePlugin->Close();
    return patchBytes;
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
    int secondaryIndex = Random::Get_Num(compatiblePacks.size());
    if (secondaryIndex == compatiblePacks.size()) return true; //don't apply anything
    QString packName = compatiblePacks.at(secondaryIndex);
    compatiblePacks.removeAt(secondaryIndex); //remove the pack that is going to be used
    return this->Apply_Music_Pack(packName, true, compatiblePacks);
}

bool Sequential_Archive_Handler::Apply_Music_Pack_At_Index(int index, bool isSecondaryPatch) {
    if (this->musicPackStrings.isEmpty()) this->Get_Music_Packs();
    assert(index < this->musicPackStrings.size());
    QStringList compatiblePacks;
    return this->Apply_Music_Pack(this->musicPackStrings.at(index), isSecondaryPatch, compatiblePacks);
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
        if (line.startsWith(Patch_Strings::STRING_CHECKSUM)) return compatibleMusicPacks;
        if (compatibleSection) { //read the compatible music packs
            if (!line.startsWith(Patch_Strings::STRING_COMMENT)) continue;
            line = line.remove(0, Patch_Strings::STRING_COMMENT.size()).trimmed();
            compatibleMusicPacks.append(line);
        } else { //find the compatible section
            if (line == STRING_COMPATIBLE_SECTION) compatibleSection = true;
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
        if (line.startsWith(Patch_Strings::STRING_CHECKSUM)) return true;
        if (line.startsWith(STRING_COMPATIBLE_SECTION)) return true;
        if (line.startsWith(STRING_INVALID_TONES)) {
            line = line.remove(0, STRING_INVALID_TONES.size()).trimmed();
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

QStringList Sequential_Archive_Handler::Get_HEXP_Files_From_File_List(const QStringList &fileList) {
    QStringList hexpFiles;
    for (QString file : fileList) {
        if (file.toLower().endsWith(".hexp")) hexpFiles.append(file);
    }
    return hexpFiles;
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
