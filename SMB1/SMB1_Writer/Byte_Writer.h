#ifndef BYTES_WRITER_H
#define BYTES_WRITER_H

#include <QFile>
#include <QByteArray>

class Level_Offset;

class Byte_Writer {
public:
    Byte_Writer(QFile *file, Level_Offset *levelOffset);
    ~Byte_Writer();

protected:
    bool Increment_Byte_At_Offset(qint64 offset, int incrementAmount);
    bool Read_Bytes_From_Offset(qint64 offset, int amount, QByteArray &bytes);
    bool Write_Bytes_To_Offset(qint64 offset, const QByteArray &bytes);

    QFile *file;
    Level_Offset *levelOffset;
};

#endif // BYTES_WRITER_H
