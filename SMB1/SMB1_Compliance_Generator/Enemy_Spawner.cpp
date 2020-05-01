#include "Enemy_Spawner.h"
#include "Enemy_Buffer.h"
#include "Object_Buffer.h"
#include "Required_Enemy_Spawns.h"
#include "Level_Crawler.h"
#include "Physics.h"
#include "Difficulty.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "../../Level-Headed/Common_Strings.h"
#include <QTime>
#include <QDebug>
#include <assert.h>

Enemy_Spawner::Enemy_Spawner(Object_Buffer *objects, Enemy_Buffer *enemies, Level_Crawler *levelCrawler,
                             Required_Enemy_Spawns *requiredEnemySpawns, SMB1_Compliance_Generator_Arguments *args) {
    assert(objects); assert(enemies); assert(requiredEnemySpawns); assert(levelCrawler);
    assert(args); assert(args->difficulty >= Difficulty::DIFFICULTY_MIN && args->difficulty <= Difficulty::DIFFICULTY_MAX);
    this->objects = objects;
    this->enemies = enemies;
    this->requiredEnemySpawns = requiredEnemySpawns;
    this->args = args;
    this->levelCrawler = levelCrawler;
    this->emergencySpawnMode = false;
}

bool Enemy_Spawner::Spawn_Enemies() {
    if (!this->levelCrawler->Crawl_Level()) return false;
    int x = 16;
    int lastX = x;
    int y = 0;

    int totalSpaces = this->levelCrawler->Get_Safe_Size();
    int numPages = totalSpaces/16;
    bool usePages = false;
    int section = 0; //used with pages to determine what group of enemies are being operated on

    //Determine the max number of enemies available
    int numEnemies = this->Calculate_Number_Of_Enemies();

    //Determine where to place the page changes
    int firstPageChange = (numPages/4)+1;
    int secondPageChange = (numPages/2)+1;
    int thirdPageChange = static_cast<int>(((static_cast<double>(numPages)/4.0)*3.0))+1;
    int firstEnemyGroup = static_cast<int>(((static_cast<double>(numEnemies)/4.0)*3.0))+1;
    int secondEnemyGroup = (numEnemies/2)+1;
    int thirdEnemyGroup = (numEnemies/4)+1;

    //Determine the average distance between each enemy
    int averageDistance = this->Calculate_Average_Distance(x, totalSpaces, numEnemies);
    if (averageDistance >= 16) usePages = true;

    int size = 1;
    x += (averageDistance/2);
    while (this->requiredEnemySpawns->Get_Num_Bytes_Left() > 1 && x < this->levelCrawler->Get_Safe_Size()) {
        if (this->Handle_Required_Enemies(lastX)) {
            x = lastX;
            x += averageDistance;
            continue;
        }

        //Recalculate the average distance each round
        averageDistance = this->Calculate_Average_Distance(x, totalSpaces, this->Calculate_Number_Of_Enemies());

        //Determine what type of enemies to spawn
        bool forceHammerBro = false;
        if (this->args->levelType == Level_Type::UNDERWATER) forceHammerBro = !this->objects->Was_Auto_Scroll_Used() && this->args->difficulty >= this->args->difficultyUnderwaterHammerBros && Random::Get_Instance().Get_Num(99) <= this->args->difficultyHammerTimeIntensity-1;
        else forceHammerBro = !this->objects->Was_Auto_Scroll_Used() && this->args->difficulty >= this->args->difficultyHammerTime && Random::Get_Instance().Get_Num(99) <= this->args->difficultyHammerTimeIntensity-1;
        if (this->args->difficultyPreventHammerTimeFromSpawningTwoHammerBrosInARow && this->enemies->Was_Last_Enemy_A_Hammer_Bro()) forceHammerBro = false;
        bool noEnemies = this->args->difficultyNoEnemies;
        if (!noEnemies && this->args->difficultyDisableAllOtherEnemiesWhenALakituSpawns) noEnemies = this->enemies->Is_Lakitu_Active();
        if (!noEnemies && this->args->difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn) noEnemies = this->objects->Were_Flying_Cheep_Cheeps_Spawned();
        if (forceHammerBro) {
            size = this->Common_Enemy(x, y, lastX, size, true, noEnemies);
        } else {
            switch (this->args->levelType) {
            case Level_Type::STANDARD_OVERWORLD:
                size = this->Spawn_Standard_Overworld_Enemy(x, y, lastX, size, noEnemies);
                break;
            case Level_Type::UNDERGROUND:
                size = this->Spawn_Underground_Enemy(x, y, lastX, size, noEnemies);
                break;
            case Level_Type::UNDERWATER:
                size = this->Spawn_Underwater_Enemy(x, y, lastX, size, noEnemies);
                break;
            case Level_Type::CASTLE:
                size = this->Spawn_Castle_Enemy(x, y, lastX, size, noEnemies);
                break;
            case Level_Type::BRIDGE:
                size = this->Spawn_Bridge_Enemy(x, y, lastX, size, noEnemies);
                break;
            case Level_Type::ISLAND:
                size = this->Spawn_Island_Enemy(x, y, lastX, size, noEnemies);
                break;
            default:
                assert(false);
            }
        }

        if (size != 0) {
            lastX = x;
        } else { //an enemy failed to spawn. Add a page change 2 pages ahead and try from there
            if (!noEnemies) {
                assert(this->Spawn_Page_Change(x, y, lastX, this->enemies->Get_Current_Page()+2, this->requiredEnemySpawns->Get_Num_Bytes_Left()));
            }
        }

        //Spawn a page change if necessary
        if (usePages && this->requiredEnemySpawns->Get_Num_Bytes_Left() >= 4) {
            switch (section) {
            case 0:
                if (this->Spawn_Page_Change(x, y, lastX, firstPageChange, firstEnemyGroup)) ++section;
                break;
            case 1:
                if (this->Spawn_Page_Change(x, y, lastX, secondPageChange, secondEnemyGroup)) ++section;
                break;
            case 2:
                if (this->Spawn_Page_Change(x, y, lastX, thirdPageChange, thirdEnemyGroup)) ++section;
                break;
            default:
                break;
            }
        }

        //Increment X
        x += averageDistance;
    }

    //Spawn the rest of the required spawns if there are any
    if (this->requiredEnemySpawns->Get_Num_Required_Enemy_Spawns() > 0) {
        this->emergencySpawnMode = true;
        this->Handle_Required_Enemies_In_Emergency_Spawn_Mode(lastX);
    }
    return true;
}

bool Enemy_Spawner::Handle_Required_Enemies(int &lastX) {
    if (this->requiredEnemySpawns->Get_Num_Required_Enemy_Spawns() == 0) return false; //nothing to do
    assert(this->enemies->Get_Num_Bytes_Left() >= this->requiredEnemySpawns->Get_Num_Required_Bytes());
    if (this->enemies->Get_Num_Bytes_Left() <= this->requiredEnemySpawns->Get_Num_Required_Bytes()+1) this->emergencySpawnMode = true;
    if (this->emergencySpawnMode) return this->Handle_Required_Enemies_In_Emergency_Spawn_Mode(lastX);
    bool enemySpawned = false;
    while (this->requiredEnemySpawns->Is_In_Range_Of_Required_Enemy(lastX)) {
        enemySpawned = this->requiredEnemySpawns->Spawn_Required_Enemy(lastX);
        assert(enemySpawned);
    }
    return enemySpawned;
}

bool Enemy_Spawner::Handle_Required_Enemies_In_Emergency_Spawn_Mode(int &lastX) {
    assert(this->emergencySpawnMode);
    while (this->requiredEnemySpawns->Get_Num_Required_Enemy_Spawns() > 0) {
        assert(this->requiredEnemySpawns->Spawn_Required_Enemy(lastX));
    }
    assert(this->requiredEnemySpawns->Get_Num_Required_Bytes() == 0);
    return true;
}

bool Enemy_Spawner::Spawn_Page_Change(int &x, int &y, int &lastX, int page, int enemyAmount) {
    //Skip the page change if necessary
    if (this->enemies->Get_Current_Page() >= page) {
        return true;
    }

    //Don't forget to spawn the required enemies
    while (this->requiredEnemySpawns->Get_Num_Required_Enemy_Spawns() > 0 && page*16 > this->requiredEnemySpawns->Get_X()) {
        //Spawn a page change if necessary
        if (!this->requiredEnemySpawns->Is_In_Range_Of_Required_Enemy(lastX)) {
            int tmpPage = this->requiredEnemySpawns->Get_X()/16;
            assert(this->enemies->Page_Change(tmpPage));
            x = (tmpPage*16);
            lastX = x;
        }
        this->requiredEnemySpawns->Spawn_Required_Enemy(lastX);
    }

    //Check to see if the page change can be skipped now
    if (this->enemies->Get_Current_Page() >= page) return true;

    //Spawn the page change if necessary
    if ((this->requiredEnemySpawns->Get_Num_Bytes_Left()/2) <= enemyAmount) {
        assert(this->enemies->Page_Change(page));
        x = (page*16);
        lastX = x;
        y = Physics::GROUND_Y;
        return true;
    }

    //Cannot move on to the next section yet
    return false;
}

void Enemy_Spawner::Spawn_Red_Koopa_If_Possible(int x, int y, int lastX) {
    //Make sure that the enemy is being spawned on a 3 block platform
    int absoluteX = lastX+x;
    if (this->levelCrawler->Is_Coordinate_A_Platform(absoluteX-1, y+1)) { //check if a platform exists to the left
        if (this->levelCrawler->Is_Coordinate_A_Platform(absoluteX+1, y+1)) { assert(this->enemies->Red_Koopa(x, y)); return; }
        if (this->levelCrawler->Is_Coordinate_A_Platform(absoluteX-2, y+1)) { assert(this->enemies->Red_Koopa(x, y)); return; }
        assert(this->enemies->Green_Koopa(x, y)); return;
    } else { //no platform to the left
        if (this->levelCrawler->Is_Coordinate_A_Platform(absoluteX+1, y+1) && this->levelCrawler->Is_Coordinate_A_Platform(absoluteX+2, y+1)) {
            assert(this->enemies->Red_Koopa(x, y)); return;
        } else {
            assert(this->enemies->Green_Koopa(x, y)); return;
        }
    }
}

int Enemy_Spawner::Spawn_Standard_Overworld_Enemy(int &x, int &y, int lastX, int size, bool noEnemies) {
    switch (Random::Get_Instance().Get_Num(3)) {
    case 0:
    case 1:
    case 2:
        return this->Common_Enemy(x, y, lastX, size, false, noEnemies);
    case 3:
        return this->Multi_Enemy(x, y, lastX, size, noEnemies);
    default:
        assert(false);
        return 0;
    }
}

int Enemy_Spawner::Spawn_Underground_Enemy(int &x, int &y, int lastX, int size, bool noEnemies) {
    switch (Random::Get_Instance().Get_Num(3)) {
    case 0:
    case 1:
    case 2:
        return this->Common_Enemy(x, y, lastX, size, false, noEnemies);
    case 3:
        return this->Multi_Enemy(x, y, lastX, size, noEnemies);
    default:
        assert(false);
        return 0;
    }
}

int Enemy_Spawner::Spawn_Underwater_Enemy(int &x, int &y, int lastX, int size, bool noEnemies) {
    //This should only be Bloopers. Cheep-cheeps will be handled via a spawner
    return this->Common_Enemy(x, y, lastX, size, false, noEnemies);
}

int Enemy_Spawner::Spawn_Castle_Enemy(int &x, int &y, int lastX, int size, bool noEnemies) {
    switch (Random::Get_Instance().Get_Num(3)) {
    case 0:
    case 1:
    case 2:
        return this->Common_Enemy(x, y, lastX, size, false, noEnemies);
    case 3:
        return this->Multi_Enemy(x, y, lastX, size, noEnemies);
    default:
        assert(false);
        return 0;
    }
}

int Enemy_Spawner::Spawn_Bridge_Enemy(int &x, int &y, int lastX, int size, bool noEnemies) {
    //TODO: Add other paratroopa types
    switch (Random::Get_Instance().Get_Num(3)) {
    case 0:
    case 1:
    case 2:
        return this->Common_Enemy(x, y, lastX, size, false, noEnemies);
    case 3:
        return this->Multi_Enemy(x, y, lastX, size, noEnemies);
    default:
        assert(false);
        return 0;
    }
}

int Enemy_Spawner::Spawn_Island_Enemy(int &x, int &y, int lastX, int size, bool noEnemies) {
    //TODO: Add other paratroopa types
    switch (Random::Get_Instance().Get_Num(3)) {
    case 0:
    case 1:
    case 2:
        return this->Common_Enemy(x, y, lastX, size, false, noEnemies);
    case 3:
        return this->Multi_Enemy(x, y, lastX, size, noEnemies);
    default:
        assert(false);
        return 0;
    }
}

int Enemy_Spawner::Calculate_Number_Of_Enemies() {
    int numEnemies = 0;
    if (this->requiredEnemySpawns->Get_Num_Bytes_Left() % 2 == 1) { //bytesLeft is odd
        numEnemies = (this->requiredEnemySpawns->Get_Num_Bytes_Left()-1)/2;
    } else {
        numEnemies = this->requiredEnemySpawns->Get_Num_Bytes_Left()/2;
    }
    return numEnemies;
}

int Enemy_Spawner::Calculate_Average_Distance(int x, int totalSpaces, int numEnemies) {
    int averageDistance = 4;
    if (totalSpaces-x < 0) return this->args->difficultyMinimumEnemyDistance; //level is too short to use all enemies
    if (numEnemies > 0) averageDistance = (totalSpaces-x)/numEnemies;
    if (averageDistance > 11) averageDistance = 11;
    int minimumDistance = this->args->difficultyMinimumEnemyDistance;
    if (this->args->levelType == Level_Type::UNDERWATER) minimumDistance = this->args->difficultyMinimumUnderwaterEnemyDistance;
    if (averageDistance < minimumDistance) averageDistance = minimumDistance;
    return averageDistance;
}

int Enemy_Spawner::Get_Random_X(int min) {
    int random = Random::Get_Instance().Get_Num(0x10);
    random += min;
    if (random > 0x10) random = 0x10;
    return random;
}

int Enemy_Spawner::Multi_Enemy(int &x, int &y, int lastX, int lastSize, bool noEnemies) {
    int tmpX = x;
    if (tmpX-lastX <= 0) return 0;
    if (noEnemies) return this->Common_Enemy(x, y, lastX, lastSize, false, true); //give up and spawn a common enemy instead

    //Determine how many enemies to spawn
    int numEnemies = 0;
    switch (Random::Get_Instance().Get_Num(1)) {
    case 0: numEnemies = 2; break;
    case 1: numEnemies = 3; break;
    default: assert(false);
    }

    //Determine the y to place the enemies at
    int tmpY = y;
    switch (Random::Get_Instance().Get_Num(1)) {
    case 0: tmpY = 0x6; break;
    case 1: tmpY = 0xA; break;
    default: assert(false);
    }

    //Try to find a place to spawn the enemies
    if (!this->Find_Safe_Coordinate_At_Y(numEnemies+1, tmpX, tmpY, lastX)) {
        //Try the other y coordinate
        if (tmpY == 0x6) tmpY = 0xA;
        else tmpY = 0x6;
        assert(tmpX == x);
        if (!this->Find_Safe_Coordinate_At_Y(numEnemies+1, tmpX, tmpY, lastX)) {
            //Try one more round...
            tmpX = lastX+lastSize;
            if (lastSize == 0) ++tmpX;
            numEnemies = 2;
            if (!this->Find_Safe_Coordinate_At_Y(numEnemies+1, tmpX, tmpY, lastX, true)) {
                //Try the other y coordinate
                if (tmpY == 0x6) tmpY = 0xA;
                else tmpY = 0x6;
                if (!this->Find_Safe_Coordinate_At_Y(numEnemies+1, tmpX, tmpY, lastX, true)) {
                    return this->Common_Enemy(x, y, lastX, lastSize, false, noEnemies); //give up and spawn a common enemy instead
                }
            }
        }
    }

    //Determine what type of enemies to spawn
    assert(tmpX > lastX);
    int spawnX = tmpX-lastX;
    assert(spawnX <= 16);
    assert(numEnemies == 2 || numEnemies == 3);
    switch (Random::Get_Instance().Get_Num(1)) {
    case 0: //Goombas
        assert(this->enemies->Goomba_Group(spawnX, tmpY, numEnemies));
        break;
    case 1: //Koopas
        assert(this->enemies->Koopa_Group(spawnX, tmpY, numEnemies));
        break;
    default:
        assert(false);
    }
    x = tmpX;
    y = tmpY;
    return numEnemies+1; //return the size the enemies take up
}

bool Enemy_Spawner::Is_Coordinate_Safe(int x, int y, int lastX) {
    //The coordinate's x cannot be less than the minimum distance
    if (x-lastX < this->args->difficultyMinimumEnemyDistance) return false;

    //Make sure the coordinate itself is empty
    if (this->levelCrawler->Is_Coordinate_Empty(x, y)) {
        //Ground is expected to be below the enemy
        if (this->levelCrawler->Is_Coordinate_Solid(x, y+1)) return true;
    }
    return false;
}

int Enemy_Spawner::Common_Enemy(int &x, int &y, int lastX, int lastSize, bool forceHammerBro, bool noEnemies) {
    int tmpX = x;
    if (tmpX-lastX <= 0) return 0; //unable to spawn anything
    int tmpY = y;

    //Try to spawn a Blooper
    if (this->args->levelType == Level_Type::UNDERWATER) {
        bool canSpawnBlooper = this->args->difficulty >= this->args->difficultyUnderwaterBloopers;
        if (!canSpawnBlooper) noEnemies = true;
        if (!noEnemies && !forceHammerBro && canSpawnBlooper) {
            int spawnX = x - lastX;
            y = Random::Get_Instance().Get_Num(9)+1;
            assert(this->enemies->Blooper(spawnX, y));
            return 1;
        }
    }

    //Try to spawn a Green Paratroopa
    if (!noEnemies && !forceHammerBro && Random::Get_Instance().Get_Num(4) == 0) {
        bool spawnParatroopa = false;
        if (!this->Find_Safe_Green_Leaping_Paratroopa_Coordinate(tmpX, tmpY, lastX)) {
            //Try again, but start closer to the last enemy
            tmpX = lastX+lastSize;
            if (lastSize == 0) ++tmpX;
            if (this->Find_Safe_Green_Leaping_Paratroopa_Coordinate(tmpX, tmpY, lastX, true)) {
                spawnParatroopa = true;
            }
        } else {
            spawnParatroopa = true;
        }
        if (spawnParatroopa) {
            int spawnX = tmpX-lastX;
            assert(this->enemies->Green_Paratroopa(spawnX, tmpY));
            x = tmpX;
            y = tmpY;
            return 1;
        }
    }

    //Try a normal enemy
    if (!this->Find_Safe_Coordinate(tmpX, tmpY, lastX)) {
        //Try again, but start closer to the last enemy
        tmpX = lastX+lastSize;
        if (lastSize == 0) ++tmpX;
        if (!this->Find_Safe_Coordinate(1, tmpX, tmpY, lastX, true)) {
            return 0;
        }
    }
    assert(tmpX > lastX);
    int spawnX = tmpX-lastX;
    assert(spawnX >= this->args->difficultyMinimumEnemyDistance);
    int random = 0;
    //Spawn Hammer Bros. in later levels
    if (noEnemies) {
        return 0;
    } else if (forceHammerBro) {
        assert(this->enemies->Hammer_Bro(spawnX, tmpY));
    } else {
        switch (this->args->levelType) {
        case Level_Type::STANDARD_OVERWORLD:
            random = Random::Get_Instance().Get_Num(8);
            if (random < 3) {
                if (this->args->difficulty >= this->args->difficultyBuzzyBeetlesReplaceLoneGoombas) assert(this->enemies->Buzzy_Beetle(spawnX, tmpY));
                else assert(this->enemies->Goomba(spawnX, tmpY));
            } else if (random < 6) assert(this->enemies->Green_Koopa(spawnX, tmpY));
            else if (random < 9) this->Spawn_Red_Koopa_If_Possible(spawnX, tmpY, lastX);
            else assert(false);
            break;
        case Level_Type::BRIDGE:
            switch (Random::Get_Instance().Get_Num(4)) {
            case 0:
            case 1:
            case 2:
                this->Spawn_Red_Koopa_If_Possible(spawnX, tmpY, lastX); break;
            case 3:
                if (this->args->difficulty >= this->args->difficultyBuzzyBeetlesReplaceLoneGoombas) assert(this->enemies->Buzzy_Beetle(spawnX, tmpY));
                else assert(this->enemies->Goomba(spawnX, tmpY));
                break;
            case 4:
                assert(this->enemies->Green_Koopa(spawnX, tmpY)); break;
            default:
                assert(false);
            }
            break;
        case Level_Type::ISLAND:
            switch (Random::Get_Instance().Get_Num(5)) {
            case 0:
            case 1:
            case 2:
            case 3:
                this->Spawn_Red_Koopa_If_Possible(spawnX, tmpY, lastX); break;
            case 4:
                if (this->args->difficulty >= this->args->difficultyBuzzyBeetlesReplaceLoneGoombas) assert(this->enemies->Buzzy_Beetle(spawnX, tmpY));
                else assert(this->enemies->Goomba(spawnX, tmpY));
                break;
            case 5:
                assert(this->enemies->Green_Koopa(spawnX, tmpY)); break;
            default:
                assert(false);
            }
            break;
        case Level_Type::UNDERGROUND: //don't spawn enemies that don't change colors with the pallette
        case Level_Type::UNDERWATER:
        case Level_Type::CASTLE:
            random = (Random::Get_Instance().Get_Num(5));
            if (random < 3) {
                if (this->args->difficulty >= this->args->difficultyBuzzyBeetlesReplaceLoneGoombas) assert(this->enemies->Buzzy_Beetle(spawnX, tmpY));
                else assert(this->enemies->Goomba(spawnX, tmpY));
            } else if (random < 6) assert(this->enemies->Green_Koopa(spawnX, tmpY));
            else assert(false);
            break;
        default:
            assert(false); return 0;
        }
    }
    x = tmpX;
    y = tmpY;
    return 1;
}


bool Enemy_Spawner::Find_Safe_Coordinate(int &x, int &y, int lastX) {
    return this->Find_Safe_Coordinate(1, x, y, lastX);
}

bool Enemy_Spawner::Find_Safe_Coordinate(int size, int &x, int &y, int lastX, bool reverse) {
    assert(size > 0);
    if (reverse) {
        for (int i = lastX+15; i >= x; --i) {
            int safeY = 0;
            if (this->Find_Safe_Coordinate_At_X(i, safeY, lastX)) {
                if (size == 1) {
                    x = i;
                    y = safeY;
                    return true;
                } else {
                    for (int j = i+1; j < i+size && this->Is_Coordinate_Safe(j, safeY, lastX); ++j) {
                        //Only return true on the last iteration if it is valid
                        if (j == i+size-1) {
                            x = i;
                            y = safeY;
                            return true;
                        }
                    }
                }
            }
        }
    } else {
        for (int i = x; i <= lastX+15; ++i) {
            int safeY = 0;
            if (this->Find_Safe_Coordinate_At_X(i, safeY, lastX)) {
                if (size == 1) {
                    x = i;
                    y = safeY;
                    return true;
                } else {
                    for (int j = i+1; j < i+size && this->Is_Coordinate_Safe(j, safeY, lastX); ++j) {
                        //Only return true on the last iteration if it is valid
                        if (j == i+size-1) {
                            x = i;
                            y = safeY;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Enemy_Spawner::Find_Safe_Coordinate_At_Y(int &x, int y, int lastX) {
    return this->Find_Safe_Coordinate_At_Y(1, x, y, lastX);
}

bool Enemy_Spawner::Find_Safe_Coordinate_At_Y(int size, int &x, int y, int lastX, bool reverse) {
    assert(size > 0);
    int numValid = 0;
    int incrementLastX = 15;
    if (size > 1) incrementLastX = 13;
    if (reverse) {
        for (int i = lastX+incrementLastX; i <= x; --i) { //use 0xD for enemy groups
            if (this->Is_Coordinate_Safe(i, y, lastX)) ++numValid;
            else numValid = 0;
            if (numValid == size) {
                x = i;
                return true;
            }
        }
    } else {
        for (int i = x; i <= lastX+incrementLastX; ++i) { //use 0xD for enemy groups
            if (this->Is_Coordinate_Safe(i, y, lastX)) ++numValid;
            else numValid = 0;
            if (numValid == size) {
                x = i-size+1;
                return true;
            }
        }
    }
    return false;
}

bool Enemy_Spawner::Find_Safe_Coordinate_At_X(int x, int &y, int lastX) {
    for (int i = Random::Get_Instance().Get_Num(11), numChecked = 0; numChecked < 13; i = (i+1)%12, ++numChecked) {
        if (this->Is_Coordinate_Safe(x, i, lastX)) {
            y = i;
            return true;
        }
    }
    return false;
}

//TODO: Remove the j > 1 if check and implement it into the for loops
bool Enemy_Spawner::Find_Safe_Green_Leaping_Paratroopa_Coordinate(int &x, int &y, int lastX, bool reverse) {
    if (reverse) {
        for (int i = lastX+15; i >= x; --i) {
            for (int j = Random::Get_Instance().Get_Num(11), numChecked = 0; numChecked < 13; j = (j+1)%12, ++numChecked) {
                //Check to see if a regular enemy can spawn here first
                if (j > 1 && this->Is_Coordinate_Safe(i, j, lastX)) {
                    //The two coordinates above cannot be solid objects
                    if (this->levelCrawler->Is_Coordinate_Used(i, j-1)) continue;
                    if (this->levelCrawler->Is_Coordinate_Used(i, j-2)) continue;

                    //Avoid low hovering neighboring blocks that speedy paratroopas can get stuck in
                    if (this->levelCrawler->Is_Coordinate_Used(i-1, j-1) && !this->levelCrawler->Is_Coordinate_Used(i-1, j)) continue;
                    if (this->levelCrawler->Is_Coordinate_Used(i-1, j-2) && !this->levelCrawler->Is_Coordinate_Used(i-1, j-1)) continue;

                    //Safe coordinate found
                    x = i;
                    y = j;
                    return true;
                }
            }
        }
    } else {
        for (int i = x; i <= lastX+15; ++i) {
            for (int j = Random::Get_Instance().Get_Num(11), numChecked = 0; numChecked < 13; j = (j+1)%12, ++numChecked) {
                //Check to see if a regular enemy can spawn here first
                if (j > 1 && this->Is_Coordinate_Safe(i, j, lastX)) {
                    //The two coordinates above cannot be solid objects
                    if (this->levelCrawler->Is_Coordinate_Used(i, j-1)) continue;
                    if (this->levelCrawler->Is_Coordinate_Used(i, j-2)) continue;
                    //Safe coordinate found
                    x = i;
                    y = j;
                    return true;
                }
            }
        }
    }
    return false;
}

//TODO: Don't allow these to spawn above y 2
bool Enemy_Spawner::Find_Safe_Green_Flying_Paratroopa_Coordinate(int &x, int &y, int lastX, bool reverse) {
    if (reverse) {
        for (int i = Random::Get_Instance().Get_Num(11), numChecked = 0; numChecked < 13; i = (i+1)%12, ++numChecked) {
            for (int j = lastX+15; j >= x; --j) {
                if (this->Scan_For_Safe_Green_Flying_Paratroopa_Spawn(x, y, lastX)) {
                    //Y was set in the function
                    x = j;
                    return true;
                }
            }
        }
    } else {
        for (int i = Random::Get_Instance().Get_Num(11), numChecked = 0; numChecked < 13; i = (i+1)%12, ++numChecked) {
            for (int j = x; j <= lastX+15; ++j) {
                if (this->Scan_For_Safe_Green_Flying_Paratroopa_Spawn(x, y, lastX)) {
                    //Y was set in the function
                    x = j;
                    return true;
                }
            }
        }
    }
    return false;
}

//TODO: Don't allow these to spawn above y 2
bool Enemy_Spawner::Scan_For_Safe_Green_Flying_Paratroopa_Spawn(int x, int &y, int lastX) {
    if (!this->Is_Coordinate_Safe(x, y, lastX)) return false;
    //Scan up to 4 blocks above the ground
    int scanDistance = 4;
    if (y < 4) scanDistance = y+1;
    for (int i = y-Random::Get_Instance().Get_Num(scanDistance-1), numScanned = 0; numScanned < scanDistance; i <= 0 ? i = y : --i) {
        //Scan possible flight path
        int numValid = 0;
        bool invalid = false;
        for (int j = x; j >= x-Physics::PARATROOPA_FLY_DISTANCE+1; --j) {
            //At least 3 spaces should not have collision
            if (this->levelCrawler->Is_Coordinate_Empty(j, i)) ++numValid;
            //Prevent a bug with paratroopas getting stuck in walls after being stomped
            if (this->Is_Coordinate_Safe(j, i-2, lastX)) invalid = true;
        }
        if (numValid >= 3 && !invalid) {
            y = i;
            return true;
        }
    }
    return false;
}

bool Enemy_Spawner::Find_Safe_Red_Paratroopa_Coordinate(int &x, int &y, int lastX, bool reverse) {
    if (reverse) {
        for (int i = lastX+15; i >= x; --i) {
            //Red paratroopas cannot be spawned lower than y = 4, otherwise they will not behave properly
            for (int j = Random::Get_Instance().Get_Num(4), numChecked = 0; numChecked < 6; j = (j+1)%5, ++numChecked) {
                //Make sure the red paratroopa has a clear flight path
                bool valid = true;
                for (int k = j; k < j+Physics::PARATROOPA_FLY_DISTANCE; ++k) {
                    if (this->levelCrawler->Is_Coordinate_Used(i, k)) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    x = i;
                    y = j;
                    return true;
                }
            }
        }
    } else {
        for (int i = x; i <= lastX+15; ++i) {
            //Red paratroopas cannot be spawned lower than y = 4, otherwise they will not behave properly
            for (int j = Random::Get_Instance().Get_Num(4), numChecked = 0; numChecked < 6; j = (j+1)%5, ++numChecked) {
                //Make sure the red paratroopa has a clear flight path
                bool valid = true;
                for (int k = j; k < j+Physics::PARATROOPA_FLY_DISTANCE; ++k) {
                    if (this->levelCrawler->Is_Coordinate_Used(i, k)) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    x = i;
                    y = j;
                    return true;
                }
            }
        }
    }
    return false;
}
