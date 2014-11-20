#include "SMB1_Writer.h"
#include "Header_Writer.h"
#include "Object_Writer.h"
#include "Enemy_Writer.h"
#include "Midpoint_Writer.h"
#include "Room_Order_Writer.h"
#include "Room_ID_Handler.h"
#include <QDebug>

bool SMB1_Writer::Header_Time(int value) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->headerWriter->Set_Time(value);
}

bool SMB1_Writer::Header_Autowalk(bool value) {
    if (!this->Are_Buffers_Allocated()) return false;
    this->headerWriter->Set_Autowalk(value);
    return true;
}

bool SMB1_Writer::Header_Starting_Position(Level_Attribute::Level_Attribute value) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->headerWriter->Set_Starting_Position(value);
}

bool SMB1_Writer::Header_Background(Background::Background value) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->headerWriter->Set_Background(value);
}

bool SMB1_Writer::Header_Level_Compliment(Level_Compliment::Level_Compliment value) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->headerWriter->Set_Level_Compliment(value);
}

bool SMB1_Writer::Header_Scenery(Scenery::Scenery value) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->headerWriter->Set_Scenery(value);
}

bool SMB1_Writer::Header_Brick(Brick::Brick value) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->headerWriter->Set_Brick(value);
}

bool SMB1_Writer::Header_Attribute(Level_Attribute::Level_Attribute attribute) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->roomIDHandler->Change_Current_Level_Attribute(attribute);
}

bool SMB1_Writer::Header_Midpoint(int value) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->midpointWriter->Set_Midpoint(value);
}

bool SMB1_Writer::Header_Midpoint(Level::Level level, int value) {
    return this->midpointWriter->Set_Midpoint(level, value);
}

bool SMB1_Writer::Room_Table_Set_Next_Level(Level::Level level) {
    if (!this->roomOrderWriter) return false;
    return this->roomOrderWriter->Set_Next_Level(level);
}

bool SMB1_Writer::Room_Table_Set_Number_Of_Worlds(int value) {
    if (!this->roomOrderWriter) return false;
    return this->roomOrderWriter->Set_Number_Of_Worlds(value);
}

bool SMB1_Writer::Object_Question_Block_With_Mushroom(int x, int y) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Question_Block_With_Mushroom(x, y);
}

bool SMB1_Writer::Object_Question_Block_With_Coin(int x, int y) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Question_Block_With_Coin(x, y);
}

bool SMB1_Writer::Object_Hidden_Block_With_Coin(int x, int y) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Hidden_Block_With_Coin(x, y);
}

bool SMB1_Writer::Object_Hidden_Block_With_1up(int x, int y) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Hidden_Block_With_1up(x, y);
}

bool SMB1_Writer::Object_Brick_With_Mushroom(int x, int y) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Brick_With_Mushroom(x, y);
}

bool SMB1_Writer::Object_Brick_With_Vine(int x, int y) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Brick_With_Vine(x, y);
}

bool SMB1_Writer::Object_Brick_With_Star(int x, int y) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Brick_With_Star(x, y);
}

bool SMB1_Writer::Object_Brick_With_10_Coins(int x, int y) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Brick_With_10_Coins(x, y);
}

bool SMB1_Writer::Object_Brick_With_1up(int x, int y) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Brick_With_1up(x, y);
}

bool SMB1_Writer::Object_Underwater_Sideways_Pipe(int x, int y) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Underwater_Sideways_Pipe(x, y);
}

bool SMB1_Writer::Object_Used_Block(int x, int y) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Used_Block(x, y);
}

bool SMB1_Writer::Object_Trampoline(int x, int y) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Trampoline(x, y);
}

bool SMB1_Writer::Object_Cannon(int x, int y, int height) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Bullet_Bill_Turret(x, y, height);
}

bool SMB1_Writer::Object_Island(int x, int y, int length) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Island(x, y, length);
}

bool SMB1_Writer::Object_Horizontal_Bricks(int x, int y, int length) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Horizontal_Bricks(x, y, length);
}

bool SMB1_Writer::Object_Horizontal_Blocks(int x, int y, int length) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Horizontal_Blocks(x, y, length);
}

bool SMB1_Writer::Object_Horizontal_Coins(int x, int y, int length) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Horizontal_Coins(x, y, length);
}

bool SMB1_Writer::Object_Vertical_Bricks(int x, int y, int height) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Vertical_Bricks(x, y, height);
}

bool SMB1_Writer::Object_Vertical_Blocks(int x, int y, int height) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Vertical_Blocks(x, y, height);
}

bool SMB1_Writer::Object_Corral(int x, int y, int height) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Corral(x, y, height);
}

bool SMB1_Writer::Object_Pipe(int x, int y, int height, bool enterable) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Pipe(x, y, height, enterable);
}

bool SMB1_Writer::Object_Hole(int x, int length, bool filledWithWater) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Hole(x, length, filledWithWater);
}

bool SMB1_Writer::Object_Bridge(int x, int yPlacement, int length) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Bridge(x, yPlacement, length);
}

bool SMB1_Writer::Object_Horizontal_Question_Blocks_With_Coins(int x, int yPlacement, int length) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Horizontal_Question_Blocks_With_Coins(x, yPlacement, length);
}

bool SMB1_Writer::Object_Page_Change(int page) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Page_Change(page);
}

bool SMB1_Writer::Object_Reverse_L_Pipe(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Reverse_L_Pipe(x);
}

bool SMB1_Writer::Object_Flagpole(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Flagpole(x);
}

bool SMB1_Writer::Object_Castle(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Castle(x);
}

bool SMB1_Writer::Object_Big_Castle(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Big_Castle(x);
}

bool SMB1_Writer::Object_Axe(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Axe(x);
}

bool SMB1_Writer::Object_Axe_Rope(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Axe_Rope(x);
}

bool SMB1_Writer::Object_Bowser_Bridge(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Bowser_Bridge(x);
}

bool SMB1_Writer::Object_Scroll_Stop(int x, bool warpZone) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Scroll_Stop(x, warpZone);
}

bool SMB1_Writer::Object_Flying_Cheep_Cheep_Spawner(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Flying_Cheep_Cheep_Spawner(x);
}

bool SMB1_Writer::Object_Swimming_Cheep_Cheep_Spawner(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Swimming_Cheep_Cheep_Spawner(x);
}

bool SMB1_Writer::Object_Bullet_Bill_Spawner(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Bullet_Bill_Spawner(x);
}

bool SMB1_Writer::Object_Cancel_Spawner(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Cancel_Spawner(x);
}

bool SMB1_Writer::Object_Loop_Command(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Loop_Command(x);
}

bool SMB1_Writer::Object_Change_Brick_And_Scenery(int x, Brick::Brick brick, Scenery::Scenery scenery) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Change_Brick_And_Scenery(x, brick, scenery);
}

bool SMB1_Writer::Object_Change_Background(int x, Background::Background background) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Change_Background(x, background);
}

bool SMB1_Writer::Object_Lift_Rope(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Lift_Rope(x);
}

bool SMB1_Writer::Object_Balance_Lift_Vertical_Rope(int x, int length) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Balance_Lift_Vertical_Rope(x, length);
}

bool SMB1_Writer::Object_Balance_Lift_Horizontal_Rope(int x, int length) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Balance_Lift_Horizontal_Rope(x, length);
}

bool SMB1_Writer::Object_Steps(int x, int width) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Steps(x, width);
}

bool SMB1_Writer::Object_End_Steps(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->End_Steps(x);
}

bool SMB1_Writer::Object_Tall_Reverse_L_Pipe(int x, int yPlacement) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Tall_Reverse_L_Pipe(x, yPlacement);
}

bool SMB1_Writer::Object_Pipe_Wall(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Pipe_Wall(x);
}

bool SMB1_Writer::Object_Nothing(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->objectWriter->Nothing(x);
}

bool SMB1_Writer::Enemy_Green_Koopa(int x, int y, bool moving, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Green_Koopa(x, y, moving, onlyHardMode);
}

bool SMB1_Writer::Enemy_Red_Koopa(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Red_Koopa(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Buzzy_Beetle(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Buzzy_Beetle(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Hammer_Bro(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Hammer_Bro(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Goomba(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Goomba(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Blooper(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Blooper(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Bullet_Bill(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Bullet_Bill(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Green_Paratroopa(int x, int y, bool moving, bool leaping, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Green_Paratroopa(x, y, moving, leaping, onlyHardMode);
}

bool SMB1_Writer::Enemy_Red_Paratroopa(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Red_Paratroopa(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Green_Cheep_Cheep(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Green_Cheep_Cheep(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Red_Cheep_Cheep(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Red_Cheep_Cheep(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Podoboo(int x, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Podoboo(x, onlyHardMode);
}

bool SMB1_Writer::Enemy_Pirana_Plant(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Pirana_Plant(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Lakitu(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Lakitu(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Spiny(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Spiny(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Bowser_Fire_Spawner(int x, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Bowser_Fire_Spawner(x, onlyHardMode);
}

bool SMB1_Writer::Enemy_Swimming_Cheep_Cheep_Spawner(int x, bool leaping, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Swimming_Cheep_Cheep_Spawner(x, leaping, onlyHardMode);
}

bool SMB1_Writer::Enemy_Bullet_Bill_Spawner(int x, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Bullet_Bill_Spawner(x, onlyHardMode);
}

bool SMB1_Writer::Enemy_Fire_Bar(int x, int y, bool clockwise, bool fast, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Fire_Bar(x, y, clockwise, fast, onlyHardMode);
}

bool SMB1_Writer::Enemy_Large_Fire_Bar(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Large_Fire_Bar(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Lift(int x, int y, bool vertical, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Lift(x, y, vertical, onlyHardMode);
}

bool SMB1_Writer::Enemy_Falling_Lift(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Falling_Lift(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Balance_Lift(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Balance_Lift(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Surfing_Lift(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Surfing_Lift(x, y, onlyHardMode);
}

bool SMB1_Writer::Enemy_Lift_Spawner(int x, int y, bool up, bool small, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Lift_Spawner(x, y, up, small, onlyHardMode);
}

bool SMB1_Writer::Enemy_Bowser(int x, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Bowser(x, onlyHardMode);
}

bool SMB1_Writer::Enemy_Warp_Zone(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Warp_Zone(x);
}

bool SMB1_Writer::Enemy_Toad(int x, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Toad(x, onlyHardMode);
}

bool SMB1_Writer::Enemy_Goomba_Group(int x, int y, int num, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Goomba_Group(x, y, num, onlyHardMode);
}

bool SMB1_Writer::Enemy_Koopa_Group(int x, int y, int num, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Koopa_Group(x, y, num, onlyHardMode);
}

bool SMB1_Writer::Enemy_Page_Change(int page) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Page_Change(page);
}

bool SMB1_Writer::Enemy_Pipe_Pointer(int x, int room, int page) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Pipe_Pointer(x, room, page);
}

bool SMB1_Writer::Enemy_Nothing(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Nothing(x);
}
