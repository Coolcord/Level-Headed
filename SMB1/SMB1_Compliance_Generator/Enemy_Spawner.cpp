#include "Enemy_Spawner.h"
#include "Enemy_Writer.h"
#include "Level_Crawler.h"
#include <QTime>
#include <QDebug>
#include <assert.h>

Enemy_Spawner::Enemy_Spawner(QFile *file, QTextStream *stream, Enemy_Writer *enemies, int numEnemyBytes) {
    assert(file);
    assert(stream);
    assert(enemies);
    this->file = file;
    this->stream = stream;
    this->enemies = enemies;
    this->levelCrawler = new Level_Crawler(this->file);
    this->bytesLeft = numEnemyBytes;
}

Enemy_Spawner::~Enemy_Spawner() {
    delete this->levelCrawler;
}

bool Enemy_Spawner::Spawn_Enemies(Brick::Brick startingBrick) {
    this->stream->flush();

    if (!this->levelCrawler->Crawl_Level(startingBrick)) return false;
    int x = 0;
    int lastX = x;
    int y = 0;

    int totalSpaces = this->levelCrawler->Get_Safe_Size();
    int numPages = totalSpaces/16;

    //Determine the max number of enemies available
    int numEnemies = 0;
    if (this->bytesLeft % 2 == 1) { //byteLeft is odd
        numEnemies = (this->bytesLeft-1)/2;
    } else {
        numEnemies = this->bytesLeft/2;
    }

    //Determine the average distance between each enemy
    int averageDistance = 0;
    if (numEnemies > 0) averageDistance = totalSpaces/numEnemies;

    //TODO: When the average Distance is greater, make use of pages instead of relative coordinates
    qDebug() << "Average Distance between enemies: " << averageDistance;
    if (averageDistance >= 12) {
        qDebug() << "That's too big! Fixing...";
        averageDistance = 12;
    }

    int size = 1;
    x = averageDistance/2;
    while (this->bytesLeft > 1) {
        //Determine what type of enemies to spawn
        switch (qrand() % 4) {
        case 0:
        case 1:
        case 2:
            size = this->Common_Enemy(x, y, lastX, size); break;
        case 3:
            size = this->Multi_Enemy(x, y, lastX, size); break;
            break;
        default:
            assert(false);
        }

        //TODO: Handle errors here... Maybe use a page change?
        if (size == 0) {
            qDebug() << "Looks like an enemy failed to spawn";
        } else {
            lastX = x;
        }

        //Increment X
        x += averageDistance;
        if (averageDistance < size) {
            x += averageDistance - size; //Prevent enemies from spawning on top of each other
        }
        this->bytesLeft -= 2;
    }
    return true;
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
    int tmpY = 0;
    switch (qrand() % 2) {
    case 0: tmpY = 0x6; break;
    case 1: tmpY = 0xA; break;
    default: assert(false);
    }

    //Fall back to spawning a normal enemy if there's no room to spawn multiples
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
    qDebug() << "Y is: " << tmpY;
    qDebug() << "Num: " << numEnemies;
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


