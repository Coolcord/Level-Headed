#ifndef ROOM_ID_HANDLER_H
#define ROOM_ID_HANDLER_H

#include "../Common SMB1 Files/Level.h"
#include <QMap>
#include <QVector>

class Room_ID_Handler
{
public:
    Room_ID_Handler();
    ~Room_ID_Handler();
    int Get_Room_ID_From_Level(Level::Level level);
    QVector<int> *Get_Midpoint_Indexes_From_Room_ID(int id);
    QVector<int> *Get_Midpoint_Indexes_From_Level(Level::Level level);

private:
    void Populate_Room_IDs();

    QMap<Level::Level, int> *roomIDs;
    QMap<int, QVector<int>*> *midpointIndexes;

    friend class Room_Order_Writer;
};

#endif // ROOM_ID_HANDLER_H
