#include "ROM_Checksum.h"
#include "ROM_Filename.h"
#include <QCryptographicHash>
#include <QByteArray>
#include <assert.h>

ROM_Checksum::ROM_Checksum() {
    //Populate the checksum map
    this->checksumMap = new QMap<QString, ROM_Type::ROM_Type>();
    this->checksumMap->insert(CHECKSUM_USA0, ROM_Type::DEFAULT);
    this->checksumMap->insert(CHECKSUM_USA1, ROM_Type::DEFAULT);
    this->checksumMap->insert(CHECKSUM_EUROPE, ROM_Type::DEFAULT);
    this->checksumMap->insert(CHECKSUM_FDS, ROM_Type::FDS);
    this->checksumMap->insert(CHECKSUM_DUCK, ROM_Type::DEFAULT);
    this->checksumMap->insert(CHECKSUM_TRACK, ROM_Type::TRACK);
    this->checksumMap->insert(CHECKSUM_DUCKE, ROM_Type::DEFAULT);
    this->checksumMap->insert(CHECKSUM_TETRIS, ROM_Type::DEFAULT);

    //Populate the filename map
    this->fileNameMap = new QMap<QString, QString>();
    this->fileNameMap->insert(CHECKSUM_USA0, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_USA1, ROM_Filename::STRING_USA1);
    this->fileNameMap->insert(CHECKSUM_EUROPE, ROM_Filename::STRING_EUROPE);
    this->fileNameMap->insert(CHECKSUM_FDS, ROM_Filename::STRING_FDS);
    this->fileNameMap->insert(CHECKSUM_DUCK, ROM_Filename::STRING_DUCK);
    this->fileNameMap->insert(CHECKSUM_TRACK, ROM_Filename::STRING_TRACK);
    this->fileNameMap->insert(CHECKSUM_DUCKE, ROM_Filename::STRING_DUCKE);
    this->fileNameMap->insert(CHECKSUM_TETRIS, ROM_Filename::STRING_TETRIS);
}

ROM_Checksum::~ROM_Checksum() {
    delete this->checksumMap;
    delete this->fileNameMap;
}

QString ROM_Checksum::Get_ROM_Checksum(QFile *file) {
    //Make sure the file is open and readable
    if (!file) return QString();
    if (!file->isOpen() || !file->isReadable()) return QString();
    if (file->size() > (1048576)) return QString(); //ROM is too big

    //Read the ROM into a buffer to perform a checksum
    file->seek(0);
    QByteArray buffer = file->readAll();
    if (buffer == NULL || buffer.size() == 0) return QString();

    //Calculate the hash and return it
    QString hash = QString(QCryptographicHash::hash(buffer, QCryptographicHash::Sha512).toHex().toUpper());
    return QString(QCryptographicHash::hash(buffer, QCryptographicHash::Sha512).toHex().toUpper());
}

ROM_Type::ROM_Type ROM_Checksum::Get_ROM_Type_From_Checksum(const QString &romChecksum) {
    assert(this->checksumMap);
    if (!this->checksumMap->contains(romChecksum)) return ROM_Type::INVALID;
    return this->checksumMap->value(romChecksum);
}

QString ROM_Checksum::Get_ROM_Filename_From_Checksum(const QString &romChecksum) {
    assert(this->fileNameMap);
    if (!this->fileNameMap->contains(romChecksum)) return QString();
    return this->fileNameMap->value(romChecksum);
}
