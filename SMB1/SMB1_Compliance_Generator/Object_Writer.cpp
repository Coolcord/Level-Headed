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
    this->endObjectCount = Physics::MIN_END_OBJECTS;
    this->totalBytes = numBytesLeft;
    this->firstPageSafety = false;
}

int Object_Writer::Get_Last_Object_Length() {
    return this->lastObjectLength;
}

void Object_Writer::Increment_Last_Object_Length(int value) {
    this->lastObjectLength += value;
}

bool Object_Writer::Was_Last_Object_A_Platform() {
    return this->lastObjectIsPlatform;
}

int Object_Writer::Get_Num_Objects_Left() {
    return (this->Get_Num_Bytes_Left()/2);
}

int Object_Writer::Get_Num_Objects_Available() {
    return (this->Get_Num_Objects_Left()-this->endObjectCount);
}

bool Object_Writer::Is_Midpoint_Ready() {
    return ((this->Get_Num_Objects_Left()-Physics::MIN_END_OBJECTS) < (this->totalBytes/4)
            || this->currentPage > 0xA);
}

bool Object_Writer::Get_First_Page_Safety() {
    return this->firstPageSafety;
}

void Object_Writer::Set_First_Page_Safety(bool firstPageSafety) {
    this->firstPageSafety = firstPageSafety;
}

void Object_Writer::Set_End_Object_Count(int value) {
    this->endObjectCount = value;
}

bool Object_Writer::Write_Object(int x, const QString &object, bool platform) {
    if (this->Write_Item(OBJECT, x, QString(object+" "+QString::number(x)))) {
        this->lastObjectIsPlatform = platform;
        this->Handle_Zones(x);
        return true;
    } else {
        return false;
    }
}

bool Object_Writer::Write_Object(int x, const QString &object, int length, bool platform) {
    if (this->Write_Object(x, object, platform)) {
        this->lastObjectLength = length;
        return true;
    } else {
        return false;
    }
}

bool Object_Writer::Write_Object(int x, const QString &object, const QString &parameters, bool platform) {
    if (this->Write_Item(OBJECT, x, QString(object+" "+QString::number(x)+" "+parameters))) {
        this->lastObjectIsPlatform = platform;
        this->Handle_Zones(x);
        return true;
    } else {
        return false;
    }
}

bool Object_Writer::Write_Object(int x, const QString &object, const QString &parameters, int length, bool platform) {
    if (this->Write_Object(x, object, parameters, platform)) {
        this->lastObjectLength = length;
        return true;
    } else {
        return false;
    }
}

bool Object_Writer::Write_Object(int x, int y, const QString &object, int length, bool platform) {
    if (!this->Is_Y_Valid(y)) return false;
    if (this->Write_Item(OBJECT, x, QString(object+" "+QString::number(x)+" "+QString::number(y)))) {
        this->lastObjectIsPlatform = platform;
        this->Handle_Zones(x);
        this->currentY = y;
        this->lastObjectLength = length;
        return true;
    } else {
        return false;
    }
}

bool Object_Writer::Write_Object(int x, int y, const QString &object, const QString &parameters, int length, bool platform) {
    if (!this->Is_Y_Valid(y)) return false;
    if (this->Write_Item(OBJECT, x, QString(object+" "+QString::number(x)+" "+QString::number(y)+" "+parameters))) {
        this->lastObjectIsPlatform = platform;
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

bool Object_Writer::Is_Y_Valid(int y) {
    return (y >= 0x0 && y <= 0xF);
}

bool Object_Writer::Is_Coordinate_Valid(int coordinate) {
    if (this->firstPageSafety) {
        this->firstPageSafety = false;
        return (coordinate >= 0x00 && coordinate <= 0x1F);
    }
    return (coordinate >= 0x0 && coordinate <= 0x10);
}

bool Object_Writer::Question_Block_With_Mushroom(int x, int y) {
    if (y > 0xB) return false;
    if (this->powerupZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_QUESTION_BLOCK_WITH_MUSHROOM, Physics::MIN_OBJECT_LENGTH, true)) {
            this->powerupZone = this->MAX_POWERUP_ZONE;
            return true;
        } else {
            return false;
        }
    } else {
        return this->Question_Block_With_Coin(x, y);
    }
}

bool Object_Writer::Question_Block_With_Mushroom_Only(int x, int y) {
    if (y > 0xB) return false;
    if (this->powerupZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_QUESTION_BLOCK_WITH_MUSHROOM, Physics::MIN_OBJECT_LENGTH, true)) {
            this->powerupZone = this->MAX_POWERUP_ZONE;
            return true;
        }
    }
    return false;
}

bool Object_Writer::Question_Block_With_Coin(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, Object_Item::STRING_QUESTION_BLOCK_WITH_COIN, Physics::MIN_OBJECT_LENGTH, true);
}

bool Object_Writer::Hidden_Block_With_Coin(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, Object_Item::STRING_HIDDEN_BLOCK_WITH_COIN, Physics::MIN_OBJECT_LENGTH, true);
}

bool Object_Writer::Hidden_Block_With_1up(int x, int y) {
    if (y > 0xB) return false;
    if (this->powerupZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_HIDDEN_BLOCK_WITH_1UP, Physics::MIN_OBJECT_LENGTH, true)) {
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
    if (y > 0xB) return false;
    if (this->powerupZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_BRICK_WITH_MUSHROOM, Physics::MIN_OBJECT_LENGTH, true)) {
            this->powerupZone = this->MAX_POWERUP_ZONE;
            return true;
        } else {
            return false;
        }
    } else {
        return this->Horizontal_Bricks(x, y, Physics::MIN_OBJECT_LENGTH);
    }
}

bool Object_Writer::Brick_With_Mushroom_Only(int x, int y) {
    if (y > 0xB) return false;
    if (this->powerupZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_BRICK_WITH_MUSHROOM, Physics::MIN_OBJECT_LENGTH, true)) {
            this->powerupZone = this->MAX_POWERUP_ZONE;
            return true;
        }
    }
    return false;
}

bool Object_Writer::Brick_With_Star(int x, int y) {
    if (y > 0xB) return false;
    if (this->powerupZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_BRICK_WITH_STAR, Physics::MIN_OBJECT_LENGTH, true)) {
            this->powerupZone = this->MAX_POWERUP_ZONE;
            return true;
        } else {
            return false;
        }
    } else {
        return this->Horizontal_Bricks(x, y, Physics::MIN_OBJECT_LENGTH);
    }
}

bool Object_Writer::Brick_With_Star_Only(int x, int y) {
    if (y > 0xB) return false;
    if (this->powerupZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_BRICK_WITH_STAR, Physics::MIN_OBJECT_LENGTH, true)) {
            this->powerupZone = this->MAX_POWERUP_ZONE;
            return true;
        }
    }
    return false;
}

bool Object_Writer::Brick_With_10_Coins(int x, int y) {
    if (y > 0xB) return false;
    if (this->coinBlockZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_BRICK_WITH_10_COINS, Physics::MIN_OBJECT_LENGTH, true)) {
            this->coinBlockZone = this->MAX_COIN_BLOCK_ZONE;
            return true;
        } else {
            return false;
        }
    } else {
        return this->Horizontal_Bricks(x, y, Physics::MIN_OBJECT_LENGTH);
    }
}

bool Object_Writer::Brick_With_10_Coins_Only(int x, int y) {
    if (y > 0xB) return false;
    if (this->coinBlockZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_BRICK_WITH_10_COINS, Physics::MIN_OBJECT_LENGTH, true)) {
            this->coinBlockZone = this->MAX_COIN_BLOCK_ZONE;
            return true;
        }
    }
    return false;
}

bool Object_Writer::Brick_With_1up(int x, int y) {
    if (y > 0xB) return false;
    if (this->powerupZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_BRICK_WITH_1UP, Physics::MIN_OBJECT_LENGTH, true)) {
            this->powerupZone = this->MAX_POWERUP_ZONE;
            return true;
        } else {
            return false;
        }
    } else {
        return this->Horizontal_Bricks(x, y, Physics::MIN_OBJECT_LENGTH);
    }
}

bool Object_Writer::Brick_With_1up_Only(int x, int y) {
    if (y > 0xB) return false;
    if (this->powerupZone == 0) {
        if (this->Write_Object(x, y, Object_Item::STRING_BRICK_WITH_1UP, Physics::MIN_OBJECT_LENGTH, true)) {
            this->powerupZone = this->MAX_POWERUP_ZONE;
            return true;
        }
    }
    return false;
}

bool Object_Writer::Used_Block(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, Object_Item::STRING_USED_BLOCK, Physics::MIN_OBJECT_LENGTH, true);
}

bool Object_Writer::Trampoline(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, Object_Item::STRING_TRAMPOLINE, Physics::MIN_OBJECT_LENGTH, true);
}

bool Object_Writer::Cannon(int x, int y, int height) {
    if (y > 0xB) return false;
    if (height < 1 || height > 16) return false;
    return this->Write_Object(x, y, Object_Item::STRING_CANNON, QString::number(height), Physics::MIN_OBJECT_LENGTH, true);
}

bool Object_Writer::Island(int x, int y, int length) {
    if (y > 0xB) return false;
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, y, Object_Item::STRING_ISLAND, QString::number(length), length, true);
}

bool Object_Writer::Horizontal_Bricks(int x, int y, int length) {
    if (y > 0xB) return false;
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, y, Object_Item::STRING_HORIZONTAL_BRICKS, QString::number(length), length, true);
}

bool Object_Writer::Horizontal_Blocks(int x, int y, int length) {
    if (y > 0xB) return false;
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, y, Object_Item::STRING_HORIZONTAL_BLOCKS, QString::number(length), length, true);
}

bool Object_Writer::Horizontal_Coins(int x, int y, int length) {
    if (y > 0xB) return false;
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, y, Object_Item::STRING_HORIZONTAL_COINS, QString::number(length), length, false);
}

bool Object_Writer::Vertical_Bricks(int x, int y, int height) {
    if (y > 0xB) return false;
    if (height < 1 || height > 16) return false;
    return this->Write_Object(x, y, Object_Item::STRING_VERTICAL_BRICKS, QString::number(height), Physics::MIN_OBJECT_LENGTH, true);
}

bool Object_Writer::Vertical_Blocks(int x, int y, int height) {
    if (y > 0xB) return false;
    if (height < 1 || height > 16) return false;
    return this->Write_Object(x, y, Object_Item::STRING_VERTICAL_BLOCKS, QString::number(height), Physics::MIN_OBJECT_LENGTH, true);
}

bool Object_Writer::Pipe(int x, int y, int height) {
    if (y > 0xB) return false;
    if (height < 2 || height > 8) return false;
    return this->Write_Object(x, y, Object_Item::STRING_PIPE, QString::number(height), Physics::PIPE_LENGTH, true);
}

bool Object_Writer::Hole(int x, int length, bool filledWithWater) {
    if (length < 1 || length > 16) return false;
    if (filledWithWater) {
        return this->Write_Object(x, Object_Item::STRING_HOLE_WITH_WATER, QString::number(length), length, false);
    } else {
        return this->Write_Object(x, Object_Item::STRING_HOLE, QString::number(length), length, false);
    }
}

bool Object_Writer::Balance_Rope(int x, int length) {
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, Object_Item::STRING_BALANCE_ROPE, length, false);
}

bool Object_Writer::Bridge(int x, int yPlacement, int length) {
    if (length < 1 || length > 16) return false;
    if (yPlacement == 0x7 || yPlacement == 0x8 || yPlacement == 0xA) {
        return this->Write_Object(x, yPlacement, Object_Item::STRING_BRIDGE, QString::number(length), length, true);
    } else {
        return false;
    }
}

bool Object_Writer::Horizontal_Question_Blocks_With_Coins(int x, int yPlacement, int length) {
    if (length < 1 || length > 16) return false;
    if (yPlacement == 0x3 || yPlacement == 0x7) {
        return this->Write_Object(x, yPlacement, Object_Item::STRING_HORIZONTAL_QUESTION_BLOCKS_WITH_COINS, QString::number(length), length, true);
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
    if (this->Write_Object(0x0, Object_Item::STRING_PAGE_CHANGE, QString::number(page), false)) {
        return true; //page skip successful
    } else {
        //Restore previous stats
        this->currentX = tmpX;
        this->currentPage = tmpPage;
        this->levelLength = tmpLevelLength;
        return false;
    }
}

bool Object_Writer::Flagpole(int x) {
    return this->Write_Object(x, Object_Item::STRING_FLAGPOLE, Physics::MIN_OBJECT_LENGTH, false);
}

bool Object_Writer::Castle(int x) {
    return this->Write_Object(x, Object_Item::STRING_CASTLE, Physics::CASTLE_LENGTH, false);
}

bool Object_Writer::Big_Castle(int x) {
    return this->Write_Object(x, Object_Item::STRING_BIG_CASTLE, Physics::CASTLE_LENGTH, false);
}

bool Object_Writer::Axe(int x) {
    return this->Write_Object(x, Object_Item::STRING_AXE, Physics::MIN_OBJECT_LENGTH, false);
}

bool Object_Writer::Axe_Rope(int x) {
    return this->Write_Object(x, Object_Item::STRING_AXE_ROPE, Physics::MIN_OBJECT_LENGTH, false);
}

bool Object_Writer::Bowser_Bridge(int x) {
    return this->Write_Object(x, Object_Item::STRING_BOWSER_BRIDGE, Physics::BOWSER_BRIDGE_LENGTH, true);
}

bool Object_Writer::Scroll_Stop(int x, bool warpZone) {
    if (warpZone) {
        return this->Write_Object(x, Object_Item::STRING_SCROLL_STOP_WARP_ZONE, false);
    } else {
        return this->Write_Object(x, Object_Item::STRING_SCROLL_STOP, false);
    }
}

bool Object_Writer::Flying_Cheep_Cheep_Spawner(int x) {
    return this->Write_Object(x, Object_Item::STRING_FLYING_CHEEP_CHEEP_SPAWNER, false);
}

bool Object_Writer::Swimming_Cheep_Cheep_Spawner(int x) {
    return this->Write_Object(x, Object_Item::STRING_SWIMMING_CHEEP_CHEEP_SPAWNER, false);
}

bool Object_Writer::Bullet_Bill_Spawner(int x) {
    return this->Write_Object(x, Object_Item::STRING_BULLET_BILL_SPAWNER, false);
}

bool Object_Writer::Cancel_Spawner(int x) {
    return this->Write_Object(x, Object_Item::STRING_CANCEL_SPAWNER, false);
}

bool Object_Writer::Loop_Command(int x) {
    return this->Write_Object(x, Object_Item::STRING_LOOP_COMMAND, false);
}

bool Object_Writer::Change_Brick_And_Scenery(int x, Brick::Brick brick, Scenery::Scenery scenery) {
    QString property = "";
    switch (brick) {
    case Brick::NO_BRICKS:                          property = Brick::STRING_NO_BRICKS; break;
    case Brick::SURFACE:                            property = Brick::STRING_SURFACE; break;
    case Brick::SURFACE_AND_CEILING:                property = Brick::STRING_SURFACE_AND_CEILING; break;
    case Brick::SURFACE_AND_CEILING_3:              property = Brick::STRING_SURFACE_AND_CEILING_3; break;
    case Brick::SURFACE_AND_CEILING_4:              property = Brick::STRING_SURFACE_AND_CEILING_4; break;
    case Brick::SURFACE_AND_CEILING_8:              property = Brick::STRING_SURFACE_AND_CEILING_8; break;
    case Brick::SURFACE_4_AND_CEILING:              property = Brick::STRING_SURFACE_4_AND_CEILING; break;
    case Brick::SURFACE_4_AND_CEILING_3:            property = Brick::STRING_SURFACE_4_AND_CEILING_3; break;
    case Brick::SURFACE_4_AND_CEILING_4:            property = Brick::STRING_SURFACE_4_AND_CEILING_4; break;
    case Brick::SURFACE_5_AND_CEILING:              property = Brick::STRING_SURFACE_5_AND_CEILING; break;
    case Brick::CEILING:                            property = Brick::STRING_CEILING; break;
    case Brick::SURFACE_5_AND_CEILING_4:            property = Brick::STRING_SURFACE_5_AND_CEILING_4; break;
    case Brick::SURFACE_8_AND_CEILING:              property = Brick::STRING_SURFACE_8_AND_CEILING; break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_5:   property = Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_5; break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_4:   property = Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_4; break;
    case Brick::ALL:                                property = Brick::STRING_ALL; break;
    default:                                        return false;
    }
    property += " ";
    switch (scenery) {
    case Scenery::NO_SCENERY:   property += Scenery::STRING_NO_SCENERY; break;
    case Scenery::ONLY_CLOUDS:  property += Scenery::STRING_ONLY_CLOUDS; break;
    case Scenery::MOUNTAINS:    property += Scenery::STRING_MOUNTAINS; break;
    case Scenery::FENCES:       property += Scenery::STRING_FENCES; break;
    default:                    return false;
    }
    return this->Write_Object(x, Object_Item::STRING_CHANGE_BRICK_AND_SCENERY, property, false);
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
    return this->Write_Object(x, Object_Item::STRING_CHANGE_BACKGROUND, property, false);
}

bool Object_Writer::Lift_Rope(int x) {
    return this->Write_Object(x, Object_Item::STRING_LIFT_ROPE, Physics::MIN_OBJECT_LENGTH, false);
}

bool Object_Writer::Balance_Lift_Rope(int x, int length) {
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, Object_Item::STRING_BALANCE_LIFT_ROPE, QString::number(length), Physics::MIN_OBJECT_LENGTH, false);
}

bool Object_Writer::Steps(int x, int width) {
    if (width < 1 || width > 8) return false;
    return this->Write_Object(x, Object_Item::STRING_STEPS, QString::number(width), width, true);
}

bool Object_Writer::End_Steps(int x) {
    return this->Write_Object(x, Object_Item::STRING_END_STEPS, Physics::END_STAIRS_LENGTH, true);
}

bool Object_Writer::Pipe_Wall(int x) {
    return this->Write_Object(x, Object_Item::STRING_PIPE_WALL, Physics::PIPE_LENGTH, true);
}

bool Object_Writer::Nothing() {
    return this->Write_Object(0, Object_Item::STRING_NOTHING, false);
}
