#include "Required_Enemy_Spawns.h"
#include "Object_Writer.h"
#include "Enemy_Writer.h"
#include <assert.h>

Required_Enemy_Spawns::Required_Enemy_Spawns(Object_Writer *object, Enemy_Writer *enemy) {
    assert(object);
    assert(enemy);
    this->enemy = enemy;
    this->numRequiredBytes = 0;
    this->requiredEnemies = new QQueue<Required_Enemy_Spawn>();
}

Required_Enemy_Spawns::~Required_Enemy_Spawns() {
    delete this->requiredEnemies;
}

bool Required_Enemy_Spawns::Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, int x, int y) {
    int previousNumRequiredBytes = this->numRequiredBytes;
    assert(Determine_Bytes_Required_For_Required_Enemy_Spawn(x));
    Required_Enemy_Spawn enemySpawn;
    enemySpawn.enemy = enemy;
    enemySpawn.x = this->object->Get_Level_Length() + x;
    enemySpawn.y = y;
    enemySpawn.numRequiredBytes = this->numRequiredBytes - previousNumRequiredBytes;
    assert(enemySpawn.numRequiredBytes == 2 || enemySpawn.numRequiredBytes == 4);
    this->requiredEnemies->append(enemySpawn);
    return true;
}

int Required_Enemy_Spawns::Get_Num_Required_Enemy_Spawns() {
    return this->requiredEnemies->size();
}

int Required_Enemy_Spawns::Get_Required_Bytes() {
    return this->numRequiredBytes;
}

Enemy_Item::Enemy_Item Required_Enemy_Spawns::Get_Enemy() {
    assert(!this->requiredEnemies->isEmpty());
    return this->requiredEnemies->first().enemy;
}

int Required_Enemy_Spawns::Get_X() {
    assert(!this->requiredEnemies->isEmpty());
    return this->requiredEnemies->first().x;
}

int Required_Enemy_Spawns::Get_Y() {
    assert(!this->requiredEnemies->isEmpty());
    return this->requiredEnemies->first().y;
}

bool Required_Enemy_Spawns::Mark_Enemy_As_Spawned() {
    if (this->requiredEnemies->isEmpty()) return false;
    this->numRequiredBytes -= this->requiredEnemies->front().numRequiredBytes;
    assert(this->numRequiredBytes >= 0);
    this->requiredEnemies->pop_front();
    return true;
}

bool Required_Enemy_Spawns::Determine_Bytes_Required_For_Required_Enemy_Spawn(int x) {
    int previousRequiredX = -1;
    if (!this->requiredEnemies->isEmpty()) previousRequiredX = this->requiredEnemies->last().x;
    int numRequiredBytes = this->numRequiredBytes;
    numRequiredBytes += 2;

    //A page change or another enemy will be necessary to make this spawn possible
    if (previousRequiredX != -1 && (x-previousRequiredX) > 0x10) {
        numRequiredBytes += 2;
    }

    if (numRequiredBytes > this->enemy->Get_Num_Bytes_Left()) return false;
    this->numRequiredBytes = numRequiredBytes;
    return true;
}



