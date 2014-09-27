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
    const QString CHECKSUM_EUROPE = "5683F4ABBF4B2F66403E063594D02183B252BDFB3E6662829EB008C00EAAF0E80671E8745AA067B9A5F46F20E91F2615AE1B67FF1E86D034FE7088677736E5B3";
    const QString CHECKSUM_FDS =    "3CB7F3B92CDC6989CEB1DDC04CECA56FAB3CF24BEDB02B655360025027730CF4A1A9D3C793CFD0E9BFD1760C7260D3A4F31F6A5F32E5C725235C57D66498ACCB";
    const QString CHECKSUM_DUCK =   "C900D4B65C956D9BD1FC5459683061017745E23F82588F6C2A152ED4AD4F83025033FB9BE32734D608CC55A4F7A82CCB716BB9CC95EA0542E5F656C69D3E298C";
    const QString CHECKSUM_TRACK =  "7815F5BD6568AE3E2FB428CEA510415FE688971A40AF2E9D354E37AC0D3B98E74F53A8F9BD9A7FBF1785A3B785EF447B5D770F3DE77D21E4B063FC7B5D7DA5EF";
    const QString CHECKSUM_DUCKE =  "01F8DAC3E8281C93A758355D0FC9618ED5239B224650BE9241F04CB8231EB3D07D8F6D59CBF3AC24D1D55CB008D7F71F8F76B6630D3DF739265EC4B477D49670";
    const QString CHECKSUM_TETRIS = "0E198DF9E0D5863F865838EAFF7090596DA548D95CE9CDA8BFBBAF0DC3E6591104140924C1107464CCEE547DA82301E615F1E7B33FB38CE3705B64B5531060D8";
};

#endif // ROM_CHECKSUM_H
