#include "Header_Writer.h"
#include "Binary_Manipulator.h"
#include <assert.h>

Header_Writer::Header_Writer(QByteArray *buffer) {
    assert(buffer);
    assert(buffer->size() == 2);
    this->buffer = buffer;
}

int Header_Writer::Get_Time() {
    //Get the bits
    QBitArray tmp = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->data()[0]));
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
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->data()[0]));
    return bits.testBit(2); //get the bit
}

Level_Attribute::Level_Attribute Header_Writer::Get_Starting_Position(){
    //Get the bits
    QBitArray tmp = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->data()[0]));
    QBitArray bits(2);
    for (int i = 3; i < 5; ++i) {
        bits.setBit(i, tmp.testBit(i));
    }

    //Determine what the data is
    int data = Binary_Manipulator::BitArray_To_Hex(bits);
    switch (data) {
    case 0x0:   return Level_Attribute::UNDERWATER;
    case 0x1:   return Level_Attribute::UNDERGROUND;
    case 0x2:   return Level_Attribute::OVERWORLD;
    case 0x3:   return Level_Attribute::CASTLE;
    default:    assert(false); return Level_Attribute::OVERWORLD;
    }
}

Background::Background Header_Writer::Get_Background() {
    //Get the bits
    QBitArray tmp = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->data()[0]));
    QBitArray bits(3);
    for (int i = 5; i < 8; ++i) {
        bits.setBit(i, tmp.testBit(i));
    }

    //Determine what the data is
    int data = Binary_Manipulator::BitArray_To_Hex(bits);
    switch (data) {
    case 0x0:   return Background::BLANK_BACKGROUND;
    case 0x1:   return Background::IN_WATER;
    case 0x2:   return Background::CASTLE_WALL;
    case 0x3:   return Background::OVER_WATER;
    case 0x4:   return Background::NIGHT;
    case 0x5:   return Background::SNOW;
    case 0x6:   return Background::NIGHT_AND_SNOW;
    case 0x7:   return Background::NIGHT_AND_FREEZE;
    default:    assert(false); return Background::BLANK_BACKGROUND;
    }
}

Level_Compliment::Level_Compliment Header_Writer::Get_Level_Compliment() {
    //Get the bits
    QBitArray tmp = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->data()[1]));
    QBitArray bits(2);
    for (int i = 0; i < 2; ++i) {
        bits.setBit(i, tmp.testBit(i));
    }

    //Determine what the data is
    int data = Binary_Manipulator::BitArray_To_Hex(bits);
    switch (data) {
    case 0x0:   return Level_Compliment::TREES;
    case 0x1:   return Level_Compliment::MUSHROOMS;
    case 0x2:   return Level_Compliment::BULLET_BILL_TURRETS;
    case 0x3:   return Level_Compliment::CLOUDS;
    default:    assert(false); return Level_Compliment::TREES;
    }
}

Scenery::Scenery Header_Writer::Get_Scenery() {
    //Get the bits
    QBitArray tmp = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->data()[1]));
    QBitArray bits(2);
    for (int i = 2; i < 4; ++i) {
        bits.setBit(i, tmp.testBit(i));
    }

    //Determine what the data is
    int data = Binary_Manipulator::BitArray_To_Hex(bits);
    switch (data) {
    case 0x0:   return Scenery::NO_SCENERY;
    case 0x1:   return Scenery::ONLY_CLOUDS;
    case 0x2:   return Scenery::MOUNTAINS;
    case 0x3:   return Scenery::FENCES;
    default:    assert(false); return Scenery::NO_SCENERY;
    }
}

Brick::Brick Header_Writer::Get_Brick() {
    //Get the bits
    QBitArray tmp = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->data()[1]));
    QBitArray bits(4);
    for (int i = 4; i < 8; ++i) {
        bits.setBit(i, tmp.testBit(i));
    }

    //Determine what the data is
    int data = Binary_Manipulator::BitArray_To_Hex(bits);
    switch (data) {
    case 0x0:   return Brick::NO_BRICKS;
    case 0x1:   return Brick::SURFACE;
    case 0x2:   return Brick::SURFACE_AND_CEILING;
    case 0x3:   return Brick::SURFACE_AND_CEILING_3;
    case 0x4:   return Brick::SURFACE_4_AND_CEILING_4;
    case 0x5:   return Brick::SURFACE_AND_CEILING_8;
    case 0x6:   return Brick::SURFACE_4_AND_CEILING;
    case 0x7:   return Brick::SURFACE_4_AND_CEILING_3;
    case 0x8:   return Brick::SURFACE_4_AND_CEILING_4;
    case 0x9:   return Brick::SURFACE_5_AND_CEILING;
    case 0xA:   return Brick::CEILING;
    case 0xB:   return Brick::SURFACE_5_AND_CEILING_4;
    case 0xC:   return Brick::SURFACE_8_AND_CEILING;
    case 0xD:   return Brick::SURFACE_AND_CEILING_AND_MIDDLE_5;
    case 0xE:   return Brick::SURFACE_AND_CEILING_AND_MIDDLE_4;
    case 0xF:   return Brick::ALL;
    default:    assert(false); return Brick::NO_BRICKS;
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
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->data()[0]));
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(bits, 0, static_cast<unsigned char>(hexDigit), 2, 3);
    int byte = Binary_Manipulator::BitArray_To_Hex(bits);
    this->buffer->data()[0] = static_cast<char>(byte);
    return true;
}

void Header_Writer::Set_Autowalk(bool value) {
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->data()[0]));
    bits.setBit(2, value); //set the bit
    int byte = Binary_Manipulator::BitArray_To_Hex(bits);
    this->buffer->data()[0] = static_cast<char>(byte);
}

bool Header_Writer::Set_Starting_Position(Level_Attribute::Level_Attribute value) {
    //Determine what to write
    int hexDigit = 0x0;
    switch (value) {
    case Level_Attribute::UNDERWATER:    hexDigit = 0x0; break;
    case Level_Attribute::OVERWORLD:     hexDigit = 0x1; break;
    case Level_Attribute::UNDERGROUND:   hexDigit = 0x2; break;
    case Level_Attribute::CASTLE:        hexDigit = 0x3; break;
    default:            return false;
    }

    //Write the data
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->data()[0]));
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(bits, 3, static_cast<unsigned char>(hexDigit), 2, 3);
    unsigned char byte = Binary_Manipulator::BitArray_To_Hex(bits);
    this->buffer->data()[0] = static_cast<char>(byte);
    return true;
}

bool Header_Writer::Set_Background(Background::Background value) {
    //Determine what to write
    int hexDigit = 0x0;
    switch (value) {
    case Background::BLANK_BACKGROUND:  hexDigit = 0x0; break;
    case Background::IN_WATER:          hexDigit = 0x1; break;
    case Background::CASTLE_WALL:       hexDigit = 0x2; break;
    case Background::OVER_WATER:        hexDigit = 0x3; break;
    case Background::NIGHT:             hexDigit = 0x4; break;
    case Background::SNOW:              hexDigit = 0x5; break;
    case Background::NIGHT_AND_SNOW:    hexDigit = 0x6; break;
    case Background::NIGHT_AND_FREEZE:  hexDigit = 0x7; break;
    default:                return false;
    }

    //Write the data
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->data()[0]));
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(bits, 5, static_cast<unsigned char>(hexDigit), 1, 3);
    unsigned char byte = Binary_Manipulator::BitArray_To_Hex(bits);
    this->buffer->data()[0] = static_cast<char>(byte);
    return true;
}

bool Header_Writer::Set_Level_Compliment(Level_Compliment::Level_Compliment value) {
    //Determine what to write
    int hexDigit = 0x0;
    switch (value) {
    case Level_Compliment::TREES:               hexDigit = 0x0; break;
    case Level_Compliment::MUSHROOMS:           hexDigit = 0x1; break;
    case Level_Compliment::BULLET_BILL_TURRETS: hexDigit = 0x2; break;
    case Level_Compliment::CLOUDS:              hexDigit = 0x3; break;
    default:                                    return false;
    }

    //Write the data
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->data()[1]));
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(bits, 0, static_cast<unsigned char>(hexDigit), 2, 3);
    unsigned char byte = Binary_Manipulator::BitArray_To_Hex(bits);
    this->buffer->data()[1] = static_cast<char>(byte);
    return true;
}

bool Header_Writer::Set_Scenery(Scenery::Scenery value) {
    //Determine what to write
    int hexDigit = 0x0;
    switch (value) {
    case Scenery::NO_SCENERY:    hexDigit = 0x0; break;
    case Scenery::ONLY_CLOUDS:   hexDigit = 0x1; break;
    case Scenery::MOUNTAINS:     hexDigit = 0x2; break;
    case Scenery::FENCES:        hexDigit = 0x3; break;
    default:            return false;
    }

    //Write the data
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->data()[1]));
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(bits, 2, static_cast<unsigned char>(hexDigit), 2, 3);
    unsigned char byte = Binary_Manipulator::BitArray_To_Hex(bits);
    this->buffer->data()[1] = static_cast<char>(byte);
    return true;
}

bool Header_Writer::Set_Brick(Brick::Brick value) {
    //Determine what to write
    int hexDigit = 0x0;
    switch (value) {
    case Brick::NO_BRICKS:                         hexDigit = 0x0; break;
    case Brick::SURFACE:                           hexDigit = 0x1; break;
    case Brick::SURFACE_AND_CEILING:               hexDigit = 0x2; break;
    case Brick::SURFACE_AND_CEILING_3:             hexDigit = 0x3; break;
    case Brick::SURFACE_AND_CEILING_4:             hexDigit = 0x4; break;
    case Brick::SURFACE_AND_CEILING_8:             hexDigit = 0x5; break;
    case Brick::SURFACE_4_AND_CEILING:             hexDigit = 0x6; break;
    case Brick::SURFACE_4_AND_CEILING_3:           hexDigit = 0x7; break;
    case Brick::SURFACE_4_AND_CEILING_4:           hexDigit = 0x8; break;
    case Brick::SURFACE_5_AND_CEILING:             hexDigit = 0x9; break;
    case Brick::CEILING:                           hexDigit = 0xA; break;
    case Brick::SURFACE_5_AND_CEILING_4:           hexDigit = 0xB; break;
    case Brick::SURFACE_8_AND_CEILING:             hexDigit = 0xC; break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_5:  hexDigit = 0xD; break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_4:  hexDigit = 0xE; break;
    case Brick::ALL:                               hexDigit = 0xF; break;
    default:                                return false;
    }

    //Write the data
    QBitArray bits = Binary_Manipulator::Hex_To_BitArray(static_cast<unsigned char>(this->buffer->data()[1]));
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(bits, 4, static_cast<unsigned char>(hexDigit));
    unsigned char byte = Binary_Manipulator::BitArray_To_Hex(bits);
    this->buffer->data()[1] = static_cast<char>(byte);
    return true;
}
