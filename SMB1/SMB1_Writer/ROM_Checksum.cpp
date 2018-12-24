#include "ROM_Checksum.h"
#include "ROM_Filename.h"
#include <QCryptographicHash>
#include <QByteArray>
#include <QDebug>
#include <assert.h>

ROM_Checksum::ROM_Checksum() {
    //Populate the checksum map
    this->checksumMap = new QMap<QString, ROM_Type::ROM_Type>();
    this->checksumMap->insert(CHECKSUM_USA0, ROM_Type::DEFAULT);
    this->checksumMap->insert(CHECKSUM_USA1, ROM_Type::DEFAULT);
    this->checksumMap->insert(CHECKSUM_COOP_CGTI_1, ROM_Type::COOP_CGTI_1);
    //Populate the filename map
    this->fileNameMap = new QMap<QString, QString>();
    this->fileNameMap->insert(CHECKSUM_USA0, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_USA1, ROM_Filename::STRING_USA1);
    this->fileNameMap->insert(CHECKSUM_COOP_CGTI_1, ROM_Filename::STRING_COOP_CGTI_1);
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
    if (!this->Check_ROM_Header(file)) return QString();
    qDebug() << "ROM Header is valid";

    //Read the ROM into a buffer to perform a checksum
    file->seek(0);
    QByteArray buffer = file->readAll();
    if (!file->reset()) return QString();
    if (buffer.size() == 0) return QString();

    //Calculate the hash and return it
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

bool ROM_Checksum::Check_ROM_Header(QFile *file) {
    assert(file);
    if (!file->seek(0)) return false;
    QByteArray buffer(4, ' ');
    if (file->read(buffer.data(), 4) != 4) return false;
    if (buffer.data() == nullptr || buffer.size() != 4) return false;

    if (this->Check_NES_ROM_Header(&buffer)) return true;
    return this->Check_FDS_ROM_Header(&buffer);
}

bool ROM_Checksum::Check_NES_ROM_Header(QByteArray *buffer) {
    assert(buffer);

    //Make sure the first 4 bytes of the header are valid: "NES"
    if (static_cast<unsigned char>(buffer->at(0)) != 0x4E) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0x45) return false;
    if (static_cast<unsigned char>(buffer->at(2)) != 0x53) return false;
    return static_cast<unsigned char>(buffer->at(3)) == 0x1A;
}

bool ROM_Checksum::Check_FDS_ROM_Header(QByteArray *buffer) {
    assert(buffer);

    //Make sure the first 4 bytes of the header are valid: "FDS"
    if (static_cast<unsigned char>(buffer->at(0)) != 0x46) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0x44) return false;
    if (static_cast<unsigned char>(buffer->at(2)) != 0x53) return false;
    return static_cast<unsigned char>(buffer->at(3)) == 0x1A;
}
