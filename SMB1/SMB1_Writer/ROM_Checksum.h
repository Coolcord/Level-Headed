#ifndef ROM_CHECKSUM_H
#define ROM_CHECKSUM_H

#include "ROM_Type.h"
#include "Unfixed_ROM_Type.h"
#include <QString>
#include <QFile>
#include <QMap>
#include <QByteArray>

class ROM_Checksum {
public:
    ROM_Checksum();
    ~ROM_Checksum();
    QString Get_ROM_Checksum(QFile *file);
    Unfixed_ROM_Type::Unfixed_ROM_Type Get_Unfixed_ROM_Type_From_Checksum(const QString &romChecksum);
    ROM_Type::ROM_Type Get_ROM_Type_From_Checksum(const QString &romChecksum);
    QString Get_ROM_Filename_From_Checksum(const QString &romChecksum);
    ROM_Type::ROM_Type Get_ROM_Type_From_Unfixed_ROM_Type(Unfixed_ROM_Type::Unfixed_ROM_Type unfixedROMType);
    void Apply_Fixes_To_Match_Checksum(QByteArray &buffer, Unfixed_ROM_Type::Unfixed_ROM_Type unfixedROMType);

private:
    bool Check_ROM_Header(QFile *file);
    bool Check_NES_ROM_Header(QByteArray *buffer);
    bool Check_No_ROM_Header(QByteArray *buffer);
    bool Check_FDS_ROM_Header(QByteArray *buffer);
    void Add_Standard_Header(QByteArray &buffer);

    QMap<QString, Unfixed_ROM_Type::Unfixed_ROM_Type> *checksumMap;
    QMap<QString, QString> *fileNameMap;

    //SHA512
    const QString CHECKSUM_USA0 =               "C2EB66533F2BFEC61F15793884777B1C711AEBD74533CD33BD5774112A0381234583B9AA05588225282FD3071A42A7A4DA27BDE485D21FC0B45B2B2CA2D5461F";
    const QString CHECKSUM_USA0_HEADERLESS =    "28E2B5A96F698F78CE34AE8328B0D264131B9417EAF855938D061C97E6ED8C37642860607E2BCF4EBDA3A838E0C2588E20A134D6A4FAF054EB09F1894E6669C3";
    const QString CHECKSUM_USA1 =               "C254479DF875E6F88F015563271624641433B15DE8242DE99EB883A745D09136C81663CA86613F02188E191B9C86F032D7C2080D4AA470B1CD9DB3695F811914";
    const QString CHECKSUM_USA1_HEADERLESS =    "4D5D45B3E796230A6B3DDCB6BE0093D9190C9A6A4285B423F863214435023A7B5FD24B65FF0B41562AB1C721288599F520461A1664D728E1B82AB60A070F4C8A";
    const QString CHECKSUM_COOP_CGTI_1 =        "CF536C1E82DA8CE57A8A0BEBDA9050A137F07F3A214AACF837EC7C1E54B921E9998E23EB25124F4759669CDC5274A8B814C0E27349A15062D32A5A2EE31B74E5";
};

#endif // ROM_CHECKSUM_H
