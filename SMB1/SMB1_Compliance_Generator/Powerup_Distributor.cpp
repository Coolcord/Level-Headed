#include "Powerup_Distributor.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "Block_Data.h"
#include "Level_Crawler.h"
#include "Object_Buffer.h"
#include "Physics.h"
#include <QVector>
#include <assert.h>
#include <cmath>

Powerup_Distributor::Powerup_Distributor(Level_Crawler *levelCrawler, Object_Buffer *objects, SMB1_Compliance_Generator_Arguments *args) {
    assert(levelCrawler); assert(objects); assert(args);
    this->objects = objects;
    this->levelCrawler = levelCrawler;
    this->args = args;
    this->Deallocate_Powerups();
    this->minPowerups = 1;
    this->maxPowerups = args->difficultyMaxPowerups;
    if (this->minPowerups > this->maxPowerups) this->minPowerups = this->maxPowerups;
    this->maxHiddenPowerups = args->difficultyMaxHiddenPowerups;
    this->maxOneUps = args->difficultyMaxOneUps;
    this->maxTenCoinBlocks = args->difficultyMaxTenCoinBlocks;
    this->maxStars = args->difficultyMaxStars;
    this->hiddenPowerupChance = args->difficultyHiddenPowerupChance;
    this->oneUpChance = args->difficultyOneUpChance;
    this->tenCoinBlockChance = args->difficultyTenCoinBlockChance;
    this->starChance = args->difficultyStarChance;
    this->fireFlowerBouncesLikeStar = args->fireFlowerBouncesLikeStar;
    assert(this->Reserve_Powerup_Objects());
}

bool Powerup_Distributor::Distribute_Powerups() {
    if (!this->levelCrawler->Crawl_Level()) return false;
    this->Find_Usable_Blocks(this->objects->Get_Question_Blocks());
    this->Find_Usable_Blocks(this->objects->Get_Brick_Blocks());
    this->Distribute_Question_Block_Items();
    this->Distribute_Brick_Block_Items();
    return true;
}

void Powerup_Distributor::Deallocate_Powerups() {
    this->numPowerups = 0;
    this->numHiddenPowerups = 0;
    this->numOneUps = 0;
    this->numStars = 0;
    this->numTenCoinBlocks = 0;
    this->minPowerups = 0;
    this->maxPowerups = 0;
    this->minHiddenPowerups = 0;
    this->maxHiddenPowerups = 0;
    this->minOneUps = 0;
    this->maxOneUps = 0;
    this->minTenCoinBlocks = 0;
    this->maxTenCoinBlocks = 0;
    this->minStars = 0;
    this->maxStars = 0;
}

void Powerup_Distributor::Find_Usable_Blocks(QMap<QString, Block_Data> *knownBlocks) {
    assert(knownBlocks);
    QMap<QString, Block_Data>::iterator iter = knownBlocks->begin();
    while (iter != knownBlocks->end()) {
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
            ++iter;
        } else {
            QString key = iter.key();
            ++iter;
            assert(knownBlocks->remove(key) == 1);
        }
    }
}

void Powerup_Distributor::Distribute_Question_Block_Items() {
    int numDistributed = 0;
    this->Distribute_Items(Object_Item::QUESTION_BLOCK_WITH_MUSHROOM, this->numPowerups, numDistributed);
    this->numHiddenPowerups += this->numPowerups-numDistributed; //reallocate remaining question block powerups to brick blocks
}

void Powerup_Distributor::Distribute_Brick_Block_Items() {
    this->Distribute_Hidden_Powerups();
    this->Distribute_Ten_Coin_Blocks();
    this->Distribute_Stars();
    this->Distribute_One_Ups();
}

void Powerup_Distributor::Distribute_One_Ups() {
    this->Distribute_Items(Object_Item::BRICK_WITH_1UP, this->numOneUps);
}

void Powerup_Distributor::Distribute_Hidden_Powerups() {
    this->Distribute_Items(Object_Item::BRICK_WITH_MUSHROOM, this->numHiddenPowerups);
}

void Powerup_Distributor::Distribute_Stars() {
    this->Distribute_Items(Object_Item::BRICK_WITH_STAR, this->numStars);
}

void Powerup_Distributor::Distribute_Ten_Coin_Blocks() {
    this->Distribute_Items(Object_Item::BRICK_WITH_10_COINS, this->numTenCoinBlocks);
}

void Powerup_Distributor::Distribute_Items(Object_Item::Object_Item item, int numItems) {
    int numDistributed = 0;
    this->Distribute_Items(item, numItems, numDistributed);
}

void Powerup_Distributor::Distribute_Items(Object_Item::Object_Item item, int numItems, int &numDistributed) {
    //Determine which blocks should be considered for distribution
    QMap<QString, Block_Data> *knownBlocks = nullptr;
    if (item == Object_Item::QUESTION_BLOCK_WITH_MUSHROOM) knownBlocks = this->objects->Get_Question_Blocks();
    else knownBlocks = this->objects->Get_Brick_Blocks();
    assert(knownBlocks);
    QVector<QMap<QString, Block_Data>::iterator> possibleBlocks(knownBlocks->size()); //allocate for worst case scenario
    int numPossibleBlocks = 0;
    for (QMap<QString, Block_Data>::iterator iter = knownBlocks->begin(); iter != knownBlocks->end(); ++iter) {
        //Check against the vertical object limit to make sure the distributor can't break the rule
        if (this->args->useVerticalObjectLimit && iter.value().groupLength > 1) {
            if (iter.value().objectItem == Object_Item::VERTICAL_BRICKS) { //all vertical insertions could break it
                if (this->objects->Get_Num_Vertical_Objects_At_X(iter.value().x) >= 3) continue;
            } else { //horizontal insertions in the middle could break it
                if (!(iter.value().x == iter.value().groupX || iter.value().groupX+(iter.value().groupLength-1) == iter.value().x)) {
                    if (this->objects->Get_Num_Vertical_Objects_At_X(iter.value().x) >= 3) continue;
                }
            }
        }

        //Determine if the block is usable
        switch (item) {
        default:
            assert(false); break;
        case Object_Item::BRICK_WITH_10_COINS:
            if (iter.value().hittable) {
                possibleBlocks[numPossibleBlocks] = iter;
                ++numPossibleBlocks;
            }
            break;
        case Object_Item::QUESTION_BLOCK_WITH_MUSHROOM:
        case Object_Item::BRICK_WITH_MUSHROOM:
            if (this->fireFlowerBouncesLikeStar) {
                if (iter.value().safeForStar) {
                    assert(iter.value().hittable);
                    assert(iter.value().safeForMushroom);
                    possibleBlocks[numPossibleBlocks] = iter;
                    ++numPossibleBlocks;
                }
            } else {
                if (iter.value().safeForMushroom) {
                    assert(iter.value().hittable);
                    possibleBlocks[numPossibleBlocks] = iter;
                    ++numPossibleBlocks;
                }
            }
            break;
        case Object_Item::BRICK_WITH_1UP:
            if (iter.value().safeForMushroom) {
                assert(iter.value().hittable);
                possibleBlocks[numPossibleBlocks] = iter;
                ++numPossibleBlocks;
            }
            break;
        case Object_Item::BRICK_WITH_STAR:
            if (iter.value().safeForStar) {
                assert(iter.value().hittable);
                assert(iter.value().safeForMushroom);

                //Only consider blocks in the first 3/4ths of the level
                if (iter.value().x < static_cast<int>(std::round((static_cast<double>(this->objects->Get_Level_Length())/4.0)*3.0))) {
                    possibleBlocks[numPossibleBlocks] = iter;
                    ++numPossibleBlocks;
                }
            }
            break;
        }
    }

    //Break up the level into sections and place 1 item in a random place of each section
    if (numItems < numPossibleBlocks) {
        int min = -1, max = -1;
        for (int i = 1; i <= numItems; ++i) {
            min = max+1;
            max = ((numPossibleBlocks-1)/numItems)*i;
            assert(max < numPossibleBlocks);
            QMap<QString, Block_Data>::iterator iter = possibleBlocks.at(Random::Get_Instance().Get_Num(min, max));
            assert(iter != knownBlocks->end());
            this->Insert_Item_At(iter.value(), item);
            ++numDistributed;
        }
    } else {
        for (int i = 0; i < numPossibleBlocks; ++i) {
            QMap<QString, Block_Data>::iterator iter = possibleBlocks.at(i);
            assert(iter != knownBlocks->end());
            this->Insert_Item_At(iter.value(), item);
            ++numDistributed;
        }
    }
}

bool Powerup_Distributor::Reserve_Powerup_Objects() {
    this->Roll_For_Powerups(this->numPowerups, this->minPowerups, this->maxPowerups);
    if (!this->objects->Reserve_Objects(this->numPowerups)) return false;
    this->Roll_For_Hidden_Items(this->numHiddenPowerups, this->minHiddenPowerups, this->maxHiddenPowerups, this->hiddenPowerupChance, false);
    if (!this->objects->Reserve_Objects(this->numHiddenPowerups)) return false;
    this->Roll_For_Hidden_Items(this->numTenCoinBlocks, this->minTenCoinBlocks, this->maxTenCoinBlocks, this->tenCoinBlockChance, true);
    if (!this->objects->Reserve_Objects(this->numTenCoinBlocks)) return false;
    this->Roll_For_Hidden_Items(this->numStars, this->minStars, this->maxStars, this->starChance, false);
    if (!this->objects->Reserve_Objects(this->numStars)) return false;
    this->Roll_For_Hidden_Items(this->numOneUps, this->minOneUps, this->maxOneUps, this->oneUpChance, false);
    return this->objects->Reserve_Objects(this->numOneUps);
}

void Powerup_Distributor::Roll_For_Powerups(int &numItems, int min, int max) {
    assert(min <= max);
    if (min == max) { numItems = min; return; }
    double percentage = static_cast<double>((99-((this->args->difficulty-1)*11)))/static_cast<double>(100.0); //0.0-0.99
    if (args->difficulty <= 1) percentage = 1.0;
    if (args->difficulty >= 10) percentage = 0.0;
    numItems = static_cast<int>(std::round(static_cast<double>(max-min)*percentage))+min;
}

void Powerup_Distributor::Roll_For_Hidden_Items(int &numItems, int min, int max, int chance, bool isTenCoinBlock) {
    if (this->objects->Get_Num_Objects_Available() <= 0) return;
    assert(min <= max);
    numItems = 0;
    if (!isTenCoinBlock) chance -= static_cast<int>(std::round((static_cast<double>(chance)/2.0)*(static_cast<double>(this->args->difficulty-1)/10.0)));
    assert(chance >= 0);
    for (int i = min; i < max; ++i) {
        if (Random::Get_Instance().Get_Num(100) <= chance) ++numItems;
        int availableObjects = this->objects->Get_Num_Objects_Available();
        assert(availableObjects > 0);
        if (availableObjects == numItems) return;
    }
}

//TODO: Check if the block can be hit from a left or right platform
bool Powerup_Distributor::Is_Block_Hittable(int x, int y) {
    //Make sure the coordinate below is empty
    if (!this->levelCrawler->Is_Coordinate_Breakable_Or_Empty(x, y+1)) return false;

    //Check if the block can be reached from below
    int platformY = 0;
    if (!this->levelCrawler->Find_Platform_Directly_Below(x, y, platformY)) return false;
    if (platformY-y > Physics::BASIC_JUMP_HEIGHT) return false;
    return true;
}

//TODO: Add path finding to make sure the powerup is reachable
bool Powerup_Distributor::Is_Block_Safe_For_Powerup(int x, int y) {
    return this->levelCrawler->Is_Coordinate_Empty(x, y-1);
}

//TODO: Add path finding to make sure the star is reachable
bool Powerup_Distributor::Is_Block_Safe_For_Star(int x, int y) {
    if (this->levelCrawler->Is_Coordinate_Used(x, y-1)) return false;
    if (this->levelCrawler->Is_Coordinate_Used(x, y-2)) return false;
    if (this->levelCrawler->Is_Coordinate_Used(x, y-3)) return false;
    if (this->levelCrawler->Is_Coordinate_Used(x+1, y-1) || this->levelCrawler->Is_Coordinate_Used(x+1, y-3)) return false;
    return true;
}

void Powerup_Distributor::Insert_Item_At(const Block_Data &block, Object_Item::Object_Item item) {
    assert(this->objects->Free_Reserved_Objects(1));
    QMap<QString, Block_Data> *blocks = nullptr;
    if (item == Object_Item::QUESTION_BLOCK_WITH_MUSHROOM) blocks = this->objects->Get_Question_Blocks();
    else blocks = this->objects->Get_Brick_Blocks();

    //Handle insertion differently depending on if the block is in a group or not
    if (block.groupLength > 1) {
        if (block.objectItem == Object_Item::VERTICAL_BRICKS) { //vertical bricks
            assert(block.x == block.groupX);
            if (block.y == block.groupY) { //handle insertion at the beginning
                //Swap out the group with the new item
                assert(this->objects->Seek_To_Object_Item(block.groupX, block.groupY, block.objectItem));
                Buffer_Data *data = this->objects->Get_Current_For_Modification();
                assert(data);
                Object_Item::Object_Item groupItem = data->objectItem;
                int oldLength = data->length;
                int newLength = oldLength-1;
                data->length = 1;
                data->objectItem = item;

                //Reinsert the group object
                assert(newLength >= 0);
                if (newLength > 0) {
                    this->Update_Group_Data(blocks, true, block.groupX, block.groupY, oldLength, block.groupX, block.groupY+1, newLength);

                    //Update Vertical Object Limit Count to reflect that the group has been removed
                    assert(this->objects->Decrement_Vertical_Object_Count_At_X(block.x));

                    //Insert the group after the item
                    assert(this->objects->Seek_To_Absolute_X(block.x));
                    assert(this->Insert_Group_Item_Into_Object_Buffer(this->objects->Get_Relative_X_From_Absolute_X(block.x), block.y+1, newLength, groupItem));
                }
            } else if (block.groupY+(block.groupLength-1) == block.y) { //handle insertion at the end
                //Shorten the group length by 1
                assert(this->objects->Seek_To_Object_Item(block.groupX, block.groupY, block.objectItem));
                Buffer_Data *data = this->objects->Get_Current_For_Modification();
                assert(data);
                int oldLength = data->length;
                --data->length;
                int newLength = data->length;
                assert(data->length >= 1);
                this->Update_Group_Data(blocks, true, block.groupX, block.groupY, oldLength, block.groupX, block.groupY, newLength);

                //Insert the new item
                assert(this->objects->Seek_To_Absolute_X(block.x));
                assert(this->Insert_Item_Into_Object_Buffer(this->objects->Get_Relative_X_From_Absolute_X(block.x), block.y, item));
            } else { //handle insertion at the middle
                assert(this->objects->Seek_To_Absolute_X(block.x));
                assert(this->Insert_Item_Into_Object_Buffer(this->objects->Get_Relative_X_From_Absolute_X(block.x), block.y, item));
            }
        } else { //horizontal bricks/blocks
            assert(block.y == block.groupY);
            if (block.x == block.groupX) { //handle insertion at the beginning
                //Swap out the group with the new item
                assert(this->objects->Seek_To_Object_Item(block.groupX, block.groupY, block.objectItem));
                Buffer_Data *data = this->objects->Get_Current_For_Modification();
                assert(data);
                Object_Item::Object_Item groupItem = data->objectItem;
                int oldLength = data->length;
                int newLength = data->length-1;
                data->length = 1;
                data->objectItem = item;

                //Reinsert the group object
                assert(newLength >= 0);
                if (newLength > 0) {
                    this->Update_Group_Data(blocks, false, block.groupX, block.groupY, oldLength, block.groupX+1, block.groupY, newLength);

                    //Update Vertical Object Limit Count to reflect that the group has been removed
                    assert(this->objects->Decrement_Vertical_Object_Count_Starting_At_X(block.x+1, newLength));

                    //Insert the group after the item
                    assert(this->objects->Seek_To_Absolute_X(block.x+1));
                    assert(this->Insert_Group_Item_Into_Object_Buffer(this->objects->Get_Relative_X_From_Absolute_X(block.x+1), block.y, newLength, groupItem));
                }
            } else if (block.groupX+(block.groupLength-1) == block.x) { //handle insertion at the end
                //Shorten the group length by 1
                assert(this->objects->Seek_To_Object_Item(block.groupX, block.groupY, block.objectItem));
                Buffer_Data *data = this->objects->Get_Current_For_Modification();
                assert(data);
                int oldLength = data->length;
                --data->length;
                int newLength = data->length;
                assert(data->length >= 1);
                this->Update_Group_Data(blocks, false, block.groupX, block.groupY, oldLength, block.groupX, block.groupY, newLength);

                //Update Vertical Object Limit Count to reflect that the group's length has been reduced by 1
                assert(this->objects->Decrement_Vertical_Object_Count_At_X(block.x));

                //Insert the new item
                assert(this->objects->Seek_To_Absolute_X(block.x));
                assert(this->Insert_Item_Into_Object_Buffer(this->objects->Get_Relative_X_From_Absolute_X(block.x), block.y, item));
            } else { //handle insertion at the middle
                assert(this->objects->Seek_To_Absolute_X(block.x));
                assert(this->Insert_Item_Into_Object_Buffer(this->objects->Get_Relative_X_From_Absolute_X(block.x), block.y, item));
            }
        }
    } else { //when the block is not in a group, swap out the item in its place
        assert(this->objects->Seek_To_Object_Item(block.x, block.y, block.objectItem));
        Buffer_Data *data = this->objects->Get_Current_For_Modification();
        assert(data);
        data->objectItem = item;
    }

    //Remove the used block from the available blocks
    assert(blocks->remove(this->objects->Get_Coordinate_Key(block.x, block.y)) == 1);
}

bool Powerup_Distributor::Insert_Item_Into_Object_Buffer(int x, int y, Object_Item::Object_Item item) {
    assert(x >= 0 && x <= 31);
    if (x > 16) this->objects->Set_Coordinate_Safety(false);
    bool success = false;
    switch (item) {
    default:                                        assert(false); return false;
    case Object_Item::BRICK_WITH_10_COINS:          success = this->objects->Brick_With_10_Coins(x, y); break;
    case Object_Item::QUESTION_BLOCK_WITH_MUSHROOM: success = this->objects->Question_Block_With_Mushroom(x, y); break;
    case Object_Item::BRICK_WITH_MUSHROOM:          success = this->objects->Brick_With_Mushroom(x, y); break;
    case Object_Item::BRICK_WITH_1UP:               success = this->objects->Brick_With_1up(x, y); break;
    case Object_Item::BRICK_WITH_STAR:              success = this->objects->Brick_With_Star(x, y); break;
    }
    this->objects->Set_Coordinate_Safety(true);
    return success;
}

bool Powerup_Distributor::Insert_Group_Item_Into_Object_Buffer(int x, int y, int length, Object_Item::Object_Item item) {
    assert(x >= 0 && x <= 31);
    if (x > 16) this->objects->Set_Coordinate_Safety(false);
    bool success = false;
    switch (item) {
    default:                                                    assert(false); return false;
    case Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS:    success = this->objects->Horizontal_Question_Blocks_With_Coins(x, y, length); break;
    case Object_Item::HORIZONTAL_BRICKS:                        success = this->objects->Horizontal_Bricks(x, y, length); break;
    case Object_Item::VERTICAL_BRICKS:                          success = this->objects->Vertical_Bricks(x, y, length); break;
    }
    this->objects->Set_Coordinate_Safety(true);
    return success;
}

void Powerup_Distributor::Update_Group_Data(QMap<QString, Block_Data> *blocks, bool vertical, int oldX, int oldY, int oldLength, int newX, int newY, int newLength) {
    assert(blocks);
    if (vertical) {
        for (int i = oldY; i < oldY+oldLength && oldY+i < 0xB; ++i) {
            QMap<QString, Block_Data>::iterator data = blocks->find(this->objects->Get_Coordinate_Key(oldX, i));
            if (data == blocks->end()) continue; //block must be used

            //Only update what's considered to be part of the group
            if (data->groupX == oldX && data->groupY == oldY && data->groupLength == oldLength) {
                data->groupX = newX; data->groupY = newY; data->groupLength = newLength;
            }
        }
    } else {
        for (int i = oldX; i < oldX+oldLength; ++i) {
            QMap<QString, Block_Data>::iterator data = blocks->find(this->objects->Get_Coordinate_Key(i, oldY));
            if (data == blocks->end()) continue; //block must be used

            //Only update what's considered to be part of the group
            if (data->groupX == oldX && data->groupY == oldY && data->groupLength == oldLength) {
                data->groupX = newX; data->groupY = newY; data->groupLength = newLength;
            }
        }
    }
}
