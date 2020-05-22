#include "Level_Script_Modifier.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "Enemy_Spawner.h"
#include "Level_Crawler.h"
#include "Pipe_Pointer_Buffer.h"
#include "Powerup_Distributor.h"
#include "Required_Enemy_Spawns.h"
#include <assert.h>

bool Level_Script_Modifier::Perform_Enemy_Chaotic_Swap(Enemy_Buffer *enemyBuffer, Level_Attribute::Level_Attribute levelAttribute, bool allowHammerBros, bool allowLakitus, bool allowContinousEnemySpawners) {
    assert(enemyBuffer);
    enemyBuffer->Seek_To_First_Item();
    int underwaterValue = 4;
    if (allowHammerBros) ++underwaterValue;
    while (!enemyBuffer->At_End()) {
        Buffer_Data *data = enemyBuffer->Get_Current_For_Modification();
        assert(data);
        switch (data->enemyItem) {
        case Enemy_Item::GREEN_KOOPA:
        case Enemy_Item::RED_KOOPA:
        case Enemy_Item::BUZZY_BEETLE:
        case Enemy_Item::GOOMBA:
        case Enemy_Item::SPINY:
        case Enemy_Item::PIRANHA_PLANT:
            this->Random_Enemy(data, levelAttribute == Level_Attribute::UNDERWATER, allowHammerBros, allowLakitus, allowContinousEnemySpawners);
            break;
        case Enemy_Item::BLOOPER:
            this->Reset_Enemy_Settings(data);
            data->y = Random::Get_Instance().Get_Num(1, 10);
            switch (underwaterValue) {
            default:    assert(false); return false;
            case 0:     data->enemyItem = Enemy_Item::BLOOPER; break;
            case 1:     data->enemyItem = Enemy_Item::BULLET_BILL; break;
            case 2:     data->enemyItem = Enemy_Item::GREEN_PARATROOPA; data->moving = true; data->leaping = false; break;
            case 3:     data->enemyItem = Enemy_Item::RED_PARATROOPA; break;
            case 4:     data->enemyItem = Enemy_Item::PODOBOO; break;
            case 5:     data->enemyItem = Enemy_Item::HAMMER_BRO; break;
            }
            assert(false); return false;
        case Enemy_Item::BULLET_BILL:
        case Enemy_Item::GREEN_PARATROOPA:
        case Enemy_Item::RED_PARATROOPA:
        case Enemy_Item::GREEN_CHEEP_CHEEP:
        case Enemy_Item::RED_CHEEP_CHEEP:
        case Enemy_Item::PODOBOO:
            this->Reset_Enemy_Settings(data);
            data->y = Random::Get_Instance().Get_Num(1, 10);
            switch (Random::Get_Instance().Get_Num(4)) {
            default:    assert(false); return false;
            case 0:     data->enemyItem = Enemy_Item::BLOOPER; break;
            case 1:     data->enemyItem = Enemy_Item::BULLET_BILL; break;
            case 2:     data->enemyItem = Enemy_Item::GREEN_PARATROOPA; data->moving = true; data->leaping = false; break;
            case 3:     data->enemyItem = Enemy_Item::RED_PARATROOPA; break;
            case 4:     data->enemyItem = Enemy_Item::PODOBOO; break;
            }
            assert(false); return false;
        case Enemy_Item::CHEEP_CHEEP_SPAWNER:
        case Enemy_Item::BULLET_BILL_SPAWNER:
            this->Reset_Enemy_Settings(data);
            if (levelAttribute == Level_Attribute::UNDERWATER) { //Swimming Cheep-Cheeps
                data->enemyItem = Enemy_Item::CHEEP_CHEEP_SPAWNER;
                data->leaping = false;
            } else {
                if (Random::Get_Instance().Get_Num(1)) {
                    data->enemyItem = Enemy_Item::BULLET_BILL_SPAWNER;
                } else { //Flying Cheep-Cheeps
                    data->enemyItem = Enemy_Item::CHEEP_CHEEP_SPAWNER;
                    data->leaping = true;
                }
            }
            break;
        case Enemy_Item::FIRE_BAR:
        case Enemy_Item::LARGE_FIRE_BAR:
            switch (Random::Get_Instance().Get_Num(4)) {
            default:    assert(false); return false;
            case 0:     data->enemyItem = Enemy_Item::FIRE_BAR; data->clockwise = false; data->fast = false; break;
            case 1:     data->enemyItem = Enemy_Item::FIRE_BAR; data->clockwise = false; data->fast = true; break;
            case 2:     data->enemyItem = Enemy_Item::FIRE_BAR; data->clockwise = true; data->fast = false; break;
            case 3:     data->enemyItem = Enemy_Item::FIRE_BAR; data->clockwise = true; data->fast = true; break;
            case 4:     data->enemyItem = Enemy_Item::LARGE_FIRE_BAR; data->clockwise = true; data->fast = false; break;
            }
            assert(false); return false;
        case Enemy_Item::GOOMBA_GROUP:
        case Enemy_Item::KOOPA_GROUP:
            if (Random::Get_Instance().Get_Num(1)) data->enemyItem = Enemy_Item::GOOMBA_GROUP;
            else data->enemyItem = Enemy_Item::KOOPA_GROUP;
            break;
        case Enemy_Item::HAMMER_BRO:
        case Enemy_Item::LAKITU:
        case Enemy_Item::LIFT:
        case Enemy_Item::FALLING_LIFT:
        case Enemy_Item::BALANCE_LIFT:
        case Enemy_Item::SURFING_LIFT:
        case Enemy_Item::LIFT_SPAWNER:
        case Enemy_Item::BOWSER:
        case Enemy_Item::BOWSER_FIRE_SPAWNER:
        case Enemy_Item::WARP_ZONE:
        case Enemy_Item::PIPE_POINTER:
        case Enemy_Item::TOAD:
        case Enemy_Item::PAGE_CHANGE:
        case Enemy_Item::NOTHING:
            break; //do nothing to these enemies
        }
    }
    return true;
}

bool Level_Script_Modifier::Redistribute_Enemies(SMB1_Compliance_Generator_Arguments &args, SMB1_Compliance_Parser_Arguments &parserArgs) {
    //Get the required enemy spawns
    Pipe_Pointer_Buffer pipePointerBuffer(parserArgs.objectBuffer, parserArgs.enemyBuffer);
    Required_Enemy_Spawns requiredEnemySpawns(parserArgs.objectBuffer, parserArgs.enemyBuffer, &pipePointerBuffer, &args);
    int numBytes = parserArgs.enemyBuffer->Get_Num_Items()*2; //we don't know how many bytes there actually are available, so just approximate based upon the number of used items
    parserArgs.enemyBuffer->Set_Num_Bytes_Left_And_Total_Bytes(10000); //trick the enemy buffer into thinking it has more space so that the required enemy spawns don't fail
    parserArgs.enemyBuffer->Seek_To_First_Item();
    while (!parserArgs.enemyBuffer->At_End()) {
        Buffer_Data data = parserArgs.enemyBuffer->Get_Current();
        switch (data.enemyItem) {
        default:
            break;
        case Enemy_Item::PODOBOO:
        case Enemy_Item::CHEEP_CHEEP_SPAWNER:
        case Enemy_Item::BULLET_BILL_SPAWNER:
        case Enemy_Item::HAMMER_BRO:
        case Enemy_Item::LAKITU:
        case Enemy_Item::LIFT:
        case Enemy_Item::FALLING_LIFT:
        case Enemy_Item::BALANCE_LIFT:
        case Enemy_Item::SURFING_LIFT:
        case Enemy_Item::LIFT_SPAWNER:
        case Enemy_Item::BOWSER:
        case Enemy_Item::BOWSER_FIRE_SPAWNER:
        case Enemy_Item::WARP_ZONE:
        case Enemy_Item::PIPE_POINTER:
        case Enemy_Item::TOAD:
            assert(requiredEnemySpawns.Add_Required_Enemy_Spawn(data));
            break;
        }
    }

    //Empty out the enemy buffer
    parserArgs.enemyBuffer->Clear_Buffer();
    parserArgs.enemyBuffer->Set_Num_Bytes_Left_And_Total_Bytes(numBytes);

    //Redistribute the enemies
    Level_Crawler levelCrawler(parserArgs.objectBuffer);
    Enemy_Spawner enemySpawner(parserArgs.objectBuffer, parserArgs.enemyBuffer, &levelCrawler, &requiredEnemySpawns, &args);
    return enemySpawner.Spawn_Enemies();
}

bool Level_Script_Modifier::Redistribute_Powerups(SMB1_Compliance_Generator_Arguments &args, SMB1_Compliance_Parser_Arguments &parserArgs) {
    //Rebuild the level without powerups
    assert(parserArgs.objectBuffer);
    Object_Buffer *objectBuffer = new Object_Buffer(parserArgs.objectBuffer->Get_Num_Items()*2, &args);
    parserArgs.objectBuffer->Seek_To_First_Item();
    while (!parserArgs.objectBuffer->At_End()) {
        Buffer_Data data = parserArgs.objectBuffer->Get_Current();
        int x = data.x;
        switch (data.objectItem) {
        case Object_Item::QUESTION_BLOCK_WITH_MUSHROOM:
        case Object_Item::QUESTION_BLOCK_WITH_COIN:
        case Object_Item::HIDDEN_BLOCK_WITH_COIN:
        case Object_Item::HIDDEN_BLOCK_WITH_1UP:
        case Object_Item::BRICK_WITH_MUSHROOM:
        case Object_Item::BRICK_WITH_VINE:
        case Object_Item::BRICK_WITH_STAR:
        case Object_Item::BRICK_WITH_10_COINS:
        case Object_Item::BRICK_WITH_1UP:
        case Object_Item::UNDERWATER_SIDEWAYS_PIPE:
        case Object_Item::USED_BLOCK:
        case Object_Item::TRAMPOLINE:
        case Object_Item::BULLET_BILL_TURRET:
        case Object_Item::ISLAND:
        case Object_Item::HORIZONTAL_BRICKS:
        case Object_Item::HORIZONTAL_BLOCKS:
        case Object_Item::HORIZONTAL_COINS:
        case Object_Item::VERTICAL_BRICKS:
        case Object_Item::VERTICAL_BLOCKS:
        case Object_Item::CORAL:
        case Object_Item::PIPE:
        case Object_Item::ENTERABLE_PIPE:
        case Object_Item::HOLE:
        case Object_Item::HOLE_WITH_WATER:
        case Object_Item::BRIDGE:
        case Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS:
        case Object_Item::PAGE_CHANGE:
        case Object_Item::REVERSE_L_PIPE:
        case Object_Item::FLAGPOLE:
        case Object_Item::CASTLE:
        case Object_Item::BIG_CASTLE:
        case Object_Item::AXE:
        case Object_Item::AXE_ROPE:
        case Object_Item::BOWSER_BRIDGE:
        case Object_Item::SCROLL_STOP:
        case Object_Item::SCROLL_STOP_WARP_ZONE:
        case Object_Item::TOGGLE_AUTO_SCROLL:
            //TODO: WRITE THIS!!!
        case Object_Item::FLYING_CHEEP_CHEEP_SPAWNER:
            objectBuffer->Flying_Cheep_Cheep_Spawner(x);
            break;
        case Object_Item::SWIMMING_CHEEP_CHEEP_SPAWNER:
            objectBuffer->Swimming_Cheep_Cheep_Spawner(x);
            break;
        case Object_Item::BULLET_BILL_SPAWNER:
            objectBuffer->Bullet_Bill_Spawner(x);
            break;
        case Object_Item::CANCEL_SPAWNER:
            objectBuffer->Cancel_Spawner(x);
            break;
        case Object_Item::LOOP_COMMAND:
            objectBuffer->Loop_Command(x);
            break;
        case Object_Item::CHANGE_BRICK_AND_SCENERY:
            objectBuffer->Change_Brick_And_Scenery(x, data.brick, data.scenery);
            break;
        case Object_Item::CHANGE_BACKGROUND:
            objectBuffer->Change_Background(x, data.background);
            break;
        case Object_Item::LIFT_ROPE:
            objectBuffer->Lift_Rope(x);
            break;
        case Object_Item::BALANCE_LIFT_VERTICAL_ROPE:
            objectBuffer->Balance_Lift_Vertical_Rope(x, data.length);
            break;
        case Object_Item::BALANCE_LIFT_HORIZONTAL_ROPE:
            objectBuffer->Balance_Lift_Horizontal_Rope(x, data.length);
            break;
        case Object_Item::STEPS:
            objectBuffer->Steps(x, data.length);
            break;
        case Object_Item::END_STEPS:
            objectBuffer->End_Steps(x);
            break;
        case Object_Item::TALL_REVERSE_L_PIPE:
            objectBuffer->Tall_Reverse_L_Pipe_Without_Pointer(x, data.y);
            break;
        case Object_Item::PIPE_WALL:
            objectBuffer->Pipe_Wall(x);
            break;
        case Object_Item::NOTHING:
            objectBuffer->Nothing();
            break;
        }
    }

    //Redistribute the powerups in the level
    Level_Crawler levelCrawler(parserArgs.objectBuffer);
    Powerup_Distributor powerupDistributor(&levelCrawler, parserArgs.objectBuffer, &args);
    return powerupDistributor.Distribute_Powerups();
}

bool Level_Script_Modifier::Random_Enemy(Buffer_Data *data, bool underwater, bool allowHammerBros, bool allowLakitus, bool allowContinousEnemySpawners) {
    //Possibly use Lakitus
    this->Reset_Enemy_Settings(data);
    if (allowLakitus && Random::Get_Instance().Get_Num(10) == 0) {
        data->enemyItem = Enemy_Item::LAKITU;
        data->y = Random::Get_Instance().Get_Num(1, 10); //random air y
        return true;
    }

    //Possibly use Continuous Enemy Spawners
    if (allowContinousEnemySpawners && !underwater && Random::Get_Instance().Get_Num(10) == 0) {
        if (underwater) { //Swimming Cheep-Cheeps
            data->enemyItem = Enemy_Item::CHEEP_CHEEP_SPAWNER;
            data->leaping = false;
            return true;
        } else {
            if (Random::Get_Instance().Get_Num(1)) {
                data->enemyItem = Enemy_Item::BULLET_BILL_SPAWNER;
                return true;
            } else { //Flying Cheep-Cheeps
                data->enemyItem = Enemy_Item::CHEEP_CHEEP_SPAWNER;
                data->leaping = true;
                return true;
            }
        }
    }

    //Get a Random Enemy
    int maxValue = 9;
    if (allowHammerBros) ++maxValue;
    int value = Random::Get_Instance().Get_Num(maxValue);
    switch (value) {
    default:    assert(false); return false;
    case 0:     data->enemyItem = Enemy_Item::GREEN_KOOPA; data->moving = true; break;
    case 1:     data->enemyItem = Enemy_Item::RED_KOOPA; break;
    case 2:     data->enemyItem = Enemy_Item::BUZZY_BEETLE; break;
    case 3:     data->enemyItem = Enemy_Item::GOOMBA; break;
    case 4:     data->enemyItem = Enemy_Item::BLOOPER; data->y = Random::Get_Instance().Get_Num(1, 10); break;
    case 5:     data->enemyItem = Enemy_Item::BULLET_BILL; data->y = Random::Get_Instance().Get_Num(1, 10); break;
    case 6:     data->enemyItem = Enemy_Item::GREEN_PARATROOPA; data->leaping = true; data->moving = true; break;
    case 7:     data->enemyItem = Enemy_Item::GREEN_PARATROOPA; data->leaping = false; data->moving = true; data->y = Random::Get_Instance().Get_Num(1, 10); break;
    case 8:     data->enemyItem = Enemy_Item::RED_PARATROOPA; break;
    case 9:     data->enemyItem = Enemy_Item::PODOBOO; break;
    case 10:    data->enemyItem = Enemy_Item::HAMMER_BRO; break;
    }
    return true;
}

void Level_Script_Modifier::Reset_Enemy_Settings(Buffer_Data *data) {
    data->leaping = false; data->onlyHardMode = false; data->moving = false; data->leaping = false; data->clockwise = false;
    data->fast = false; data->small = false; data->vertical = false; data->up = false;
}
