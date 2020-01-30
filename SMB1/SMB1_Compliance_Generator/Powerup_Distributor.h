#ifndef POWERUP_DISTRIBUTOR_H
#define POWERUP_DISTRIBUTOR_H

#include "SMB1_Compliance_Generator_Arguments.h"
#include <QMap>

struct Block_Data;
class Object_Buffer;
class Level_Crawler;

class Powerup_Distributor {
public:
    Powerup_Distributor(Level_Crawler *levelCrawler, Object_Buffer *objects, SMB1_Compliance_Generator_Arguments *args);
    ~Powerup_Distributor() {}
    bool Distribute_Powerups();

private:
    void Find_Usable_Blocks(QMap<QString, Block_Data> *knownBlocks);
    bool Distribute_Question_Block_Powerups();
    bool Distribute_Hidden_Powerups();
    bool Distribute_Ten_Coin_Blocks();
    bool Reserve_Powerup_Objects();
    bool Is_Block_Hittable(int x, int y);
    bool Is_Block_Safe_For_Powerup(int x, int y);
    bool Is_Block_Safe_For_Star(int x, int y);

    Object_Buffer *objects;
    Level_Crawler *levelCrawler;
    SMB1_Compliance_Generator_Arguments *args;
};

#endif // POWERUP_DISTRIBUTOR_H
