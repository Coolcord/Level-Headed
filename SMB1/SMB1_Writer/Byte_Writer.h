#ifndef BYTES_WRITER_H
#define BYTES_WRITER_H

#include <QFile>
#include <QByteArray>

class Level_Offset;

class Byte_Writer
{
public:
    Byte_Writer(QFile *file, Level_Offset *levelOffset);
    ~Byte_Writer();

protected:
    bool Write_Bytes_To_Offset(int offset, const QByteArray &bytes);

    QFile *file;
    Level_Offset *levelOffset;
};

#endif // BYTES_WRITER_H
