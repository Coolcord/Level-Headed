#include "Powerup_Distributor.h"
#include "../../Common_Files/Random.h"
#include "Block_Data.h"
#include "Level_Crawler.h"
#include "Object_Buffer.h"
#include "Physics.h"
#include <QVector>
#include <assert.h>

const static int MIN_POWERUPS = 1;
const static int MAX_POWERUPS = 3;
const static int MIN_HIDDEN_POWERUPS = 0;
const static int MAX_HIDDEN_POWERUPS = 2;
const static int MIN_ONE_UPS = 0;
const static int MAX_ONE_UPS = 1;
const static int MIN_STARS = 0;
const static int MAX_STARS = 2;
const static int MIN_TEN_COIN_BLOCKS = 0;
const static int MAX_TEN_COIN_BLOCKS = 2;
const static int CHANCE_ITEM_DIFFICULTY_MODIFIER = 10;
const static int CHANCE_POWERUPS = 50;
const static int CHANCE_HIDDEN_POWERUPS = 50;
const static int CHANCE_ONE_UPS = 50;
const static int CHANCE_STARS = 50;
const static int CHANCE_TEN_COIN_BLOCKS = 50;

Powerup_Distributor::Powerup_Distributor(Level_Crawler *levelCrawler, Object_Buffer *objects, SMB1_Compliance_Generator_Arguments *args) {
    assert(levelCrawler); assert(objects); assert(args);
    this->objects = objects;
    this->levelCrawler = levelCrawler;
    this->args = args;
    this->numPowerups = 0;
    this->numHiddenPowerups = 0;
    this->numOneUps = 0;
    this->numStars = 0;
    this->numTenCoinBlocks = 0;
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
    return this->Distribute_Items(Object_Item::QUESTION_BLOCK_WITH_MUSHROOM, this->numPowerups);
}

void Powerup_Distributor::Distribute_Brick_Block_Items() {
    this->Distribute_Hidden_Powerups();
    this->Distribute_One_Ups();
    this->Distribute_Stars();
    this->Distribute_Ten_Coin_Blocks();
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
                possibleBlocks[numPossibleBlocks] = iter;
                ++numPossibleBlocks;
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
        }
    } else {
        for (int i = 0; i < numPossibleBlocks; ++i) {
            QMap<QString, Block_Data>::iterator iter = possibleBlocks.at(i);
            assert(iter != knownBlocks->end());
            this->Insert_Item_At(iter.value(), item);
        }
    }
}

bool Powerup_Distributor::Reserve_Powerup_Objects() {
    //Don't allocate anything on short levels
    if (this->objects->Get_Num_Objects_Available() < MAX_POWERUPS+MAX_HIDDEN_POWERUPS+MAX_ONE_UPS+MAX_STARS+MAX_TEN_COIN_BLOCKS) return true;

    this->Roll_For_Items(this->numPowerups, MIN_POWERUPS, MAX_POWERUPS, CHANCE_POWERUPS);
    this->Roll_For_Items(this->numHiddenPowerups, MIN_HIDDEN_POWERUPS, MAX_HIDDEN_POWERUPS, CHANCE_HIDDEN_POWERUPS);
    this->Roll_For_Items(this->numOneUps, MIN_ONE_UPS, MAX_ONE_UPS, CHANCE_ONE_UPS);
    this->Roll_For_Items(this->numStars, MIN_STARS, MAX_STARS, CHANCE_STARS);
    this->Roll_For_Items(this->numTenCoinBlocks, MIN_TEN_COIN_BLOCKS, MAX_TEN_COIN_BLOCKS, CHANCE_TEN_COIN_BLOCKS);
    if (!this->objects->Reserve_Objects(this->numPowerups)) return false;
    if (!this->objects->Reserve_Objects(this->numHiddenPowerups)) return false;
    if (!this->objects->Reserve_Objects(this->numOneUps)) return false;
    if (!this->objects->Reserve_Objects(this->numStars)) return false;
    return this->objects->Reserve_Objects(this->numTenCoinBlocks);
}

void Powerup_Distributor::Roll_For_Items(int &numItems, int min, int max, int chance) {
    assert(min <= max);
    numItems = 0;
    int maxChance = 100+((this->args->difficulty-1)*CHANCE_ITEM_DIFFICULTY_MODIFIER);
    for (int i = min; i < max; ++i) {
        if (Random::Get_Instance().Get_Num(maxChance) <= chance) ++numItems;
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
                this->Update_Group_Data(blocks, true, block.x, block.y, oldLength, block.x, block.y+1, newLength);

                //Update Vertical Object Limit Count to reflect that the group has been removed
                assert(this->objects->Decrement_Vertical_Object_Count_At_X(block.x));

                //Insert the group after the item (no need to seek here)
                int x = block.x-this->objects->Get_Absolute_X();
                assert(this->Insert_Group_Item_Into_Object_Buffer(x, block.y+1, newLength, groupItem));
            } else if (block.groupY+(block.groupLength-1) == block.y) { //handle insertion at the end
                //Shorten the group length by 1
                assert(this->objects->Seek_To_Object_Item(block.groupX, block.groupY, block.objectItem));
                Buffer_Data *data = this->objects->Get_Current_For_Modification();
                assert(data);
                int oldLength = data->length;
                --data->length;
                int newLength = data->length;
                assert(data->length >= 1);
                this->Update_Group_Data(blocks, true, block.x, block.y, oldLength, block.x, block.y, newLength);

                //Insert the new item (no need to seek here)
                int x = block.x-this->objects->Get_Absolute_X();
                assert(this->Insert_Item_Into_Object_Buffer(x, block.y, item));
            } else { //handle insertion at the middle
                assert(this->objects->Seek_To_Absolute_X(block.x));
                int x = block.x-this->objects->Get_Absolute_X();
                assert(this->Insert_Item_Into_Object_Buffer(x, block.y, item));
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
                this->Update_Group_Data(blocks, false, block.x, block.y, oldLength, block.x+1, block.y, newLength);

                //Update Vertical Object Limit Count to reflect that the group has been removed
                assert(this->objects->Decrement_Vertical_Object_Count_Starting_At_X(block.x+1, newLength));

                //Insert the group after the item
                assert(this->objects->Seek_To_Absolute_X(block.x+1));
                int x = (block.x+1)-this->objects->Get_Absolute_X();
                assert(this->Insert_Group_Item_Into_Object_Buffer(x, block.y, newLength, groupItem));
            } else if (block.groupX+(block.groupLength-1) == block.x) { //handle insertion at the end
                //Shorten the group length by 1
                assert(this->objects->Seek_To_Object_Item(block.groupX, block.groupY, block.objectItem));
                Buffer_Data *data = this->objects->Get_Current_For_Modification();
                assert(data);
                int oldLength = data->length;
                --data->length;
                int newLength = data->length;
                assert(data->length >= 1);
                this->Update_Group_Data(blocks, false, block.x, block.y, oldLength, block.x, block.y, newLength);

                //Update Vertical Object Limit Count to reflect that the group's length has been reduced by 1
                assert(this->objects->Decrement_Vertical_Object_Count_At_X(block.x));

                //Insert the new item
                assert(this->objects->Seek_To_Absolute_X(block.x));
                int x = block.x-this->objects->Get_Absolute_X();
                assert(this->Insert_Item_Into_Object_Buffer(x, block.y, item));
            } else { //handle insertion at the middle
                assert(this->objects->Seek_To_Absolute_X(block.x));
                int x = block.x-this->objects->Get_Absolute_X();
                assert(this->Insert_Item_Into_Object_Buffer(x, block.y, item));
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
    assert(x >= 0);
    switch (item) {
    default:                                        assert(false); return false;
    case Object_Item::BRICK_WITH_10_COINS:          return this->objects->Brick_With_10_Coins(x, y);
    case Object_Item::QUESTION_BLOCK_WITH_MUSHROOM: return this->objects->Question_Block_With_Mushroom(x, y);
    case Object_Item::BRICK_WITH_MUSHROOM:          return this->objects->Brick_With_Mushroom(x, y);
    case Object_Item::BRICK_WITH_1UP:               return this->objects->Brick_With_1up(x, y);
    case Object_Item::BRICK_WITH_STAR:              return this->objects->Brick_With_Star(x, y);
    }
}

bool Powerup_Distributor::Insert_Group_Item_Into_Object_Buffer(int x, int y, int length, Object_Item::Object_Item item) {
    assert(x >= 0);
    switch (item) {
    default:                                                    assert(false); return false;
    case Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS:    return this->objects->Horizontal_Question_Blocks_With_Coins(x, y, length);
    case Object_Item::HORIZONTAL_BRICKS:                        return this->objects->Horizontal_Bricks(x, y, length);
    case Object_Item::VERTICAL_BRICKS:                          return this->objects->Vertical_Bricks(x, y, length);
    }
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
