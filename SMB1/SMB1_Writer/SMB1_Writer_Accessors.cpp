#include "SMB1_Writer.h"
#include "Header_Writer.h"
#include "Object_Writer.h"
#include "Enemy_Writer.h"
#include "Midpoint_Writer.h"
#include "Room_Order_Writer.h"
#include "Room_ID_Handler.h"
#include "Hacks.h"
#include "Music.h"
#include "Powerups.h"
#include "Graphics.h"
#include "Sequential_Archive_Handler.h"
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

bool SMB1_Writer::Object_Toggle_Auto_Scroll(int x) {
    if (!this->Are_Buffers_Allocated()) return false;
    if (!this->hacks) return false;
    if (!this->hacks->Was_Castle_Loop_Replaced_With_Autoscroll_Object()) return this->objectWriter->Nothing(x);
    else return this->objectWriter->Toggle_Auto_Scroll(x);
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

bool SMB1_Writer::Enemy_Piranha_Plant(int x, int y, bool onlyHardMode) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Piranha_Plant(x, y, onlyHardMode);
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

bool SMB1_Writer::Enemy_Pipe_Pointer(int x, const QString &levelSlot, int page) {
    if (!this->Are_Buffers_Allocated()) return false;
    return this->enemyWriter->Pipe_Pointer(x, levelSlot, page);
}

bool SMB1_Writer::Send_Object_Bytes_From_One_Level_To_Another(Level::Level fromLevel, Level::Level toLevel, int numBytes) {
    if (this->Are_Buffers_Allocated()) return false;
    if (!this->roomIDHandler) return false;
    return this->roomIDHandler->Send_Object_Bytes_From_One_Level_To_Another(fromLevel, toLevel, numBytes);
}

bool SMB1_Writer::Send_Enemy_Bytes_From_One_Level_To_Another(Level::Level fromLevel, Level::Level toLevel, int numBytes) {
    if (this->Are_Buffers_Allocated()) return false;
    if (!this->roomIDHandler) return false;
    return this->roomIDHandler->Send_Enemy_Bytes_From_One_Level_To_Another(fromLevel, toLevel, numBytes);
}

bool SMB1_Writer::Hacks_Add_Luigi_Game() {
    if (!this->hacks) return false;
    return this->hacks->Add_Luigi_Game();
}

bool SMB1_Writer::Hacks_Always_Autoscroll() {
    if (!this->hacks) return false;
    return this->hacks->Always_Autoscroll();
}

bool SMB1_Writer::Hacks_Black_Piranha_Plants() {
    if (!this->hacks) return false;
    return this->hacks->Black_Piranha_Plants();
}

bool SMB1_Writer::Hacks_Bouncy_Spiny_Eggs() {
    if (!this->hacks) return false;
    return this->hacks->Bouncy_Spiny_Eggs();
}

bool SMB1_Writer::Hacks_Enable_God_Mode() {
    if (!this->hacks) return false;
    if (!this->hacks->Start_With_Fire_Flower_On_Room_Change()) return false;
    if (!this->hacks->Moon_Jump()) return false;
    if (!this->hacks->Fireballs_Kill_Everything_Onscreen()) return false;
    if (!this->hacks->Unlimited_Time()) return false;
    return this->hacks->Invincibility();
}

bool SMB1_Writer::Hacks_Enable_Hitting_Underwater_Blocks() {
    if (!this->hacks) return false;
    return this->hacks->Enable_Hitting_Underwater_Blocks();
}

bool SMB1_Writer::Hacks_Enable_Piranha_Plants_On_First_Level() {
    if (!this->hacks) return false;
    return this->hacks->Enable_Piranha_Plants_On_First_Level();
}

bool SMB1_Writer::Hacks_Enable_Walking_Hammer_Bros(int difficulty) {
    if (!this->hacks) return false;
    return this->hacks->Enable_Walking_Hammer_Bros(difficulty);
}

bool SMB1_Writer::Hacks_Fix_Lakitu_Throw_Arc() {
    if (!this->hacks) return false;
    return this->hacks->Fix_Lakitu_Throw_Arc();
}

bool SMB1_Writer::Hacks_Fix_Life_Counter_Bugs() {
    if (!this->hacks) return false;
    return this->hacks->Fix_Life_Counter_Bugs();
}

bool SMB1_Writer::Hacks_Hard_Mode_Does_Not_Affect_Lift_Size() {
    if (!this->hacks) return false;
    return this->hacks->Hard_Mode_Does_Not_Affect_Lift_Size();
}

bool SMB1_Writer::Hacks_Infinite_Lives() {
    if (!this->hacks) return false;
    return this->hacks->Infinite_Lives();
}

bool SMB1_Writer::Hacks_Permadeath() {
    if (!this->hacks) return false;
    return this->hacks->Permadeath();
}

bool SMB1_Writer::Hacks_Real_Time() {
    if (!this->hacks) return false;
    return this->hacks->Real_Time();
}

bool SMB1_Writer::Hacks_Red_Piranha_Plants() {
    if (!this->hacks) return false;
    return this->hacks->Red_Piranha_Plants();
}

bool SMB1_Writer::Hacks_Remove_Vertical_Object_Limit() {
    if (!this->hacks) return false;
    return this->hacks->Remove_Vertical_Object_Limit();
}

bool SMB1_Writer::Hacks_Replace_Castle_Loop_With_Autoscroll_Object() {
    if (!this->hacks) return false;
    return this->hacks->Replace_Castle_Loop_With_Autoscroll_Object();
}

bool SMB1_Writer::Hacks_Replace_Castle_Loop_With_Fire_Bros() {
    if (!this->hacks) return false;
    return this->hacks->Replace_Castle_Loop_With_Fire_Bros();
}

bool SMB1_Writer::Hacks_Replace_Mario_With_Luigi() {
    if (!this->hacks) return false;
    return this->hacks->Replace_Mario_With_Luigi();
}

bool SMB1_Writer::Hacks_Set_Basic_Enemy_Speed(int speed) {
    if (!this->hacks) return false;
    return this->hacks->Set_Basic_Enemy_Speed(speed);
}

bool SMB1_Writer::Hacks_Set_Bullet_Bill_Speed(int speed) {
    if (!this->hacks) return false;
    return this->hacks->Set_Bullet_Bill_Speed(speed);
}

bool SMB1_Writer::Hacks_Set_Lakitu_Respawn_Speed(int value) {
    if (!this->hacks) return false;
    return this->hacks->Set_Lakitu_Respawn_Speed(value);
}

bool SMB1_Writer::Hacks_Set_Number_Of_Worlds(int value) {
    if (!this->hacks) return false;
    return this->hacks->Set_Number_Of_Worlds(value);
}

bool SMB1_Writer::Hacks_Set_Starting_Lives(int lives) {
    if (!this->hacks) return false;
    return this->hacks->Set_Starting_Lives(lives);
}

bool SMB1_Writer::Hacks_Speedy_Objects_And_Enemies() {
    if (!this->hacks) return false;
    return this->hacks->Speedy_Objects_And_Enemies();
}

bool SMB1_Writer::Hacks_Start_With_Fire_Flower_On_Room_Change() {
    if (!this->hacks) return false;
    return this->hacks->Start_With_Fire_Flower_On_Room_Change();
}

bool SMB1_Writer::Hacks_Taking_Damage_As_Fire_Reverts_To_Super() {
    if (!this->hacks) return false;
    return this->hacks->Taking_Damage_As_Fire_Reverts_To_Super();
}

bool SMB1_Writer::Hacks_Top_Of_Flagpole_Gives_1UP() {
    if (!this->hacks) return false;
    return this->hacks->Top_Of_Flagpole_Gives_1UP();
}

bool SMB1_Writer::Hacks_Write_Watermark() {
    if (!this->hacks) return false;
    return this->hacks->Write_Watermark();
}

QStringList SMB1_Writer::Music_Get_Music_Packs() {
    if (!this->sequentialArchiveHandler) return QStringList();
    return this->sequentialArchiveHandler->Get_Music_Packs();
}

bool SMB1_Writer::Music_Apply_Music_Pack(int index) {
    if (!this->sequentialArchiveHandler) return false;
    return this->sequentialArchiveHandler->Apply_Music_Pack_At_Index(index);
}

int SMB1_Writer::Music_Get_Number_Of_Music_Packs() {
    if (!this->sequentialArchiveHandler) return false;
    return this->sequentialArchiveHandler->Get_Number_Of_Music_Packs();
}

bool SMB1_Writer::Music_Disable() {
    if (!this->music) return false;
    return this->music->Disable_Music();
}

bool SMB1_Writer::Music_Set_Combine_Music_Packs(bool combineMusicPacks) {
    if (!this->sequentialArchiveHandler) return false;
    this->sequentialArchiveHandler->Set_Combine_Music_Packs(combineMusicPacks);
    return true;
}

bool SMB1_Writer::Music_Tone_Color_1() {
    if (!this->music) return false;
    return this->music->Tone_Color_1();
}

bool SMB1_Writer::Music_Tone_Color_2() {
    if (!this->music) return false;
    return this->music->Tone_Color_2();
}

bool SMB1_Writer::Music_Tone_Color_3() {
    if (!this->music) return false;
    return this->music->Tone_Color_3();
}

bool SMB1_Writer::Music_Tone_Color_4() {
    if (!this->music) return false;
    return this->music->Tone_Color_4();
}

bool SMB1_Writer::Music_Tone_Color_5() {
    if (!this->music) return false;
    return this->music->Tone_Color_5();
}

bool SMB1_Writer::Music_Tone_Color_6() {
    if (!this->music) return false;
    return this->music->Tone_Color_6();
}

bool SMB1_Writer::Music_Tone_Color_7() {
    if (!this->music) return false;
    return this->music->Tone_Color_7();
}

bool SMB1_Writer::Music_Tone_Color_8() {
    if (!this->music) return false;
    return this->music->Tone_Color_8();
}

bool SMB1_Writer::Music_Tone_Color_9() {
    if (!this->music) return false;
    return this->music->Tone_Color_9();
}

bool SMB1_Writer::Music_Tone_Color_10() {
    if (!this->music) return false;
    return this->music->Tone_Color_10();
}

bool SMB1_Writer::Music_Tone_Color_11() {
    if (!this->music) return false;
    return this->music->Tone_Color_11();
}

bool SMB1_Writer::Music_Tone_Color_12() {
    if (!this->music) return false;
    return this->music->Tone_Color_12();
}

bool SMB1_Writer::Music_Tone_Color_13() {
    if (!this->music) return false;
    return this->music->Tone_Color_13();
}

bool SMB1_Writer::Music_Tone_Color_14() {
    if (!this->music) return false;
    return this->music->Tone_Color_14();
}

bool SMB1_Writer::Music_Tone_Color_15() {
    if (!this->music) return false;
    return this->music->Tone_Color_15();
}

bool SMB1_Writer::Music_Tone_Color_16() {
    if (!this->music) return false;
    return this->music->Tone_Color_16();
}

bool SMB1_Writer::Music_Tone_Color_17() {
    if (!this->music) return false;
    return this->music->Tone_Color_17();
}

bool SMB1_Writer::Music_Tone_Color_18() {
    if (!this->music) return false;
    return this->music->Tone_Color_18();
}

bool SMB1_Writer::Powerups_Replace_1UP_With_Poison_Mushroom() {
    if (!this->powerups) return false;
    return this->powerups->Replace_1UP_With_Poison_Mushroom();
}

bool SMB1_Writer::Powerups_Replace_1UP_With_Poison_Mushroom_If_Not_Fire_Mario() {
    if (!this->powerups) return false;
    return this->powerups->Replace_1UP_With_Poison_Mushroom_If_Not_Fire_Mario();
}

bool SMB1_Writer::Powerups_Replace_1UP_With_Swimming_Mushroom() {
    if (!this->powerups) return false;
    return this->powerups->Replace_1UP_With_Swimming_Mushroom();
}

bool SMB1_Writer::Powerups_Replace_Fire_Flower_With_Cutter_Flower() {
    if (!this->powerups) return false;
    return this->powerups->Replace_Fire_Flower_With_Cutter_Flower();
}

bool SMB1_Writer::Powerups_Replace_Fire_Flower_With_Double_Jump_Star() {
    if (!this->powerups) return false;
    return this->powerups->Replace_Fire_Flower_With_Double_Jump_Star();
}

bool SMB1_Writer::Powerups_Replace_Fire_Flower_With_Fire_Star() {
    if (!this->powerups) return false;
    return this->powerups->Replace_Fire_Flower_With_Fire_Star();
}

bool SMB1_Writer::Powerups_Replace_Fire_Flower_With_Hammer_Suit() {
    if (!this->powerups) return false;
    return this->powerups->Replace_Fire_Flower_With_Hammer_Suit();
}

bool SMB1_Writer::Powerups_Replace_Fire_Flower_With_Poison_Bubbles() {
    if (!this->powerups) return false;
    return this->powerups->Replace_Fire_Flower_With_Poison_Bubbles();
}

bool SMB1_Writer::Powerups_Replace_Fire_Flower_With_Power_Wand() {
    if (!this->powerups) return false;
    return this->powerups->Replace_Fire_Flower_With_Power_Wand();
}

bool SMB1_Writer::Powerups_Replace_Fire_Flower_With_Slime_Flower() {
    if (!this->powerups) return false;
    return this->powerups->Replace_Fire_Flower_With_Slime_Flower();
}

bool SMB1_Writer::Powerups_Replace_Fire_Flower_With_Spinball_Flower() {
    if (!this->powerups) return false;
    return this->powerups->Replace_Fire_Flower_With_Spinball_Flower();
}

QStringList SMB1_Writer::Graphics_Get_Graphic_Packs() {
    if (!this->sequentialArchiveHandler) return QStringList();
    return this->sequentialArchiveHandler->Get_Graphics_Packs();
}

bool SMB1_Writer::Graphics_Apply_Graphic_Pack(int index) {
    if (!this->sequentialArchiveHandler) return false;
    return this->sequentialArchiveHandler->Apply_Graphics_Pack_At_Index(index);
}

int SMB1_Writer::Graphics_Get_Number_Of_Graphic_Packs() {
    if (!this->sequentialArchiveHandler) return false;
    return this->sequentialArchiveHandler->Get_Number_Of_Graphics_Packs();
}

bool SMB1_Writer::Graphics_Change_1UP_Palette(int palette) {
    if (!this->graphics) return false;
    return this->graphics->Change_1UP_Palette(palette);
}

bool SMB1_Writer::Graphics_Write_Title_Screen_For_1_Player_Game() {
    if (!this->graphics) return false;
    return this->graphics->Write_Title_Screen_For_1_Player_Game();
}

bool SMB1_Writer::Graphics_Write_Title_Screen_For_2_Player_Game() {
    if (!this->graphics) return false;
    return this->graphics->Write_Title_Screen_For_2_Player_Game();
}

bool SMB1_Writer::Graphics_Write_Title_Screen_For_Partial_Game() {
    if (!this->graphics) return false;
    return this->graphics->Write_Title_Screen_For_Partial_Game();
}
