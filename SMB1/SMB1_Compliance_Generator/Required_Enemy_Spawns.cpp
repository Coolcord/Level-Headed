#include "Required_Enemy_Spawns.h"
#include "Object_Writer.h"
#include "Enemy_Writer.h"
#include "Pipe_Pointer_Writer.h"
#include <assert.h>

Required_Enemy_Spawns::Required_Enemy_Spawns(Object_Writer *object, Enemy_Writer *enemy, Pipe_Pointer_Writer *pipePointer) {
    assert(object);
    assert(enemy);
    assert(pipePointer);
    this->object = object;
    this->enemy = enemy;
    this->pipePointer = pipePointer;
    this->numRequiredBytes = 0;
    this->requiredEnemies = new QQueue<Required_Enemy_Spawn>();
}

Required_Enemy_Spawns::~Required_Enemy_Spawns() {
    delete this->requiredEnemies;
}

bool Required_Enemy_Spawns::Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, int x) {
    return this->Add_Required_Enemy_Spawn(enemy, this->Get_Initialized_Extra_Enemy_Args(), x, 0);
}

bool Required_Enemy_Spawns::Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, int x, int y) {
    return this->Add_Required_Enemy_Spawn(enemy, this->Get_Initialized_Extra_Enemy_Args(), x, y);
}

bool Required_Enemy_Spawns::Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, Extra_Enemy_Args args, int x) {
    return this->Add_Required_Enemy_Spawn(enemy, args, x, 0);
}

bool Required_Enemy_Spawns::Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, Extra_Enemy_Args args, int x, int y) {
    int previousNumRequiredBytes = this->numRequiredBytes;
    assert(Determine_Bytes_Required_For_Required_Enemy_Spawn(enemy, x));
    Required_Enemy_Spawn enemySpawn;
    enemySpawn.enemy = enemy;
    enemySpawn.args = args;
    enemySpawn.x = this->object->Get_Level_Length() + x;
    enemySpawn.y = y;
    enemySpawn.numRequiredBytes = this->numRequiredBytes - previousNumRequiredBytes;
    assert(enemySpawn.numRequiredBytes >= 2 && enemySpawn.numRequiredBytes <= 5);
    this->requiredEnemies->append(enemySpawn);
    return true;
}

Extra_Enemy_Args Required_Enemy_Spawns::Get_Initialized_Extra_Enemy_Args() {
    Extra_Enemy_Args args;
    args.onlyHardMode = false;
    args.moving = true;
    args.leaping = true;
    args.clockwise = false;
    args.fast = false;
    args.small = false;
    args.vertical = false;
    args.up = true;
    args.room = 0;
    args.page = 0;
    args.num = 3;
    return args;
}

bool Required_Enemy_Spawns::Spawn_Required_Enemy(int &lastX) {
    if (this->requiredEnemies->isEmpty()) return false;
    int nextX = this->requiredEnemies->first().x;
    int y = this->requiredEnemies->first().y;
    int x = nextX - lastX;
    lastX += x;
    assert(x >= 0);
    assert(lastX == nextX);
    Enemy_Item::Enemy_Item enemy = this->requiredEnemies->first().enemy;
    Extra_Enemy_Args args = this->requiredEnemies->first().args;
    this->requiredEnemies->pop_front();
    switch (enemy) {
    case Enemy_Item::GREEN_KOOPA:           return this->enemy->Green_Koopa(x, y, args.moving, args.onlyHardMode);
    case Enemy_Item::RED_KOOPA:             return this->enemy->Red_Koopa(x, y, args.onlyHardMode);
    case Enemy_Item::BUZZY_BEETLE:          return this->enemy->Buzzy_Beetle(x, y, args.onlyHardMode);
    case Enemy_Item::HAMMER_BRO:            return this->enemy->Hammer_Bro(x, y, args.onlyHardMode);
    case Enemy_Item::GOOMBA:                return this->enemy->Goomba(x, y, args.onlyHardMode);
    case Enemy_Item::BLOOPER:               return this->enemy->Blooper(x, y, args.onlyHardMode);
    case Enemy_Item::BULLET_BILL:           return this->enemy->Bullet_Bill(x, y, args.onlyHardMode);
    case Enemy_Item::GREEN_PARATROOPA:      return this->enemy->Green_Paratroopa(x, y, args.moving, args.leaping, args.onlyHardMode);
    case Enemy_Item::RED_PARATROOPA:        return this->enemy->Red_Paratroopa(x, y, args.onlyHardMode);
    case Enemy_Item::GREEN_CHEEP_CHEEP:     return this->enemy->Green_Cheep_Cheep(x, y, args.onlyHardMode);
    case Enemy_Item::RED_CHEEP_CHEEP:       return this->enemy->Red_Cheep_Cheep(x, y, args.onlyHardMode);
    case Enemy_Item::PODOBOO:               return this->enemy->Podoboo(x, y, args.onlyHardMode);
    case Enemy_Item::PIRANA_PLANT:          return this->enemy->Pirana_Plant(x, y, args.onlyHardMode);
    case Enemy_Item::LAKITU:                return this->enemy->Lakitu(x, y, args.onlyHardMode);
    case Enemy_Item::SPINY:                 return this->enemy->Spiny(x, y, args.onlyHardMode);
    case Enemy_Item::BOWSER_FIRE_SPAWNER:   return this->enemy->Bowser_Fire_Spawner(x, args.onlyHardMode);
    case Enemy_Item::CHEEP_CHEEP_SPAWNER:   return this->enemy->Cheep_Cheep_Spawner(x, args.leaping, args.onlyHardMode);
    case Enemy_Item::BULLET_BILL_SPAWNER:   return this->enemy->Bullet_Bill_Spawner(x, args.onlyHardMode);
    case Enemy_Item::FIRE_BAR:              return this->enemy->Fire_Bar(x, y, args.clockwise, args.fast, args.onlyHardMode);
    case Enemy_Item::LARGE_FIRE_BAR:        return this->enemy->Large_Fire_Bar(x, y, args.onlyHardMode);
    case Enemy_Item::LIFT:                  return this->enemy->Lift(x, y, args.vertical, args.onlyHardMode);
    case Enemy_Item::FALLING_LIFT:          return this->enemy->Falling_Lift(x, y, args.onlyHardMode);
    case Enemy_Item::BALANCE_LIFT:          return this->enemy->Balance_Lift(x, y, args.onlyHardMode);
    case Enemy_Item::SURFING_LIFT:          return this->enemy->Surfing_Lift(x, y, args.onlyHardMode);
    case Enemy_Item::LIFT_SPAWNER:          return this->enemy->Lift_Spawner(x, y, args.up, args.small, args.onlyHardMode);
    case Enemy_Item::BOWSER:                return this->enemy->Bowser(x, args.onlyHardMode);
    case Enemy_Item::WARP_ZONE:             return this->enemy->Warp_Zone(x);
    case Enemy_Item::PIPE_POINTER:          return this->pipePointer->Pipe_Pointer(x, args.room, args.page);
    case Enemy_Item::TOAD:                  return this->enemy->Toad(x);
    case Enemy_Item::GOOMBA_GROUP:          return this->enemy->Goomba_Group(x, y, args.num, args.onlyHardMode);
    case Enemy_Item::KOOPA_GROUP:           return this->enemy->Koopa_Group(x, y, args.num, args.onlyHardMode);
    case Enemy_Item::PAGE_CHANGE:           assert(false); return false;
    case Enemy_Item::NOTHING:               return this->enemy->Nothing(x);
    default:                                assert(false); return false;
    }
}

bool Required_Enemy_Spawns::Is_In_Range_Of_Required_Enemy(int x) {
    if (this->requiredEnemies->isEmpty()) return false;
    assert(this->requiredEnemies->first().x >= x);
    return (this->requiredEnemies->first().x-x <= 0x10);
}

int Required_Enemy_Spawns::Get_Num_Required_Enemy_Spawns() {
    return this->requiredEnemies->size();
}

int Required_Enemy_Spawns::Get_Num_Required_Bytes() {
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

bool Required_Enemy_Spawns::Determine_Bytes_Required_For_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, int x) {
    int previousRequiredX = -1;
    if (!this->requiredEnemies->isEmpty()) previousRequiredX = this->requiredEnemies->last().x;
    int numRequiredBytes = this->numRequiredBytes;
    numRequiredBytes += 2;
    if (enemy == Enemy_Item::PIPE_POINTER) ++numRequiredBytes;

    //A page change or another enemy will be necessary to make this spawn possible
    if (previousRequiredX != -1 && (x-previousRequiredX) > 0x10) {
        numRequiredBytes += 2;
    }

    if (numRequiredBytes > this->enemy->Get_Num_Bytes_Left()) return false;
    this->numRequiredBytes = numRequiredBytes;
    return true;
}



