#include "Enemy_Spawner.h"
#include "Enemy_Writer.h"
#include "Level_Crawler.h"
#include "Physics.h"
#include <QTime>
#include <QDebug>
#include <assert.h>

Enemy_Spawner::Enemy_Spawner(QFile *file, QTextStream *stream, Enemy_Writer *enemies) {
    assert(file);
    assert(stream);
    assert(enemies);
    this->file = file;
    this->stream = stream;
    this->enemies = enemies;
    this->levelCrawler = new Level_Crawler(this->file);
}

Enemy_Spawner::~Enemy_Spawner() {
    delete this->levelCrawler;
}

bool Enemy_Spawner::Spawn_Enemies(Brick::Brick startingBrick, Level_Type::Level_Type levelType) {
    this->stream->flush();

    if (!this->levelCrawler->Crawl_Level(startingBrick)) return false;
    int x = 0;
    int lastX = x;
    int y = 0;

    int totalSpaces = this->levelCrawler->Get_Safe_Size();
    int numPages = totalSpaces/16;
    bool usePages = false;
    int section = 0; //used with pages to determine what group of enemies are being operated on

    //Determine the max number of enemies available
    int numEnemies = 0;
    if (this->enemies->Get_Num_Bytes_Left() % 2 == 1) { //byteLeft is odd
        numEnemies = (this->enemies->Get_Num_Bytes_Left()-1)/2;
    } else {
        numEnemies = this->enemies->Get_Num_Bytes_Left()/2;
    }

    //Determine where to place the page changes
    int firstPageChange = (numPages/4)+1;
    int secondPageChange = (numPages/2)+1;
    int thirdPageChange = static_cast<int>(((static_cast<double>(numPages)/4.0)*3.0))+1;
    int firstEnemyGroup = static_cast<int>(((static_cast<double>(numEnemies)/4.0)*3.0))+1;
    int secondEnemyGroup = (numEnemies/2)+1;
    int thirdEnemyGroup = (numEnemies/4)+1;

    //Determine the average distance between each enemy
    int averageDistance = 0;
    if (numEnemies > 0) averageDistance = totalSpaces/numEnemies;
    if (averageDistance >= 16) {
        usePages = true;
        qDebug() << "Using pages!";
    }
    if (averageDistance > 11) {
        averageDistance = 11;
    }

    int size = 1;
    x = averageDistance/2;
    while (this->enemies->Get_Num_Bytes_Left() > 1) {
        //Determine what type of enemies to spawn
        switch (levelType) {
        case Level_Type::STANDARD_OVERWORLD:
            size = this->Spawn_Standard_Overworld_Enemy(x, y, lastX, size);
            break;
        case Level_Type::UNDERGROUND:
            size = 0;
            break; //TODO: Implement this...
        case Level_Type::UNDERWATER:
            size = 0;
            break; //TODO: Implement this...
        case Level_Type::CASTLE:
            size = 0;
            break; //TODO: Implement this...
        case Level_Type::BRIDGE:
            size = 0;
            break; //TODO: Implement this...
        case Level_Type::ISLAND:
            size = 0;
            break; //TODO: Implement this...
        default:
            assert(false);
        }

        //TODO: Handle errors here... Maybe use a page change?
        if (size == 0) {
            qDebug() << "Looks like an enemy failed to spawn";
        } else {
            lastX = x;
        }

        //Spawn a page change if necessary
        if (usePages && this->enemies->Get_Num_Bytes_Left() >= 4) {
            switch (section) {
            case 0:
                if (this->Spawn_Page_Change(x, y, lastX, averageDistance, firstPageChange, firstEnemyGroup)) ++section;
                break;
            case 1:
                if (this->Spawn_Page_Change(x, y, lastX, averageDistance, secondPageChange, secondEnemyGroup)) ++section;
                break;
            case 2:
                if (this->Spawn_Page_Change(x, y, lastX, averageDistance, thirdPageChange, thirdEnemyGroup)) ++section;
                break;
            default:
                break;
            }
        }

        //Increment X
        x += averageDistance;
        if (averageDistance < size) {
            x += averageDistance - size; //Prevent enemies from spawning on top of each other
        }
    }
    return true;
}

bool Enemy_Spawner::Spawn_Page_Change(int &x, int &y, int &lastX, int averageDistance, int page, int enemyAmount) {
    //Skip the page change if necessary
    if (this->enemies->Get_Current_Page()+1 >= page) {
        return true;
    }

    //Spawn the page change if necessary
    if ((this->enemies->Get_Num_Bytes_Left()/2) <= enemyAmount) {
        qDebug() << "Page Change spawned at: " << page;
        assert(this->enemies->Page_Change(page));
        x = ((page-1)*16)+1;
        qDebug() << "X is now: " << x;
        lastX = x;
        y = Physics::GROUND_Y;
        return true;
    }

    //Cannot move on to the next section yet
    return false;
}

int Enemy_Spawner::Spawn_Standard_Overworld_Enemy(int &x, int &y, int lastX, int size) {
    switch (qrand() % 4) {
    case 0:
    case 1:
    case 2:
        return this->Common_Enemy(x, y, lastX, size);
    case 3:
        return this->Multi_Enemy(x, y, lastX, size);
    default:
        assert(false);
        return 0;
    }
}

int Enemy_Spawner::Spawn_Underground_Enemy(int &x, int &y, int lastX, int size) {
    //TODO: Implement this...
    return 0;
}

int Enemy_Spawner::Spawn_Underwater_Enemy(int &x, int &y, int lastX, int size) {
    //TODO: Implement this...
    return 0;
}

int Enemy_Spawner::Spawn_Castle_Enemy(int &x, int &y, int lastX, int size) {
    //TODO: Implement this...
    return 0;
}

int Enemy_Spawner::Spawn_Bridge_Enemy(int &x, int &y, int lastX, int size) {
    //TODO: Implement this...
    return 0;
}

int Enemy_Spawner::Spawn_Island_Enemy(int &x, int &y, int lastX, int size) {
    //TODO: Implement this...
    return 0;
}

int Enemy_Spawner::Get_Random_X(int min) {
    int random = qrand() % 0x11;
    random += min;
    if (random > 0x10) random = 0x10;
    return random;
}

int Enemy_Spawner::Multi_Enemy(int &x, int &y, int lastX, int lastSize) {
    int tmpX = x;
    assert(tmpX-lastX > 0);

    //Determine how many enemies to spawn
    int numEnemies = 0;
    switch (qrand() % 2) {
    case 0: numEnemies = 2; break;
    case 1: numEnemies = 3; break;
    default: assert(false);
    }

    //Determine the y to place the enemies at
    int tmpY = y;
    switch (qrand() % 2) {
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
                    return this->Common_Enemy(x, y, lastX, lastSize); //give up and spawn a common enemy instead
                }
            }
        }
    }

    //Determine what type of enemies to spawn
    assert(tmpX > lastX);
    int spawnX = tmpX-lastX;
    assert(spawnX <= 16);
    assert(numEnemies == 2 || numEnemies == 3);
    switch (qrand() % 2) {
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

int Enemy_Spawner::Common_Enemy(int &x, int &y, int lastX, int lastSize) {
    int tmpX = x;
    assert(tmpX-lastX > 0);
    int tmpY = y;
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
    switch (qrand() % 3) {
    case 0:
        assert(this->enemies->Goomba(spawnX, tmpY)); break;
    case 1:
        assert(this->enemies->Green_Koopa(spawnX, tmpY)); break;
    case 2:
        assert(this->enemies->Red_Koopa(spawnX, tmpY)); break;
    default:
        assert(false);
    }
    x = tmpX;
    y = tmpY;
    return 1;
}

