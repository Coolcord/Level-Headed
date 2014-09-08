#include "Object_Writer.h"

bool Object_Writer::Write_Object(int x, const QString &object) {
    if (!this->Is_Coordinate_Valid(x)) return false;
    return this->Write_Item(OBJECT, QString(object+" "+QString(x)));
}

bool Object_Writer::Write_Object(int x, const QString &object, const QString &parameters) {
    if (!this->Is_Coordinate_Valid(x)) return false;
    return this->Write_Item(OBJECT, QString(object+" "+QString(x)+" "+parameters));
}

bool Object_Writer::Write_Object(int x, int y, const QString &object) {
    if (!this->Are_Coordinates_Valid(x, y)) return false;
    return this->Write_Item(OBJECT, QString(object+" "+QString(x)+" "+QString(y)));
}

bool Object_Writer::Write_Object(int x, int y, const QString &object, const QString &parameters) {
    if (!this->Are_Coordinates_Valid(x, y)) return false;
    return this->Write_Item(OBJECT, QString(object+" "+QString(x)+" "+QString(y)+" "+parameters));
}

bool Object_Writer::Question_Block_With_Mushroom(int x, int y) {
    return this->Write_Object(x, y, Object_Item::STRING_QUESTION_BLOCK_WITH_MUSHROOM);
}

bool Object_Writer::Question_Block_With_Coin(int x, int y) {
    return this->Write_Object(x, y, Object_Item::STRING_QUESTION_BLOCK_WITH_COIN);
}

bool Object_Writer::Hidden_Block_With_Coin(int x, int y) {
    return this->Write_Object(x, y, Object_Item::STRING_HIDDEN_BLOCK_WITH_COIN);
}

bool Object_Writer::Hidden_Block_With_1up(int x, int y) {
    return this->Write_Object(x, y, "Hidden_Block_With_1up");
}

bool Object_Writer::Brick_With_Mushroom(int x, int y) {
    return this->Write_Object(x, y, "Brick_With_Mushroom");
}

bool Object_Writer::Brick_With_Vine(int x, int y) {
    return this->Write_Object(x, y, "Brick_With_Vine");
}

bool Object_Writer::Brick_With_Star(int x, int y) {
    return this->Write_Object(x, y, "Brick_With_Star");
}

bool Object_Writer::Brick_With_10_Coins(int x, int y) {
    return this->Write_Object(x, y, "Brick_With_10_Coins");
}

bool Object_Writer::Brick_With_1up(int x, int y) {
    return this->Write_Object(x, y, "Brick_With_1up");
}

bool Object_Writer::Underwater_Sideways_Pipe(int x, int y) {
    return this->Write_Object(x, y, "Underwater_Sideways_Pipe");
}

bool Object_Writer::Used_Block(int x, int y) {
    return this->Write_Object(x, y, "Used_Block");
}

bool Object_Writer::Trampoline(int x, int y) {
    return this->Write_Object(x, y, "Trampoline");
}

bool Object_Writer::Cannon(int x, int y, int height) {
    if (height < 1 || height > 16) return false;
    return this->Write_Object(x, y, "Cannon", QString(height));
}

bool Object_Writer::Island(int x, int y, int length) {
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, y, "Island", QString(length));
}

bool Object_Writer::Horizontal_Bricks(int x, int y, int length) {
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, y, "Horizontal_Bricks", QString(length));
}

bool Object_Writer::Horizontal_Blocks(int x, int y, int length) {
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, y, "Horizontal_Blocks", QString(length));
}

bool Object_Writer::Horizontal_Coins(int x, int y, int length) {
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, y, "Horizontal_Coins", QString(length));
}

bool Object_Writer::Vertical_Bricks(int x, int y, int height) {
    if (height < 1 || height > 16) return false;
    return this->Write_Object(x, y, "Vertical_Bricks", QString(height));
}

bool Object_Writer::Vertical_Blocks(int x, int y, int height) {
    if (height < 1 || height > 16) return false;
    return this->Write_Object(x, y, "Vertical_Blocks", QString(height));
}

bool Object_Writer::Pipe(int x, int y, int height, bool enterable) {
    if (height < 1 || height > 16) return false;
    QString object = "";
    if (enterable) {
        object = "Enterable_Pipe";
    } else {
        object = "Pipe";
    }
    return this->Write_Object(x, y, object, QString(height));
}

bool Object_Writer::Hole(int x, int length, bool filledWithWater) {
    if (length < 1 || length > 16) return false;
    QString object = "";
    if (filledWithWater) {
        object = "Hole_With_Water";
    } else {
        object = "Hole";
    }
    return this->Write_Object(x, object, QString(length));
}

bool Object_Writer::Balance_Rope(int x, int length) {
    if (length < 1 || length > 16) return false;
    return this->Write_Object(x, "Balance_Rope");
}

bool Object_Writer::Bridge(int x, int yPlacement, int length) {
    if (length < 1 || length > 16) return false;
    if (yPlacement == 0x7 || yPlacement == 0x8 || yPlacement == 0xA) {
        return this->Write_Object(x, yPlacement, "Bridge", QString(length));
    } else {
        return false;
    }
}

bool Object_Writer::Horizontal_Question_Blocks_With_Coins(int x, int yPlacement, int length) {
    if (length < 1 || length > 16) return false;
    if (yPlacement == 0x3 || yPlacement == 0x7) {
        return this->Write_Object(x, yPlacement, "Horizontal_Question_Blocks_With_Coins", QString(length));
    } else {
        return false;
    }
}

bool Object_Writer::Page_Skip(int x, int amount) {
    if (amount < 0x00 || amount > 0x3F) return false;
    return this->Write_Object(x, "Page_Skip", QString(amount));
}

bool Object_Writer::Reverse_L_Pipe(int x) {
    return this->Write_Object(x, "Reverse_L_Pipe");
}

bool Object_Writer::Flagpole(int x) {
    return this->Write_Object(x, "Flagpole");
}

bool Object_Writer::Axe(int x) {
    return this->Write_Object(x, "Axe");
}

bool Object_Writer::Axe_Rope(int x) {
    return this->Write_Object(x, "Axe_Rope");
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
