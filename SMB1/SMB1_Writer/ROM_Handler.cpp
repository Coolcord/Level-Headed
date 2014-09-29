#include "ROM_Handler.h"
#include "ROM_Checksum.h"
#include "ROM_Filename.h"
#include "../../Level-Headed/Common_Strings.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QMap>
#include <QStringList>
#include <QCryptographicHash>
#include <QByteArray>
#include <QUrl>
#include <QDebug>
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

QString ROM_Handler::Install_ROM() {
    QString fileLocation = QFileDialog::getOpenFileName(this->parent, "Open a ROM");
    if (fileLocation == NULL || fileLocation.isEmpty()) return QString();

    QFile file(fileLocation);
    if (!file.exists()) {
        this->Show_Error(fileLocation + " could not be found!");
        return QString();
    }
    if (!file.open(QFile::ReadWrite)) {
        file.close();
        this->Show_Error(Common_Strings::LEVEL_HEADED +
                         " does not have proper read/write permissions to access " + file.fileName() + ". Cannot continue!");
        return QString();
    }

    //Verify ROM Checksum
    QString checksum = this->romChecksum->Get_ROM_Checksum(&file);
    ROM_Type::ROM_Type romType = this->romChecksum->Get_ROM_Type_From_Checksum(checksum);
    if (romType == ROM_Type::INVALID) return QString(); //TODO: Throw an error here

    //Check if the current ROM is already installed
    QString fileName = this->romChecksum->Get_ROM_Filename_From_Checksum(checksum);
    QFile installedFile(this->romFolderLocation + "/" + fileName);
    if (installedFile.exists()) {
        if (!installedFile.remove()) {
            file.close();
            this->Show_Error(Common_Strings::LEVEL_HEADED +
                             " does not have proper read/write permissions to access " + installedFile.fileName() + ". Cannot continue!");
            return QString();
        }
    }
    if (!installedFile.open(QFile::ReadWrite)) {
        file.close();
        this->Show_Error(Common_Strings::LEVEL_HEADED +
                         " does not have proper read/write permissions to access " + installedFile.fileName() + ". Cannot continue!");
        return QString();
    }

    //Install the ROM
    file.reset();
    installedFile.reset();
    QByteArray buffer = file.readAll();
    if (buffer.isEmpty() || installedFile.write(buffer) == -1) {
        this->Show_Error("Unable to install the ROM!");
        return QString();
    }
    file.close();
    installedFile.close();
    QMessageBox::information(this->parent, Common_Strings::LEVEL_HEADED, "ROM successfully installed!", Common_Strings::OK);
    return fileName;
}

QFile *ROM_Handler::Load_Local_ROM(const QString &fileName, bool &cancel) {
    if (cancel) return false;
    QFile loadFile(this->romFolderLocation + "/" + fileName);
    if (!loadFile.exists()) {
        this->Show_Error(fileName + " is not installed!");
        return NULL;
    }
    if (!loadFile.open(QFile::ReadWrite)) {
        loadFile.close();
        this->Show_Error(Common_Strings::LEVEL_HEADED +
                         " does not have proper read/write permissions to access " + this->file->fileName() + ". Cannot continue!");
        return NULL;
    }

    //Calculate the checksum
    QString romChecksum = this->romChecksum->Get_ROM_Checksum(&loadFile);
    this->romType = this->romChecksum->Get_ROM_Type_From_Checksum(romChecksum);

    //Make sure everything is valid
    if (this->romType == ROM_Type::INVALID) {
        QString loadedFileName = loadFile.fileName();
        loadFile.close();
        loadFile.remove();
        this->Show_Error(loadedFileName + " is not a valid SMB1 ROM!");
        return NULL;
    }

    //Make a copy of the ROM for the new hacked version
    QString fileLocation = QFileDialog::getSaveFileName(this->parent, "Save Location");
    qDebug() << "Saving at: " << fileLocation;
    if (fileLocation == NULL || fileLocation.isEmpty()) {
        cancel = true;
        loadFile.close();
        return NULL;
    }

    //Open the new save file and delete it if it currently exists
    this->file = new QFile(fileLocation);
    if (this->file->exists()) {
        if (!this->file->remove()) {
            loadFile.close();
            this->Show_Error(Common_Strings::LEVEL_HEADED +
                             " does not have proper read/write permissions to access " + this->file->fileName() + ". Cannot continue!");
            return NULL;
        }
    }
    if (!this->file->open(QFile::ReadWrite)) {
        loadFile.close();
        this->Show_Error(Common_Strings::LEVEL_HEADED +
                         " does not have proper read/write permissions to access " + this->file->fileName() + ". Cannot continue!");
        return NULL;
    }

    //Copy the ROM to the new location
    QByteArray buffer = loadFile.readAll();
    if (buffer.isEmpty() || this->file->write(buffer) == -1) {
        loadFile.close();
        this->Show_Error("Unable to copy the game to " + this->file->fileName());
        return NULL;
    }

    //Get Read permissions
    this->file->close();
    assert(this->file->exists());
    if (!this->file->open(QFile::ReadWrite)) {
        loadFile.close();
        this->Show_Error(Common_Strings::LEVEL_HEADED +
                         " does not have proper read/write permissions to access " + this->file->fileName() + ". Cannot continue!");
        return NULL;
    }
    return this->file;
}

QFile *ROM_Handler::Load_First_Local_ROM(bool &cancel) {
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
        if (cancel) break;
        if (this->Load_Local_ROM(fileName, cancel)) {
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
    foreach (QString romName, romFolder.entryList(QDir::Files)) {
        QFile rom(romName);
        if (!rom.exists()) continue;
        if (!rom.open(QFile::ReadWrite)) {
            success = false;
            continue;
        }

        //Delete the ROM if it is invalid
        ROM_Type::ROM_Type romType = this->romChecksum->Get_ROM_Type_From_Checksum(this->romChecksum->Get_ROM_Checksum(&rom));
        rom.close();
        if (romType == ROM_Type::INVALID) {
            if (!romFolder.remove(romName)) {
                success = false;
            }
        }
    }
    this->file = tmpFile;
    this->romType = tmpROMType;

    return success;
}

ROM_Type::ROM_Type ROM_Handler::Get_ROM_Type() {
    if (!this->file) return ROM_Type::INVALID;
    return this->romType;
}

void ROM_Handler::Show_Error(const QString &error) {
    //Perform Cleanup
    if (this->file) {
        if (this->file->isOpen()) this->file->close();
        if (this->file->exists()) this->file->remove();
    }
    delete this->file;
    this->file = NULL;
    this->romType = ROM_Type::INVALID;

    //Show the error
    QMessageBox::critical(this->parent, Common_Strings::LEVEL_HEADED, Common_Strings::LEVEL_HEADED +
                          error, Common_Strings::OK);
}
