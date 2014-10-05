#include "Level_Crawler.h"
#include "Physics.h"
#include "../Common SMB1 Files/Object_Item_String.h"
#include <QTime>
#include <QDebug>
#include <assert.h>
#include <QVector>

Level_Crawler::Level_Crawler(QFile *file) : SMB1_Compliance_Map() {
    assert(file);
    this->file = file;
    this->endDetected = false;
    this->safeSize = 0;
    this->badCoordinates = new QMap<QString, bool>();
}

Level_Crawler::~Level_Crawler() {
    delete this->badCoordinates;
}

bool Level_Crawler::Crawl_Level(Brick::Brick startingBrick) {
    if (!this->file || !this->file->isOpen() || !this->file->isReadable()) return false;
    if (!this->file->reset()) return false;
    this->brick = startingBrick;
    this->file = file;
    this->endDetected = false;
    this->safeSize = 0;

    int x = 0;
    int holeCrawlSteps = 0;
    //Read the Objects to Determine Safe Spots to Place Enemies
    QString line;
    do {
        line = this->file->readLine();
        line.chop(1);

        assert(this->Parse_Object(line, x, holeCrawlSteps));

        if (!endDetected) safeSize = x-1;

    } while (line != NULL && !this->file->atEnd());

    return true;
    //return this->Draw_Map();
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
        for (int i = lastX+16; i >= x; --i) {
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
        for (int i = x; i <= lastX+16; ++i) {
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
    if (reverse) {
        for (int i = lastX+16; i <= x; --i) {
            if (this->Is_Coordinate_Safe(i, y)) ++numValid;
            else numValid = 0;
            if (numValid == size) {
                x = i;
                return true;
            }
        }
    } else {
        for (int i = x; i <= lastX+16; ++i) {
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

void Level_Crawler::Crawl_Forward_With_Hole(int x, int spaces, int &holeCrawlSteps) {
    assert(x >= 0);
    assert(spaces >= 0);
    for (int i = spaces; i > 0; --i, ++x, --holeCrawlSteps) {
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
    if (line == NULL || line.isEmpty()) return false;
    QStringList elements = line.split(' ');
    if (elements.at(0) != "O:") return false;

    //Crawl forward according to the brick pattern
    bool valid = false;
    int steps = elements.at(2).toInt(&valid);
    assert(valid);
    if (holeCrawlSteps == 0) this->Crawl_Forward(x, steps);
    else this->Crawl_Forward_With_Hole(x, steps, holeCrawlSteps);
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
        length = elements.at(3).toInt(&valid);
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
        this->endDetected = true;
        this->Mark_Bad_X(x);
        return true;
    case Object_Item::CASTLE:
    case Object_Item::BIG_CASTLE:
        for (int i = 0; i < 5; ++i) {
            this->Mark_Bad_X(x+i);
        }
        return true;
    case Object_Item::AXE:
        this->endDetected = true;
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
        length = elements.at(3).toInt(&valid);
        assert(valid);
        for (int i = 0; i < length; ++i) {
            for (int j = 0; j <= i; ++j) {
                this->Mark_Bad_Coordinate(x+i, y-j);
            }
        }
        return true;
    case Object_Item::END_STEPS:
        this->endDetected = true;
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

bool Level_Crawler::Draw_Map() {
    QFile file("E:/Level-Headed/build-Level-Headed-Desktop_Qt_5_3_MinGW_32bit-Debug/debug/dump.txt");
    if (!file.open(QFile::ReadWrite | QFile::Truncate)) return false;
    QTextStream stream(&file);

    int maxX = this->Get_Safe_Size();
    bool map[maxX][13];
    for (int i = 0; i < maxX; ++i) {
        for (int j = 0; j < 13; ++j) {
            map[i][j] = false;
        }
    }

    //Determine what to draw
    QVector<QString> keys = this->badCoordinates->keys().toVector();
    for (int i = 0; i < keys.length(); ++i) {
        int x = this->Get_X_From_Key(keys.at(i));
        int y = this->Get_Y_From_Key(keys.at(i));
        if (x == -1 || y == -1) continue;
        map[x][y] = true;
    }

    //Draw the map
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < maxX; ++j) {
            if (map[j][i]) {
                stream << "1";
            } else {
                stream << " ";
            }
        }
        stream << "\n";
    }

    stream.flush();
    file.close();
    return true;
}

int Level_Crawler::Get_X_From_Key(const QString &key) {
    if (key == NULL || key.isEmpty()) return -1;
    QStringList coordinates = key.split("x");
    return coordinates.at(0).toInt();
}

int Level_Crawler::Get_Y_From_Key(const QString &key) {
    if (key == NULL || key.isEmpty()) return 0;
    QStringList coordinates = key.split("x");
    return coordinates.at(1).toInt();
}