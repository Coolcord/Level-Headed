#ifndef ROOM_ID_HANDLER_H
#define ROOM_ID_HANDLER_H

#include "../Common SMB1 Files/Level.h"
#include "../Common SMB1 Files/Level_Attribute.h"
#include <QMap>
#include <QVector>

class Room_Order_Writer;

class Room_ID_Handler
{
public:
    Room_ID_Handler();
    ~Room_ID_Handler();
    void Set_Room_Order_Writer(Room_Order_Writer *roomOrderWriter);
    Level::Level Get_Current_Level();
    void Set_Current_Level(Level::Level level);

    bool Get_Room_ID_From_Level(Level::Level level, unsigned char &id);
    QVector<unsigned char> *Get_Midpoint_Indexes_From_Room_ID(unsigned char id);
    QVector<unsigned char> *Get_Midpoint_Indexes_From_Current_Level();
    QVector<unsigned char> *Get_Midpoint_Indexes_From_Level(Level::Level level);
    Level_Attribute::Level_Attribute Get_Level_Attribute_From_Current_Level();
    Level_Attribute::Level_Attribute Get_Level_Attribute_From_Level(Level::Level level);
    Level_Attribute::Level_Attribute Get_Level_Attribute_From_ID(unsigned char id);
    bool Change_Current_Level_ID(unsigned char newID);
    bool Change_Level_ID(Level::Level level, unsigned char newID);
    bool Change_Room_ID(unsigned char oldID, unsigned char newID);
    bool Change_Current_Level_Attribute(Level_Attribute::Level_Attribute attribute);
    bool Change_Level_Attribute(Level::Level level, Level_Attribute::Level_Attribute attribute);
    bool Change_Room_Attribute(unsigned char id, Level_Attribute::Level_Attribute attribute);

private:
    void Populate_Room_IDs();

    Level::Level currentLevel;
    QMap<Level::Level, unsigned char> *roomIDs;
    QMap<unsigned char, QVector<unsigned char>*> *midpointIndexes;
    Room_Order_Writer *roomOrderWriter;

    friend class Room_Order_Writer;
};

#endif // ROOM_ID_HANDLER_H
