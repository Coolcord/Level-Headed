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
    this->checksumMap->insert(CHECKSUM_PC10, Unfixed_ROM_Type::PC10);
    this->checksumMap->insert(CHECKSUM_PC10_HEADERLESS, Unfixed_ROM_Type::PC10_Headerless);
    this->checksumMap->insert(CHECKSUM_P1, Unfixed_ROM_Type::P1);
    this->checksumMap->insert(CHECKSUM_P1_HEADERLESS, Unfixed_ROM_Type::P1_Headerless);
    this->checksumMap->insert(CHECKSUM_TR1, Unfixed_ROM_Type::TR1);
    this->checksumMap->insert(CHECKSUM_TR1_HEADERLESS, Unfixed_ROM_Type::TR1_Headerless);
    this->checksumMap->insert(CHECKSUM_TR2, Unfixed_ROM_Type::TR2);
    this->checksumMap->insert(CHECKSUM_TR2_HEADERLESS, Unfixed_ROM_Type::TR2_Headerless);
    this->checksumMap->insert(CHECKSUM_J25, Unfixed_ROM_Type::J25);
    this->checksumMap->insert(CHECKSUM_J25_HEADERLESS, Unfixed_ROM_Type::J25_Headerless);
    this->checksumMap->insert(CHECKSUM_TRACK, Unfixed_ROM_Type::TRACK);
    this->checksumMap->insert(CHECKSUM_TRACK_HEADERLESS, Unfixed_ROM_Type::TRACK_Headerless);
    this->checksumMap->insert(CHECKSUM_TRACK_ALT, Unfixed_ROM_Type::TRACK);
    this->checksumMap->insert(CHECKSUM_TRACK_ALT_HEADERLESS, Unfixed_ROM_Type::TRACK_Headerless);
    this->checksumMap->insert(CHECKSUM_EUROPE, Unfixed_ROM_Type::EUROPE);
    this->checksumMap->insert(CHECKSUM_EUROPE_HEADERLESS, Unfixed_ROM_Type::EUROPE_Headerless);
    this->checksumMap->insert(CHECKSUM_DUCKE, Unfixed_ROM_Type::DUCKE);
    this->checksumMap->insert(CHECKSUM_DUCKE_ALT_HEADER, Unfixed_ROM_Type::EUROPE);
    this->checksumMap->insert(CHECKSUM_DUCKE_HEADERLESS, Unfixed_ROM_Type::DUCKE_Headerless);
    this->checksumMap->insert(CHECKSUM_TETRIS, Unfixed_ROM_Type::TETRIS);
    this->checksumMap->insert(CHECKSUM_TETRIS_HEADERLESS, Unfixed_ROM_Type::TETRIS_Headerless);
    this->checksumMap->insert(CHECKSUM_TETRIS_ALT, Unfixed_ROM_Type::TETRIS);
    this->checksumMap->insert(CHECKSUM_TETRIS_ALT_HEADERLESS, Unfixed_ROM_Type::TETRIS_Headerless);
    this->checksumMap->insert(CHECKSUM_E25, Unfixed_ROM_Type::E25);
    this->checksumMap->insert(CHECKSUM_E25_HEADERLESS, Unfixed_ROM_Type::E25_Headerless);
    this->checksumMap->insert(CHECKSUM_FDS, Unfixed_ROM_Type::FDS);
    this->checksumMap->insert(CHECKSUM_FDS_HEADERLESS, Unfixed_ROM_Type::FDS_Headerless);
    this->checksumMap->insert(CHECKSUM_COOP_CGTI_1, Unfixed_ROM_Type::COOP_CGTI_1);

    //Populate the filename map
    this->fileNameMap = new QMap<QString, QString>();
    this->fileNameMap->insert(CHECKSUM_USA0, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_USA0_HEADERLESS, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_USA1, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_USA1_HEADERLESS, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_DUCK, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_DUCK_HEADERLESS, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_PC10, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_PC10_HEADERLESS, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_P1, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_P1_HEADERLESS, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_TR1, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_TR1_HEADERLESS, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_TR2, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_TR2_HEADERLESS, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_J25, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_J25_HEADERLESS, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_TRACK, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_TRACK_HEADERLESS, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_TRACK_ALT, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_TRACK_ALT_HEADERLESS, ROM_Filename::STRING_USA0);
    this->fileNameMap->insert(CHECKSUM_EUROPE, ROM_Filename::STRING_EUROPE);
    this->fileNameMap->insert(CHECKSUM_EUROPE_HEADERLESS, ROM_Filename::STRING_EUROPE);
    this->fileNameMap->insert(CHECKSUM_DUCKE, ROM_Filename::STRING_EUROPE);
    this->fileNameMap->insert(CHECKSUM_DUCKE_ALT_HEADER, ROM_Filename::STRING_EUROPE);
    this->fileNameMap->insert(CHECKSUM_DUCKE_HEADERLESS, ROM_Filename::STRING_EUROPE);
    this->fileNameMap->insert(CHECKSUM_TETRIS, ROM_Filename::STRING_EUROPE);
    this->fileNameMap->insert(CHECKSUM_TETRIS_HEADERLESS, ROM_Filename::STRING_EUROPE);
    this->fileNameMap->insert(CHECKSUM_TETRIS_ALT, ROM_Filename::STRING_EUROPE);
    this->fileNameMap->insert(CHECKSUM_TETRIS_ALT_HEADERLESS, ROM_Filename::STRING_EUROPE);
    this->fileNameMap->insert(CHECKSUM_E25, ROM_Filename::STRING_EUROPE);
    this->fileNameMap->insert(CHECKSUM_E25_HEADERLESS, ROM_Filename::STRING_EUROPE);
    this->fileNameMap->insert(CHECKSUM_FDS, ROM_Filename::STRING_FDS);
    this->fileNameMap->insert(CHECKSUM_FDS_HEADERLESS, ROM_Filename::STRING_FDS);
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
    case Unfixed_ROM_Type::USA0:
    case Unfixed_ROM_Type::USA0_Headerless:
    case Unfixed_ROM_Type::USA1:
    case Unfixed_ROM_Type::USA1_Headerless:
    case Unfixed_ROM_Type::DUCK:
    case Unfixed_ROM_Type::DUCK_Headerless:
    case Unfixed_ROM_Type::PC10:
    case Unfixed_ROM_Type::PC10_Headerless:
    case Unfixed_ROM_Type::P1:
    case Unfixed_ROM_Type::P1_Headerless:
    case Unfixed_ROM_Type::TR1:
    case Unfixed_ROM_Type::TR1_Headerless:
    case Unfixed_ROM_Type::TR2:
    case Unfixed_ROM_Type::TR2_Headerless:
    case Unfixed_ROM_Type::TRACK:
    case Unfixed_ROM_Type::TRACK_Headerless:
    case Unfixed_ROM_Type::J25:
    case Unfixed_ROM_Type::J25_Headerless:
        return ROM_Type::DEFAULT;
    case Unfixed_ROM_Type::EUROPE:
    case Unfixed_ROM_Type::EUROPE_Headerless:
    case Unfixed_ROM_Type::DUCKE:
    case Unfixed_ROM_Type::DUCKE_Headerless:
    case Unfixed_ROM_Type::TETRIS:
    case Unfixed_ROM_Type::TETRIS_Headerless:
    case Unfixed_ROM_Type::E25:
    case Unfixed_ROM_Type::E25_Headerless:
        return ROM_Type::EUROPE;
    case Unfixed_ROM_Type::FDS:
    case Unfixed_ROM_Type::FDS_Headerless:
        return ROM_Type::FDS;
    case Unfixed_ROM_Type::COOP_CGTI_1:
        return ROM_Type::COOP_CGTI_1;
    case Unfixed_ROM_Type::INVALID:
        return ROM_Type::INVALID;
    }
    return ROM_Type::INVALID;
}

bool ROM_Checksum::Apply_Fixes_To_Match_Checksum(QByteArray &buffer, Unfixed_ROM_Type::Unfixed_ROM_Type unfixedROMType) {
    //Add a header if necessary
    if (buffer.isEmpty()) return false;
    switch (unfixedROMType) {
    default: break;
    case Unfixed_ROM_Type::USA0_Headerless:
    case Unfixed_ROM_Type::USA1_Headerless:
    case Unfixed_ROM_Type::DUCK_Headerless:
    case Unfixed_ROM_Type::PC10_Headerless:
    case Unfixed_ROM_Type::P1_Headerless:
    case Unfixed_ROM_Type::TR1_Headerless:
    case Unfixed_ROM_Type::TR2_Headerless:
    case Unfixed_ROM_Type::TRACK_Headerless:
    case Unfixed_ROM_Type::EUROPE_Headerless:
    case Unfixed_ROM_Type::DUCKE_Headerless:
    case Unfixed_ROM_Type::TETRIS_Headerless:
    case Unfixed_ROM_Type::J25_Headerless:
    case Unfixed_ROM_Type::E25_Headerless:
        this->Add_Standard_Header(buffer); break;
    case Unfixed_ROM_Type::FDS_Headerless:
        this->Add_Famicom_Header(buffer); break;
    }

    //Extract or patch the ROM if necessary
    QString expectedChecksum = CHECKSUM_USA0;
    switch (unfixedROMType) {
    case Unfixed_ROM_Type::INVALID:
        return false;
    case Unfixed_ROM_Type::USA0:
    case Unfixed_ROM_Type::EUROPE:
    case Unfixed_ROM_Type::FDS:
    case Unfixed_ROM_Type::COOP_CGTI_1:
        return true;
    case Unfixed_ROM_Type::FDS_Headerless:
        expectedChecksum = CHECKSUM_FDS; break;
    case Unfixed_ROM_Type::USA0_Headerless:
        break;
    case Unfixed_ROM_Type::USA1_Headerless:
    case Unfixed_ROM_Type::USA1:
        this->Convert_PRG1_To_PRG0(buffer); break;
    case Unfixed_ROM_Type::DUCK_Headerless:
    case Unfixed_ROM_Type::DUCK:
        this->Convert_DUCK_To_PRG0(buffer); break;
    case Unfixed_ROM_Type::PC10_Headerless:
    case Unfixed_ROM_Type::PC10:
        this->Convert_PC10_To_PRG0(buffer); break;
    case Unfixed_ROM_Type::P1_Headerless:
    case Unfixed_ROM_Type::P1:
        this->Convert_P1_To_PRG0(buffer); break;
    case Unfixed_ROM_Type::TR1_Headerless:
    case Unfixed_ROM_Type::TR2_Headerless:
    case Unfixed_ROM_Type::TR1:
    case Unfixed_ROM_Type::TR2:
        this->Convert_Trained_To_PRG0(buffer); break;
    case Unfixed_ROM_Type::TRACK_Headerless:
    case Unfixed_ROM_Type::TRACK:
        this->Convert_TRACK_To_PRG0(buffer); break;
    case Unfixed_ROM_Type::J25:
    case Unfixed_ROM_Type::J25_Headerless:
        this->Convert_25th_Anniversary_To_Normal(buffer); break;
    case Unfixed_ROM_Type::EUROPE_Headerless:
        expectedChecksum = CHECKSUM_EUROPE; break;
    case Unfixed_ROM_Type::DUCKE:
    case Unfixed_ROM_Type::DUCKE_Headerless:
        this->Convert_DUCKE_To_Europe(buffer);
        expectedChecksum = CHECKSUM_EUROPE;
        break;
    case Unfixed_ROM_Type::TETRIS:
    case Unfixed_ROM_Type::TETRIS_Headerless:
        this->Convert_TETRIS_To_Europe(buffer);
        expectedChecksum = CHECKSUM_EUROPE;
        break;
    case Unfixed_ROM_Type::E25:
    case Unfixed_ROM_Type::E25_Headerless:
        this->Convert_25th_Anniversary_To_Normal(buffer);
        expectedChecksum = CHECKSUM_EUROPE;
        break;
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
    if (this->Check_No_ROM_Header(&buffer)) return true;
    else if (this->Check_Track_No_ROM_Header(&buffer)) return true;
    else if (this->Check_FDS_No_ROM_Header(&buffer)) return true;
    else if (this->Check_Tetris_No_ROM_Header(&buffer)) return true;
    else return this->Check_Trained_No_ROM_Header(&buffer);
}

bool ROM_Checksum::Check_NES_ROM_Header(QByteArray *buffer) {
    //Make sure the first 4 bytes of the header are valid: "NES"
    assert(buffer);
    if (static_cast<unsigned char>(buffer->at(0)) != 0x4E) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0x45) return false;
    if (static_cast<unsigned char>(buffer->at(2)) != 0x53) return false;
    return static_cast<unsigned char>(buffer->at(3)) == 0x1A;
}

bool ROM_Checksum::Check_FDS_ROM_Header(QByteArray *buffer) {
    //Make sure the first 4 bytes of the header are valid: "FDS"
    assert(buffer);
    if (static_cast<unsigned char>(buffer->at(0)) != 0x46) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0x44) return false;
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

bool ROM_Checksum::Check_FDS_No_ROM_Header(QByteArray *buffer) {
    assert(buffer);
    if (static_cast<unsigned char>(buffer->at(0)) != 0x4F) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0x2D) return false;
    if (static_cast<unsigned char>(buffer->at(2)) != 0x48) return false;
    return static_cast<unsigned char>(buffer->at(3)) == 0x56;
}

bool ROM_Checksum::Check_Tetris_No_ROM_Header(QByteArray *buffer) {
    assert(buffer);
    if (static_cast<unsigned char>(buffer->at(0)) != 0x48) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0xA9) return false;
    if (static_cast<unsigned char>(buffer->at(2)) != 0x00) return false;
    return static_cast<unsigned char>(buffer->at(3)) == 0x85;
}

bool ROM_Checksum::Check_Track_No_ROM_Header(QByteArray *buffer) {
    assert(buffer);
    if (static_cast<unsigned char>(buffer->at(0)) != 0x01) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0x29) return false;
    if (static_cast<unsigned char>(buffer->at(2)) != 0x20) return false;
    return static_cast<unsigned char>(buffer->at(3)) == 0xF0;
}

bool ROM_Checksum::Check_Trained_No_ROM_Header(QByteArray *buffer) {
    assert(buffer);
    if (static_cast<unsigned char>(buffer->at(0)) != 0xFC) return false;
    if (static_cast<unsigned char>(buffer->at(1)) != 0x06) return false;
    if (static_cast<unsigned char>(buffer->at(2)) != 0xF0) return false;
    return static_cast<unsigned char>(buffer->at(3)) == 0x36;
}

void ROM_Checksum::Add_Standard_Header(QByteArray &buffer) {
    buffer.prepend(QByteArray::fromHex(QString("4e45531a020101000000000000000000").toLatin1()));
}

void ROM_Checksum::Add_Famicom_Header(QByteArray &buffer) {
    buffer.prepend(QByteArray::fromHex(QString("4644531A010000000000000000000000").toLatin1()));
}

void ROM_Checksum::Convert_PRG1_To_PRG0(QByteArray &buffer) {
    buffer.replace(0x72DA, 4, QByteArray(4, static_cast<char>(0xFF)));
}

void ROM_Checksum::Convert_DUCK_To_PRG0(QByteArray &buffer) {
    buffer.remove(0x12010, 0x2000);
    buffer.remove(0x8010, 0x8000);
    buffer.replace(0x0000, 25, QByteArray::fromHex(QString("4e45531a02010100000000000000000078D8A9108D0020A2FF").toLatin1()));
    buffer.replace(0x9440, 16, QByteArray::fromHex(QString("FFFFFFFFFFFFFFFF0101010101010101").toLatin1()));
    buffer.replace(0x9F72, 1, QByteArray(1, static_cast<char>(0x43)));
}

void ROM_Checksum::Convert_PC10_To_PRG0(QByteArray &buffer) {
    buffer.remove(0xA010, 0x2000);
    buffer.replace(0x0004, 4, QByteArray::fromHex(QString("02010100").toLatin1()));
}

void ROM_Checksum::Convert_P1_To_PRG0(QByteArray &buffer) {
    this->Write_Bytes_To_Buffer(buffer, 0x0013, QByteArray(1, static_cast<char>(0x10)));
    this->Write_Bytes_To_Buffer(buffer, 0x027C, QByteArray::fromHex(QString("AEFC07F04AC940D046").toLatin1()));
    this->Write_Bytes_To_Buffer(buffer, 0x028D, QByteArray::fromHex(QString("8DA207").toLatin1()));
    this->Write_Bytes_To_Buffer(buffer, 0x02AF, QByteArray::fromHex(QString("E88A29078D6B07").toLatin1()));
    this->Write_Bytes_To_Buffer(buffer, 0x10E9, QByteArray(1, static_cast<char>(0x60)));
    this->Write_Bytes_To_Buffer(buffer, 0x356A, QByteArray(1, static_cast<char>(0x0A)));
    this->Write_Bytes_To_Buffer(buffer, 0x7FFA, QByteArray::fromHex(QString("15161617171819191A1A1C1D1D").toLatin1()));
    this->Write_Bytes_To_Buffer(buffer, 0x9ED3, QByteArray::fromHex(QString("2620C6542620E65426210654262085014420865448209A014920A5C94620BAC94A20A60AD0D1D8D8"
            "DED1D0DADED120C60AD2D3DBDBDBD9DBDCDBDF20E60AD4D5D4D9DBE2D4DADBE021060AD6D7D6D7E1"
            "26D6DDE1E1212614D0E8D1D0D1DED1D8D0D126DED1DED1D0D1D0D126214614DB4242DB42DB42DBDB"
            "4226DB42DB42DB42DB4226216646DB216C0EDFDBDBDB26DBDFDBDFDBDBE4E526218614DBDBDBDE43"
            "DBE0DBDBDB26DBE3DBE0DBDBE6E32621A614DBDBDBDB42DBDBDBD4D926DBD9DBDBD4D9D4D9E721C5"
            "165F95959595959595959798789596959597989798957A21ED0ECF01090805241712171D0E170D18").toLatin1()));
}

void ROM_Checksum::Convert_TRACK_To_PRG0(QByteArray &buffer) {
    buffer.remove(0x2A010, 0x6000);
    buffer.remove(0x10010, 0x18000);
    buffer.remove(0x0010, 0x8000);
    buffer.replace(0x0000, 16, QByteArray::fromHex(QString("4e45531a020101000000000000000000").toLatin1()));
    buffer.replace(0x72DA, 4, QByteArray(4, static_cast<char>(0xFF)));
    buffer.replace(0x800C, 2, QByteArray::fromHex(QString("0080").toLatin1()));
}

void ROM_Checksum::Convert_Trained_To_PRG0(QByteArray &buffer) {
    buffer.remove(0x0010, 0x200);
    buffer.replace(0x0000, 16, QByteArray::fromHex(QString("4e45531a020101000000000000000000").toLatin1()));
    buffer.replace(0x0E8B, 3, QByteArray::fromHex(QString("9DFC06").toLatin1()));
    buffer.replace(0x3453, 4, QByteArray::fromHex(QString("2818100C").toLatin1()));
    buffer.replace(0x589D, 2, QByteArray::fromHex(QString("F006").toLatin1()));
}

void ROM_Checksum::Convert_25th_Anniversary_To_Normal(QByteArray &buffer) {
    buffer.replace(0x9543, 59, QByteArray::fromHex(QString("787373737F7F80A0878F8E8E8600FFFF3F9F9F9F1FFE0105C1E17171F17E7E7F7E7E7F7FFF8181808181A080FF7F7FFF7F7FFFFFFFF1C1C181C1C5").toLatin1()));
}

void ROM_Checksum::Convert_DUCKE_To_Europe(QByteArray &buffer) {
    buffer.remove(0x12010, 0x2000);
    buffer.remove(0x8010, 0x8000);
    buffer.replace(0x0000, 25, QByteArray::fromHex(QString("4E45531A02010100000000000000000078D8A9108D0020A2FF").toLatin1()));
    buffer.replace(0x9440, 16, QByteArray::fromHex(QString("FFFFFFFFFFFFFFFF0101010101010101").toLatin1()));
    buffer.replace(0x9F72, 1, QByteArray(1, static_cast<char>(0x43)));
}

void ROM_Checksum::Convert_TETRIS_To_Europe(QByteArray &buffer) {
    buffer.remove(0x60010, 0x20000);
    buffer.remove(0x20010, 0x3E000);
    buffer.remove(0x0010, 0x18000);
    buffer.replace(0x0000, 16, QByteArray::fromHex(QString("4e45531a020101000000000000000000").toLatin1()));
}

void ROM_Checksum::Write_Bytes_To_Buffer(QByteArray &buffer, int offset, const QByteArray &bytes) {
    buffer.replace(offset, bytes.size(), bytes);
}
