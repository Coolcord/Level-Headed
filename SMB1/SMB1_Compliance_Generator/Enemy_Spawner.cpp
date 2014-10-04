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
    int lastSize = 1;
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

    x = averageDistance/2;
    while (this->bytesLeft > 1) {
        int size = this->Common_Enemy(x, y, lastX, lastSize);
        if (size == 0) {
            qDebug() << "Looks like an enemy failed to spawn";
        } else {
            lastSize = size;
            lastX = x;
        }

        x += averageDistance;
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

int Enemy_Spawner::Common_Enemy(int &x, int &y, int lastX, int lastSize) {
    assert(lastSize > 0);
    int tmpX = x;
    assert(tmpX-lastX > 0);
    int tmpY = y;
    if (!this->levelCrawler->Find_Safe_Coordinate(lastSize, tmpX, tmpY, lastX)) return 0;
    assert(tmpX > lastX);
    int spawnX = tmpX-lastX;
    switch(qrand() % 4) {
    case 0:
        assert(this->enemies->Goomba(spawnX, tmpY)); break;
    case 1:
        assert(this->enemies->Green_Koopa(spawnX, tmpY)); break;
    case 2:
        assert(this->enemies->Red_Koopa(spawnX, tmpY)); break;
    case 3:
        assert(this->enemies->Green_Paratroopa(spawnX, tmpY, true, true)); break;
    default:
        assert(false);
    }
    x = tmpX;
    y = tmpY;
    return 1;
}


