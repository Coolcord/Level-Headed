#include "Powerup_Distributor.h"
#include "Block_Data.h"
#include "Level_Crawler.h"
#include "Object_Buffer.h"
#include <assert.h>

Powerup_Distributor::Powerup_Distributor(Level_Crawler *levelCrawler, Object_Buffer *objects, SMB1_Compliance_Generator_Arguments *args) {
    assert(levelCrawler); assert(objects); assert(args);
    this->objects = objects;
    this->levelCrawler = levelCrawler;
    this->args = args;
    assert(this->Reserve_Powerup_Objects());
}

bool Powerup_Distributor::Distribute_Powerups() {
    return true; //TODO: REMOVE THIS!!!

    this->Find_Usable_Blocks(this->objects->Get_Question_Blocks());
    this->Find_Usable_Blocks(this->objects->Get_Brick_Blocks());
    if (!this->Distribute_Question_Block_Powerups()) return false;
    if (!this->Distribute_Hidden_Powerups()) return false;
    return this->Distribute_Ten_Coin_Blocks();
}

void Powerup_Distributor::Find_Usable_Blocks(QMap<QString, Block_Data> *knownBlocks) {
    assert(knownBlocks);
    for (QMap<QString, Block_Data>::iterator iter = knownBlocks->begin(); iter != knownBlocks->end(); ++iter) {
        Block_Data data = iter.value();
        if (this->Is_Block_Hittable(data.x, data.y)) {
            iter.value().hittable = true;
            if (this->Is_Block_Safe_For_Powerup(data.x, data.y)) {
                iter.value().safeForMushroom = true;
                iter.value().safeForStar = this->Is_Block_Safe_For_Powerup(data.x, data.y);
            } else {
                iter.value().safeForMushroom = false;
                iter.value().safeForStar = false;
            }
        } else {
            assert(knownBlocks->remove(iter.key()) == 1);
        }
    }
}

bool Powerup_Distributor::Distribute_Question_Block_Powerups() {

}

bool Powerup_Distributor::Distribute_Hidden_Powerups() {

}

bool Powerup_Distributor::Distribute_Ten_Coin_Blocks() {

}

bool Powerup_Distributor::Reserve_Powerup_Objects() {
    return true; //TODO: REMOVE THIS!!!
}

bool Powerup_Distributor::Is_Block_Hittable(int x, int y) {

}

bool Powerup_Distributor::Is_Block_Safe_For_Powerup(int x, int y) {
    return this->levelCrawler->Is_Coordinate_Empty(x, y-1);
}

bool Powerup_Distributor::Is_Block_Safe_For_Star(int x, int y) {
    if (this->levelCrawler->Is_Coordinate_Used(x, y-1)) return false;
    if (this->levelCrawler->Is_Coordinate_Used(x, y-2)) return false;
    if (this->levelCrawler->Is_Coordinate_Used(x, y-3)) return false;
    if (this->levelCrawler->Is_Coordinate_Used(x+1, y-1) || this->levelCrawler->Is_Coordinate_Used(x+1, y-3)) return false;
    return true;
}
