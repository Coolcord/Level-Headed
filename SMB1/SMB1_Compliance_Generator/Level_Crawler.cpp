#include "Level_Crawler.h"
#include "Physics.h"

Level_Crawler::Level_Crawler() {
    this->safeCoordinates = new QMap<QString, bool>();
}

Level_Crawler::~Level_Crawler() {
    delete this->safeCoordinates;
}

bool Level_Crawler::Crawl_Level(QFile *file) {
    if (!file->isOpen() || !file->reset()) return false;

    int x = 0;
    int y = 0;

    /*
    //Read the Header
    do {

    } while (line != NULL && !file->atEnd());

    //Read the Objects to Determine Safe Spots to Place Enemies
    */
}
