#ifndef MIDPOINT_WRITER_H
#define MIDPOINT_WRITER_H

#include "../Common_SMB1_Files/Level.h"
#include "Binary_Manipulator.h"
#include <QFile>
#include <QMap>

class Room_ID_Handler;
class Level_Offset;

class Midpoint_Writer {
public:
    Midpoint_Writer(QFile *file, Level_Offset *levelOffsets, Room_ID_Handler *roomIDHandler);
    ~Midpoint_Writer();
    bool Read_Midpoints();
    bool Write_Midpoints();
    bool Get_Midpoint_At_Level(Level::Level level, int &midpoint);
    bool Set_Midpoint(Level::Level level, int worldNum, int levelNum, int value);
    void Set_More_Than_4_Levels_Per_World(bool value);

private:
    QFile *file;
    Level_Offset *levelOffsets;
    Room_ID_Handler *roomIDHandler;
    QByteArray *buffer;
    QMap<Level::Level, int> *midpointMap;
    bool moreThan4LevelsPerWorld;
};

#endif // MIDPOINT_WRITER_H
