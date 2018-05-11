#ifndef CONTINUOUS_ENEMIES_SPAWNER_H
#define CONTINUOUS_ENEMIES_SPAWNER_H

#include "SMB1_Compliance_Generator_Arguments.h"

class Object_Writer;
class Required_Enemy_Spawns;

class Continuous_Enemies_Spawner {
public:
    Continuous_Enemies_Spawner(SMB1_Compliance_Generator_Arguments *args, Object_Writer *object, Required_Enemy_Spawns *requiredEnemySpawns);
    ~Continuous_Enemies_Spawner() {}
    bool Create_Continuous_Enemies_Spawner(int x);

private:
    bool Try_To_Create_Continuous_Flying_Cheep_Cheeps(int x, int expectedDifficulty);
    bool Try_To_Create_Continuous_Lakitus(int x, int expectedDifficulty);
    bool Try_To_Create_Continuous_Offscreen_Bullet_Bills(int x, int expectedDifficulty);

    SMB1_Compliance_Generator_Arguments *args;
    Object_Writer *object;
    Required_Enemy_Spawns *requiredEnemySpawns;
};

#endif // CONTINUOUS_ENEMIES_SPAWNER_H
