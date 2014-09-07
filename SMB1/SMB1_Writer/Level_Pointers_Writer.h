#ifndef LEVEL_POINTERS_WRITER_H
#define LEVEL_POINTERS_WRITER_H

#include "Binary_Manipulator.h"
#include "../Common SMB1 Files/Level_Attribute.h"
#include <QFile>

class Level_Offset;

class Level_Pointers_Writer
{
public:
    Level_Pointers_Writer(QFile *file, Level_Offset *levels);
    ~Level_Pointers_Writer();
    //TODO: Add a way to rewrite the room order table
    //TODO: Add a way to change room IDs

private:
    bool Write_And_Deallocate_Buffer(const int offset, QByteArray *buffer);

    const static int ROOM_ORDER_TABLE = 0x1CCC;
    const static int OBJECT_POINTER_HEADER = 0x1D38;
    const static int ENEMY_POINTER_HEADER = 0x1CF0;
    const static int OBJECT_POINTER_LOW_BYTE = 0x1D3C;
    const static int OBJECT_POINTER_HIGH_BYTE = 0x1D5E;
    const static int ENEMY_POINTER_LOW_BYTE = 0x1CF4;
    const static int ENEMY_POINTER_HIGH_BYTE = 0x1D16;

    QFile *file;
    Level_Offset *levels;
    QByteArray *roomOrder;
    QByteArray *objectHeader;
    QByteArray *enemyHeader;
    QByteArray *objectLowByte;
    QByteArray *objectHighByte;
    QByteArray *enemyLowByte;
    QByteArray *enemyHighByte;
};

#endif // LEVEL_POINTERS_WRITER_H
