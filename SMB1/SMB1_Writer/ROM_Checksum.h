#ifndef ROM_CHECKSUM_H
#define ROM_CHECKSUM_H

#include "ROM_Type.h"
#include <QString>
#include <QFile>
#include <QMap>
#include <QByteArray>

class ROM_Checksum {
public:
    ROM_Checksum();
    ~ROM_Checksum();
    QString Get_ROM_Checksum(QFile *file);
    ROM_Type::ROM_Type Get_ROM_Type_From_Checksum(const QString &romChecksum);
    QString Get_ROM_Filename_From_Checksum(const QString &romChecksum);

private:
    bool Check_ROM_Header(QFile *file);
    bool Check_NES_ROM_Header(QByteArray *buffer);
    bool Check_FDS_ROM_Header(QByteArray *buffer);

    QMap<QString, ROM_Type::ROM_Type> *checksumMap;
    QMap<QString, QString> *fileNameMap;

    const QString CHECKSUM_USA0 =   "C2EB66533F2BFEC61F15793884777B1C711AEBD74533CD33BD5774112A0381234583B9AA05588225282FD3071A42A7A4DA27BDE485D21FC0B45B2B2CA2D5461F";
    const QString CHECKSUM_USA1 =   "C254479DF875E6F88F015563271624641433B15DE8242DE99EB883A745D09136C81663CA86613F02188E191B9C86F032D7C2080D4AA470B1CD9DB3695F811914";
    const QString CHECKSUM_FDS =    "3CB7F3B92CDC6989CEB1DDC04CECA56FAB3CF24BEDB02B655360025027730CF4A1A9D3C793CFD0E9BFD1760C7260D3A4F31F6A5F32E5C725235C57D66498ACCB";
};

#endif // ROM_CHECKSUM_H
