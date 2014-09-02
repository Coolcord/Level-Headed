#include "Object_Writer.h"
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

}

bool Object_Writer::Island(int x, int y, int length) {

}

bool Object_Writer::Horizontal_Bricks(int x, int y, int length) {
    if (length > 16 || length < 1) return false;
    return this->Write_Object(x, y, 0x2, length);
}

bool Object_Writer::Horizontal_Blocks(int x, int y, int length) {
    if (length > 16 || length < 1) return false;
    return this->Write_Object(x, y, 0x3, length);
}

bool Object_Writer::Horizontal_Coins(int x, int y, int length) {
    if (length > 16 || length < 1) return false;
    return this->Write_Object(x, y, 0x4, length);
}

bool Object_Writer::Vertical_Bricks(int x, int y, int height) {
    if (height > 16 || height < 1) return false;
    return this->Write_Object(x, y, 0x5, height);
}

bool Object_Writer::Vertical_Blocks(int x, int y, int height) {
    if (height > 16 || height < 1) return false;
    return this->Write_Object(x, y, 0x6, height);
}

bool Object_Writer::Pipe(int x, int y, int height, bool enterable = false) {
    if (height > 9 || height < 2) return false;
    if (enterable) height += 8; //set the bit
    return this->Write_Object(x, y, 0x7, height);
}

bool Object_Writer::Hole(int x, int length, bool filledWithWater = false) {
    if (length > 16 || length < 1) return false;
    if (filledWithWater) return this->Write_Object(x, 0xC, 0x5, length);
    else return this->Write_Object(x, 0xC, 0x0, length);
}

bool Object_Writer::Balance_Rope(int x, int length) {

}

bool Object_Writer::Bridge(int x, int yPlacement, int length) {

}

bool Object_Writer::Horizontal_Question_Blocks_With_Coins(int x, int yPlacement, int length) {

}

bool Object_Writer::Page_Skip(int x, int amount) {

}

bool Object_Writer::Reverse_L_Pipe(int x) {

}

bool Object_Writer::Flagpole(int x) {

}

bool Object_Writer::Axe(int x) {

}

bool Object_Writer::Axe_Rope(int x) {

}

bool Object_Writer::Bowser_Bridge(int x) {

}

bool Object_Writer::Scroll_Stop(int x, bool warpZone) {

}

bool Object_Writer::Flying_Cheep_Cheep_Spawner(int x) {

}

bool Object_Writer::Swimming_Cheep_Cheep_Spawner(int x) {

}

bool Object_Writer::Bullet_Bill_Spawner(int x) {

}

bool Object_Writer::Cancel_Spawner(int x) {

}

bool Object_Writer::Loop_Command(int x) {

}

bool Object_Writer::Change_Brick_And_Scenery(int x, Brick brick, Scenery scenery) {

}

bool Object_Writer::Change_Background(int x, Background background) {

}

bool Object_Writer::Lift_Rope(int x) {

}

bool Object_Writer::Balance_Lift_Rope(int x, int length) {

}

bool Object_Writer::Steps(int x, int width) {

}

bool Object_Writer::End_Steps(int x) {

}

bool Object_Writer::Tall_Reverse_L_Pipe(int x, int yPlacement) {

}

bool Object_Writer::Pipe_Wall(int x) {

}

bool Object_Writer::Nothing(int x) {

}
