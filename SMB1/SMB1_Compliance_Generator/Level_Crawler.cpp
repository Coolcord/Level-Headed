#include "Level_Crawler.h"
#include "Physics.h"
#include "../Common SMB1 Files/Object_Item_String.h"
#include <QTime>
#include <assert.h>

Level_Crawler::Level_Crawler(Brick::Brick brick) : SMB1_Compliance_Map() {
    this->brick = brick;
    this->badCoordinates = new QMap<QString, bool>();
    this->stream = NULL;
}

Level_Crawler::~Level_Crawler() {
    delete this->badCoordinates;
    delete this->stream;
}

bool Level_Crawler::Crawl_Level(QFile *file) {
    if (!file->isOpen() || !file->reset()) return false;

    //Create a text stream for the file
    if (this->stream) {
        delete this->stream;
        this->stream = NULL;
    }
    this->stream = new QTextStream(file);

    int x = 0;
    int previousX = x;
    int holeCrawlSteps = 0;
    //Read the Objects to Determine Safe Spots to Place Enemies
    QString line;
    do {
        line = this->stream->readLine();
        line.chop(1); //remove the newline character
        previousX = x;
        assert(this->Parse_Object(line, x, holeCrawlSteps));

        //Handle Step crawling for Holes
        if (holeCrawlSteps > 0) {
            holeCrawlSteps -= x - previousX;
            if (holeCrawlSteps < 0) holeCrawlSteps = 0;
        }

    } while (line != NULL && !file->atEnd());

    return true;
}

bool Level_Crawler::Find_Safe_Coordinate(int &x, int &y, int lastX) {
    return this->Find_Safe_Coordinate(1, x, y, lastX);
}

bool Level_Crawler::Find_Safe_Coordinate(int size, int &x, int &y, int lastX) {
    assert(size > 0);
    for (int i = x-lastX; i < 16; ++i) {
        int safeY = 0;
        if (this->Find_Safe_Coordinate_At_X(i, safeY)) {
            bool valid = true;
            for (int j = 0; j < size-1 && valid; ++j) {
                if (!this->Is_Coordinate_Safe(j, safeY)) valid = false;
            }
            if (valid) {
                //Safe Coordinate Found
                x = i;
                y = safeY;
                return true;
            }
        }

    }
    return false;
}

bool Level_Crawler::Find_Safe_Coordinate_At_X(int x, int &y) {
    for (int i = qrand() % 12, numChecked = 0; numChecked < 13; i = (i+1)%12, ++numChecked) {
        if (this->Is_Coordinate_Safe(x, i)) {
            y = i;
            return true;
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
    for (int i = spaces; i > 0; --i, ++x) {
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
            for (int i = 0; i < 3; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            }
            break;
        case Brick::SURFACE_AND_CEILING_4:
            this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1);
            for (int i = 0; i < 4; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            }
            break;
        case Brick::SURFACE_AND_CEILING_8:
            this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1);
            for (int i = 0; i < 8; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            }
            break;
        case Brick::SURFACE_4_AND_CEILING:
            for (int i = 0; i < 4; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1-i);
            }
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::SURFACE_4_AND_CEILING_3:
            for (int i = 0; i < 4; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1-i);
            }
            for (int i = 0; i < 3; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            }
            break;
        case Brick::SURFACE_4_AND_CEILING_4:
            for (int i = 0; i < 4; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1-i);
            }
            for (int i = 0; i < 4; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            }
            break;
        case Brick::SURFACE_5_AND_CEILING:
            for (int i = 0; i < 5; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1-i);
            }
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::CEILING:
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::SURFACE_5_AND_CEILING_4:
            for (int i = 0; i < 5; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1-i);
            }
            for (int i = 0; i < 4; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            }
            break;
        case Brick::SURFACE_8_AND_CEILING:
            for (int i = 0; i < 8; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1-i);
            }
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::SURFACE_AND_CEILING_AND_MIDDLE_5:
            this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1);
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            for (int i = 3; i < 8; ++i) {
                this->Mark_Bad_Coordinate(x, i);
            }
            break;
        case Brick::SURFACE_AND_CEILING_AND_MIDDLE_4:
            this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1);
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            for (int i = 3; i < 7; ++i) {
                this->Mark_Bad_Coordinate(x, i);
            }
            break;
        case Brick::ALL:
            for (int i = 0; i <= Physics::GROUND_Y+1; ++i) {
                this->Mark_Bad_Coordinate(x, i);
            }
            break;
        default: assert(false);
        }
    }
}

void Level_Crawler::Crawl_Forward_With_Hole(int x, int spaces) {
    assert(x >= 0);
    assert(spaces >= 0);
    for (int i = spaces; i > 0; --i, ++x) {
        switch (this->brick) {
        case Brick::NO_BRICKS:
            case Brick::SURFACE:
            continue; //Nothing to do
        case Brick::SURFACE_AND_CEILING:
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::SURFACE_AND_CEILING_3:
            for (int i = 0; i < 3; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            }
            break;
        case Brick::SURFACE_AND_CEILING_4:
            for (int i = 0; i < 4; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            }
            break;
        case Brick::SURFACE_AND_CEILING_8:
            for (int i = 0; i < 8; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            }
            break;
        case Brick::SURFACE_4_AND_CEILING:
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::SURFACE_4_AND_CEILING_3:
            for (int i = 0; i < 3; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            }
            break;
        case Brick::SURFACE_4_AND_CEILING_4:
            for (int i = 0; i < 4; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            }
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
            for (int i = 0; i < 4; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y+i);
            }
            break;
        case Brick::SURFACE_8_AND_CEILING:
            for (int i = 4; i < 8; ++i) {
                this->Mark_Bad_Coordinate(x, Physics::GROUND_Y+1-i);
            }
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            break;
        case Brick::SURFACE_AND_CEILING_AND_MIDDLE_5:
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            for (int i = 3; i < 8; ++i) {
                this->Mark_Bad_Coordinate(x, i);
            }
            break;
        case Brick::SURFACE_AND_CEILING_AND_MIDDLE_4:
            this->Mark_Bad_Coordinate(x, Physics::HIGHEST_Y);
            for (int i = 3; i < 7; ++i) {
                this->Mark_Bad_Coordinate(x, i);
            }
            break;
        case Brick::ALL:
            for (int i = 0; i <= Physics::GROUND_Y+1; ++i) {
                this->Mark_Bad_Coordinate(x, i);
            }
            break;
        default: assert(false);
        }
    }
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

bool Level_Crawler::Parse_Object(const QString &line, int &x, int &holeCrawlSteps) {
    QStringList elements = line.split(' ');
    assert(elements.at(0) == "O:");

    //Crawl forward according to the brick pattern
    bool valid = false;
    int steps = elements.at(2).toInt(&valid);
    assert(valid);
    if (holeCrawlSteps == 0) this->Crawl_Forward(x, steps);
    else this->Crawl_Forward_With_Hole(x, steps);
    x += steps;
    int y = 0;
    int length = 0;

    QMap<QString, Object_Item::Object_Item>::iterator iter = this->objects->find(elements.at(1));
    if (iter == this->objects->end()) return false; //not found
    switch (iter.value()) {
    case Object_Item::QUESTION_BLOCK_WITH_MUSHROOM:
    case Object_Item::QUESTION_BLOCK_WITH_COIN:
    case Object_Item::BRICK_WITH_MUSHROOM:
    case Object_Item::BRICK_WITH_VINE:
    case Object_Item::BRICK_WITH_STAR:
    case Object_Item::BRICK_WITH_10_COINS:
    case Object_Item::BRICK_WITH_1UP:
    case Object_Item::USED_BLOCK:
        valid = false;
        y = elements.at(3).toInt(&valid);
        assert(valid);
        this->Mark_Bad_Coordinate(x, y);
        return true;
    case Object_Item::UNDERWATER_SIDEWAYS_PIPE:
    case Object_Item::TRAMPOLINE:
        valid = false;
        y = elements.at(3).toInt(&valid);
        assert(valid);
        this->Mark_Bad_Coordinate(x, y);
        this->Mark_Bad_Coordinate(x, y+1);
        return true;
    case Object_Item::CANNON:
        valid = false;
        y = elements.at(3).toInt(&valid);
        assert(valid);
        valid = false;
        length = elements.at(4).toInt(&valid);
        assert(valid);
        for (int i = 0; i < length; ++i) {
            this->Mark_Bad_Coordinate(x, i);
        }
        return true;
    case Object_Item::ISLAND:
        valid = false;
        y = elements.at(3).toInt(&valid);
        assert(valid);
        valid = false;
        length = elements.at(4).toInt(&valid);
        assert(valid);
        for (int i = 0; i < length; ++i) {
            this->Mark_Bad_Coordinate(x+i, y);
        }
        return true;
    case Object_Item::HORIZONTAL_BRICKS:
    case Object_Item::HORIZONTAL_BLOCKS:
    case Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS:
        valid = false;
        y = elements.at(3).toInt(&valid);
        assert(valid);
        valid = false;
        length = elements.at(4).toInt(&valid);
        assert(valid);
        for (int i = 0; i < length; ++i) {
            this->Mark_Bad_Coordinate(x+i, y);
        }
        return true;
    case Object_Item::VERTICAL_BRICKS:
    case Object_Item::VERTICAL_BLOCKS:
        valid = false;
        y = elements.at(3).toInt(&valid);
        assert(valid);
        valid = false;
        length = elements.at(4).toInt(&valid);
        assert(valid);
        for (int i = 0; i < length; ++i) {
            this->Mark_Bad_Coordinate(x, y+i);
        }
        return true;
    case Object_Item::HORIZONTAL_COINS:
        valid = false;
        y = elements.at(3).toInt(&valid);
        assert(valid);
        valid = false;
        length = elements.at(4).toInt(&valid);
        assert(valid);
        for (int i = 0; i < length; ++i) {
            for (int j = y; j >= 0; --j) {
                this->Mark_Bad_Coordinate(x+i, j);
            }
        }
        return true;
    case Object_Item::PIPE:
    case Object_Item::ENTERABLE_PIPE:
        valid = false;
        y = elements.at(3).toInt(&valid);
        assert(valid);
        valid = false;
        length = elements.at(4).toInt(&valid);
        assert(valid);
        for (int i = 0; i < length; ++i) {
            this->Mark_Bad_Coordinate(x, y+i);
        }
        for (int i = 0; i < length; ++i) {
            this->Mark_Bad_Coordinate(x+1, y+i);
        }
        return true;
    case Object_Item::HOLE:
    case Object_Item::HOLE_WITH_WATER:
        valid = false;
        length = elements.at(4).toInt(&valid);
        assert(valid);
        holeCrawlSteps = length;
        return true;
    case Object_Item::BALANCE_ROPE:
        this->Clear_X(x);
        return true;
    case Object_Item::BRIDGE:
        valid = false;
        y = elements.at(3).toInt(&valid);
        assert(valid);
        valid = false;
        length = elements.at(4).toInt(&valid);
        assert(valid);
        for (int i = 0; i < length; ++i) {
            this->Mark_Bad_Coordinate(x+i, y);
        }
        return true;
    case Object_Item::PAGE_CHANGE:
        valid = false;
        length = elements.at(3).toInt(&valid);
        assert(valid);
        this->Crawl_Forward(x, (length*16)-x);
        x = length * 16;
        return true;
    case Object_Item::REVERSE_L_PIPE:
        this->Mark_Bad_Coordinate(x, Physics::GROUND_Y-1);
        this->Mark_Bad_Coordinate(x, Physics::GROUND_Y);
        this->Mark_Bad_Coordinate(x+1, Physics::GROUND_Y-1);
        this->Mark_Bad_Coordinate(x+1, Physics::GROUND_Y);
        for (int i = 0; i < 2; ++i) {
            for (int j = Physics::GROUND_Y; j > Physics::GROUND_Y-4; --j) {
                this->Mark_Bad_Coordinate(x+i, j);
            }
        }
        return true;
    case Object_Item::FLAGPOLE:
        this->Mark_Bad_X(x);
        return true;
    case Object_Item::CASTLE:
    case Object_Item::BIG_CASTLE:
        for (int i = 0; i < 5; ++i) {
            this->Mark_Bad_X(x+i);
        }
        return true;
    case Object_Item::AXE:
        this->Mark_Bad_Coordinate(x, 6);
        return true;
    case Object_Item::AXE_ROPE:
        this->Mark_Bad_Coordinate(x, 7);
        return true;
    case Object_Item::BOWSER_BRIDGE:
        for (int i = 0; i < 13; ++i) {
            this->Mark_Bad_Coordinate(x+i, 8);
        }
        return true;
    case Object_Item::LIFT_ROPE:
        this->Mark_Bad_X(x);
        return true;
    case Object_Item::STEPS:
        y = Physics::GROUND_Y;
        valid = false;
        length = elements.at(4).toInt(&valid);
        assert(valid);
        for (int i = 0; i < length; ++i) {
            for (int j = 0; j <= i; ++j) {
                this->Mark_Bad_Coordinate(x+i, y-j);
            }
        }
        return true;
    case Object_Item::END_STEPS:
        y = Physics::GROUND_Y;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j <= i; ++j) {
                this->Mark_Bad_Coordinate(x+i, y-j);
            }
        }
        for (int i = 0; i < 8; ++i) {
            this->Mark_Bad_Coordinate(x+8, y-i);
        }
        return true;
    case Object_Item::TALL_REVERSE_L_PIPE:
        valid = false;
        y = elements.at(3).toInt(&valid);
        assert(valid);
        this->Mark_Bad_Coordinate(x, y);
        this->Mark_Bad_Coordinate(x, y+1);
        this->Mark_Bad_Coordinate(x+1, y);
        this->Mark_Bad_Coordinate(x+1, y+1);
        this->Mark_Bad_X(x+2);
        this->Mark_Bad_X(x+3);
        return true;
    case Object_Item::PIPE_WALL:
        for (int i = 0; i < 2; ++i) {
            this->Mark_Bad_X(x+i);
        }
        return true;
    case Object_Item::HIDDEN_BLOCK_WITH_COIN:
    case Object_Item::HIDDEN_BLOCK_WITH_1UP:
    case Object_Item::BALANCE_LIFT_ROPE:
    case Object_Item::SCROLL_STOP:
    case Object_Item::SCROLL_STOP_WARP_ZONE:
    case Object_Item::FLYING_CHEEP_CHEEP_SPAWNER:
    case Object_Item::SWIMMING_CHEEP_CHEEP_SPAWNER:
    case Object_Item::BULLET_BILL_SPAWNER:
    case Object_Item::CANCEL_SPAWNER:
    case Object_Item::LOOP_COMMAND:
    case Object_Item::CHANGE_BRICK_AND_SCENERY:
    case Object_Item::CHANGE_BACKGROUND:
    case Object_Item::NOTHING:
        return true; //nothing to do
    default:
        assert(false);
        return false; //invalid object
    }
}
