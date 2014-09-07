#ifndef LEVEL_OFFSET_H
#define LEVEL_OFFSET_H

#include "ROM_Type.h"
#include "Level.h"
#include <QFile>
#include <QByteArray>

const static int BAD_OFFSET = -1;

class Level_Offset {
public:
    Level_Offset(QFile *file);
    int Get_Level_Object_Offset(Level level);
    int Get_Level_Enemy_Offset(Level level);
    int Fix_Offset(int offset);


private:
    bool Check_ROM_Header();
    bool Check_NES_ROM_Header(QByteArray *buffer);
    bool Check_FDS_ROM_Header(QByteArray *buffer);
    ROM_Type Determine_ROM_Type();

    QFile *file;
    ROM_Type romType;
};

#endif // LEVEL_OFFSET_H
