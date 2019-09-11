#ifndef TEXT_H
#define TEXT_H

#include "Byte_Writer.h"
#include <QByteArray>
#include <QString>

class Text : public Byte_Writer {
public:
    Text(QFile *file, Level_Offset *levelOffset);
    ~Text() {}
    bool Set_Mario_Name(const QString &name);
    bool Set_Luigi_Name(const QString &name);
    QByteArray Convert_String_To_SMB_Bytes(const QString &string);

};

#endif // TEXT_H
