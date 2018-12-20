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

    //SHA512
    const QString CHECKSUM_USA0 =   "C2EB66533F2BFEC61F15793884777B1C711AEBD74533CD33BD5774112A0381234583B9AA05588225282FD3071A42A7A4DA27BDE485D21FC0B45B2B2CA2D5461F";
    const QString CHECKSUM_USA1 =   "C254479DF875E6F88F015563271624641433B15DE8242DE99EB883A745D09136C81663CA86613F02188E191B9C86F032D7C2080D4AA470B1CD9DB3695F811914";
    const QString CHECKSUM_DUCK =   "C900D4B65C956D9BD1FC5459683061017745E23F82588F6C2A152ED4AD4F83025033FB9BE32734D608CC55A4F7A82CCB716BB9CC95EA0542E5F656C69D3E298C";
    const QString CHECKSUM_TRACK =  "7815F5BD6568AE3E2FB428CEA510415FE688971A40AF2E9D354E37AC0D3B98E74F53A8F9BD9A7FBF1785A3B785EF447B5D770F3DE77D21E4B063FC7B5D7DA5EF";
    const QString CHECKSUM_COOP_CGTI_1 = "5CF8226F766B0E4D1163A6D21EED4FC094FBD48CC42C031604B1D17605DD042062779AEBFA81BBC883B8FA80C6DCC108F05FAC2D37A5D77ED3C6A34C00BC444C";
};

#endif // ROM_CHECKSUM_H
