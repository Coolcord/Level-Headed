#ifndef LEVEL_OFFSET_H
#define LEVEL_OFFSET_H

#include "ROM_Type.h"
#include "../Common SMB1 Files/Level.h"
#include <QFile>
#include <QByteArray>

const static int BAD_OFFSET = -1;

class Level_Offset {
public:
    Level_Offset(QFile *file, ROM_Type::ROM_Type romType);
    int Get_Level_Object_Offset(Level::Level level);
    int Get_Level_Enemy_Offset(Level::Level level);
    int Fix_Offset(int offset);
    ROM_Type::ROM_Type Get_ROM_Type();


private:
    QFile *file;
    ROM_Type::ROM_Type romType;
};

#endif // LEVEL_OFFSET_H
