#include "ROM_Handler.h"
#include "ROM_Checksum.h"
#include "ROM_Filename.h"
#include "../../Level-Headed/Common_Strings.h"
#include "SMB1_Writer_Strings.h"
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

ROM_Handler::ROM_Handler(QWidget *parent, const QString &applicationLocation) {
    this->parent = parent;
    this->romType = ROM_Type::INVALID;
    this->romChecksum = new ROM_Checksum();
    this->applicationLocation = applicationLocation;
    this->romFolderLocation = this->applicationLocation + "/" + Common_Strings::STRING_DATA + "/" + Common_Strings::STRING_GAME_NAME;
}

ROM_Handler::~ROM_Handler() {
    delete this->romChecksum;
}

QString ROM_Handler::Install_ROM() {
    QString fileLocation = QFileDialog::getOpenFileName(this->parent, "Open a ROM", this->applicationLocation, "NES ROMs (*.nes *.fds)");
    if (fileLocation == nullptr || fileLocation.isEmpty()) return QString();

    QFile file(fileLocation);
    if (!file.exists()) {
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                              fileLocation + " could not be found!", Common_Strings::STRING_OK);
        return QString();
    }
    if (!file.open(QFile::ReadWrite)) {
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                              Common_Strings::STRING_LEVEL_HEADED +
                              " does not have proper read/write permissions to access " +
                              file.fileName() + ". Cannot continue!", Common_Strings::STRING_OK);
        return QString();
    }

    //Verify ROM Checksum
    QString checksum = this->romChecksum->Get_ROM_Checksum(&file);
    ROM_Type::ROM_Type romType = this->romChecksum->Get_ROM_Type_From_Checksum(checksum);
    if (romType == ROM_Type::INVALID) {
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                              "This does not appear to be a valid SMB1 ROM!", Common_Strings::STRING_OK);
        return QString();
    }

    //Check if the current ROM is already installed
    QDir().mkdir(this->romFolderLocation); //make the dir if it doesn't exist
    QString fileName = this->romChecksum->Get_ROM_Filename_From_Checksum(checksum);
    QFile installedFile(this->romFolderLocation + "/" + fileName);
    if (installedFile.exists()) {
        if (!installedFile.remove()) {
            file.close();
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED, Common_Strings::STRING_LEVEL_HEADED +
                                  " does not have proper read/write permissions to access " +
                                  installedFile.fileName() + ". Cannot continue!", Common_Strings::STRING_OK);
            return QString();
        }
    }
    if (!installedFile.open(QFile::ReadWrite)) {
        file.close();
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                              Common_Strings::STRING_LEVEL_HEADED +
                              " does not have proper read/write permissions to access " + installedFile.fileName() +
                              ". Cannot continue!", Common_Strings::STRING_OK);
        return QString();
    }

    //Install the ROM
    file.reset();
    installedFile.reset();
    QByteArray buffer = file.readAll();
    if (buffer.isEmpty() || installedFile.write(buffer) == -1) {
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                              Common_Strings::STRING_LEVEL_HEADED +
                              " does not have proper read/write permissions. Cannot continue!", Common_Strings::STRING_OK);
        return QString();
    }
    file.close();
    installedFile.close();
    QMessageBox::information(this->parent, Common_Strings::STRING_LEVEL_HEADED, "ROM successfully installed!", Common_Strings::STRING_OK);
    return fileName;
}

QFile *ROM_Handler::Load_Local_ROM(const QString &fileName, bool &cancel) {
    if (cancel) return nullptr;
    QFile loadFile(this->romFolderLocation + "/" + fileName);
    if (!loadFile.exists()) {
        return nullptr;
    }
    if (!loadFile.open(QFile::ReadWrite)) {
        cancel = true;
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                              Common_Strings::STRING_LEVEL_HEADED +
                              " does not have proper read/write permissions to access " + this->file->fileName() +
                              ". Cannot continue!", Common_Strings::STRING_OK);
        return nullptr;
    }

    //Calculate the checksum
    QString romChecksum = this->romChecksum->Get_ROM_Checksum(&loadFile);
    this->romType = this->romChecksum->Get_ROM_Type_From_Checksum(romChecksum);

    //Make sure everything is valid
    if (this->romType == ROM_Type::INVALID) {
        QString loadedFileName = loadFile.fileName();
        loadFile.close();
        loadFile.remove();
        cancel = true;
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                              loadedFileName + " is not a valid SMB1 ROM!", Common_Strings::STRING_OK);
        return nullptr;
    }

    //Make a copy of the ROM for the new hacked version
    QString extension = loadFile.fileName().split('.').last().toLower();
    QString extensionFilter = "";
    if (extension == "fds") {
        extensionFilter = "Famicom Images (*.fds)";
    } else if (extension == "nes") {
        extensionFilter = "NES ROMs (*.nes)";
    } else {
        extensionFilter = "NES ROMs (*.nes *.fds)";
    }

    //Ask the user where they want to save the output ROM
    if (this->outputROMLocation.isEmpty()) {
        this->outputROMLocation = QFileDialog::getSaveFileName(this->parent, "Save Location", this->applicationLocation, extensionFilter);
    }
    qDebug() << "Saving at: " << this->outputROMLocation;
    if (this->outputROMLocation == nullptr || this->outputROMLocation.isEmpty()) {
        cancel = true;
        loadFile.close();
        return nullptr;
    }

    //Open the new save file and delete it if it currently exists
    this->file = new QFile(this->outputROMLocation);
    if (this->file->exists()) {
        if (!this->file->remove()) {
            loadFile.close();
            cancel = true;
            this->Show_Error(Common_Strings::STRING_LEVEL_HEADED +
                             " does not have proper read/write permissions to access " +
                             this->file->fileName() + ". Cannot continue!");
            return nullptr;
        }
    }
    if (!this->file->open(QFile::ReadWrite)) {
        loadFile.close();
        cancel = true;
        this->Show_Error(Common_Strings::STRING_LEVEL_HEADED +
                         " does not have proper read/write permissions to access " +
                         this->file->fileName() + ". Cannot continue!");
        return nullptr;
    }

    //Copy the ROM to the new location
    QByteArray buffer = loadFile.readAll();
    if (buffer.isEmpty() || this->file->write(buffer) == -1) {
        loadFile.close();
        cancel = true;
        this->Show_Error("Unable to copy the game to " + this->file->fileName());
        return nullptr;
    }

    //Get Read permissions
    this->file->close();
    assert(this->file->exists());
    if (!this->file->open(QFile::ReadWrite)) {
        loadFile.close();
        cancel = true;
        this->Show_Error(Common_Strings::STRING_LEVEL_HEADED +
                         " does not have proper read/write permissions to access " +
                         this->file->fileName() + ". Cannot continue!");
        return nullptr;
    }
    return this->file;
}

QFile *ROM_Handler::Load_First_Local_ROM(bool &cancel) {
    QStringList fileNames;
    fileNames.append(ROM_Filename::STRING_USA0);
    fileNames.append(ROM_Filename::STRING_USA1);
    fileNames.append(ROM_Filename::STRING_DUCK);
    fileNames.append(ROM_Filename::STRING_TRACK);
    fileNames.append(ROM_Filename::STRING_COOP_CGTI_1);

    //Attempt to open each supported ROM
    foreach (QString fileName, fileNames) {
        if (cancel) break;
        if (this->Load_Local_ROM(fileName, cancel)) {
            return this->file;
        }
    }

    return nullptr;
}

bool ROM_Handler::Clean_ROM_Directory() {
    QMap<QString, bool> fileNames;
    fileNames.insert(ROM_Filename::STRING_USA0, true);
    fileNames.insert(ROM_Filename::STRING_USA1, true);
    fileNames.insert(ROM_Filename::STRING_DUCK, true);
    fileNames.insert(ROM_Filename::STRING_TRACK, true);
    fileNames.insert(ROM_Filename::STRING_COOP_CGTI_1, true);

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

QStringList ROM_Handler::Get_Installed_ROMs() {
    QStringList fileNames;
    fileNames.append(ROM_Filename::STRING_USA0);
    fileNames.append(ROM_Filename::STRING_USA1);
    fileNames.append(ROM_Filename::STRING_DUCK);
    fileNames.append(ROM_Filename::STRING_TRACK);
    fileNames.append(ROM_Filename::STRING_COOP_CGTI_1);
    QStringList installedROMs;

    //Attempt to open each supported ROM
    foreach (QString fileName, fileNames) {
        QFile loadFile(this->romFolderLocation + "/" + fileName);
        if (!loadFile.exists()) continue;
        if (!loadFile.open(QFile::ReadWrite)) continue;

        //Calculate the checksum
        QString romChecksum = this->romChecksum->Get_ROM_Checksum(&loadFile);
        this->romType = this->romChecksum->Get_ROM_Type_From_Checksum(romChecksum);

        //Ignore the ROM if it is invalid
        if (this->romType == ROM_Type::INVALID) {
            loadFile.close();
            loadFile.remove();
            continue;
        }

        //The ROM must be valid
        installedROMs.append(fileName);
    }

    return installedROMs;
}

ROM_Type::ROM_Type ROM_Handler::Get_ROM_Type() const {
    if (!this->file) return ROM_Type::INVALID;
    return this->romType;
}

QString ROM_Handler::Get_Ouput_ROM_Location() {
    return this->outputROMLocation;
}

void ROM_Handler::Set_Output_ROM_Location(const QString &location) {
    this->outputROMLocation = location;
}

void ROM_Handler::Show_Error(const QString &error) {
    //Perform Cleanup
    if (this->file) {
        if (this->file->isOpen()) this->file->close();
        if (this->file->exists()) this->file->remove();
    }
    delete this->file;
    this->file = nullptr;
    this->romType = ROM_Type::INVALID;

    //Show the error
    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                          error, Common_Strings::STRING_OK);
}
