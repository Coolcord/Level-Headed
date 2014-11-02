#ifndef ROOM_ID_HANDLER_H
#define ROOM_ID_HANDLER_H

#include "../Common SMB1 Files/Level.h"
#include "../Common SMB1 Files/Level_Attribute.h"
#include <QMap>
#include <QVector>

class Room_ID_Handler
{
public:
    Room_ID_Handler();
    ~Room_ID_Handler();
    Level::Level Get_Current_Level();
    void Set_Current_Level(Level::Level level);

    bool Get_Room_ID_From_Level(Level::Level level, unsigned char &id);
    QVector<unsigned char> *Get_Midpoint_Indexes_From_Room_ID(unsigned char id);
    QVector<unsigned char> *Get_Midpoint_Indexes_From_Current_Level();
    QVector<unsigned char> *Get_Midpoint_Indexes_From_Level(Level::Level level);
    Level_Attribute::Level_Attribute Get_Level_Attribute_From_Current_Level();
    Level_Attribute::Level_Attribute Get_Level_Attribute_From_Level(Level::Level level);
    Level_Attribute::Level_Attribute Get_Level_Attribute_From_ID(unsigned char id);

private:
    void Populate_Room_IDs();

    Level::Level currentLevel;
    QMap<Level::Level, unsigned char> *roomIDs;
    QMap<unsigned char, QVector<unsigned char>*> *midpointIndexes;

    friend class Room_Order_Writer;
};

#endif // ROOM_ID_HANDLER_H
