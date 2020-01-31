#ifndef POWERUP_DISTRIBUTOR_H
#define POWERUP_DISTRIBUTOR_H

#include "../Common_SMB1_Files/Object_Item.h"
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
    void Distribute_Question_Block_Items();
    void Distribute_Brick_Block_Items();
    void Distribute_One_Ups();
    void Distribute_Hidden_Powerups();
    void Distribute_Stars();
    void Distribute_Ten_Coin_Blocks();
    void Distribute_Items(Object_Item::Object_Item item, int numItems);
    bool Reserve_Powerup_Objects();
    void Roll_For_Items(int &numItems, int min, int max, int chance);
    bool Is_Block_Hittable(int x, int y);
    bool Is_Block_Safe_For_Powerup(int x, int y);
    bool Is_Block_Safe_For_Star(int x, int y);
    void Insert_Item_At(const Block_Data &block, Object_Item::Object_Item item);
    bool Insert_Item_Into_Object_Buffer(int x, int y, Object_Item::Object_Item item);
    bool Insert_Group_Item_Into_Object_Buffer(int x, int y, int length, Object_Item::Object_Item item);
    void Update_Group_Data(QMap<QString, Block_Data> *blocks, bool vertical, int oldX, int oldY, int oldLength, int newX, int newY, int newLength);

    Object_Buffer *objects;
    Level_Crawler *levelCrawler;
    SMB1_Compliance_Generator_Arguments *args;
    int numPowerups;
    int numHiddenPowerups;
    int numOneUps;
    int numStars;
    int numTenCoinBlocks;
};

#endif // POWERUP_DISTRIBUTOR_H
