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
            switch (Random::Get_Instance().Get_Num(underwaterValue)) {
            default:    assert(false); return false;
            case 0:     data->enemyItem = Enemy_Item::BLOOPER; break;
            case 1:     data->enemyItem = Enemy_Item::BULLET_BILL; break;
            case 2:     data->enemyItem = Enemy_Item::GREEN_PARATROOPA; data->moving = true; data->leaping = false; break;
            case 3:     data->enemyItem = Enemy_Item::RED_PARATROOPA; break;
            case 4:     data->enemyItem = Enemy_Item::PODOBOO; break;
            case 5:     data->enemyItem = Enemy_Item::HAMMER_BRO; break;
            }
            break;
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
            break;
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
            break;
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
        enemyBuffer->Seek_To_Next();
    }
    return true;
}

bool Level_Script_Modifier::Redistribute_Enemies(SMB1_Compliance_Generator_Arguments &args, SMB1_Compliance_Parser_Arguments &parserArgs) {
    //Get the required enemy spawns
    int numBytes = parserArgs.enemyBuffer->Get_Num_Bytes_Used();
    if (numBytes < 2) return true; //nothing to do
    Pipe_Pointer_Buffer pipePointerBuffer(parserArgs.objectBuffer, parserArgs.enemyBuffer);
    parserArgs.enemyBuffer->Set_Num_Bytes_Left(10000); //trick the enemy buffer into thinking it has more space so that the required enemy spawns don't fail
    Required_Enemy_Spawns requiredEnemySpawns(parserArgs.objectBuffer, parserArgs.enemyBuffer, &pipePointerBuffer, &args);
    parserArgs.enemyBuffer->Seek_To_First_Item();
    while (!parserArgs.enemyBuffer->At_End()) {
        Buffer_Data data = parserArgs.enemyBuffer->Get_Current();
        switch (data.enemyItem) {
        default:
            break;
        case Enemy_Item::FIRE_BAR:
        case Enemy_Item::LARGE_FIRE_BAR:
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
        parserArgs.enemyBuffer->Seek_To_Next();
    }

    //Empty out the enemy buffer
    parserArgs.enemyBuffer->Clear_Buffer();
    parserArgs.enemyBuffer->Set_Num_Bytes_Left(numBytes);

    //Redistribute the enemies
    Level_Crawler levelCrawler(parserArgs.objectBuffer);
    Enemy_Spawner enemySpawner(parserArgs.objectBuffer, parserArgs.enemyBuffer, &levelCrawler, &requiredEnemySpawns, &args);
    return enemySpawner.Spawn_Enemies();
}

bool Level_Script_Modifier::Redistribute_Powerups(SMB1_Compliance_Generator_Arguments &args, SMB1_Compliance_Parser_Arguments &parserArgs) {
    assert(parserArgs.objectBuffer);
    int numPowerups = 0, numHiddenPowerups = 0, numTenCoinBlocks = 0, numStars = 0, numOneUps = 0;

    //Remove all of the powerups from the level
    parserArgs.objectBuffer->Seek_To_First_Item();
    while (!parserArgs.objectBuffer->At_End()) {
        Buffer_Data *data = parserArgs.objectBuffer->Get_Current_For_Modification();
        switch (data->objectItem) {
        default:
            break;
        case Object_Item::QUESTION_BLOCK_WITH_MUSHROOM:
            data->objectItem = Object_Item::QUESTION_BLOCK_WITH_COIN;
            ++numPowerups;
            break;
        case Object_Item::BRICK_WITH_MUSHROOM:
            data->objectItem = Object_Item::HORIZONTAL_BRICKS;
            data->length = 1;
            ++numHiddenPowerups;
            break;
        case Object_Item::BRICK_WITH_STAR:
            data->objectItem = Object_Item::HORIZONTAL_BRICKS;
            data->length = 1;
            ++numStars;
            break;
        case Object_Item::BRICK_WITH_10_COINS:
            data->objectItem = Object_Item::HORIZONTAL_BRICKS;
            data->length = 1;
            ++numTenCoinBlocks;
            break;
        case Object_Item::BRICK_WITH_1UP:
            data->objectItem = Object_Item::HORIZONTAL_BRICKS;
            data->length = 1;
            ++numOneUps;
            break;
        }
        parserArgs.objectBuffer->Seek_To_Next();
    }
    parserArgs.objectBuffer->Set_Num_Bytes_Left(0);

    //Redistribute the powerups in the level
    Level_Crawler levelCrawler(parserArgs.objectBuffer);
    Powerup_Distributor powerupDistributor(&levelCrawler, parserArgs.objectBuffer, &args, false);
    powerupDistributor.Set_Num_Powerups(numPowerups);
    powerupDistributor.Set_Num_Hidden_Powerups(numHiddenPowerups);
    powerupDistributor.Set_Num_One_Ups(numOneUps);
    powerupDistributor.Set_Num_Stars(numStars);
    powerupDistributor.Set_Num_Ten_Coin_Blocks(numTenCoinBlocks);
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
