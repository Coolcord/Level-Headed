#ifndef ENEMY_BYTES_TRACKER_H
#define ENEMY_BYTES_TRACKER_H

#include "../Common_SMB1_Files/Level.h"
#include <QFile>
#include <QMap>

class Level_Offset;

class Enemy_Bytes_Tracker
{
public:
    Enemy_Bytes_Tracker(QFile *file, Level_Offset *levelOffset);
    ~Enemy_Bytes_Tracker();
    int Get_Enemy_Byte_Count_In_Level(Level::Level level);
    void Set_Enemy_Byte_Count_In_Level(Level::Level level, int numBytes);
    bool Calculate_Enemy_Bytes_In_All_Levels();

private:
    void Initialize_Enemy_Byte_Counts();
    bool Calculate_Enemy_Bytes_In_Level(Level::Level level);

    QFile *file;
    Level_Offset *levelOffset;
    QMap<Level::Level, int> *enemyByteCount;
};

#endif // ENEMY_BYTES_TRACKER_H
