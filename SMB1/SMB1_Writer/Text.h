#ifndef TEXT_H
#define TEXT_H

#include "Byte_Writer.h"
#include <QByteArray>
#include <QString>

class Text : public Byte_Writer {
public:
    Text(QFile *file, Level_Offset *levelOffset) : Byte_Writer(file, levelOffset) {}
    ~Text() {}
    QByteArray Convert_String_To_SMB_Bytes(const QString &string);
    bool Replace_But_Our_Princess_Is_In_Another_Castle();

};

#endif // TEXT_H
