#ifndef ROOM_ID_HANDLER_H
#define ROOM_ID_HANDLER_H

#include "../Common_SMB1_Files/Level.h"
#include "../Common_SMB1_Files/Level_Attribute.h"
#include <QFile>
#include <QMap>
#include <QVector>

class Level_Offset;
class Enemy_Bytes_Tracker;
class Room_Order_Writer;
class Room_Address_Writer;

class Room_ID_Handler
{
public:
    Room_ID_Handler(QFile *file, Level_Offset *levelOffset);
    ~Room_ID_Handler();
    void Set_Room_Order_Writer(Room_Order_Writer *roomOrderWriter);
    void Set_Room_Address_Writer(Room_Address_Writer *roomAddressWriter);
    void Set_Enemy_Bytes_Tracker(Enemy_Bytes_Tracker *enemyBytesTracker);
    Level::Level Get_Current_Level();
    int Get_Current_Level_Num();
    int Get_Current_World_Num();
    void Set_Current_Level(Level::Level level);
    void Set_Current_Level_Num(int levelNum);
    void Set_Current_World_Num(int worldNum);

    bool Get_Room_ID_From_Level(Level::Level level, unsigned char &id);
    bool Get_Level_From_Room_ID(unsigned char id, Level::Level &level);
    QVector<unsigned char> *Get_Midpoint_Indexes_From_Room_ID(unsigned char id);
    QVector<unsigned char> *Get_Midpoint_Indexes_From_Current_Level();
    QVector<unsigned char> *Get_Midpoint_Indexes_From_Level(Level::Level level);
    Level_Attribute::Level_Attribute Get_Level_Attribute_From_Current_Level();
    Level_Attribute::Level_Attribute Get_Level_Attribute_From_Level(Level::Level level);
    Level_Attribute::Level_Attribute Get_Level_Attribute_From_ID(unsigned char id);
    bool Change_Current_Level_Attribute(Level_Attribute::Level_Attribute attribute);
    bool Change_Level_Attribute(Level::Level level, Level_Attribute::Level_Attribute attribute);
    bool Change_Room_Attribute(unsigned char oldRoomID, Level_Attribute::Level_Attribute attribute);
    unsigned char Get_Value_From_Attribute(Level_Attribute::Level_Attribute attribute);
    bool Send_Object_Bytes_From_One_Level_To_Another(Level::Level fromLevel, Level::Level toLevel, int numBytes);
    bool Send_Enemy_Bytes_From_One_Level_To_Another(Level::Level fromLevel, Level::Level toLevel, int numBytes);

private:
    void Update_Room_IDs(unsigned char oldRoomNum, unsigned char newRoomNum, unsigned char oldAttribute, unsigned char newAttribute);
    void Update_Room_ID(Level::Level level, unsigned char oldRoomNum, unsigned char newRoomNum, unsigned char oldAttribute, unsigned char newAttribute);
    bool Update_Pipe_Pointers(const QMap<unsigned char, Level::Level> &oldRoomIDs);
    bool Update_Pipe_Pointers_At_Level(const QMap<unsigned char, Level::Level> &oldRoomIDs, Level::Level level);
    void Populate_Room_IDs();
    bool Send_Bytes_From_One_Offset_To_Another(qint64 fromOffset, qint64 toOffset, int numBytes, bool enemies);
    bool Read_First_Byte(qint64 offset, unsigned char &c);
    bool Fix_Level_Addresses(qint64 fromOffset, qint64 toOffset, int numBytes);

    QFile *file;
    Level::Level currentLevel;
    int worldNum;
    int levelNum;
    Level_Offset *levelOffset;
    Enemy_Bytes_Tracker *enemyBytesTracker;
    QMap<Level::Level, unsigned char> *roomIDs;
    QMap<unsigned char, QVector<unsigned char>*> *midpointIndexes;
    Room_Order_Writer *roomOrderWriter;
    Room_Address_Writer *roomAddressWriter;

    friend class Room_Order_Writer;
};

#endif // ROOM_ID_HANDLER_H
