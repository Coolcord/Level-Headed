#include "Header_Writer.h"
#include "Binary_Manipulator.h"
#include <assert.h>

Header_Writer::Header_Writer(QByteArray *buffer)
{
    assert(buffer);
    assert(buffer->size() == 2);
    this->buffer = buffer;
}

int Header_Writer::Get_Time() {
    //Get the bits
    QBitArray tmp = Binary_Manipulator::Hex_To_BitArray(static_cast<int>(this->buffer->data()[0]));
    QBitArray bits(2);
    for (int i = 0; i < 2; ++i) {
        bits.setBit(i, tmp.testBit(i));
    }

    //Determine what the data is
    int data = Binary_Manipulator::BitArray_To_Hex(bits);
    switch (data) {
    case 0x0:   return 0;
    case 0x1:   return 400;
    case 0x2:   return 300;
    case 0x3:   return 200;
    default:    assert(false); return -1;
    }
}

bool Header_Writer::Get_Autowalk() {
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<int>(this->buffer->data()[0]));
    return bits.testBit(2); //get the bit
}

Level_Attribute Header_Writer::Get_Starting_Position(){
    //Get the bits
    QBitArray tmp = Binary_Manipulator::Hex_To_BitArray(static_cast<int>(this->buffer->data()[0]));
    QBitArray bits(2);
    for (int i = 3; i < 5; ++i) {
        bits.setBit(i, tmp.testBit(i));
    }

    //Determine what the data is
    int data = Binary_Manipulator::BitArray_To_Hex(bits);
    switch (data) {
    case 0x0:   return UNDERWATER;
    case 0x1:   return UNDERGROUND;
    case 0x2:   return OVERWORLD;
    case 0x3:   return CASTLE;
    default:    assert(false); return OVERWORLD;
    }
}

Background Header_Writer::Get_Background() {
    //Get the bits
    QBitArray tmp = Binary_Manipulator::Hex_To_BitArray(static_cast<int>(this->buffer->data()[0]));
    QBitArray bits(3);
    for (int i = 5; i < 8; ++i) {
        bits.setBit(i, tmp.testBit(i));
    }

    //Determine what the data is
    int data = Binary_Manipulator::BitArray_To_Hex(bits);
    switch (data) {
    case 0x0:   return BLANK_BACKGROUND;
    case 0x1:   return IN_WATER;
    case 0x2:   return CASTLE_WALL;
    case 0x3:   return OVER_WATER;
    case 0x4:   return NIGHT;
    case 0x5:   return SNOW;
    case 0x6:   return NIGHT_AND_SNOW;
    case 0x7:   return NIGHT_AND_FREEZE;
    default:    assert(false); return BLANK_BACKGROUND;
    }
}

Level_Compliment Header_Writer::Get_Level_Compliment() {
    //Get the bits
    QBitArray tmp = Binary_Manipulator::Hex_To_BitArray(static_cast<int>(this->buffer->data()[1]));
    QBitArray bits(2);
    for (int i = 0; i < 2; ++i) {
        bits.setBit(i, tmp.testBit(i));
    }

    //Determine what the data is
    int data = Binary_Manipulator::BitArray_To_Hex(bits);
    switch (data) {
    case 0x0:   return TREES;
    case 0x1:   return MUSHROOMS;
    case 0x2:   return BULLET_BILL_TURRETS;
    case 0x3:   return CLOUDS;
    default:    assert(false); return TREES;
    }
}

Scenery Header_Writer::Get_Scenery() {
    //Get the bits
    QBitArray tmp = Binary_Manipulator::Hex_To_BitArray(static_cast<int>(this->buffer->data()[1]));
    QBitArray bits(2);
    for (int i = 2; i < 4; ++i) {
        bits.setBit(i, tmp.testBit(i));
    }

    //Determine what the data is
    int data = Binary_Manipulator::BitArray_To_Hex(bits);
    switch (data) {
    case 0x0:   return NO_SCENERY;
    case 0x1:   return ONLY_CLOUDS;
    case 0x2:   return MOUNTAINS;
    case 0x3:   return FENCES;
    default:    assert(false); return NO_SCENERY;
    }
}

Brick Header_Writer::Get_Brick() {
    //Get the bits
    QBitArray tmp = Binary_Manipulator::Hex_To_BitArray(static_cast<int>(this->buffer->data()[1]));
    QBitArray bits(4);
    for (int i = 4; i < 8; ++i) {
        bits.setBit(i, tmp.testBit(i));
    }

    //Determine what the data is
    int data = Binary_Manipulator::BitArray_To_Hex(bits);
    switch (data) {
    case 0x0:   return NO_BRICKS;
    case 0x1:   return SURFACE;
    case 0x2:   return SURFACE_AND_CEILING;
    case 0x3:   return SURFACE_AND_CEILING_3;
    case 0x4:   return SURFACE_4_AND_CEILING_4;
    case 0x5:   return SURFACE_AND_CEILING_8;
    case 0x6:   return SURFACE_4_AND_CEILING;
    case 0x7:   return SURFACE_4_AND_CEILING_3;
    case 0x8:   return SURFACE_4_AND_CEILING_4;
    case 0x9:   return SURFACE_5_AND_CEILING;
    case 0xA:   return CEILING;
    case 0xB:   return SURFACE_5_AND_CEILING_4;
    case 0xC:   return SURFACE_8_AND_CEILING;
    case 0xD:   return SURFACE_AND_CEILING_AND_MIDDLE_5;
    case 0xE:   return SURFACE_AND_CEILING_AND_MIDDLE_4;
    case 0xF:   return ALL;
    default:    assert(false); return NO_BRICKS;
    }
}

bool Header_Writer::Set_Time(int value) {
    //Determine what to write
    int hexDigit = 0x0;
    switch (value) {
    case 0:     hexDigit = 0x0; break;
    case 400:   hexDigit = 0x1; break;
    case 300:   hexDigit = 0x2; break;
    case 200:   hexDigit = 0x3; break;
    default:    return false;
    }

    //Write the data
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<int>(this->buffer->data()[0]));
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(bits, 0, hexDigit, 0, 1);
    int byte = Binary_Manipulator::BitArray_To_Hex(bits);
    this->buffer->data()[0] = static_cast<char>(byte);
    return true;
}

void Header_Writer::Set_Autowalk(bool value) {
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<int>(this->buffer->data()[0]));
    bits.setBit(2, value); //set the bit
    int byte = Binary_Manipulator::BitArray_To_Hex(bits);
    this->buffer->data()[0] = static_cast<char>(byte);
}

bool Header_Writer::Set_Starting_Position(Level_Attribute value) {
    //Determine what to write
    int hexDigit = 0x0;
    switch (value) {
    case OVERWORLD:     hexDigit = 0x2; break;
    case UNDERGROUND:   hexDigit = 0x1; break;
    case UNDERWATER:    hexDigit = 0x0; break;
    case CASTLE:        hexDigit = 0x3; break;
    default:            return false;
    }

    //Write the data
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<int>(this->buffer->data()[0]));
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(bits, 2, hexDigit, 0, 1);
    int byte = Binary_Manipulator::BitArray_To_Hex(bits);
    this->buffer->data()[0] = static_cast<char>(byte);
    return true;
}

bool Header_Writer::Set_Background(Background value) {
    //Determine what to write
    int hexDigit = 0x0;
    switch (value) {
    case BLANK_BACKGROUND:  hexDigit = 0x0; break;
    case IN_WATER:          hexDigit = 0x1; break;
    case CASTLE_WALL:       hexDigit = 0x2; break;
    case OVER_WATER:        hexDigit = 0x3; break;
    case NIGHT:             hexDigit = 0x4; break;
    case SNOW:              hexDigit = 0x5; break;
    case NIGHT_AND_SNOW:    hexDigit = 0x6; break;
    case NIGHT_AND_FREEZE:  hexDigit = 0x7; break;
    default:                return false;
    }

    //Write the data
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<int>(this->buffer->data()[0]));
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(bits, 5, hexDigit, 0, 2);
    int byte = Binary_Manipulator::BitArray_To_Hex(bits);
    this->buffer->data()[0] = static_cast<char>(byte);
    return true;
}

bool Header_Writer::Set_Level_Compliment(Level_Compliment value) {
    //Determine what to write
    int hexDigit = 0x0;
    switch (value) {
    case TREES:                 hexDigit = 0x0; break;
    case MUSHROOMS:             hexDigit = 0x1; break;
    case BULLET_BILL_TURRETS:   hexDigit = 0x2; break;
    case CLOUDS:                hexDigit = 0x3; break;
    default:                    return false;
    }

    //Write the data
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<int>(this->buffer->data()[1]));
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(bits, 0, hexDigit, 0, 1);
    int byte = Binary_Manipulator::BitArray_To_Hex(bits);
    this->buffer->data()[1] = static_cast<char>(byte);
    return true;
}

bool Header_Writer::Set_Scenery(Scenery value) {
    //Determine what to write
    int hexDigit = 0x0;
    switch (value) {
    case NO_SCENERY:    hexDigit = 0x0; break;
    case ONLY_CLOUDS:   hexDigit = 0x1; break;
    case MOUNTAINS:     hexDigit = 0x2; break;
    case FENCES:        hexDigit = 0x3; break;
    default:            return false;
    }

    //Write the data
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<int>(this->buffer->data()[1]));
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(bits, 2, hexDigit, 0, 1);
    int byte = Binary_Manipulator::BitArray_To_Hex(bits);
    this->buffer->data()[1] = static_cast<char>(byte);
    return true;
}

bool Header_Writer::Set_Brick(Brick value) {
    //Determine what to write
    int hexDigit = 0x0;
    switch (value) {
    case NO_BRICKS:                         hexDigit = 0x0; break;
    case SURFACE:                           hexDigit = 0x1; break;
    case SURFACE_AND_CEILING:               hexDigit = 0x2; break;
    case SURFACE_AND_CEILING_3:             hexDigit = 0x3; break;
    case SURFACE_AND_CEILING_4:             hexDigit = 0x4; break;
    case SURFACE_AND_CEILING_8:             hexDigit = 0x5; break;
    case SURFACE_4_AND_CEILING:             hexDigit = 0x6; break;
    case SURFACE_4_AND_CEILING_3:           hexDigit = 0x7; break;
    case SURFACE_4_AND_CEILING_4:           hexDigit = 0x8; break;
    case SURFACE_5_AND_CEILING:             hexDigit = 0x9; break;
    case CEILING:                           hexDigit = 0xA; break;
    case SURFACE_5_AND_CEILING_4:           hexDigit = 0xB; break;
    case SURFACE_8_AND_CEILING:             hexDigit = 0xC; break;
    case SURFACE_AND_CEILING_AND_MIDDLE_5:  hexDigit = 0xD; break;
    case SURFACE_AND_CEILING_AND_MIDDLE_4:  hexDigit = 0xE; break;
    case ALL:                               hexDigit = 0xF; break;
    default:                                return false;
    }

    //Write the data
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<int>(this->buffer->data()[1]));
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(bits, 4, hexDigit);
    int byte = Binary_Manipulator::BitArray_To_Hex(bits);
    this->buffer->data()[1] = static_cast<char>(byte);
    return true;
}
