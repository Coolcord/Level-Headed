#ifndef MIDPOINT_WRITER_H
#define MIDPOINT_WRITER_H

#include "../Common SMB1 Files/Level.h"
#include "Binary_Manipulator.h"
#include <QFile>

class Level_Offset;

class Midpoint_Writer
{
public:
    Midpoint_Writer(QFile *file, Level_Offset *levelOffsets);
    bool Read_Midpoints();
    bool Write_Midpoints();
    bool Set_Midpoint(int index, int value);

private:
    QFile *file;
    Level_Offset *levelOffsets;
    QByteArray *buffer;
};

#endif // MIDPOINT_WRITER_H
