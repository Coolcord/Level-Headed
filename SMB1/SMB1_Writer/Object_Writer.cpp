#include "Object_Writer.h"
#include "Header_Writer.h"
#include "Room_ID_Handler.h"
#include "../Common_SMB1_Files/Level_Attribute.h"
#include <assert.h>

Object_Writer::Object_Writer(QByteArray *b, Header_Writer *hw, Room_ID_Handler *ridh) : Item_Writer(b, hw, ridh) {}

bool Object_Writer::Write_Object(int x, int y, int objectByte) {
    return this->Write_Item(x, y, objectByte);
}

bool Object_Writer::Write_Object(int x, int y, int firstObjectHexDigit, int secondObjectHexDigit) {
    if (!Binary_Manipulator::Is_Valid_Hex_Digit(static_cast<unsigned char>(firstObjectHexDigit))) return false;
    if (!Binary_Manipulator::Is_Valid_Hex_Digit(static_cast<unsigned char>(secondObjectHexDigit))) return false;
    QBitArray objectBits(8, false);
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(objectBits, 0, static_cast<unsigned char>(firstObjectHexDigit)); //write the object
    Binary_Manipulator::Write_Hex_Digit_To_BitArray(objectBits, 4, static_cast<unsigned char>(secondObjectHexDigit)); //write the properties
    return this->Write_Object(x, y, Binary_Manipulator::BitArray_To_Hex(objectBits));
}

bool Object_Writer::Fill_Buffer() {
    while (this->Is_Safe_To_Write_Item()) {
        assert(this->Nothing(0));
    }
    return true;
}

bool Object_Writer::Question_Block_With_Mushroom(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, 0x00);
}

bool Object_Writer::Question_Block_With_Coin(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, 0x01);
}

bool Object_Writer::Hidden_Block_With_Coin(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, 0x02);
}

bool Object_Writer::Hidden_Block_With_1up(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, 0x03);
}

bool Object_Writer::Brick_With_Mushroom(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, 0x04);
}

bool Object_Writer::Brick_With_Vine(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, 0x05);
}

bool Object_Writer::Brick_With_Star(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, 0x06);
}

bool Object_Writer::Brick_With_10_Coins(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, 0x07);
}

bool Object_Writer::Brick_With_1up(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, 0x08);
}

bool Object_Writer::Underwater_Sideways_Pipe(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, 0x09);
}

bool Object_Writer::Used_Block(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, 0x0A);
}

bool Object_Writer::Trampoline(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(x, y, 0x0B);
}

bool Object_Writer::Bullet_Bill_Turret(int x, int y, int height) {
    if (y > 0xB) return false;
    if (height > 16 || height < 1) return false;
    Level_Compliment::Level_Compliment levelCompliment = this->headerWriter->Get_Level_Compliment();
    if (levelCompliment == Level_Compliment::BULLET_BILL_TURRETS) {
        return this->Write_Object(x, y, 0x1, height-1);
    } else {
        return false;
    }
}

bool Object_Writer::Island(int x, int y, int length) {
    if (y > 0xB) return false;
    if (length > 16 || length < 1) return false;
    Level_Compliment::Level_Compliment levelCompliment = this->headerWriter->Get_Level_Compliment();
    if (levelCompliment != Level_Compliment::BULLET_BILL_TURRETS) {
        return this->Write_Object(x, y, 0x1, length-1);
    } else {
        return false;
    }
}

bool Object_Writer::Horizontal_Bricks(int x, int y, int length) {
    if (y > 0xB) return false;
    if (length > 16 || length < 1) return false;
    return this->Write_Object(x, y, 0x2, length-1);
}

bool Object_Writer::Horizontal_Blocks(int x, int y, int length) {
    if (y > 0xB) return false;
    if (length > 16 || length < 1) return false;
    return this->Write_Object(x, y, 0x3, length-1);
}

bool Object_Writer::Horizontal_Coins(int x, int y, int length) {
    if (y > 0xB) return false;
    if (length > 16 || length < 1) return false;
    return this->Write_Object(x, y, 0x4, length-1);
}

bool Object_Writer::Vertical_Bricks(int x, int y, int height) {
    if (y > 0xB) return false;
    if (height > 16 || height < 1) return false;
    if (this->roomIDHandler->Get_Level_Attribute_From_Current_Level() == Level_Attribute::UNDERWATER) return false;
    return this->Write_Object(x, y, 0x5, height-1);
}

bool Object_Writer::Vertical_Blocks(int x, int y, int height) {
    if (y > 0xB) return false;
    if (height > 16 || height < 1) return false;
    return this->Write_Object(x, y, 0x6, height-1);
}

bool Object_Writer::Coral(int x, int y, int height) {
    if (y > 0xB) return false;
    if (height > 16 || height < 1) return false;
    if (this->roomIDHandler->Get_Level_Attribute_From_Current_Level() != Level_Attribute::UNDERWATER) return false;
    return this->Write_Object(x, y, 0x5, height-1);
}

bool Object_Writer::Pipe(int x, int y, int height, bool enterable) {
    if (y > 0xB) return false;
    if (height > 8 || height < 2) return false;
    --height;
    if (enterable) height += 8; //set the bit
    return this->Write_Object(x, y, 0x7, height);
}

bool Object_Writer::Hole(int x, int length, bool filledWithWater) {
    if (length > 16 || length < 1) return false;
    if (filledWithWater) return this->Write_Object(x, 0xC, 0x5, length-1);
    else return this->Write_Object(x, 0xC, 0x0, length-1);
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

bool Object_Writer::Page_Change(int page) {
    bool success = false;
    int tmpX = this->currentX;
    int tmpY = this->currentY;
    int tmpPage = this->currentPage;
    this->currentX = 0;
    this->currentY = 0;
    this->currentPage = page;
    if (page >= 0x00 && page <= 0x0F) {
        success = this->Write_Object(0x0, 0xD, 0x0, page);
    } else if (page >= 0x10 && page <= 0x1F) {
        page -= 0x10;
        success = this->Write_Object(0x0, 0xD, 0x1, page);
    } else if (page >= 0x20 && page <= 0x2F) {
        page -= 0x20;
        success = this->Write_Object(0x0, 0xD, 0x2, page);
    } else if (page >= 0x30 && page <= 0x3F) {
        page -= 0x30;
        success = this->Write_Object(0x0, 0xD, 0x3, page);
    }
    if (!success) {
        this->currentX = tmpX;
        this->currentY = tmpY;
        this->currentPage = tmpPage;
    }
    return success;
}

bool Object_Writer::Reverse_L_Pipe(int x) {
    if (this->currentX+x == 0xF) return false;
    return this->Write_Object(x, 0xD, 0x40);
}

bool Object_Writer::Flagpole(int x) {
    return this->Write_Object(x, 0xA, 0x0D);
}

bool Object_Writer::Castle(int x) {
    return this->Write_Object(x, 0xF, 0x26);
}

bool Object_Writer::Big_Castle(int x) {
    return this->Write_Object(x, 0xF, 0x20);
}

bool Object_Writer::Axe(int x) {
    if (this->currentX+x == 0xF) return false;
    return this->Write_Object(x, 0xD, 0x42);
}

bool Object_Writer::Axe_Rope(int x) {
    if (this->currentX+x == 0xF) return false;
    return this->Write_Object(x, 0xD, 0x43);
}

bool Object_Writer::Bowser_Bridge(int x) {
    if (this->currentX+x == 0xF) return false;
    return this->Write_Object(x, 0xD, 0x44);
}

bool Object_Writer::Scroll_Stop(int x, bool warpZone) {
    if (this->currentX+x == 0xF) return false;
    if (warpZone) return this->Write_Object(x, 0xD, 0x45);
    return this->Write_Object(x, 0xD, 0x46);
}

bool Object_Writer::Toggle_Auto_Scroll(int x) {
    if (this->currentX+x == 0xF) return false;
    return this->Write_Object(x, 0xD, 0x47);
}

bool Object_Writer::Flying_Cheep_Cheep_Spawner(int x) {
    if (this->currentX+x == 0xF) return false;
    return this->Write_Object(x, 0xD, 0x48);
}

bool Object_Writer::Swimming_Cheep_Cheep_Spawner(int x) {
    if (this->currentX+x == 0xF) return false;
    if (this->roomIDHandler->Get_Level_Attribute_From_Current_Level() != Level_Attribute::UNDERWATER) return false;
    return this->Write_Object(x, 0xD, 0x49);
}

bool Object_Writer::Bullet_Bill_Spawner(int x) {
    if (this->currentX+x == 0xF) return false;
    if (this->roomIDHandler->Get_Level_Attribute_From_Current_Level() == Level_Attribute::UNDERWATER) return false;
    return this->Write_Object(x, 0xD, 0x49);
}

bool Object_Writer::Cancel_Spawner(int x) {
    if (this->currentX+x == 0xF) return false;
    return this->Write_Object(x, 0xD, 0x4A);
}

bool Object_Writer::Loop_Command(int x) {
    if (this->currentX+x == 0xF) return false;
    return this->Write_Object(x, 0xD, 0x4B);
}

bool Object_Writer::Change_Brick_And_Scenery(int x, Brick::Brick brick, Scenery::Scenery scenery) {
    int firstHexDigit = 0x0;
    int secondHexDigit = 0x0;
    switch (scenery) {
    case Scenery::NO_SCENERY:    firstHexDigit = 0x0; break;
    case Scenery::ONLY_CLOUDS:   firstHexDigit = 0x1; break;
    case Scenery::MOUNTAINS:     firstHexDigit = 0x2; break;
    case Scenery::FENCES:        firstHexDigit = 0x3; break;
    }
    switch (brick) {
    case Brick::NO_BRICKS:                         secondHexDigit = 0x0; break;
    case Brick::SURFACE:                           secondHexDigit = 0x1; break;
    case Brick::SURFACE_AND_CEILING:               secondHexDigit = 0x2; break;
    case Brick::SURFACE_AND_CEILING_3:             secondHexDigit = 0x3; break;
    case Brick::SURFACE_AND_CEILING_4:             secondHexDigit = 0x4; break;
    case Brick::SURFACE_AND_CEILING_8:             secondHexDigit = 0x5; break;
    case Brick::SURFACE_4_AND_CEILING:             secondHexDigit = 0x6; break;
    case Brick::SURFACE_4_AND_CEILING_3:           secondHexDigit = 0x7; break;
    case Brick::SURFACE_4_AND_CEILING_4:           secondHexDigit = 0x8; break;
    case Brick::SURFACE_5_AND_CEILING:             secondHexDigit = 0x9; break;
    case Brick::CEILING:                           secondHexDigit = 0xA; break;
    case Brick::SURFACE_5_AND_CEILING_4:           secondHexDigit = 0xB; break;
    case Brick::SURFACE_8_AND_CEILING:             secondHexDigit = 0xC; break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_5:  secondHexDigit = 0xD; break;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_4:  secondHexDigit = 0xE; break;
    case Brick::ALL:                               secondHexDigit = 0xF; break;
    }
    return this->Write_Object(x, 0xE, firstHexDigit, secondHexDigit);
}

bool Object_Writer::Change_Background(int x, Background::Background background) {
    int property = 0x0;
    switch (background) {
    case Background::BLANK_BACKGROUND:  property = 0x0; break;
    case Background::IN_WATER:          property = 0x1; break;
    case Background::CASTLE_WALL:       property = 0x2; break;
    case Background::OVER_WATER:        property = 0x3; break;
    case Background::NIGHT:             property = 0x4; break;
    case Background::SNOW:              property = 0x5; break;
    case Background::NIGHT_AND_SNOW:    property = 0x6; break;
    case Background::NIGHT_AND_FREEZE:  property = 0x7; break;
    }
    return this->Write_Object(x, 0xE, 0x4, property);
}

bool Object_Writer::Lift_Rope(int x) {
    return this->Write_Object(x, 0xF, 0x00);
}

bool Object_Writer::Balance_Lift_Vertical_Rope(int x, int length) {
    if (length > 16 || length < 1) return false;
    return this->Write_Object(x, 0xF, 0x1, length-1);
}

bool Object_Writer::Balance_Lift_Horizontal_Rope(int x, int length) {
    if (length > 16 || length < 3) return false;
    return this->Write_Object(x, 0xC, 0x1, length-1);
}

bool Object_Writer::Steps(int x, int width) {
    if (width < 1 || width > 8) return false;
    return this->Write_Object(x, 0xF, 0x3, width-1);
}

bool Object_Writer::End_Steps(int x) {
    if (this->currentX+x == 0xF) return false;
    return this->Write_Object(x, 0xF, 0x38);
}

bool Object_Writer::Tall_Reverse_L_Pipe(int x, int yPlacement) {
    if (yPlacement < 0x2 || yPlacement > 0xA) return false;
    return this->Write_Object(x, 0xF, 0x4, yPlacement);
}

bool Object_Writer::Pipe_Wall(int x) {
    return this->Write_Object(x, 0xF, 0x4F);
}

bool Object_Writer::Nothing(int x) {
    return this->Write_Object(x, 0xF, 0x60);
}
