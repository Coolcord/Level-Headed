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
    if (fileLocation == NULL || fileLocation.isEmpty()) return QString(); //TODO: Throw an error here

    QFile file(fileLocation);
    if (!file.exists() || !file.open(QFile::ReadWrite)) {
        file.close();
        return QString(); //TODO: Throw an error here
    }

    //Verify ROM Checksum
    QString checksum = this->romChecksum->Get_ROM_Checksum(&file);
    ROM_Type::ROM_Type romType = this->romChecksum->Get_ROM_Type_From_Checksum(checksum);
    if (romType == ROM_Type::INVALID) return QString(); //TODO: Throw an error here

    //Check if the current ROM is already installed
    QString fileName = this->romChecksum->Get_ROM_Filename_From_Checksum(checksum);
    QFile installedFile(this->romFolderLocation + "/" + fileName);
    if (installedFile.exists()) return QString(); //TODO: Throw an error here
    if (!installedFile.open(QFile::ReadWrite)) return QString(); //TODO: Throw an error here

    //Install the ROM
    file.seek(0);
    QByteArray buffer = file.readAll();
    if (buffer.isEmpty()) return QString(); //TODO: Throw an error here
    installedFile.seek(0);
    if (installedFile.write(buffer) == -1) return QString(); //TODO: Throw an error here
    file.close();
    installedFile.close();
    qDebug() << "ROM successfully installed";
    return fileName;
}

QFile *ROM_Handler::Load_Local_ROM(const QString &fileName) {
    qDebug() << "Load_Local_ROM() called!";
    QFile loadFile(this->romFolderLocation + "/" + fileName);
    if (!loadFile.exists() || !loadFile.open(QFile::ReadWrite)) {
        loadFile.close();
        return NULL;
    }

    //Calculate the checksum
    QString romChecksum = this->romChecksum->Get_ROM_Checksum(&loadFile);
    this->romType = this->romChecksum->Get_ROM_Type_From_Checksum(romChecksum);

    //Make sure everything is valid
    if (this->romType == ROM_Type::INVALID) {
        qDebug() << "ROM is invalid";
        loadFile.close();
        loadFile.remove(); //the file is not valid, so delete it
        return NULL;
    }

    //Make a copy of the ROM for the new hacked version
    QString fileLocation = QFileDialog::getSaveFileName(this->parent, "Save Location");
    qDebug() << "Saving at: " << fileLocation;
    if (fileLocation == NULL || fileLocation.isEmpty()) {
        qDebug() << "Failed to get a proper save location";
        loadFile.close();
        return NULL;
    }

    //Open the new save file and delete it if it currently exists
    this->file = new QFile(fileLocation);
    if (this->file->exists()) {
        if (!this->file->remove()) {
            this->file->close();
            delete this->file;
            this->file = NULL;
            loadFile.close();
            //TODO: Throw an error here saying that the file cannot be overwritten
            return NULL;
        }
    }
    if (!this->file->open(QFile::ReadWrite)) {
        this->file->close();
        delete this->file;
        this->file = NULL;
        loadFile.close();
        qDebug() << "Unable to create the save game";
        return NULL;
    }

    //Copy the ROM to the new location
    QByteArray buffer = loadFile.readAll();
    if (buffer.isEmpty()) {
        this->file->close();
        delete this->file;
        this->file = NULL;
        loadFile.close();
        qDebug() << "Buffer was empty";
        //TODO: Show an error saying that the ROM could not be read
        return NULL;
    }

    //Write the ROM at the new save location
    if (this->file->write(buffer) == -1) {
        this->file->close();
        delete this->file;
        this->file = NULL;
        loadFile.close();
        qDebug() << "Unable to copy the game";
        //TODO: Show an error saying that the ROM could not be written
        return NULL;
    }

    //Get Read permissions
    this->file->close();
    assert(this->file->exists());
    if (!this->file->open(QFile::ReadWrite)) {
        qDebug() << "Failed to change permissions to file!";
        this->file->close();
        delete this->file;
        this->file = NULL;
        loadFile.close();
        qDebug() << "Unable to copy the game";
        //TODO: Show an error saying that the ROM could not be written
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
