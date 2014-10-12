#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "../Common SMB1 Files/Brick.h"
#include "../Common SMB1 Files/Level_Type.h"
#include <QFile>
#include <QTextStream>

class Enemy_Writer;
class Level_Crawler;

class Enemy_Spawner
{
public:
    Enemy_Spawner(QFile *file, QTextStream *stream, Enemy_Writer *enemies);
    ~Enemy_Spawner();
    bool Spawn_Enemies(Brick::Brick startingBrick, Level_Type::Level_Type levelType);

private:
    int Spawn_Standard_Overworld_Enemy(int &x, int &y, int lastX, int size);
    int Spawn_Underground_Enemy(int &x, int &y, int lastX, int size);
    int Spawn_Underwater_Enemy(int &x, int &y, int lastX, int size);
    int Spawn_Castle_Enemy(int &x, int &y, int lastX, int size);
    int Spawn_Bridge_Enemy(int &x, int &y, int lastX, int size);
    int Spawn_Island_Enemy(int &x, int &y, int lastX, int size);

    int Get_Random_X(int min);
    int Common_Enemy(int &x, int &y, int lastX, int lastSize);
    int Multi_Enemy(int &x, int &y, int lastX, int lastSize);

    QFile *file;
    QTextStream *stream;
    Enemy_Writer *enemies;
    Level_Crawler *levelCrawler;
};

#endif // ENEMY_SPAWNER_H
