#include "Object_Writer.h"
#include "Header_Writer.h"
#include <assert.h>

bool Object_Writer::Is_Safe_To_Write_Object() {
    return (this->How_Many_Bytes_Left() >= 2);
}

bool Object_Writer::Write_Object(int x, int y, int objectByte) {
    if (!this->Is_Safe_To_Write_Object()) return false;
    if (!this->Write_Coordinates(x, y)) return false;
    if (this->pageFlag) {
        objectByte += 128; //set the page flag
        assert(objectByte <= 0xFF);
    }
    return this->Write_Byte_To_Buffer(objectByte);
}

bool Object_Writer::Write_Object(int x, int y, int firstObjectHexDigit, int secondObjectHexDigit) {
    if (!Binary_Manipulator::Is_Valid_Hex_Digit(firstObjectHexDigit)) return false;
    if (!Binary_Manipulator::Is_Valid_Hex_Digit(secondObjectHexDigit)) return false;
    QBitArray objectBits(8, false);
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(objectBits, 0, firstObjectHexDigit); //write the object
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(objectBits, 0, secondObjectHexDigit); //write the properties
    return this->Write_Object(x, y, Binary_Manipulator::BitArray_To_Hex(objectBits));
}

bool Object_Writer::Question_Block_With_Mushroom(int x, int y) {
    return this->Write_Object(x, y, 0x00);
}

bool Object_Writer::Question_Block_With_Coin(int x, int y) {
    return this->Write_Object(x, y, 0x01);
}

bool Object_Writer::Hidden_Block_With_Coin(int x, int y) {
    return this->Write_Object(x, y, 0x02);
}

bool Object_Writer::Hidden_Block_With_1up(int x, int y) {
    return this->Write_Object(x, y, 0x03);
}

bool Object_Writer::Brick_With_Mushroom(int x, int y) {
    return this->Write_Object(x, y, 0x04);
}

bool Object_Writer::Brick_With_Vine(int x, int y) {
    return this->Write_Object(x, y, 0x05);
}

bool Object_Writer::Brick_With_Star(int x, int y) {
    return this->Write_Object(x, y, 0x06);
}

bool Object_Writer::Brick_With_10_Coins(int x, int y) {
    return this->Write_Object(x, y, 0x07);
}

bool Object_Writer::Brick_With_1up(int x, int y) {
    return this->Write_Object(x, y, 0x08);
}

bool Object_Writer::Underwater_Sideways_Pipe(int x, int y) {
    return this->Write_Object(x, y, 0x09);
}

bool Object_Writer::Used_Block(int x, int y) {
    return this->Write_Object(x, y, 0x0A);
}

bool Object_Writer::Trampoline(int x, int y) {
    return this->Write_Object(x, y, 0x0B);
}

bool Object_Writer::Cannon(int x, int y, int height) {
    if (height > 16 || height < 1) return false;
    Level_Compliment levelCompliment = this->headerWriter->Get_Level_Compliment();
    if (levelCompliment == BULLET_BILL_TURRETS) {
        return this->Write_Object(x, y, 0x1, height);
    } else {
        return false;
    }
}

bool Object_Writer::Island(int x, int y, int length) {
    if (length > 16 || length < 1) return false;
    Level_Compliment levelCompliment = this->headerWriter->Get_Level_Compliment();
    if (levelCompliment != BULLET_BILL_TURRETS) {
        return this->Write_Object(x, y, 0x1, length);
    } else {
        return false;
    }
}

bool Object_Writer::Horizontal_Bricks(int x, int y, int length) {
    if (length > 16 || length < 1) return false;
    return this->Write_Object(x, y, 0x2, length-1);
}

bool Object_Writer::Horizontal_Blocks(int x, int y, int length) {
    if (length > 16 || length < 1) return false;
    return this->Write_Object(x, y, 0x3, length-1);
}

bool Object_Writer::Horizontal_Coins(int x, int y, int length) {
    if (length > 16 || length < 1) return false;
    return this->Write_Object(x, y, 0x4, length-1);
}

bool Object_Writer::Vertical_Bricks(int x, int y, int height) {
    if (height > 16 || height < 1) return false;
    return this->Write_Object(x, y, 0x5, height-1);
}

bool Object_Writer::Vertical_Blocks(int x, int y, int height) {
    if (height > 16 || height < 1) return false;
    return this->Write_Object(x, y, 0x6, height-1);
}

bool Object_Writer::Pipe(int x, int y, int height, bool enterable = false) {
    if (height > 9 || height < 2) return false;
    if (enterable) height += 8; //set the bit
    return this->Write_Object(x, y, 0x7, height);
}

bool Object_Writer::Hole(int x, int length, bool filledWithWater = false) {
    if (length > 16 || length < 1) return false;
    if (filledWithWater) return this->Write_Object(x, 0xC, 0x5, length-1);
    else return this->Write_Object(x, 0xC, 0x0, length-1);
}

bool Object_Writer::Balance_Rope(int x, int length) {
    if (length > 16 || length < 1) return false;
    return this->Write_Object(x, 0xC, 0x1, length-1);
}

bool Object_Writer::Bridge(int x, int yPlacement, int length) {
    if (length > 16 || length < 1) return false;
    if (yPlacement == 0x7) return this->Write_Object(x, 0xC, 0x2, length-1);
    else if (yPlacement == 0x8) return this->Write_Object(x, 0xC, 0x3, length-1);
    else if (yPlacement == 0xA) return this->Write_Object(x, 0xC, 0x4, length-1);
    return false;
}

bool Object_Writer::Horizontal_Question_Blocks_With_Coins(int x, int yPlacement, int length) {
    if (length > 16 || length < 1) return false;
    if (yPlacement == 0x3) return this->Write_Object(x, 0xC, 0x6, length-1);
    else if (yPlacement == 0x7) return this->Write_Object(x, 0xC, 0x7, length-1);
    return false;
}

bool Object_Writer::Page_Skip(int x, int amount) {
    if (amount >= 0x00 && amount <= 0x0F) return this->Write_Object(x, 0xD, 0x0, amount);
    else if (amount >= 0x10 && amount <= 0x1F) return this->Write_Object(x, 0xD, 0x1, amount);
    else if (amount >= 0x20 && amount <= 0x2F) return this->Write_Object(x, 0xD, 0x2, amount);
    else if (amount >= 0x30 && amount <= 0x3F) return this->Write_Object(x, 0xD, 0x3, amount);
    return false;
}

bool Object_Writer::Reverse_L_Pipe(int x) {
    return this->Write_Object(x, 0xD, 0x40);
}

bool Object_Writer::Flagpole(int x) {
    return this->Write_Object(x, 0xD, 0x41);
}

bool Object_Writer::Axe(int x) {
    return this->Write_Object(x, 0xD, 0x42);
}

bool Object_Writer::Axe_Rope(int x) {
    return this->Write_Object(x, 0xD, 0x43);
}

bool Object_Writer::Bowser_Bridge(int x) {
    return this->Write_Object(x, 0xD, 0x44);
}

bool Object_Writer::Scroll_Stop(int x, bool warpZone) {
    if (warpZone) return this->Write_Object(x, 0xD, 0x45);
    else return this->Write_Object(x, 0xD, 0x46);
}

bool Object_Writer::Flying_Cheep_Cheep_Spawner(int x) {
    return this->Write_Object(x, 0xD, 0x48);
}

bool Object_Writer::Swimming_Cheep_Cheep_Spawner(int x) {

}

bool Object_Writer::Bullet_Bill_Spawner(int x) {

}

bool Object_Writer::Cancel_Spawner(int x) {
    return this->Write_Object(x, 0xD, 0x4A);
}

bool Object_Writer::Loop_Command(int x) {
    return this->Write_Object(x, 0xD, 0x4B);
}

bool Object_Writer::Change_Brick_And_Scenery(int x, Brick brick, Scenery scenery) {
    int firstHexDigit = 0x0;
    int secondHexDigit = 0x0;
    switch (scenery) {
    case NO_SCENERY:    firstHexDigit = 0x0; break;
    case ONLY_CLOUDS:   firstHexDigit = 0x1; break;
    case MOUNTAINS:     firstHexDigit = 0x2; break;
    case FENCES:        firstHexDigit = 0x3; break;
    default:            return false;
    }
    switch (brick) {
    case NO_BRICKS:                         secondHexDigit = 0x0; break;
    case SURFACE:                           secondHexDigit = 0x1; break;
    case SURFACE_AND_CEILING:               secondHexDigit = 0x2; break;
    case SURFACE_AND_CEILING_3:             secondHexDigit = 0x3; break;
    case SURFACE_AND_CEILING_4:             secondHexDigit = 0x4; break;
    case SURFACE_AND_CEILING_8:             secondHexDigit = 0x5; break;
    case SURFACE_4_AND_CEILING:             secondHexDigit = 0x6; break;
    case SURFACE_4_AND_CEILING_3:           secondHexDigit = 0x7; break;
    case SURFACE_4_AND_CEILING_4:           secondHexDigit = 0x8; break;
    case SURFACE_5_AND_CEILING:             secondHexDigit = 0x9; break;
    case CEILING:                           secondHexDigit = 0xA; break;
    case SURFACE_5_AND_CEILING_4:           secondHexDigit = 0xB; break;
    case SURFACE_8_AND_CEILING:             secondHexDigit = 0xC; break;
    case SURFACE_AND_CEILING_AND_MIDDLE_5:  secondHexDigit = 0xD; break;
    case SURFACE_AND_CEILING_AND_MIDDLE_4:  secondHexDigit = 0xE; break;
    case ALL:                               secondHexDigit = 0xF; break;
    default:                                return false;
    }
    return this->Write_Object(x, 0xE, firstHexDigit, secondHexDigit);
}

bool Object_Writer::Change_Background(int x, Background background) {
    int property = 0x0;
    switch (background) {
    case BLANK_BACKGROUND:  property = 0x0; break;
    case IN_WATER:          property = 0x1; break;
    case CASTLE_WALL:       property = 0x2; break;
    case OVER_WATER:        property = 0x3; break;
    case NIGHT:             property = 0x4; break;
    case SNOW:              property = 0x5; break;
    case NIGHT_AND_SNOW:    property = 0x6; break;
    case NIGHT_AND_FREEZE:  property = 0x7; break;
    default:                return false;
    }
    return this->Write_Object(x, 0xE, 0x4, property);
}

bool Object_Writer::Lift_Rope(int x) {
    return this->Write_Object(x, 0xF, 0x00);
}

bool Object_Writer::Balance_Lift_Rope(int x, int length) {
    if (length > 16 || length < 1) return false;
    return this->Write_Object(x, 0xF, 0x1, length-1);
}

bool Object_Writer::Steps(int x, int width) {
    if (width < 1 || width > 8) return false;
    return this->Write_Object(x, 0xF, 0x3, width-1);
}

bool Object_Writer::End_Steps(int x) {
    return this->Write_Object(x, 0xF, 0x38);
}

bool Object_Writer::Tall_Reverse_L_Pipe(int x, int yPlacement) {
    if (yPlacement < 0x1 || yPlacement > 0xA) return false;
    return this->Write_Object(x, 0xF, 0x4, yPlacement);
}

bool Object_Writer::Pipe_Wall(int x) {
    return this->Write_Object(x, 0xF, 0x4F);
}

bool Object_Writer::Nothing(int x) {
    return this->Write_Object(x, 0xF, 0x60);
}
