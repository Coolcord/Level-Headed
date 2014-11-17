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
    bool disableCoordinateSafety = false;
    assert(Determine_Bytes_Required_For_Required_Enemy_Spawn(enemy, disableCoordinateSafety, x));
    if (this->numRequiredBytes > this->enemy->Get_Num_Bytes_Left()) {
        this->numRequiredBytes = previousNumRequiredBytes;
        return false;
    }
    Required_Enemy_Spawn enemySpawn;
    enemySpawn.enemy = enemy;
    enemySpawn.args = args;
    enemySpawn.x = this->object->Get_Level_Length() + x;
    enemySpawn.y = y;
    enemySpawn.numRequiredBytes = this->numRequiredBytes - previousNumRequiredBytes;
    enemySpawn.disableCoordinateSafety = disableCoordinateSafety;
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
    int numRequiredBytes = this->requiredEnemies->first().numRequiredBytes;
    assert(this->requiredEnemies->front().numRequiredBytes <= this->enemy->Get_Num_Bytes_Left());
    bool disableCoordinateSafety = this->requiredEnemies->front().disableCoordinateSafety;
    lastX += x;
    assert(x >= 0);
    assert(lastX == nextX);
    Enemy_Item::Enemy_Item enemy = this->requiredEnemies->first().enemy;
    Extra_Enemy_Args args = this->requiredEnemies->first().args;

    //Spawn a page change if necessary
    if (!this->requiredEnemies->first().disableCoordinateSafety && !this->Is_In_Range_Of_Required_Enemy(x)) {
        int page = nextX/16;
        assert(this->enemy->Page_Change(page));
        lastX = page*16;
        x = nextX - lastX;
    }

    this->enemy->Set_Coordinate_Safety(!disableCoordinateSafety);
    this->requiredEnemies->pop_front();
    bool success = false;
    switch (enemy) {
    case Enemy_Item::GREEN_KOOPA:           success = this->enemy->Green_Koopa(x, y, args.moving, args.onlyHardMode); break;
    case Enemy_Item::RED_KOOPA:             success = this->enemy->Red_Koopa(x, y, args.onlyHardMode); break;
    case Enemy_Item::BUZZY_BEETLE:          success = this->enemy->Buzzy_Beetle(x, y, args.onlyHardMode); break;
    case Enemy_Item::HAMMER_BRO:            success = this->enemy->Hammer_Bro(x, y, args.onlyHardMode); break;
    case Enemy_Item::GOOMBA:                success = this->enemy->Goomba(x, y, args.onlyHardMode); break;
    case Enemy_Item::BLOOPER:               success = this->enemy->Blooper(x, y, args.onlyHardMode); break;
    case Enemy_Item::BULLET_BILL:           success = this->enemy->Bullet_Bill(x, y, args.onlyHardMode); break;
    case Enemy_Item::GREEN_PARATROOPA:      success = this->enemy->Green_Paratroopa(x, y, args.moving, args.leaping, args.onlyHardMode); break;
    case Enemy_Item::RED_PARATROOPA:        success = this->enemy->Red_Paratroopa(x, y, args.onlyHardMode); break;
    case Enemy_Item::GREEN_CHEEP_CHEEP:     success = this->enemy->Green_Cheep_Cheep(x, y, args.onlyHardMode); break;
    case Enemy_Item::RED_CHEEP_CHEEP:       success = this->enemy->Red_Cheep_Cheep(x, y, args.onlyHardMode); break;
    case Enemy_Item::PODOBOO:               success = this->enemy->Podoboo(x, y, args.onlyHardMode); break;
    case Enemy_Item::PIRANA_PLANT:          success = this->enemy->Pirana_Plant(x, y, args.onlyHardMode); break;
    case Enemy_Item::LAKITU:                success = this->enemy->Lakitu(x, y, args.onlyHardMode); break;
    case Enemy_Item::SPINY:                 success = this->enemy->Spiny(x, y, args.onlyHardMode); break;
    case Enemy_Item::BOWSER_FIRE_SPAWNER:   success = this->enemy->Bowser_Fire_Spawner(x, args.onlyHardMode); break;
    case Enemy_Item::CHEEP_CHEEP_SPAWNER:   success = this->enemy->Cheep_Cheep_Spawner(x, args.leaping, args.onlyHardMode); break;
    case Enemy_Item::BULLET_BILL_SPAWNER:   success = this->enemy->Bullet_Bill_Spawner(x, args.onlyHardMode); break;
    case Enemy_Item::FIRE_BAR:              success = this->enemy->Fire_Bar(x, y, args.clockwise, args.fast, args.onlyHardMode); break;
    case Enemy_Item::LARGE_FIRE_BAR:        success = this->enemy->Large_Fire_Bar(x, y, args.onlyHardMode); break;
    case Enemy_Item::LIFT:                  success = this->enemy->Lift(x, y, args.vertical, args.onlyHardMode); break;
    case Enemy_Item::FALLING_LIFT:          success = this->enemy->Falling_Lift(x, y, args.onlyHardMode); break;
    case Enemy_Item::BALANCE_LIFT:          success = this->enemy->Balance_Lift(x, y, args.onlyHardMode); break;
    case Enemy_Item::SURFING_LIFT:          success = this->enemy->Surfing_Lift(x, y, args.onlyHardMode); break;
    case Enemy_Item::LIFT_SPAWNER:          success = this->enemy->Lift_Spawner(x, y, args.up, args.small, args.onlyHardMode); break;
    case Enemy_Item::BOWSER:                success = this->enemy->Bowser(x, args.onlyHardMode); break;
    case Enemy_Item::WARP_ZONE:             success = this->enemy->Warp_Zone(x); break;
    case Enemy_Item::PIPE_POINTER:          success = this->pipePointer->Pipe_Pointer(x, args.room, args.page); break;
    case Enemy_Item::TOAD:                  success = this->enemy->Toad(x); break;
    case Enemy_Item::GOOMBA_GROUP:          success = this->enemy->Goomba_Group(x, y, args.num, args.onlyHardMode); break;
    case Enemy_Item::KOOPA_GROUP:           success = this->enemy->Koopa_Group(x, y, args.num, args.onlyHardMode); break;
    case Enemy_Item::PAGE_CHANGE:           assert(false); break; return false;
    case Enemy_Item::NOTHING:               success = this->enemy->Nothing(x); break;
    default:                                assert(false); break; return false;
    }
    if (success) this->numRequiredBytes -= numRequiredBytes;
    this->enemy->Set_Coordinate_Safety(true);
    return success;
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

bool Required_Enemy_Spawns::Determine_Bytes_Required_For_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, bool &disableSafety, int x) {
    int previousRequiredX = 0;
    if (!this->requiredEnemies->isEmpty()) previousRequiredX = this->requiredEnemies->last().x;
    disableSafety = false;
    int numRequiredBytes = this->numRequiredBytes;
    numRequiredBytes += 2;
    if (enemy == Enemy_Item::PIPE_POINTER) ++numRequiredBytes;

    //Check to see if a page change is required
    int previousAbsoluteX = previousRequiredX%0x10;
    int amountUntilNextPage = 0x10 - previousAbsoluteX;
    assert(amountUntilNextPage <= 0x10);
    if (0xF+amountUntilNextPage > x) {
        numRequiredBytes += 2;
    } else {
        disableSafety = true; //it's technically possible to spawn the enemy without a page change, so do so to conserve bytes
    }

    if (numRequiredBytes > this->enemy->Get_Num_Bytes_Left()) return false;
    this->numRequiredBytes = numRequiredBytes;
    return true;
}



