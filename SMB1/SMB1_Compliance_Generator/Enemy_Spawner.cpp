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
    while (this->bytesLeft > 1) {
        int size = this->Common_Enemy(x, y, lastX, lastSize);
        if (size == 0) {
            qDebug() << "Looks like an enemy failed to spawn";
        } else {
            lastSize = size;
            lastX = x;
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

int Enemy_Spawner::Common_Enemy(int &x, int &y, int lastX, int lastSize) {
    assert(lastSize > 0);
    int tmpX = this->Get_Random_X(lastSize)+x;
    assert(tmpX-lastX > 0);
    int tmpY = y;
    if (!this->levelCrawler->Find_Safe_Coordinate(lastSize, tmpX, tmpY, lastX)) return 0;
    assert(tmpX > lastX);
    assert(this->enemies->Goomba(tmpX-lastX, tmpY));
    x = tmpX;
    y = tmpY;
    return 1;
}


