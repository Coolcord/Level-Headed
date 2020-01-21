#include "Level_Crawler.h"
#include "../../Common_Files/Random.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../Common_SMB1_Files/Object_Item_String.h"
#include "../Common_SMB1_Files/Brick_String.h"
#include "Object_Buffer.h"
#include "Object_Buffer_Data.h"
#include "Physics.h"
#include <QTime>
#include <QDebug>
#include <assert.h>
#include <QVector>

Level_Crawler::Level_Crawler(Object_Buffer *objects) {
    assert(objects);
    this->objects = objects;
    this->endDetected = false;
    this->safeSize = 0;
    this->badCoordinates = new QMap<QString, bool>();
}

Level_Crawler::~Level_Crawler() {
    delete this->badCoordinates;
}

bool Level_Crawler::Crawl_Level(Brick::Brick startingBrick) {
    this->brick = startingBrick;
    this->nextBrick = startingBrick;
    this->endDetected = false;
    this->safeSize = 0;
    int x = 0;
    int holeCrawlSteps = 0;

    //Read the Objects to Determine Safe Spots to Place Enemies
    this->objects->Seek_To_Beginning();
    while (!this->objects->At_End()) {
        Object_Buffer_Data data = this->objects->Get_Current();
        assert(this->Parse_Object(data, x, holeCrawlSteps));
        if (!this->endDetected) this->safeSize = x-1;
        this->objects->Seek_To_Next();
    }

    return true;
    //return this->Draw_Map(); //Debug code
}

int Level_Crawler::Get_Safe_Size() {
    return this->safeSize;
}

bool Level_Crawler::Find_Safe_Coordinate(int &x, int &y, int lastX) {
    return this->Find_Safe_Coordinate(1, x, y, lastX);
}

bool Level_Crawler::Find_Safe_Coordinate(int size, int &x, int &y, int lastX, bool reverse) {
    assert(size > 0);
    if (reverse) {
        for (int i = lastX+15; i >= x; --i) {
            int safeY = 0;
            if (this->Find_Safe_Coordinate_At_X(i, safeY)) {
                if (size == 1) {
                    x = i;
                    y = safeY;
                    return true;
                } else {
                    for (int j = i+1; j < i+size && this->Is_Coordinate_Safe(j, safeY); ++j) {
                        //Only return true on the last iteration if it is valid
                        if (j == i+size-1) {
                            x = i;
                            y = safeY;
                            return true;
                        }
                    }
                }
            }
        }
    } else {
        for (int i = x; i <= lastX+15; ++i) {
            int safeY = 0;
            if (this->Find_Safe_Coordinate_At_X(i, safeY)) {
                if (size == 1) {
                    x = i;
                    y = safeY;
                    return true;
                } else {
                    for (int j = i+1; j < i+size && this->Is_Coordinate_Safe(j, safeY); ++j) {
                        //Only return true on the last iteration if it is valid
                        if (j == i+size-1) {
                            x = i;
                            y = safeY;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Level_Crawler::Find_Safe_Coordinate_At_Y(int &x, int y, int lastX) {
    return this->Find_Safe_Coordinate_At_Y(1, x, y, lastX);
}

bool Level_Crawler::Find_Safe_Coordinate_At_Y(int size, int &x, int y, int lastX, bool reverse) {
    assert(size > 0);
    int numValid = 0;
    int incrementLastX = 15;
    if (size > 1) incrementLastX = 13;
    if (reverse) {
        for (int i = lastX+incrementLastX; i <= x; --i) { //use 0xD for enemy groups
            if (this->Is_Coordinate_Safe(i, y)) ++numValid;
            else numValid = 0;
            if (numValid == size) {
                x = i;
                return true;
            }
        }
    } else {
        for (int i = x; i <= lastX+incrementLastX; ++i) { //use 0xD for enemy groups
            if (this->Is_Coordinate_Safe(i, y)) ++numValid;
            else numValid = 0;
            if (numValid == size) {
                x = i-size+1;
                return true;
            }
        }
    }
    return false;
}

bool Level_Crawler::Find_Safe_Coordinate_At_X(int x, int &y) {
    for (int i = Random::Get_Instance().Get_Num(11), numChecked = 0; numChecked < 13; i = (i+1)%12, ++numChecked) {
        if (this->Is_Coordinate_Safe(x, i)) {
            y = i;
            return true;
        }
    }
    return false;
}

//TODO: Remove the j > 1 if check and implement it into the for loops
bool Level_Crawler::Find_Safe_Green_Leaping_Paratroopa_Coordinate(int &x, int &y, int lastX, bool reverse) {
    if (reverse) {
        for (int i = lastX+15; i >= x; --i) {
            for (int j = Random::Get_Instance().Get_Num(11), numChecked = 0; numChecked < 13; j = (j+1)%12, ++numChecked) {
                //Check to see if a regular enemy can spawn here first
                if (j > 1 && this->Is_Coordinate_Safe(i, j)) {
                    //The two coordinates above cannot be solid objects
                    if (this->badCoordinates->contains(this->Make_Key(i, j-1))) continue;
                    if (this->badCoordinates->contains(this->Make_Key(i, j-2))) continue;
                    //Safe coordinate found
                    x = i;
                    y = j;
                    return true;
                }
            }
        }
    } else {
        for (int i = x; i <= lastX+15; ++i) {
            for (int j = Random::Get_Instance().Get_Num(11), numChecked = 0; numChecked < 13; j = (j+1)%12, ++numChecked) {
                //Check to see if a regular enemy can spawn here first
                if (j > 1 && this->Is_Coordinate_Safe(i, j)) {
                    //The two coordinates above cannot be solid objects
                    if (this->badCoordinates->contains(this->Make_Key(i, j-1))) continue;
                    if (this->badCoordinates->contains(this->Make_Key(i, j-2))) continue;
                    //Safe coordinate found
                    x = i;
                    y = j;
                    return true;
                }
            }
        }
    }
    return false;
}

//TODO: Don't allow these to spawn above y 2
bool Level_Crawler::Find_Safe_Green_Flying_Paratroopa_Coordinate(int &x, int &y, int lastX, bool reverse) {
    if (reverse) {
        for (int i = Random::Get_Instance().Get_Num(11), numChecked = 0; numChecked < 13; i = (i+1)%12, ++numChecked) {
            for (int j = lastX+15; j >= x; --j) {
                if (this->Scan_For_Safe_Green_Flying_Paratroopa_Spawn(x, y)) {
                    //Y was set in the function
                    x = j;
                    return true;
                }
            }
        }
    } else {
        for (int i = Random::Get_Instance().Get_Num(11), numChecked = 0; numChecked < 13; i = (i+1)%12, ++numChecked) {
            for (int j = x; j <= lastX+15; ++j) {
                if (this->Scan_For_Safe_Green_Flying_Paratroopa_Spawn(x, y)) {
                    //Y was set in the function
                    x = j;
                    return true;
                }
            }
        }
    }
    return false;
}

//TODO: Don't allow these to spawn above y 2
bool Level_Crawler::Scan_For_Safe_Green_Flying_Paratroopa_Spawn(int x, int &y) {
    if (!this->Is_Coordinate_Safe(x, y)) return false;
    //Scan up to 4 blocks above the ground
    int scanDistance = 4;
    if (y < 4) scanDistance = y+1;
    for (int i = y-Random::Get_Instance().Get_Num(scanDistance-1), numScanned = 0; numScanned < scanDistance; i <= 0 ? i = y : --i) {
        //Scan possible flight path
        int numValid = 0;
        bool invalid = false;
        for (int j = x; j >= x-Physics::PARATROOPA_FLY_DISTANCE+1; --j) {
            //At least 3 spaces should not have collision
            if (!this->badCoordinates->contains(this->Make_Key(j, i))) ++numValid;
            //Prevent a bug with paratroopas getting stuck in walls after being stomped
            if (this->Is_Coordinate_Safe(j, i-2)) invalid = true;
        }
        if (numValid >= 3 && !invalid) {
            y = i;
            return true;
        }
    }
    return false;
}

bool Level_Crawler::Find_Safe_Red_Paratroopa_Coordinate(int &x, int &y, int lastX, bool reverse) {
    if (reverse) {
        for (int i = lastX+15; i >= x; --i) {
            //Red paratroopas cannot be spawned lower than y = 4, otherwise they will not behave properly
            for (int j = Random::Get_Instance().Get_Num(4), numChecked = 0; numChecked < 6; j = (j+1)%5, ++numChecked) {
                //Make sure the red paratroopa has a clear flight path
                bool valid = true;
                for (int k = j; k < j+Physics::PARATROOPA_FLY_DISTANCE; ++k) {
                    if (this->badCoordinates->contains(this->Make_Key(i, k))) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    x = i;
                    y = j;
                    return true;
                }
            }
        }
    } else {
        for (int i = x; i <= lastX+15; ++i) {
            //Red paratroopas cannot be spawned lower than y = 4, otherwise they will not behave properly
            for (int j = Random::Get_Instance().Get_Num(4), numChecked = 0; numChecked < 6; j = (j+1)%5, ++numChecked) {
                //Make sure the red paratroopa has a clear flight path
                bool valid = true;
                for (int k = j; k < j+Physics::PARATROOPA_FLY_DISTANCE; ++k) {
                    if (this->badCoordinates->contains(this->Make_Key(i, k))) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    x = i;
                    y = j;
                    return true;
                }
            }
        }
    }
    return false;
}

bool Level_Crawler::Is_Coordinate_Safe(int x, int y) {
    QString key = this->Make_Key(x, y);
    if (!this->badCoordinates->contains(key)) {
        key = this->Make_Key(x, y+1);
        //Ground is expected to be below the enemy
        if (this->badCoordinates->contains(key)) return true;
    }
    return false;
}

void Level_Crawler::Crawl_Forward(int x, int spaces) {
    assert(x >= 0);
    assert(spaces >= 0);
    bool changeBrick = false;
    for (int i = spaces; i > 0; --i, ++x) {
        if (changeBrick) this->brick = this->nextBrick; //brick changes don't happen until one space later
        if (this->brick != this->nextBrick) changeBrick = true;
        switch (this->brick) {
        case Brick::NO_BRICKS:
            continue; //Nothing to do
        case Brick::SURFACE:
            this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1);
            break;
        case Brick::SURFACE_AND_CEILING:
            this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1);
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::SURFACE_AND_CEILING_3:
            this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1);
            for (int i = 0; i < 3; ++i) this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            break;
        case Brick::SURFACE_AND_CEILING_4:
            this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1);
            for (int i = 0; i < 4; ++i) this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            break;
        case Brick::SURFACE_AND_CEILING_8:
            this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1);
            for (int i = 0; i < 8; ++i) this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            break;
        case Brick::SURFACE_4_AND_CEILING:
            for (int i = 0; i < 4; ++i) this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1-i);
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::SURFACE_4_AND_CEILING_3:
            for (int i = 0; i < 4; ++i) this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1-i);
            for (int i = 0; i < 3; ++i) this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            break;
        case Brick::SURFACE_4_AND_CEILING_4:
            for (int i = 0; i < 4; ++i) this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1-i);
            for (int i = 0; i < 4; ++i) this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            break;
        case Brick::SURFACE_5_AND_CEILING:
            for (int i = 0; i < 5; ++i) this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1-i);
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::CEILING:
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::SURFACE_5_AND_CEILING_4:
            for (int i = 0; i < 5; ++i) this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1-i);
            for (int i = 0; i < 4; ++i) this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            break;
        case Brick::SURFACE_8_AND_CEILING:
            for (int i = 0; i < 8; ++i) this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1-i);
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::SURFACE_AND_CEILING_AND_MIDDLE_5:
            this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1);
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            for (int i = 3; i < 8; ++i) this->Mark_Bad_Coordinate(x, i);
            break;
        case Brick::SURFACE_AND_CEILING_AND_MIDDLE_4:
            this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1);
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            for (int i = 3; i < 7; ++i) this->Mark_Bad_Coordinate(x, i);
            break;
        case Brick::ALL:
            for (int i = 0; i <= Physics::GROUND_Y+1; ++i) this->Mark_Bad_Coordinate(x, i);
            break;
        }
    }
    if (changeBrick) this->brick = this->nextBrick;
}

void Level_Crawler::Crawl_Forward_With_Hole(int x, int spaces, int &holeCrawlSteps) {
    assert(x >= 0);
    assert(spaces >= 0);
    bool changeBrick = false;
    for (int i = spaces; i > 0; --i, ++x, --holeCrawlSteps) {
        if (changeBrick) this->brick = this->nextBrick; //brick changes don't happen until one space later
        if (this->brick != this->nextBrick) changeBrick = true;
        assert(holeCrawlSteps >= 0);
        if (holeCrawlSteps == 0) return this->Crawl_Forward(x, i);
        switch (this->brick) {
        case Brick::NO_BRICKS:
            case Brick::SURFACE:
            break; //Nothing to do
        case Brick::SURFACE_AND_CEILING:
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::SURFACE_AND_CEILING_3:
            for (int i = 0; i < 3; ++i) this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            break;
        case Brick::SURFACE_AND_CEILING_4:
            for (int i = 0; i < 4; ++i) this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            break;
        case Brick::SURFACE_AND_CEILING_8:
            for (int i = 0; i < 8; ++i) this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            break;
        case Brick::SURFACE_4_AND_CEILING:
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::SURFACE_4_AND_CEILING_3:
            for (int i = 0; i < 3; ++i) this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            break;
        case Brick::SURFACE_4_AND_CEILING_4:
            for (int i = 0; i < 4; ++i) this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            break;
        case Brick::SURFACE_5_AND_CEILING:
            this->Mark_Bad_Coordinate(x, Physics::GROUND_Y-3);
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::CEILING:
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::SURFACE_5_AND_CEILING_4:
            this->Mark_Bad_Coordinate(x, Physics::GROUND_Y-3);
            for (int i = 0; i < 4; ++i) this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            break;
        case Brick::SURFACE_8_AND_CEILING:
            for (int i = 4; i < 8; ++i) this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1-i);
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::SURFACE_AND_CEILING_AND_MIDDLE_5:
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            for (int i = 3; i < 8; ++i) this->Mark_Bad_Coordinate(x, i);
            break;
        case Brick::SURFACE_AND_CEILING_AND_MIDDLE_4:
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            for (int i = 3; i < 7; ++i) this->Mark_Bad_Coordinate(x, i);
            break;
        case Brick::ALL:
            for (int i = 0; i <= Physics::GROUND_Y+1; ++i) this->Mark_Bad_Coordinate(x, i);
            break;
        }
    }
    if (changeBrick) this->brick = this->nextBrick;
}

void Level_Crawler::Mark_Bad_Coordinate(int x, int y) {
    if (y > 11) return;
    QString key = this->Make_Key(x, y);
    if (this->badCoordinates->contains(key)) return;
    this->badCoordinates->insert(key, true);
}

void Level_Crawler::Mark_Bad_X(int x) {
    for (int i = 0; i < 12; ++i) {
        this->Mark_Bad_Coordinate(x, i);
    }
}

void Level_Crawler::Clear_X(int x) {
    for (int i = 0; i < 12; ++i) {
        QString key = this->Make_Key(x, i);
        if (this->badCoordinates->contains(key)) {
            this->badCoordinates->remove(key);
        }
    }
}

QString Level_Crawler::Make_Key(int x, int y) {
    return (QString::number(x) + "x" + QString::number(y));
}

bool Level_Crawler::Parse_Object(const Object_Buffer_Data &data, int &x, int &holeCrawlSteps) {
    //Crawl forward according to the brick pattern
    int steps = data.x;
    if (holeCrawlSteps == 0) this->Crawl_Forward(x, steps);
    else this->Crawl_Forward_With_Hole(x, steps, holeCrawlSteps);
    x += steps;
    int y = 0;
    int length = 0;

    switch (data.objectItem) {
    case Object_Item::QUESTION_BLOCK_WITH_MUSHROOM:
    case Object_Item::QUESTION_BLOCK_WITH_COIN:
    case Object_Item::BRICK_WITH_MUSHROOM:
    case Object_Item::BRICK_WITH_VINE:
    case Object_Item::BRICK_WITH_STAR:
    case Object_Item::BRICK_WITH_10_COINS:
    case Object_Item::BRICK_WITH_1UP:
    case Object_Item::USED_BLOCK:
        y = data.y;
        this->Mark_Bad_Coordinate(x, y);
        return true;
    case Object_Item::UNDERWATER_SIDEWAYS_PIPE:
    case Object_Item::TRAMPOLINE:
        y = data.y;
        this->Mark_Bad_Coordinate(x, y);
        this->Mark_Bad_Coordinate(x, y+1);
        return true;
    case Object_Item::ISLAND:
        y = data.y;
        length = data.length;
        for (int i = 0; i < length; ++i) this->Mark_Bad_Coordinate(x+i, y);
        return true;
    case Object_Item::HORIZONTAL_BRICKS:
    case Object_Item::HORIZONTAL_BLOCKS:
    case Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS:
        y = data.y;
        length = data.length;
        for (int i = 0; i < length; ++i) this->Mark_Bad_Coordinate(x+i, y);
        return true;
    case Object_Item::BULLET_BILL_TURRET:
    case Object_Item::CORAL:
    case Object_Item::VERTICAL_BRICKS:
    case Object_Item::VERTICAL_BLOCKS:
        y = data.y;
        length = data.height;
        for (int i = 0; i < length; ++i) this->Mark_Bad_Coordinate(x, y+i);
        return true;
    case Object_Item::HORIZONTAL_COINS:
        return true; //ignore coins
    case Object_Item::PIPE:
    case Object_Item::ENTERABLE_PIPE:
        y = data.y;
        length = data.height;
        for (int i = 0; i < length; ++i) this->Mark_Bad_Coordinate(x, y+i);
        for (int i = 0; i < length; ++i) this->Mark_Bad_Coordinate(x+1, y+i);
        return true;
    case Object_Item::HOLE:
    case Object_Item::HOLE_WITH_WATER:
        length = data.length;
        holeCrawlSteps = length;
        return true;
    case Object_Item::BALANCE_LIFT_VERTICAL_ROPE:
        this->Clear_X(x);
        return true;
    case Object_Item::BRIDGE:
        y = data.y;
        length = data.length;
        for (int i = 0; i < length; ++i) this->Mark_Bad_Coordinate(x+i, y);
        return true;
    case Object_Item::PAGE_CHANGE:
        length = data.page;
        this->Crawl_Forward(x, (length*16)-x);
        x = length * 16;
        return true;
    case Object_Item::REVERSE_L_PIPE:
        this->Mark_Bad_Coordinate(x, Physics::GROUND_Y-1);
        this->Mark_Bad_Coordinate(x, Physics::GROUND_Y);
        this->Mark_Bad_Coordinate(x+1, Physics::GROUND_Y-1);
        this->Mark_Bad_Coordinate(x+1, Physics::GROUND_Y);
        for (int i = 0; i < 2; ++i) {
            for (int j = Physics::GROUND_Y; j > Physics::GROUND_Y-4; --j) this->Mark_Bad_Coordinate(x+i, j);
        }
        return true;
    case Object_Item::FLAGPOLE:
        this->endDetected = true;
        this->Mark_Bad_X(x);
        return true;
    case Object_Item::CASTLE:
    case Object_Item::BIG_CASTLE:
        for (int i = 0; i < 5; ++i) this->Mark_Bad_X(x+i);
        return true;
    case Object_Item::AXE:
        this->endDetected = true;
        this->Mark_Bad_Coordinate(x, 6);
        return true;
    case Object_Item::AXE_ROPE:
        this->Mark_Bad_Coordinate(x, 7);
        return true;
    case Object_Item::BOWSER_BRIDGE:
        this->endDetected = true;
        for (int i = 0; i < 13; ++i) this->Mark_Bad_Coordinate(x+i, 8);
        return true;
    case Object_Item::LIFT_ROPE:
        this->Mark_Bad_X(x);
        return true;
    case Object_Item::STEPS:
        y = Physics::GROUND_Y;
        length = data.length;
        for (int i = 0; i < length; ++i) {
            for (int j = 0; j <= i; ++j) this->Mark_Bad_Coordinate(x+i, y-j);
        }
        return true;
    case Object_Item::END_STEPS:
        this->endDetected = true;
        y = Physics::GROUND_Y;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j <= i; ++j) this->Mark_Bad_Coordinate(x+i, y-j);
        }
        for (int i = 0; i < 8; ++i) this->Mark_Bad_Coordinate(x+8, y-i);
        return true;
    case Object_Item::TALL_REVERSE_L_PIPE:
        y = data.y;
        this->Mark_Bad_Coordinate(x, y);
        this->Mark_Bad_Coordinate(x, y+1);
        this->Mark_Bad_Coordinate(x+1, y);
        this->Mark_Bad_Coordinate(x+1, y+1);
        this->Mark_Bad_X(x+2);
        this->Mark_Bad_X(x+3);
        return true;
    case Object_Item::PIPE_WALL:
        for (int i = 0; i < 2; ++i) this->Mark_Bad_X(x+i);
        return true;
    case Object_Item::CHANGE_BRICK_AND_SCENERY:
        this->nextBrick = data.brick;
        return true;
    case Object_Item::HIDDEN_BLOCK_WITH_COIN:
    case Object_Item::HIDDEN_BLOCK_WITH_1UP:
    case Object_Item::BALANCE_LIFT_HORIZONTAL_ROPE:
    case Object_Item::SCROLL_STOP:
    case Object_Item::SCROLL_STOP_WARP_ZONE:
    case Object_Item::TOGGLE_AUTO_SCROLL:
    case Object_Item::FLYING_CHEEP_CHEEP_SPAWNER:
    case Object_Item::SWIMMING_CHEEP_CHEEP_SPAWNER:
    case Object_Item::BULLET_BILL_SPAWNER:
    case Object_Item::CANCEL_SPAWNER:
    case Object_Item::LOOP_COMMAND:
    case Object_Item::CHANGE_BACKGROUND:
    case Object_Item::NOTHING:
        return true; //nothing to do
    }
    assert(false); return false;
}

bool Level_Crawler::Draw_Map() {
    QFile file("F:/Documents/Source_Code/Level-Headed/build-Level-Headed-Desktop_Qt_5_9_3_MinGW_32bit-Debug/debug/dump.txt");
    if (!file.open(QFile::ReadWrite | QFile::Truncate)) return false;
    QTextStream stream(&file);

    int maxX = this->Get_Safe_Size();
    int maxY = 13;

    //Allocate a 2D Vector to use as the map
    QVector<QVector<bool>> map(maxX);
    for (int i = 0; i < maxX; ++i) {
        map[i].resize(maxY);
    }

    //Initialize all elements to false
    for (int i = 0; i < maxX; ++i) {
        for (int j = 0; j < maxY; ++j) {
            map[i][j] = false;
        }
    }

    //Determine what to draw
    QVector<QString> keys = this->badCoordinates->keys().toVector();
    for (int i = 0; i < keys.length(); ++i) {
        int x = this->Get_X_From_Key(keys.at(i));
        int y = this->Get_Y_From_Key(keys.at(i));
        if (x == -1 || y == -1) continue;
        if (x >= maxX || y >= maxY) continue;

        qDebug() << "MaxX: " << maxX;
        qDebug() << "MaxY: " << maxY;
        qDebug() << "X: " << x;
        qDebug() << "Y: " << y;

        assert(x >= 0 && x < maxX);
        assert(y >= 0 && y < maxY);
        map[x][y] = true;
    }

    //Draw the map
    for (int i = 0; i < maxY; ++i) {
        for (int j = 0; j < maxX; ++j) {
            if (map[j][i]) {
                stream << "X";
            } else {
                stream << " ";
            }
        }
        stream << Common_Strings::STRING_NEW_LINE;
    }

    stream.flush();
    file.close();
    return true;
}

int Level_Crawler::Get_X_From_Key(const QString &key) {
    if (key == nullptr || key.isEmpty()) return -1;
    QStringList coordinates = key.split("x");
    return coordinates.at(0).toInt();
}

int Level_Crawler::Get_Y_From_Key(const QString &key) {
    if (key == nullptr || key.isEmpty()) return 0;
    QStringList coordinates = key.split("x");
    return coordinates.at(1).toInt();
}
