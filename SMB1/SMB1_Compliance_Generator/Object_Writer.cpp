#include "Object_Writer.h"
#include "../Common SMB1 Files/Object_Item_String.h"
#include "../Common SMB1 Files/Background_String.h"
#include "../Common SMB1 Files/Brick_String.h"
#include "../Common SMB1 Files/Scenery_String.h"
#include "Physics.h"

Object_Writer::Object_Writer(QTextStream *stream, int numBytesLeft) : Item_Writer(stream, numBytesLeft) {
    this->lastObjectLength = 0;
    this->coinBlockZone = 0;
    this->powerupZone = 0;
}

int Object_Writer::Get_Last_Object_Length() {
    return this->lastObjectLength;
}

void Object_Writer::Increment_Last_Object_Length(int value) {
    this->lastObjectLength += value;
}

bool Object_Writer::Write_Object(int x, const QString &object) {
    if (this->Write_Item(OBJECT, x, QString(object+" "+QString::number(x)))) {
        this->Handle_Zones(x);
        return true;
    } else {
        return false;
    }
}

bool Object_Writer::Write_Object(int x, const QString &object, int length) {
    if (this->Write_Object(x, object)) {
        this->lastObjectLength = length;
        return true;
    } else {
        return false;
    }
}

bool Object_Writer::Write_Object(int x, const QString &object, const QString &parameters) {
    if (this->Write_Item(OBJECT, x, QString(object+" "+QString::number(x)+" "+parameters))) {
        this->Handle_Zones(x);
        return true;
    } else {
        return false;
    }
}

bool Object_Writer::Write_Object(int x, const QString &object, const QString &parameters, int length) {
    if (this->Write_Object(x, object, parameters)) {
        this->lastObjectLength = length;
        return true;
    } else {
        return false;
    }
}

bool Object_Writer::Write_Object(int x, int y, const QString &object, int length) {
    if (!this->Is_Coordinate_Valid(y)) return false;
    if (this->Write_Item(OBJECT, x, QString(object+" "+QString::number(x)+" "+QString::number(y)))) {
        this->Handle_Zones(x);
        this->currentY = y;
        this->lastObjectLength = length;
        return true;
    } else {
        return false;
    }
}

bool Object_Writer::Write_Object(int x, int y, const QString &object, const QString &parameters, int length) {
    if (!this->Is_Coordinate_Valid(y)) return false;
    if (this->Write_Item(OBJECT, x, QString(object+" "+QString::number(x)+" "+QString::number(y)+" "+parameters))) {
        this->Handle_Zones(x);
        this->currentY = y;
        this->lastObjectLength = length;
        return true;
    } else {
        return false;
    }
}

void Object_Writer::Handle_Zones(int x) {
    if (this->coinBlockZone > 0) this->coinBlockZone -= x;
    if (this->powerupZone > 0) this->powerupZone -= x;
    if (this->coinBlockZone < 0) this->coinBlockZone = 0;
    if (this->powerupZone < 0) this->powerupZone = 0;
}

bool Object_Writer::Question_Block_With_Mushroom(int x, int y) {
    if (this->powerupZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_QUESTION_BLOCK_WITH_MUSHROOM, Physics::MIN_OBJECT_LENGTH)) {
            this->powerupZone = this->MAX_POWERUP_ZONE;
            return true;
        } else {
            return false;
        }
    } else {
        return this->Question_Block_With_Coin(x, y);
    }
}

bool Object_Writer::Question_Block_With_Coin(int x, int y) {
    return this->Write_Object(x, y, Object_Item::STRING_QUESTION_BLOCK_WITH_COIN, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Writer::Hidden_Block_With_Coin(int x, int y) {
    return this->Write_Object(x, y, Object_Item::STRING_HIDDEN_BLOCK_WITH_COIN, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Writer::Hidden_Block_With_1up(int x, int y) {
    if (this->powerupZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_HIDDEN_BLOCK_WITH_1UP, Physics::MIN_OBJECT_LENGTH)) {
            this->powerupZone = this->MAX_POWERUP_ZONE;
            return true;
        } else {
            return false;
        }
    } else {
        return this->Hidden_Block_With_Coin(x, y);
    }
}

bool Object_Writer::Brick_With_Mushroom(int x, int y) {
    if (this->powerupZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_BRICK_WITH_MUSHROOM, Physics::MIN_OBJECT_LENGTH)) {
            this->powerupZone = this->MAX_POWERUP_ZONE;
            return true;
        } else {
            return false;
        }
    } else {
        return this->Horizontal_Bricks(x, y, Physics::MIN_OBJECT_LENGTH);
    }
}

bool Object_Writer::Brick_With_Star(int x, int y) {
    if (this->powerupZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_BRICK_WITH_STAR, Physics::MIN_OBJECT_LENGTH)) {
            this->powerupZone = this->MAX_POWERUP_ZONE;
            return true;
        } else {
            return false;
        }
    } else {
        return this->Horizontal_Bricks(x, y, Physics::MIN_OBJECT_LENGTH);
    }
}

bool Object_Writer::Brick_With_10_Coins(int x, int y) {
    if (this->coinBlockZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_BRICK_WITH_10_COINS, Physics::MIN_OBJECT_LENGTH)) {
            this->coinBlockZone = this->MAX_COIN_BLOCK_ZONE;
            return true;
        } else {
            return false;
        }
    } else {
        return this->Horizontal_Bricks(x, y, Physics::MIN_OBJECT_LENGTH);
    }
}

bool Object_Writer::Brick_With_1up(int x, int y) {
    if (this->powerupZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_BRICK_WITH_1UP, Physics::MIN_OBJECT_LENGTH)) {
            this->powerupZone = this->MAX_POWERUP_ZONE;
            return true;
        } else {
            return false;
        }
    } else {
        return this->Horizontal_Bricks(x, y, Physics::MIN_OBJECT_LENGTH);
    }
}

bool Object_Writer::Used_Block(int x, int y) {
    return this->Write_Object(x, y, Object_Item::STRING_USED_BLOCK, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Writer::Trampoline(int x, int y) {
    return this->Write_Object(x, y, Object_Item::STRING_TRAMPOLINE, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Writer::Cannon(int x, int y, int height) {
    if (height < 1 || height > 16) return false;
    return this->Write_Object(x, y, Object_Item::STRING_CANNON, QString::number(height), Physics::MIN_OBJECT_LENGTH);
}

bool Object_Writer::Island(int x, int y, int length) {
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, y, Object_Item::STRING_ISLAND, QString::number(length), length);
}

bool Object_Writer::Horizontal_Bricks(int x, int y, int length) {
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, y, Object_Item::STRING_HORIZONTAL_BRICKS, QString::number(length), length);
}

bool Object_Writer::Horizontal_Blocks(int x, int y, int length) {
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, y, Object_Item::STRING_HORIZONTAL_BLOCKS, QString::number(length), length);
}

bool Object_Writer::Horizontal_Coins(int x, int y, int length) {
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, y, Object_Item::STRING_HORIZONTAL_COINS, QString::number(length), length);
}

bool Object_Writer::Vertical_Bricks(int x, int y, int height) {
    if (height < 1 || height > 16) return false;
    return this->Write_Object(x, y, Object_Item::STRING_VERTICAL_BRICKS, QString::number(height), Physics::MIN_OBJECT_LENGTH);
}

bool Object_Writer::Vertical_Blocks(int x, int y, int height) {
    if (height < 1 || height > 16) return false;
    return this->Write_Object(x, y, Object_Item::STRING_VERTICAL_BLOCKS, QString::number(height), Physics::MIN_OBJECT_LENGTH);
}

bool Object_Writer::Pipe(int x, int y, int height) {
    if (height < 2 || height > 8) return false;
    return this->Write_Object(x, y, Object_Item::STRING_PIPE, QString::number(height), Physics::PIPE_LENGTH);
}

bool Object_Writer::Hole(int x, int length, bool filledWithWater) {
    if (length < 1 || length > 16) return false;
    if (filledWithWater) {
        return this->Write_Object(x, Object_Item::STRING_HOLE_WITH_WATER, QString::number(length), length);
    } else {
        return this->Write_Object(x, Object_Item::STRING_HOLE, QString::number(length), length);
    }
}

bool Object_Writer::Balance_Rope(int x, int length) {
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, Object_Item::STRING_BALANCE_ROPE, length);
}

bool Object_Writer::Bridge(int x, int yPlacement, int length) {
    if (length < 1 || length > 16) return false;
    if (yPlacement == 0x7 || yPlacement == 0x8 || yPlacement == 0xA) {
        return this->Write_Object(x, yPlacement, Object_Item::STRING_BRIDGE, QString::number(length), length);
    } else {
        return false;
    }
}

bool Object_Writer::Horizontal_Question_Blocks_With_Coins(int x, int yPlacement, int length) {
    if (length < 1 || length > 16) return false;
    if (yPlacement == 0x3 || yPlacement == 0x7) {
        return this->Write_Object(x, yPlacement, Object_Item::STRING_HORIZONTAL_QUESTION_BLOCKS_WITH_COINS, QString::number(length), length);
    } else {
        return false;
    }
}

bool Object_Writer::Page_Change(int page) {
    if (page < 0x00 || page > 0x3F) return false;
    int tmpX = this->currentX;
    int tmpPage = this->currentPage;
    int tmpLevelLength = this->levelLength;
    if (!this->Handle_Level_Length_On_Page_Change(page)) return false;
    if (this->Write_Object(0x0, Object_Item::STRING_PAGE_SKIP, QString::number(page))) {
        return true; //page skip successful
    } else {
        //Restore previous stats
        this->currentX = tmpX;
        this->currentPage = tmpPage;
        this->levelLength = tmpLevelLength;
        return false;
    }
}

bool Object_Writer::Reverse_L_Pipe(int x) {
    return this->Write_Object(x, Object_Item::STRING_REVERSE_L_PIPE);
}

bool Object_Writer::Flagpole(int x) {
    return this->Write_Object(x, Object_Item::STRING_FLAGPOLE, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Writer::Castle(int x) {
    return this->Write_Object(x, Object_Item::STRING_CASTLE);
}

bool Object_Writer::Big_Castle(int x) {
    return this->Write_Object(x, Object_Item::STRING_BIG_CASTLE);
}

bool Object_Writer::Axe(int x) {
    return this->Write_Object(x, Object_Item::STRING_AXE, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Writer::Axe_Rope(int x) {
    return this->Write_Object(x, Object_Item::STRING_AXE_ROPE, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Writer::Bowser_Bridge(int x) {
    return this->Write_Object(x, Object_Item::STRING_BOWSER_BRIDGE);
}

bool Object_Writer::Scroll_Stop(int x, bool warpZone) {
    if (warpZone) {
        return this->Write_Object(x, Object_Item::STRING_SCROLL_STOP_WARP_ZONE);
    } else {
        return this->Write_Object(x, Object_Item::STRING_SCROLL_STOP);
    }
}

bool Object_Writer::Flying_Cheep_Cheep_Spawner(int x) {
    return this->Write_Object(x, Object_Item::STRING_FLYING_CHEEP_CHEEP_SPAWNER);
}

bool Object_Writer::Swimming_Cheep_Cheep_Spawner(int x) {
    return this->Write_Object(x, Object_Item::STRING_SWIMMING_CHEEP_CHEEP_SPAWNER);
}

bool Object_Writer::Bullet_Bill_Spawner(int x) {
    return this->Write_Object(x, Object_Item::STRING_BULLET_BILL_SPAWNER);
}

bool Object_Writer::Cancel_Spawner(int x) {
    return this->Write_Object(x, Object_Item::STRING_CANCEL_SPAWNER);
}

bool Object_Writer::Loop_Command(int x) {
    return this->Write_Object(x, Object_Item::STRING_LOOP_COMMAND);
}

bool Object_Writer::Change_Brick_And_Scenery(int x, Brick::Brick brick, Scenery::Scenery scenery) {
    QString property = "";
    switch (scenery) {
    case Scenery::NO_SCENERY:   property = Scenery::STRING_NO_SCENERY; break;
    case Scenery::ONLY_CLOUDS:  property = Scenery::STRING_ONLY_CLOUDS; break;
    case Scenery::MOUNTAINS:    property = Scenery::STRING_MOUNTAINS; break;
    case Scenery::FENCES:       property = Scenery::STRING_FENCES; break;
    default:                    return false;
    }
    property += " ";
    switch (brick) {
    case Brick::NO_BRICKS:                          property += Brick::STRING_NO_BRICKS; break;
    case Brick::SURFACE:                            property += Brick::STRING_SURFACE; break;
    case Brick::SURFACE_AND_CEILING:                property += Brick::STRING_SURFACE_AND_CEILING; break;
    case Brick::SURFACE_AND_CEILING_3:              property += Brick::STRING_SURFACE_AND_CEILING_3; break;
    case Brick::SURFACE_AND_CEILING_4:              property += Brick::STRING_SURFACE_AND_CEILING_4; break;
    case Brick::SURFACE_AND_CEILING_8:              property += Brick::STRING_SURFACE_AND_CEILING_8; break;
    case Brick::SURFACE_4_AND_CEILING:              property += Brick::STRING_SURFACE_4_AND_CEILING; break;
    case Brick::SURFACE_4_AND_CEILING_3:            property += Brick::STRING_SURFACE_4_AND_CEILING_3; break;
    case Brick::SURFACE_4_AND_CEILING_4:            property += Brick::STRING_SURFACE_4_AND_CEILING_4; break;
    case Brick::SURFACE_5_AND_CEILING:              property += Brick::STRING_SURFACE_5_AND_CEILING; break;
    case Brick::CEILING:                            property += Brick::STRING_CEILING; break;
    case Brick::SURFACE_5_AND_CEILING_4:            property += Brick::STRING_SURFACE_5_AND_CEILING_4; break;
    case Brick::SURFACE_8_AND_CEILING:              property += Brick::STRING_SURFACE_8_AND_CEILING; break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_5:   property += Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_5; break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_4:   property += Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_4; break;
    case Brick::ALL:                                property += Brick::STRING_ALL; break;
    default:                                        return false;
    }
    return this->Write_Object(x, Object_Item::STRING_CHANGE_BRICK_AND_SCENERY, property);
}

bool Object_Writer::Change_Background(int x, Background::Background background) {
    QString property = "";
    switch (background) {
    case Background::BLANK_BACKGROUND:  property = Background::STRING_BLANK_BACKGROUND; break;
    case Background::IN_WATER:          property = Background::STRING_IN_WATER; break;
    case Background::CASTLE_WALL:       property = Background::STRING_CASTLE_WALL; break;
    case Background::OVER_WATER:        property = Background::STRING_OVER_WATER; break;
    case Background::NIGHT:             property = Background::STRING_NIGHT; break;
    case Background::SNOW:              property = Background::STRING_SNOW; break;
    case Background::NIGHT_AND_SNOW:    property = Background::STRING_NIGHT_AND_SNOW; break;
    case Background::NIGHT_AND_FREEZE:  property = Background::STRING_NIGHT_AND_FREEZE; break;
    default:                            return false;
    }
    return this->Write_Object(x, Object_Item::STRING_CHANGE_BACKGROUND, property);
}

bool Object_Writer::Lift_Rope(int x) {
    return this->Write_Object(x, Object_Item::STRING_LIFT_ROPE);
}

bool Object_Writer::Balance_Lift_Rope(int x, int length) {
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, Object_Item::STRING_BALANCE_LIFT_ROPE, QString::number(length));
}

bool Object_Writer::Steps(int x, int width) {
    if (width < 1 || width > 8) return false;
    return this->Write_Object(x, Object_Item::STRING_STEPS, QString::number(width));
}

bool Object_Writer::End_Steps(int x) {
    return this->Write_Object(x, Object_Item::STRING_END_STEPS);
}

bool Object_Writer::Pipe_Wall(int x) {
    return this->Write_Object(x, Object_Item::STRING_PIPE_WALL);
}

bool Object_Writer::Nothing() {
    return this->Write_Object(0, Object_Item::STRING_NOTHING);
}
