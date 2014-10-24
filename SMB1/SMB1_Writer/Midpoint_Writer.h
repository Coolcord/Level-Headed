#ifndef MIDPOINT_WRITER_H
#define MIDPOINT_WRITER_H

#include "../Common SMB1 Files/Level.h"
#include "Binary_Manipulator.h"
#include <QFile>

class Room_ID_Handler;
class Level_Offset;

class Midpoint_Writer
{
public:
    Midpoint_Writer(QFile *file, Level_Offset *levelOffsets, Room_ID_Handler *roomIDHandler);
    ~Midpoint_Writer();
    bool Read_Midpoints();
    bool Write_Midpoints();
    void Set_Current_Level(Level::Level level);
    bool Set_Midpoint(int value);

private:
    QFile *file;
    Level_Offset *levelOffsets;
    Room_ID_Handler *roomIDHandler;
    QByteArray *buffer;
    Level::Level currentLevel;
};

#endif // MIDPOINT_WRITER_H
