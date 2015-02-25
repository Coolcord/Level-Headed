#ifndef ROOM_ADDRESS_WRITER_H
#define ROOM_ADDRESS_WRITER_H

#include <QFile>
#include <QByteArray>

class Room_ID_Handler;
class Level_Offset;

class Room_Address_Writer
{
public:
    Room_Address_Writer(QFile *file, Level_Offset *levelOffset);
    ~Room_Address_Writer();
    bool Read_Room_Address_Tables();
    bool Write_Room_Address_Tables();
    unsigned int Get_Room_ID_Object_Offset_From_Table(unsigned char roomID);
    unsigned int Get_Room_ID_Enemy_Offset_From_Table(unsigned char roomID);

private:
    bool Read_Into_Buffer(int offset, int amount, QByteArray *buffer);
    bool Write_Buffer(int offset, QByteArray *buffer);

    QFile *file;
    Level_Offset *levelOffset;
    QByteArray *lowObjectBuffer;
    QByteArray *highObjectBuffer;
    QByteArray *lowEnemyBuffer;
    QByteArray *highEnemyBuffer;
    QByteArray *objectsHeaderBuffer;
    QByteArray *enemiesHeaderBuffer;

    const static int OBJECT_ADDRESS_HEADER = 0x1D38;
    const static int ENEMY_ADDRESS_HEADER = 0x1CF0;
    const static int OBJECT_ADDRESS_LOW_BYTE = 0x1D3C;
    const static int OBJECT_ADDRESS_HIGH_BYTE = 0x1D5E;
    const static int ENEMY_ADDRESS_LOW_BYTE = 0x1CF4;
    const static int ENEMY_ADDRESS_HIGH_BYTE = 0x1D16;

    friend class Room_ID_Handler;
};

#endif // ROOM_ADDRESS_WRITER_H
