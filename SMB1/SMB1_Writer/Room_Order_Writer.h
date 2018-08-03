#ifndef ROOM_ORDER_WRITER_H
#define ROOM_ORDER_WRITER_H

#include "../Common_SMB1_Files/Level.h"
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
    QVector<unsigned char> *Get_Midpoints_From_Room_Order_Table(unsigned char id);

private:
    bool Write_Bytes_To_Offset(qint64 offset, const QByteArray &bytes);
    void Populate_Midpoint_Indexes_In_Handler();
    bool Fix_Room_Order_Table_Header();
    bool Scan_Level_For_End_Objects(Level::Level level, bool &endOfWorld);

    QFile *file;
    Room_ID_Handler *roomIDHandler;
    Level_Offset *levelOffset;
    QByteArray *buffer;
    int currentByte;

    friend class Room_ID_Handler;
};

#endif // ROOM_ORDER_WRITER_H
