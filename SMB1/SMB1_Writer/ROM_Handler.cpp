#include "ROM_Handler.h"
#include "ROM_Checksum.h"
#include "ROM_Filename.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QMap>
#include <QStringList>
#include <QCryptographicHash>
#include <QByteArray>
#include <assert.h>

ROM_Handler::ROM_Handler(QWidget *parent, QString romFolderLocation) {
    assert(parent);
    this->parent = parent;
    this->romType = ROM_Type::INVALID;
    this->romChecksum = new ROM_Checksum();
    this->romFolderLocation = romFolderLocation;
}

ROM_Handler::~ROM_Handler() {
    delete this->romChecksum;
}

bool ROM_Handler::Install_ROM() {
    //QString fileLocation = QFileDialog::getOpenFileName(this->parent, "Open a ROM");
}

QFile *ROM_Handler::Load_Local_ROM(const QString &fileName) {
    this->file = new QFile(this->romFolderLocation + "/" + fileName);
    if (!this->file->exists() || !this->file->open(QFile::ReadWrite)) {
        this->file->close();
        delete this->file;
        this->file = NULL;
        return NULL;
    }

    //Calculate the checksum
    QString romChecksum = this->romChecksum->Get_ROM_Checksum(this->file);
    this->romType = this->romChecksum->Get_ROM_Type_From_Checksum(romChecksum);
    QString properFileName = this->romChecksum->Get_ROM_Filename_From_Checksum(romChecksum);

    //Make sure everything is valid
    if (this->romType == ROM_Type::INVALID || this->file->fileName() != properFileName) {
        this->romType = ROM_Type::INVALID;
        this->file->close();
        delete this->file;
        this->file = NULL;
        return NULL;
    }
    return this->file;
}

QFile *ROM_Handler::Load_First_Local_ROM() {
    QStringList fileNames;
    fileNames.append(ROM_Filename::STRING_USA0);
    fileNames.append(ROM_Filename::STRING_USA1);
    fileNames.append(ROM_Filename::STRING_EUROPE);
    fileNames.append(ROM_Filename::STRING_FDS);
    fileNames.append(ROM_Filename::STRING_DUCK);
    fileNames.append(ROM_Filename::STRING_TRACK);
    fileNames.append(ROM_Filename::STRING_DUCKE);
    fileNames.append(ROM_Filename::STRING_TETRIS);

    //Attempt to open each supported ROM
    foreach (QString fileName, fileNames) {
        if (this->Load_Local_ROM(fileName)) {
            return this->file;
        }
    }
    return NULL;
}

bool ROM_Handler::Clean_ROM_Directory() {
    QMap<QString, bool> fileNames;
    fileNames.insert(ROM_Filename::STRING_USA0, true);
    fileNames.insert(ROM_Filename::STRING_USA1, true);
    fileNames.insert(ROM_Filename::STRING_EUROPE, true);
    fileNames.insert(ROM_Filename::STRING_FDS, true);
    fileNames.insert(ROM_Filename::STRING_DUCK, true);
    fileNames.insert(ROM_Filename::STRING_TRACK, true);
    fileNames.insert(ROM_Filename::STRING_DUCKE, true);
    fileNames.insert(ROM_Filename::STRING_TETRIS, true);

    //Delete all folders
    QDir romFolder(this->romFolderLocation);
    if (!romFolder.exists()) return false;
    bool success = true;
    foreach (QString folder, romFolder.entryList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
        QDir dir(folder);
        if (!dir.removeRecursively()) success = false;
    }

    //Delete all invalid roms
    QFile *tmpFile = this->file;
    ROM_Type::ROM_Type tmpROMType = this->romType;
    foreach (QString rom, romFolder.entryList(QDir::Files)) {
        if (!this->Load_Local_ROM(rom)) {
            if (!romFolder.remove(rom)) {
                success = false;
            }
        }
    }
    this->file = tmpFile;
    this->romType = tmpROMType;

    return success;
}

ROM_Type::ROM_Type ROM_Handler::Get_ROM_Type() {
    return this->romType;
}

ROM_Type::ROM_Type ROM_Handler::Determine_ROM_Type() {
    if (!this->Check_ROM_Header()) return ROM_Type::INVALID; //bad header
    this->romChecksum->Get_ROM_Checksum(this->file);
}

bool ROM_Handler::Check_ROM_Header() {
    if (!this->file->seek(0)) return false;
    QByteArray buffer(4, ' ');
    if (this->file->read(buffer.data(), 4) != 4) return false;
    if (buffer.data() == NULL || buffer.size() != 4) return false;

    if (this->Check_NES_ROM_Header(&buffer)) return true;
    return this->Check_FDS_ROM_Header(&buffer);
}

bool ROM_Handler::Check_NES_ROM_Header(QByteArray *buffer) {
    assert(buffer);

    //Make sure the first 4 bytes of the header are valid: "NES"
    if (static_cast<unsigned char>(buffer->at(0)) != 0x4E) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0x45) return false;
    if (static_cast<unsigned char>(buffer->at(2)) != 0x53) return false;
    return static_cast<unsigned char>(buffer->at(3)) == 0x1A;
}

bool ROM_Handler::Check_FDS_ROM_Header(QByteArray *buffer) {
    assert(buffer);

    //Make sure the first 4 bytes of the header are valid: "FDS"
    if (static_cast<unsigned char>(buffer->at(0)) != 0x46) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0x44) return false;
    if (static_cast<unsigned char>(buffer->at(2)) != 0x53) return false;
    return static_cast<unsigned char>(buffer->at(3)) == 0x1A;
}
