#include "Enemy_Spawner.h"
#include "Enemy_Writer.h"
#include "Required_Enemy_Spawns.h"
#include "Level_Crawler.h"
#include "Physics.h"
#include "Difficulty.h"
#include "../Common_SMB1_Files/Level_Type_String.h"
#include "../../Common_Files/Random.h"
#include "../../Level-Headed/Common_Strings.h"
#include <QTime>
#include <QDebug>
#include <assert.h>

Enemy_Spawner::Enemy_Spawner(QFile *file, QTextStream *stream, Enemy_Writer *enemies,
                             Required_Enemy_Spawns *requiredEnemySpawns, SMB1_Compliance_Generator_Arguments *args) {
    assert(file);
    assert(stream);
    assert(enemies);
    assert(requiredEnemySpawns);
    assert(args);
    assert(args->difficulty >= Difficulty::DIFFICULTY_MIN && args->difficulty <= Difficulty::DIFFICULTY_MAX);
    this->file = file;
    this->stream = stream;
    this->enemies = enemies;
    this->requiredEnemySpawns = requiredEnemySpawns;
    this->args = args;
    this->levelCrawler = new Level_Crawler(this->file);
    this->emergencySpawnMode = false;
}

Enemy_Spawner::~Enemy_Spawner() {
    delete this->levelCrawler;
}

bool Enemy_Spawner::Spawn_Enemies(Brick::Brick startingBrick) {
    this->stream->flush();
    if (this->stream->status() != QTextStream::Ok) return false;

    if (!this->levelCrawler->Crawl_Level(startingBrick)) return false;
    *(this->stream) << Level_Type::STRING_BREAK << Common_Strings::STRING_NEW_LINE;
    if (this->stream->status() != QTextStream::Ok) return false;
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
        bool forceHammerBro = this->args->difficulty >= this->args->difficultyHammerTime && Random::Get_Num(99) <= this->args->difficultyHammerTimeIntensity-1;
        bool noEnemies = this->args->difficultyNoEnemies;
        if (!noEnemies && this->args->difficultyDisableAllOtherEnemiesWhenALakituSpawns) noEnemies = this->enemies->Was_Lakitu_Spawned();
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

        //TODO: Handle errors here... Maybe use a page change?
        if (size == 0) {
            qDebug() << "Looks like an enemy failed to spawn";
        } else {
            lastX = x;
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

    //Add a seperator at the end of the file
    *(this->stream) << Level_Type::STRING_BREAK << Common_Strings::STRING_NEW_LINE;
    if (this->stream->status() != QTextStream::Ok) return false;
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
    if (this->enemies->Get_Current_Page() >= page-1) {
        return true;
    }

    //Don't forget to spawn the required enemies
    if (this->requiredEnemySpawns->Get_Num_Required_Enemy_Spawns() > 0) {
        while (page*16 > this->requiredEnemySpawns->Get_X()) {
            //Spawn a page change if necessary
            if (!this->requiredEnemySpawns->Is_In_Range_Of_Required_Enemy(lastX)) {
                int page = lastX/16;
                assert(this->enemies->Page_Change(page));
                x = (page*16);
                lastX = x;
            }
            this->requiredEnemySpawns->Spawn_Required_Enemy(lastX);
        }
        //Check to see if the page change can be skipped now
        if (this->enemies->Get_Current_Page() >= page-1) return true;
    }

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

int Enemy_Spawner::Spawn_Standard_Overworld_Enemy(int &x, int &y, int lastX, int size, bool noEnemies) {
    switch (Random::Get_Num(3)) {
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
    switch (Random::Get_Num(3)) {
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
    switch (Random::Get_Num(3)) {
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
    switch (Random::Get_Num(3)) {
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
    return 0;
}

int Enemy_Spawner::Spawn_Island_Enemy(int &x, int &y, int lastX, int size, bool noEnemies) {
    //TODO: Add other paratroopa types
    switch (Random::Get_Num(3)) {
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
    return 0;
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
    assert(totalSpaces-x >= 0);
    if (numEnemies > 0) averageDistance = (totalSpaces-x)/numEnemies;
    if (averageDistance > 11) averageDistance = 11;
    if (averageDistance < 4) averageDistance = 4;
    return averageDistance;
}

int Enemy_Spawner::Get_Random_X(int min) {
    int random = Random::Get_Num(0x10);
    random += min;
    if (random > 0x10) random = 0x10;
    return random;
}

int Enemy_Spawner::Multi_Enemy(int &x, int &y, int lastX, int lastSize, bool noEnemies) {
    int tmpX = x;
    assert(tmpX-lastX > 0);
    if (noEnemies) return this->Common_Enemy(x, y, lastX, lastSize, false, true); //give up and spawn a common enemy instead

    //Determine how many enemies to spawn
    int numEnemies = 0;
    switch (Random::Get_Num(1)) {
    case 0: numEnemies = 2; break;
    case 1: numEnemies = 3; break;
    default: assert(false);
    }

    //Determine the y to place the enemies at
    int tmpY = y;
    switch (Random::Get_Num(1)) {
    case 0: tmpY = 0x6; break;
    case 1: tmpY = 0xA; break;
    default: assert(false);
    }

    //Try to find a place to spawn the enemies
    if (!this->levelCrawler->Find_Safe_Coordinate_At_Y(numEnemies+1, tmpX, tmpY, lastX)) {
        //Try the other y coordinate
        if (tmpY == 0x6) tmpY = 0xA;
        else tmpY = 0x6;
        assert(tmpX == x);
        if (!this->levelCrawler->Find_Safe_Coordinate_At_Y(numEnemies+1, tmpX, tmpY, lastX)) {
            //Try one more round...
            tmpX = lastX+lastSize;
            if (lastSize == 0) ++tmpX;
            numEnemies = 2;
            if (!this->levelCrawler->Find_Safe_Coordinate_At_Y(numEnemies+1, tmpX, tmpY, lastX, true)) {
                //Try the other y coordinate
                if (tmpY == 0x6) tmpY = 0xA;
                else tmpY = 0x6;
                if (!this->levelCrawler->Find_Safe_Coordinate_At_Y(numEnemies+1, tmpX, tmpY, lastX, true)) {
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
    switch (Random::Get_Num(1)) {
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

int Enemy_Spawner::Common_Enemy(int &x, int &y, int lastX, int lastSize, bool forceHammerBro, bool noEnemies) {
    int tmpX = x;
    assert(tmpX-lastX > 0);
    int tmpY = y;

    if (!noEnemies && !forceHammerBro && this->args->levelType == Level_Type::UNDERWATER) {
        int spawnX = x - lastX;
        y = Random::Get_Num(9)+1;
        assert(this->enemies->Blooper(spawnX, y));
        return 1;
    }

    //Try to spawn a Green Paratroopa
    if (!noEnemies && !forceHammerBro && Random::Get_Num(4) == 0) {
        bool spawnParatroopa = false;
        if (!this->levelCrawler->Find_Safe_Green_Leaping_Paratroopa_Coordinate(tmpX, tmpY, lastX)) {
            //Try again, but start closer to the last enemy
            tmpX = lastX+lastSize;
            if (lastSize == 0) ++tmpX;
            if (this->levelCrawler->Find_Safe_Green_Leaping_Paratroopa_Coordinate(tmpX, tmpY, lastX, true)) {
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
    if (!this->levelCrawler->Find_Safe_Coordinate(tmpX, tmpY, lastX)) {
        //Try again, but start closer to the last enemy
        tmpX = lastX+lastSize;
        if (lastSize == 0) ++tmpX;
        if (!this->levelCrawler->Find_Safe_Coordinate(1, tmpX, tmpY, lastX, true)) {
            return 0;
        }
    }
    assert(tmpX > lastX);
    int spawnX = tmpX-lastX;
    int random = 0;
    //Spawn Hammer Bros. in later levels
    if (forceHammerBro) {
        assert(this->enemies->Hammer_Bro(spawnX, tmpY));
    } else if (noEnemies) {
        assert(this->enemies->Nothing(spawnX));
    } else {
        switch (this->args->levelType) {
        case Level_Type::STANDARD_OVERWORLD:
            random = Random::Get_Num(9);
            if (random < 3) assert(this->enemies->Goomba(spawnX, tmpY));
            else if (random < 6) assert(this->enemies->Green_Koopa(spawnX, tmpY));
            else if (random < 9) assert(this->enemies->Red_Koopa(spawnX, tmpY));
            else if (random < 10) assert(this->enemies->Buzzy_Beetle(spawnX, tmpY));
            else assert(false);
            break;
        case Level_Type::BRIDGE:
            switch (Random::Get_Num(4)) {
            case 0:
            case 1:
            case 2:
                assert(this->enemies->Red_Koopa(spawnX, tmpY)); break;
            case 3:
                assert(this->enemies->Goomba(spawnX, tmpY)); break;
            case 4:
                assert(this->enemies->Green_Koopa(spawnX, tmpY)); break;
            default:
                assert(false);
            }
            break;
        case Level_Type::ISLAND:
            switch (Random::Get_Num(5)) {
            case 0:
            case 1:
            case 2:
            case 3:
                assert(this->enemies->Red_Koopa(spawnX, tmpY)); break;
            case 4:
                assert(this->enemies->Goomba(spawnX, tmpY)); break;
            case 5:
                assert(this->enemies->Green_Koopa(spawnX, tmpY)); break;
            default:
                assert(false);
            }
            break;
        case Level_Type::UNDERGROUND: //don't spawn enemies that don't change colors with the pallette
        case Level_Type::UNDERWATER:
        case Level_Type::CASTLE:
            random = (Random::Get_Num(6));
            if (random < 3) assert(this->enemies->Goomba(spawnX, tmpY));
            else if (random < 6) assert(this->enemies->Green_Koopa(spawnX, tmpY));
            else if (random < 7) assert(this->enemies->Buzzy_Beetle(spawnX, tmpY));
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

