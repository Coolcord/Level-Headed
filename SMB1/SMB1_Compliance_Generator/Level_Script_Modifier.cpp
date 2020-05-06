#include "Level_Script_Modifier.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "Enemy_Buffer.h"
#include "Object_Buffer.h"
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

bool Level_Script_Modifier::Redistribute_Enemies(Enemy_Buffer *enemyBuffer) {
    assert(enemyBuffer);
    return false; //TODO: WRITE THIS!!!
}

bool Level_Script_Modifier::Redistribute_Powerups(Object_Buffer *objectBuffer) {
    assert(objectBuffer);
    return false; //TODO: WRITE THIS!!!
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
