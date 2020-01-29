#include "Powerup_Distributor.h"
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

    if (!this->Find_Usable_Blocks()) return false;
    if (!this->Distribute_Question_Block_Powerups()) return false;
    if (!this->Distribute_Hidden_Powerups()) return false;
    return this->Distribute_Ten_Coin_Blocks();
}

bool Powerup_Distributor::Find_Usable_Blocks() {

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
