#ifndef ROOM_ORDER_WRITER_H
#define ROOM_ORDER_WRITER_H

#include "../Common SMB1 Files/Level.h"
#include <QByteArray>
#include <QVector>
#include <QFile>

class Room_ID_Handler;
class Level_Offset;

class Room_Order_Writer
{
public:
    Room_Order_Writer(QFile *file, Level_Offset *levelOffset, Room_ID_Handler *roomIDHandler);
    ~Room_Order_Writer();
    bool Read_Room_Order_Table();
    bool Write_Room_Order_Table();
    bool Set_Next_Level(Level::Level level);
    bool Set_Number_Of_Worlds(int value);
    QVector<unsigned char> *Get_Midpoints_From_Room_Order_Table(unsigned char id);
    bool Write_Number_Of_Worlds_To_Offset(int offset, const QByteArray &worldByte);

private:
    void Populate_Midpoint_Indexes_In_Handler();
    bool Update_Room_ID(unsigned char oldID, unsigned char newID);

    QFile *file;
    Room_ID_Handler *roomIDHandler;
    Level_Offset *levelOffset;
    QByteArray *buffer;
    int currentByte;

    friend class Room_ID_Handler;
};

#endif // ROOM_ORDER_WRITER_H
