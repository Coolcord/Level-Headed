#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "../Common SMB1 Files/Brick.h"
#include <QFile>
#include <QTextStream>

class Enemy_Writer;
class Level_Crawler;

class Enemy_Spawner
{
public:
    Enemy_Spawner(QFile *file, QTextStream *stream, Enemy_Writer *enemies, int numEnemyBytes);
    ~Enemy_Spawner();
    bool Spawn_Enemies(Brick::Brick startingBrick);

private:
    int Get_Random_X(int min);
    int Common_Enemy(int &x, int &y, int lastX, int lastSize);

    QFile *file;
    QTextStream *stream;
    Enemy_Writer *enemies;
    Level_Crawler *levelCrawler;
    int bytesLeft;
};

#endif // ENEMY_SPAWNER_H
