#ifndef LEVEL_OFFSET_H
#define LEVEL_OFFSET_H

#include "ROM_Type.h"
#include "../Common SMB1 Files/Level.h"
#include <QFile>
#include <QByteArray>

class Room_ID_Handler;
class Room_Address_Writer;

const static int BAD_OFFSET = -1;

class Level_Offset {
public:
    Level_Offset(QFile *file, ROM_Type::ROM_Type romType);
    void Set_Extras(Room_ID_Handler *roomIDHandler, Room_Address_Writer *roomAddressWriter);
    int Get_Level_Object_Offset(Level::Level level);
    int Get_Level_Enemy_Offset(Level::Level level);
    int Fix_Offset(int offset);
    ROM_Type::ROM_Type Get_ROM_Type();

private:
    QFile *file;
    ROM_Type::ROM_Type romType;
    Room_ID_Handler *roomIDHandler;
    Room_Address_Writer *roomAddressWriter;
};

#endif // LEVEL_OFFSET_H
