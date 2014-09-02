#ifndef SMB1_WRITER_H
#define SMB1_WRITER_H

#include "Level_Offsets.h"
#include <QString>
#include <QFile>
#include <QByteArray>

class SMB1_Writer {
public:
    SMB1_Writer(QString romLocation);
    ~SMB1_Writer();
    bool New_Level(const int objectOffset, const int enemyOffset);
    bool Write_Level();


private:
    bool Are_Buffers_Allocated();
    void Deallocate_Buffers();
    bool Write_Buffer(const int offset, QByteArray *buffer);
    bool Read_Level_Header();
    bool Read_Objects();
    bool Read_Enemies();

    int objectOffset;
    int enemyOffset;
    QFile *file;
    QByteArray *headerBuffer;
    QByteArray *objectsBuffer;
    QByteArray *enemiesBuffer;
};

#endif // SMB1_WRITER_H
