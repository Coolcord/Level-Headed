#include "Required_Enemy_Spawns.h"
#include "Object_Buffer.h"
#include "Enemy_Buffer.h"
#include "Pipe_Pointer_Buffer.h"
#include <QDebug>
#include <assert.h>

Required_Enemy_Spawns::Required_Enemy_Spawns(Object_Buffer *object, Enemy_Buffer *enemies, Pipe_Pointer_Buffer *pipePointer, SMB1_Compliance_Generator_Arguments *args) {
    assert(object);
    assert(enemies);
    assert(pipePointer);
    assert(args);
    this->objects = object;
    this->enemies = enemies;
    this->pipePointer = pipePointer;
    this->args = args;
    this->numRequiredBytes = 0;
    this->numEndBytes = 0;
    this->warpZoneX = -1;
    this->requiredEnemies = new QQueue<Required_Enemy_Spawn>();
}

Required_Enemy_Spawns::~Required_Enemy_Spawns() {
    delete this->requiredEnemies;
}

bool Required_Enemy_Spawns::Add_Required_Enemy_Spawn(const Buffer_Data &data) {
    Extra_Enemy_Args extraEnemyArgs = this->Get_Initialized_Extra_Enemy_Args();
    extraEnemyArgs.onlyHardMode = data.onlyHardMode; extraEnemyArgs.moving = data.moving; extraEnemyArgs.leaping = data.leaping;
    extraEnemyArgs.clockwise = data.clockwise; extraEnemyArgs.fast = data.fast; extraEnemyArgs.small = data.small;
    extraEnemyArgs.vertical = data.vertical; extraEnemyArgs.up = data.up; extraEnemyArgs.level = data.level;
    extraEnemyArgs.world = data.world; extraEnemyArgs.page = data.page; extraEnemyArgs.num = data.num;
    return this->Add_Required_Enemy_Spawn_At_Aboslute_X(data.enemyItem, extraEnemyArgs, data.absoluteX, data.y);
}

bool Required_Enemy_Spawns::Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, int x) {
    return this->Add_Required_Enemy_Spawn_At_Aboslute_X(enemy, this->Get_Initialized_Extra_Enemy_Args(), this->objects->Get_Level_Length()+x, 0);
}

bool Required_Enemy_Spawns::Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, int x, int y) {
    return this->Add_Required_Enemy_Spawn_At_Aboslute_X(enemy, this->Get_Initialized_Extra_Enemy_Args(), this->objects->Get_Level_Length()+x, y);
}

bool Required_Enemy_Spawns::Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, Extra_Enemy_Args args, int x) {
    return this->Add_Required_Enemy_Spawn_At_Aboslute_X(enemy, args, this->objects->Get_Level_Length()+x, 0);
}

bool Required_Enemy_Spawns::Add_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, Extra_Enemy_Args args, int x, int y) {
    return this->Add_Required_Enemy_Spawn_At_Aboslute_X(enemy, args, this->objects->Get_Level_Length()+x, y);
}

bool Required_Enemy_Spawns::Is_Safe_To_Add_Required_Enemy_Spawn(int x) {
    int tmpNumRequiredBytes = this->numRequiredBytes;
    bool disableSafety = false;
    if (this->Determine_Bytes_Required_For_Required_Enemy_Spawn(Enemy_Item::GOOMBA, disableSafety, this->objects->Get_Level_Length()+x)) {
        this->numRequiredBytes = tmpNumRequiredBytes;
        return true;
    } else {
        return false;
    }
}

int Required_Enemy_Spawns::Get_Num_Bytes_Left() {
    return this->enemies->Get_Num_Bytes_Left()-(this->numEndBytes+this->numRequiredBytes);
}

int Required_Enemy_Spawns::Get_Num_End_Bytes() {
    return this->numEndBytes;
}

bool Required_Enemy_Spawns::Set_Num_End_Bytes(int value) {
    if (this->numRequiredBytes+value > this->enemies->Get_Num_Bytes_Left()) return false;
    this->numEndBytes = value;
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
    args.level = Level::WORLD_1_LEVEL_1;
    args.world = 0;
    args.page = 0;
    args.num = 3;
    args.allowSpawnAfterCancelSpawner = true;
    return args;
}

bool Required_Enemy_Spawns::Spawn_Required_Enemy(int &lastX) {
    if (this->requiredEnemies->isEmpty()) return false;
    bool firstEnemy = this->enemies->Get_First_Enemy();
    this->enemies->Set_First_Enemy(false);
    int nextX = this->requiredEnemies->first().x;
    int y = this->requiredEnemies->first().y;

    //Ignore enemies that spawn after a cancel spawner upon request
    Extra_Enemy_Args args = this->requiredEnemies->first().args;
    if (!args.allowSpawnAfterCancelSpawner) {
        int cancelSpawnerX = this->objects->Get_Cancel_Spawner_X();
        if (cancelSpawnerX != -1 && nextX >= cancelSpawnerX) {
            this->requiredEnemies->pop_front();
            this->numRequiredBytes -= numRequiredBytes;
            return true; //ignore this enemy with no error as requested
        }
    }

    //Handle the x values
    int previousX = this->enemies->Get_Level_Length();
    assert(previousX <= nextX);
    int x = nextX - previousX;
    assert(x >= 0);

    int numRequiredBytes = this->requiredEnemies->first().numRequiredBytes;
    assert(this->requiredEnemies->front().numRequiredBytes <= this->enemies->Get_Num_Bytes_Left());
    bool disableCoordinateSafety = this->requiredEnemies->front().disableCoordinateSafety;
    assert(x >= 0);
    Enemy_Item::Enemy_Item enemy = this->requiredEnemies->first().enemy;

    //Check to see if a page change is required
    int previousAbsoluteX = previousX%0x10;
    int amountUntilNextPage = 0x10 - previousAbsoluteX;
    assert(amountUntilNextPage <= 0x10);

    if (x > 0x10 && x <= 0xF+amountUntilNextPage) {
        disableCoordinateSafety = true; //the coordinate safety should get disabled
    } else if (x > 0x10) { //spawn the page change
        assert(numRequiredBytes > 3);
        int page = nextX/16;
        assert(this->enemies->Page_Change(page));
        previousX = page*16;
        x = nextX - previousX;
        assert(x < 0x10);
    }

    this->enemies->Set_Coordinate_Safety(!disableCoordinateSafety);
    this->requiredEnemies->pop_front();
    bool success = false;
    switch (enemy) {
    case Enemy_Item::GREEN_KOOPA:           success = this->enemies->Green_Koopa(x, y, args.moving, args.onlyHardMode); break;
    case Enemy_Item::RED_KOOPA:             success = this->enemies->Red_Koopa(x, y, args.onlyHardMode); break;
    case Enemy_Item::BUZZY_BEETLE:          success = this->enemies->Buzzy_Beetle(x, y, args.onlyHardMode); break;
    case Enemy_Item::HAMMER_BRO:            success = this->enemies->Hammer_Bro(x, y, args.onlyHardMode); break;
    case Enemy_Item::GOOMBA:                success = this->enemies->Goomba(x, y, args.onlyHardMode); break;
    case Enemy_Item::BLOOPER:               success = this->enemies->Blooper(x, y, args.onlyHardMode); break;
    case Enemy_Item::BULLET_BILL:           success = this->enemies->Bullet_Bill(x, y, args.onlyHardMode); break;
    case Enemy_Item::GREEN_PARATROOPA:      success = this->enemies->Green_Paratroopa(x, y, args.moving, args.leaping, args.onlyHardMode); break;
    case Enemy_Item::RED_PARATROOPA:        success = this->enemies->Red_Paratroopa(x, y, args.onlyHardMode); break;
    case Enemy_Item::GREEN_CHEEP_CHEEP:     success = this->enemies->Green_Cheep_Cheep(x, y, args.onlyHardMode); break;
    case Enemy_Item::RED_CHEEP_CHEEP:       success = this->enemies->Red_Cheep_Cheep(x, y, args.onlyHardMode); break;
    case Enemy_Item::PODOBOO:               success = this->enemies->Podoboo(x, args.onlyHardMode); break;
    case Enemy_Item::PIRANHA_PLANT:         success = this->enemies->Piranha_Plant(x, y, args.onlyHardMode); break;
    case Enemy_Item::LAKITU:                success = this->enemies->Lakitu(x, y, args.onlyHardMode); break;
    case Enemy_Item::SPINY:                 success = this->enemies->Spiny(x, y, args.onlyHardMode); break;
    case Enemy_Item::BOWSER_FIRE_SPAWNER:   success = this->enemies->Bowser_Fire_Spawner(x, args.onlyHardMode); break;
    case Enemy_Item::CHEEP_CHEEP_SPAWNER:   success = this->enemies->Swimming_Cheep_Cheep_Spawner(x, args.leaping, args.onlyHardMode); break;
    case Enemy_Item::BULLET_BILL_SPAWNER:   success = this->enemies->Bullet_Bill_Spawner(x, args.onlyHardMode); break;
    case Enemy_Item::FIRE_BAR:              success = this->enemies->Fire_Bar(x, y, args.clockwise, args.fast, args.onlyHardMode); break;
    case Enemy_Item::LARGE_FIRE_BAR:        success = this->enemies->Large_Fire_Bar(x, y, args.onlyHardMode); break;
    case Enemy_Item::LIFT:                  success = this->enemies->Lift(x, y, args.vertical, args.onlyHardMode); break;
    case Enemy_Item::FALLING_LIFT:          success = this->enemies->Falling_Lift(x, y, args.onlyHardMode); break;
    case Enemy_Item::BALANCE_LIFT:          success = this->enemies->Balance_Lift(x, y, args.onlyHardMode); break;
    case Enemy_Item::SURFING_LIFT:          success = this->enemies->Surfing_Lift(x, y, args.onlyHardMode); break;
    case Enemy_Item::LIFT_SPAWNER:          success = this->enemies->Lift_Spawner(x, y, args.up, args.small, args.onlyHardMode); break;
    case Enemy_Item::BOWSER:                success = this->enemies->Bowser(x, args.onlyHardMode); break;
    case Enemy_Item::WARP_ZONE:             success = this->enemies->Warp_Zone(x); break;
    case Enemy_Item::PIPE_POINTER:          success = this->pipePointer->Pipe_Pointer(x, args.level, args.world, args.page); break;
    case Enemy_Item::TOAD:                  success = this->enemies->Toad(x); break;
    case Enemy_Item::GOOMBA_GROUP:          success = this->enemies->Goomba_Group(x, y, args.num, args.onlyHardMode); break;
    case Enemy_Item::KOOPA_GROUP:           success = this->enemies->Koopa_Group(x, y, args.num, args.onlyHardMode); break;
    case Enemy_Item::PAGE_CHANGE:           assert(false); return false;
    default:                                assert(false); return false;
    }
    if (success) {
        this->numRequiredBytes -= numRequiredBytes;
        lastX = nextX;
    } else {
        this->enemies->Set_First_Enemy(firstEnemy);
    }
    this->enemies->Set_Coordinate_Safety(true);
    return success;
}

bool Required_Enemy_Spawns::Is_In_Range_Of_Required_Enemy(int x) {
    if (this->requiredEnemies->isEmpty()) return false;
    if (this->requiredEnemies->first().x < x) {
        return false;
    }
    return (this->requiredEnemies->first().x-x <= 0x10);
}

int Required_Enemy_Spawns::Get_Num_Required_Enemy_Spawns() {
    return this->requiredEnemies->size();
}

int Required_Enemy_Spawns::Get_Num_Required_Bytes() {
    return this->numRequiredBytes+this->numEndBytes;
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

bool Required_Enemy_Spawns::Was_Warp_Zone_Used(int &x) {
    if (this->warpZoneX != -1) {
        x = this->warpZoneX;
        return true;
    } else {
        return false;
    }
}

bool Required_Enemy_Spawns::Add_Required_Enemy_Spawn_At_Aboslute_X(Enemy_Item::Enemy_Item enemy, Extra_Enemy_Args args, int absoluteX, int y) {
    //Skip the Enemy if No Enemies is Enabled
    bool skipEnemy = this->args->difficultyNoEnemies;
    switch (enemy) {
    case Enemy_Item::FIRE_BAR:
    case Enemy_Item::LARGE_FIRE_BAR:
        if (this->args->difficulty < this->args->difficultyCastleFireBars) skipEnemy = true;
        break;
    case Enemy_Item::BALANCE_LIFT:
    case Enemy_Item::FALLING_LIFT:
    case Enemy_Item::LIFT:
    case Enemy_Item::LIFT_SPAWNER:
    case Enemy_Item::SURFING_LIFT:
    case Enemy_Item::PIPE_POINTER:
    case Enemy_Item::TOAD:
        skipEnemy = false;
        break;
    case Enemy_Item::WARP_ZONE:
        this->warpZoneX = absoluteX;
        skipEnemy = false;
        break;
    default:
        break;
    }
    if (skipEnemy) return true;

    int previousNumRequiredBytes = this->numRequiredBytes;
    bool disableCoordinateSafety = false;
    assert(Determine_Bytes_Required_For_Required_Enemy_Spawn(enemy, disableCoordinateSafety, absoluteX));
    if (this->numRequiredBytes+this->numEndBytes > this->enemies->Get_Num_Bytes_Left()) {
        this->numRequiredBytes = previousNumRequiredBytes;
        return false;
    }

    Required_Enemy_Spawn enemySpawn;
    enemySpawn.enemy = enemy;
    enemySpawn.args = args;
    enemySpawn.x = absoluteX;
    enemySpawn.y = y;
    enemySpawn.numRequiredBytes = this->numRequiredBytes - previousNumRequiredBytes;
    enemySpawn.disableCoordinateSafety = disableCoordinateSafety;
    assert(enemySpawn.numRequiredBytes >= 2 && enemySpawn.numRequiredBytes <= 5);
    this->requiredEnemies->append(enemySpawn);
    return true;
}

bool Required_Enemy_Spawns::Determine_Bytes_Required_For_Required_Enemy_Spawn(Enemy_Item::Enemy_Item enemy, bool &disableSafety, int absoluteX) {
    int previousRequiredX = 0;
    if (!this->requiredEnemies->isEmpty()) previousRequiredX = this->requiredEnemies->last().x;
    int distance = absoluteX - previousRequiredX;
    assert(distance >= 0);
    disableSafety = false;
    int tmpNumRequiredBytes = this->numRequiredBytes;
    tmpNumRequiredBytes += 2;
    if (enemy == Enemy_Item::PIPE_POINTER) ++tmpNumRequiredBytes;

    //Check to see if a page change is required
    int previousAbsoluteX = previousRequiredX%0x10;
    int amountUntilNextPage = 0x10 - previousAbsoluteX;
    assert(amountUntilNextPage <= 0x10);

    if (distance > 0x10 && distance <= 0xF+amountUntilNextPage) {
        disableSafety = true; //it's technically possible to spawn the enemy without a page change, so do so to conserve bytes
    } else if (distance > 0x10) {
        tmpNumRequiredBytes += 2; //a page change or another enemy will be necessary to reach this point
    }

    int numBytes = tmpNumRequiredBytes - this->numRequiredBytes;
    assert(numBytes > 0);
    if (this->numRequiredBytes+this->numEndBytes+numBytes > this->enemies->Get_Num_Bytes_Left()) return false;
    this->numRequiredBytes = tmpNumRequiredBytes;
    return true;
}
