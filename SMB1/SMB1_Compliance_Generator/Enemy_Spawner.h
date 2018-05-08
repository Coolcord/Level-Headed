#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Level_Type.h"
#include "SMB1_Compliance_Generator_Arguments.h"
#include <QFile>
#include <QTextStream>

class Enemy_Writer;
class Level_Crawler;
class Required_Enemy_Spawns;

class Enemy_Spawner
{
public:
    Enemy_Spawner(QFile *file, QTextStream *stream, Enemy_Writer *enemies, Required_Enemy_Spawns *requiredEnemySpawns, SMB1_Compliance_Generator_Arguments *args);
    ~Enemy_Spawner();
    bool Spawn_Enemies(Brick::Brick startingBrick);

private:
    bool Handle_Required_Enemies(int &lastX);
    bool Handle_Required_Enemies_In_Emergency_Spawn_Mode(int &lastX);

    int Spawn_Standard_Overworld_Enemy(int &x, int &y, int lastX, int size);
    int Spawn_Underground_Enemy(int &x, int &y, int lastX, int size);
    int Spawn_Underwater_Enemy(int &x, int &y, int lastX, int size);
    int Spawn_Castle_Enemy(int &x, int &y, int lastX, int size);
    int Spawn_Bridge_Enemy(int &x, int &y, int lastX, int size);
    int Spawn_Island_Enemy(int &x, int &y, int lastX, int size);

    int Calculate_Number_Of_Enemies();
    int Calculate_Average_Distance(int x, int totalSpaces, int numEnemies);
    bool Spawn_Page_Change(int &x, int &y, int &lastX, int page, int enemyAmount);
    int Get_Random_X(int min);
    int Common_Enemy(int &x, int &y, int lastX, int lastSize);
    int Multi_Enemy(int &x, int &y, int lastX, int lastSize);

    QFile *file;
    QTextStream *stream;
    Enemy_Writer *enemies;
    Required_Enemy_Spawns *requiredEnemySpawns;
    Level_Crawler *levelCrawler;
    bool emergencySpawnMode;
    SMB1_Compliance_Generator_Arguments *args;
};

#endif // ENEMY_SPAWNER_H
