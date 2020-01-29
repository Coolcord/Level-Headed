#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Level_Type.h"
#include "SMB1_Compliance_Generator_Arguments.h"

class Enemy_Buffer;
class Object_Buffer;
class Level_Crawler;
class Required_Enemy_Spawns;

class Enemy_Spawner {
public:
    Enemy_Spawner(Object_Buffer *objects, Enemy_Buffer *enemies, Level_Crawler *levelCrawler, Required_Enemy_Spawns *requiredEnemySpawns, SMB1_Compliance_Generator_Arguments *args);
    ~Enemy_Spawner() {}
    bool Spawn_Enemies();

private:
    bool Handle_Required_Enemies(int &lastX);
    bool Handle_Required_Enemies_In_Emergency_Spawn_Mode(int &lastX);

    int Spawn_Standard_Overworld_Enemy(int &x, int &y, int lastX, int size, bool noEnemies);
    int Spawn_Underground_Enemy(int &x, int &y, int lastX, int size, bool noEnemies);
    int Spawn_Underwater_Enemy(int &x, int &y, int lastX, int size, bool noEnemies);
    int Spawn_Castle_Enemy(int &x, int &y, int lastX, int size, bool noEnemies);
    int Spawn_Bridge_Enemy(int &x, int &y, int lastX, int size, bool noEnemies);
    int Spawn_Island_Enemy(int &x, int &y, int lastX, int size, bool noEnemies);

    int Calculate_Number_Of_Enemies();
    int Calculate_Average_Distance(int x, int totalSpaces, int numEnemies);
    bool Spawn_Page_Change(int &x, int &y, int &lastX, int page, int enemyAmount);
    int Get_Random_X(int min);
    int Common_Enemy(int &x, int &y, int lastX, int lastSize, bool forceHammerBro, bool noEnemies);
    int Multi_Enemy(int &x, int &y, int lastX, int lastSize, bool noEnemies);

    bool Is_Coordinate_Safe(int x, int y);
    bool Find_Safe_Coordinate(int &x, int &y, int lastX);
    bool Find_Safe_Coordinate(int size, int &x, int &y, int lastX, bool reverse = false);
    bool Find_Safe_Coordinate_At_Y(int &x, int y, int lastX);
    bool Find_Safe_Coordinate_At_Y(int size, int &x, int y, int lastX, bool reverse = false);
    bool Find_Safe_Coordinate_At_X(int x, int &y);
    bool Find_Safe_Green_Leaping_Paratroopa_Coordinate(int &x, int &y, int lastX, bool reverse = false);
    bool Find_Safe_Green_Flying_Paratroopa_Coordinate(int &x, int &y, int lastX, bool reverse = false);
    bool Find_Safe_Red_Paratroopa_Coordinate(int &x, int &y, int lastX, bool reverse = false);
    bool Scan_For_Safe_Green_Flying_Paratroopa_Spawn(int x, int &y);

    Enemy_Buffer *enemies;
    Object_Buffer *objects;
    Required_Enemy_Spawns *requiredEnemySpawns;
    Level_Crawler *levelCrawler;
    bool emergencySpawnMode;
    SMB1_Compliance_Generator_Arguments *args;
};

#endif // ENEMY_SPAWNER_H
