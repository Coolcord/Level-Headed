#ifndef CONTINUOUS_ENEMIES_SPAWNER_H
#define CONTINUOUS_ENEMIES_SPAWNER_H

#include "../Common_SMB1_Files/Enemy_Item.h"
#include "SMB1_Compliance_Generator_Arguments.h"

class Object_Buffer;
class Required_Enemy_Spawns;

class Continuous_Enemies_Spawner {
public:
    Continuous_Enemies_Spawner(SMB1_Compliance_Generator_Arguments *args, Object_Buffer *object, Required_Enemy_Spawns *requiredEnemySpawns);
    ~Continuous_Enemies_Spawner() {}
    Enemy_Item::Enemy_Item Create_Continuous_Enemies_Spawner(int x);
    Enemy_Item::Enemy_Item Create_Midpoint_Continuous_Enemies_Spawner(int x);

private:
    Enemy_Item::Enemy_Item Determine_Continuous_Enemies_Spawner(int x);
    Enemy_Item::Enemy_Item Try_To_Create_Continuous_Flying_Cheep_Cheeps(int x, int expectedDifficulty);
    Enemy_Item::Enemy_Item Try_To_Create_Continuous_Lakitus(int x, int expectedDifficulty);
    Enemy_Item::Enemy_Item Try_To_Create_Continuous_Offscreen_Bullet_Bills(int x, int expectedDifficulty);

    SMB1_Compliance_Generator_Arguments *args;
    Object_Buffer *object;
    Required_Enemy_Spawns *requiredEnemySpawns;
    Enemy_Item::Enemy_Item lastSpawn;
};

#endif // CONTINUOUS_ENEMIES_SPAWNER_H
