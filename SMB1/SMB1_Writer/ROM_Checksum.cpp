#include "ROM_Checksum.h"
#include "ROM_Filename.h"
#include <QCryptographicHash>
#include <QByteArray>
#include <QDebug>
#include <assert.h>

ROM_Checksum::ROM_Checksum() {
    //Populate the checksum map
    this->checksumMap = new QMap<QString, Unfixed_ROM_Type::Unfixed_ROM_Type>();
    this->checksumMap->insert(CHECKSUM_USA0, Unfixed_ROM_Type::USA0);
    this->checksumMap->insert(CHECKSUM_USA0_HEADERLESS, Unfixed_ROM_Type::USA0_Headerless);
    this->checksumMap->insert(CHECKSUM_USA1, Unfixed_ROM_Type::USA1);
    this->checksumMap->insert(CHECKSUM_USA1_HEADERLESS, Unfixed_ROM_Type::USA1_Headerless);
    this->checksumMap->insert(CHECKSUM_DUCK, Unfixed_ROM_Type::DUCK);
    this->checksumMap->insert(CHECKSUM_DUCK_HEADERLESS, Unfixed_ROM_Type::DUCK_Headerless);
    this->checksumMap->insert(CHECKSUM_COOP_CGTI_1, Unfixed_ROM_Type::COOP_CGTI_1);
    //Populate the filename map
    this->fileNameMap = new QMap<QString, QString>();
    this->fileNameMap->insert(CHECKSUM_USA0, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_USA0_HEADERLESS, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_USA1, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_USA1_HEADERLESS, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_DUCK, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_DUCK_HEADERLESS, ROM_Filename::STRING_USA0);
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

Unfixed_ROM_Type::Unfixed_ROM_Type ROM_Checksum::Get_Unfixed_ROM_Type_From_Checksum(const QString &romChecksum) {
    assert(this->checksumMap);
    if (!this->checksumMap->contains(romChecksum)) return Unfixed_ROM_Type::INVALID;
    return this->checksumMap->value(romChecksum);
}

ROM_Type::ROM_Type ROM_Checksum::Get_ROM_Type_From_Checksum(const QString &romChecksum) {
    return this->Get_ROM_Type_From_Unfixed_ROM_Type(this->Get_Unfixed_ROM_Type_From_Checksum(romChecksum));
}

QString ROM_Checksum::Get_ROM_Filename_From_Checksum(const QString &romChecksum) {
    assert(this->fileNameMap);
    if (!this->fileNameMap->contains(romChecksum)) return QString();
    return this->fileNameMap->value(romChecksum);
}

ROM_Type::ROM_Type ROM_Checksum::Get_ROM_Type_From_Unfixed_ROM_Type(Unfixed_ROM_Type::Unfixed_ROM_Type unfixedROMType) {
    switch (unfixedROMType) {
    case Unfixed_ROM_Type::USA0:            return ROM_Type::DEFAULT;
    case Unfixed_ROM_Type::USA0_Headerless: return ROM_Type::DEFAULT;
    case Unfixed_ROM_Type::USA1:            return ROM_Type::DEFAULT;
    case Unfixed_ROM_Type::USA1_Headerless: return ROM_Type::DEFAULT;
    case Unfixed_ROM_Type::DUCK:            return ROM_Type::DEFAULT;
    case Unfixed_ROM_Type::DUCK_Headerless: return ROM_Type::DEFAULT;
    case Unfixed_ROM_Type::COOP_CGTI_1:     return ROM_Type::COOP_CGTI_1;
    case Unfixed_ROM_Type::INVALID:         return ROM_Type::INVALID;
    }
    return ROM_Type::INVALID;
}

bool ROM_Checksum::Apply_Fixes_To_Match_Checksum(QByteArray &buffer, Unfixed_ROM_Type::Unfixed_ROM_Type unfixedROMType) {
    if (buffer.isEmpty()) return false;
    QString expectedChecksum = QString();
    switch (unfixedROMType) {
    case Unfixed_ROM_Type::USA0:
        return true;
    case Unfixed_ROM_Type::USA0_Headerless:
        this->Add_Standard_Header(buffer);
        expectedChecksum = CHECKSUM_USA0;
        break;
    case Unfixed_ROM_Type::USA1:
        this->Convert_PRG1_To_PRG0(buffer);
        expectedChecksum = CHECKSUM_USA0;
        break;
    case Unfixed_ROM_Type::USA1_Headerless:
        this->Add_Standard_Header(buffer);
        this->Convert_PRG1_To_PRG0(buffer);
        expectedChecksum = CHECKSUM_USA0;
        break;
    case Unfixed_ROM_Type::DUCK:
        this->Convert_DUCK_To_PRG0(buffer);
        expectedChecksum = CHECKSUM_USA0;
        break;
    case Unfixed_ROM_Type::DUCK_Headerless:
        this->Add_Standard_Header(buffer);
        this->Convert_DUCK_To_PRG0(buffer);
        expectedChecksum = CHECKSUM_USA0;
        break;
    case Unfixed_ROM_Type::COOP_CGTI_1:
        return true;
    case Unfixed_ROM_Type::INVALID:
        return false;
    }
    QString actualChecksum = QString(QCryptographicHash::hash(buffer, QCryptographicHash::Sha512).toHex().toUpper());
    return expectedChecksum == actualChecksum;
}

bool ROM_Checksum::Check_ROM_Header(QFile *file) {
    assert(file);
    if (!file->seek(0)) return false;
    QByteArray buffer(4, ' ');
    if (file->read(buffer.data(), 4) != 4) return false;
    if (buffer.data() == nullptr || buffer.size() != 4) return false;

    //Check the Header as Normal
    bool hasHeader = false;
    if (this->Check_NES_ROM_Header(&buffer)) hasHeader = true;
    else if (this->Check_FDS_ROM_Header(&buffer)) hasHeader = true;
    if (hasHeader) return true;

    //If there is no Header, check the code at 0x9
    if (!file->seek(0x9)) return false;
    if (file->read(buffer.data(), 4) != 4) return false;
    if (buffer.data() == nullptr || buffer.size() != 4) return false;
    return this->Check_No_ROM_Header(&buffer);
}

bool ROM_Checksum::Check_NES_ROM_Header(QByteArray *buffer) {
    assert(buffer);

    //Make sure the first 4 bytes of the header are valid: "NES"
    if (static_cast<unsigned char>(buffer->at(0)) != 0x4E) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0x45) return false;
    if (static_cast<unsigned char>(buffer->at(2)) != 0x53) return false;
    return static_cast<unsigned char>(buffer->at(3)) == 0x1A;
}

bool ROM_Checksum::Check_No_ROM_Header(QByteArray *buffer) {
    assert(buffer);
    if (static_cast<unsigned char>(buffer->at(0)) != 0x9A) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0xAD) return false;
    if (static_cast<unsigned char>(buffer->at(2)) != 0x02) return false;
    return static_cast<unsigned char>(buffer->at(3)) == 0x20;
}

bool ROM_Checksum::Check_FDS_ROM_Header(QByteArray *buffer) {
    assert(buffer);

    //Make sure the first 4 bytes of the header are valid: "FDS"
    if (static_cast<unsigned char>(buffer->at(0)) != 0x46) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0x44) return false;
    if (static_cast<unsigned char>(buffer->at(2)) != 0x53) return false;
    return static_cast<unsigned char>(buffer->at(3)) == 0x1A;
}

void ROM_Checksum::Add_Standard_Header(QByteArray &buffer) {
    buffer.prepend(QByteArray::fromHex(QString("4e45531a020101000000000000000000").toLatin1()));
}

void ROM_Checksum::Convert_PRG1_To_PRG0(QByteArray &buffer) {
    buffer.replace(0x72DA, 4, QByteArray(4, static_cast<char>(0xFF)));
}

void ROM_Checksum::Convert_DUCK_To_PRG0(QByteArray &buffer) {
    buffer.remove(0x12010, 0x2000);
    buffer.remove(0x8010, 0x8000);
    buffer.replace(0x0000, 16, QByteArray::fromHex(QString("4e45531a020101000000000000000000").toLatin1()));
    buffer.replace(0x0010, 9, QByteArray::fromHex(QString("78D8A9108D0020A2FF").toLatin1()));
    buffer.replace(0x9440, 16, QByteArray::fromHex(QString("FFFFFFFFFFFFFFFF0101010101010101").toLatin1()));
    buffer.replace(0x9F72, 1, QByteArray(1, static_cast<char>(0x43)));
}
