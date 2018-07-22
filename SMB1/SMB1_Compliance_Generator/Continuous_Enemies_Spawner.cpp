#include "Continuous_Enemies_Spawner.h"
#include "../../Common_Files/Random.h"
#include "Object_Writer.h"
#include "Required_Enemy_Spawns.h"
#include <array>
#include <algorithm>
#include <assert.h>

Continuous_Enemies_Spawner::Continuous_Enemies_Spawner(SMB1_Compliance_Generator_Arguments *args, Object_Writer *object, Required_Enemy_Spawns *requiredEnemySpawns) {
    assert(args);
    assert(object);
    assert(requiredEnemySpawns);
    this->args = args;
    this->object = object;
    this->requiredEnemySpawns = requiredEnemySpawns;
}

Enemy_Item::Enemy_Item Continuous_Enemies_Spawner::Create_Continuous_Enemies_Spawner(int x) {
    //Pull these Values Based on the Level Type
    int flyingCheepCheeps = 0;
    int lakitus = 0;
    int offscreenBulletBills = 0;
    switch (this->args->levelType) {
    default: assert(false);
    case Level_Type::STANDARD_OVERWORLD:
        flyingCheepCheeps = this->args->difficultyStandardOverworldFlyingCheepCheeps;
        lakitus = this->args->difficultyStandardOverworldLakitus;
        offscreenBulletBills = this->args->difficultyStandardOverworldOffscreenBulletBills;
        break;
    case Level_Type::UNDERGROUND:
        flyingCheepCheeps = this->args->difficultyUndergroundFlyingCheepCheeps;
        lakitus = this->args->difficultyUndergroundLakitus;
        offscreenBulletBills = this->args->difficultyUndergroundOffscreenBulletBills;
        break;
    case Level_Type::UNDERWATER:
        flyingCheepCheeps = this->args->difficultyUnderwaterFlyingCheepCheeps;
        lakitus = this->args->difficultyUnderwaterLakitus;
        offscreenBulletBills = this->args->difficultyUnderwaterSwimmingCheepCheeps;
        break;
    case Level_Type::BRIDGE:
        flyingCheepCheeps = this->args->difficultyBridgeFlyingCheepCheeps;
        lakitus = this->args->difficultyBridgeLakitus;
        offscreenBulletBills = this->args->difficultyBridgeOffscreenBulletBills;
        break;
    case Level_Type::ISLAND:
        flyingCheepCheeps = this->args->difficultyIslandFlyingCheepCheeps;
        lakitus = this->args->difficultyIslandLakitus;
        offscreenBulletBills = this->args->difficultyIslandOffscreenBulletBills;
        break;
    case Level_Type::CASTLE:
        flyingCheepCheeps = this->args->difficultyCastleFlyingCheepCheeps;
        lakitus = this->args->difficultyCastleLakitus;
        offscreenBulletBills = this->args->difficultyCastleOffscreenBulletBills;
        break;
    }

    //Get the Specified Spawner by Priority
    std::array<unsigned int, 3> order = {0,1,2};
    switch (this->args->difficultySpawnerPriority) {
    case 0: //Random
        std::random_shuffle(order.begin(), order.end());
        break;
    case 1: //Lakitus > Flying Cheep-Cheeps > Bullet Bills
        order = {0,1,2};
        break;
    case 2: //Flying Cheep-Cheeps > Lakitus > Bullet Bills
        order = {1,0,2};
        break;
    }

    //Handle the Spawner After the Order Has Been Defined
    for (unsigned int i = 0; i < order.size(); ++i) {
        switch (order.at(i)) {
        default: assert(false);
        case 0:
            if (this->Try_To_Create_Continuous_Lakitus(x, lakitus) != Enemy_Item::NOTHING) return Enemy_Item::LAKITU;
            break;
        case 1:
            if (this->Try_To_Create_Continuous_Flying_Cheep_Cheeps(x, flyingCheepCheeps) != Enemy_Item::NOTHING) return Enemy_Item::CHEEP_CHEEP_SPAWNER;
            break;
        case 2:
            if (this->Try_To_Create_Continuous_Offscreen_Bullet_Bills(x, offscreenBulletBills) != Enemy_Item::NOTHING) return Enemy_Item::BULLET_BILL_SPAWNER;
            break;
        }
    }
    return Enemy_Item::NOTHING;
}

Enemy_Item::Enemy_Item Continuous_Enemies_Spawner::Try_To_Create_Continuous_Flying_Cheep_Cheeps(int x, int expectedDifficulty) {
    if (this->args->difficulty >= expectedDifficulty) {
        if (this->object->Get_Absolute_X(x) == 0xF) assert(this->object->Flying_Cheep_Cheep_Spawner(x+1));
        else assert(this->object->Flying_Cheep_Cheep_Spawner(x));
        return Enemy_Item::CHEEP_CHEEP_SPAWNER;
    }
    return Enemy_Item::NOTHING; //nothing was created
}

Enemy_Item::Enemy_Item Continuous_Enemies_Spawner::Try_To_Create_Continuous_Lakitus(int x, int expectedDifficulty) {
    if (!this->object->Is_Auto_Scroll_Active() && this->args->difficulty >= expectedDifficulty &&
            Random::Get_Num(99)+1 <= this->args->difficultyLakituSpawnChancePerLevel) {
        assert(this->requiredEnemySpawns->Add_Required_Enemy_Spawn(Enemy_Item::LAKITU, x, 0x0));
        return Enemy_Item::LAKITU;
    }
    return Enemy_Item::NOTHING; //nothing was created
}

Enemy_Item::Enemy_Item Continuous_Enemies_Spawner::Try_To_Create_Continuous_Offscreen_Bullet_Bills(int x, int expectedDifficulty) {
    if (this->args->difficulty >= expectedDifficulty) {
        if (this->args->levelType == Level_Type::UNDERWATER) {
            if (this->object->Get_Absolute_X(x) == 0xF) assert(this->object->Swimming_Cheep_Cheep_Spawner(x+1));
            else assert(this->object->Swimming_Cheep_Cheep_Spawner(x));
        } else {
            if (this->object->Get_Absolute_X(x) == 0xF) assert(this->object->Bullet_Bill_Spawner(x+1));
            else assert(this->object->Bullet_Bill_Spawner(x));
        }
        return Enemy_Item::BULLET_BILL_SPAWNER;
    }
    return Enemy_Item::NOTHING; //nothing was created
}
