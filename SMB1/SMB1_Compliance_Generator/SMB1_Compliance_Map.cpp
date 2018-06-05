#include "SMB1_Compliance_Map.h"
#include "../Common_SMB1_Files/Object_Item_String.h"
#include "../Common_SMB1_Files/Enemy_Item_String.h"

SMB1_Compliance_Map::SMB1_Compliance_Map() {
    //Build the map for the Objects
    this->objects = new QMap<QString, Object_Item::Object_Item>();
    this->Populate_Object_Map();

    //Build the map for the Enemies
    this->enemies = new QMap<QString, Enemy_Item::Enemy_Item>();
    this->Populate_Enemy_Map();
}

SMB1_Compliance_Map::~SMB1_Compliance_Map() {
    delete this->objects;
    delete this->enemies;
}

void SMB1_Compliance_Map::Populate_Object_Map() {
    this->objects->clear();
    this->objects->insert(Object_Item::STRING_QUESTION_BLOCK_WITH_MUSHROOM, Object_Item::QUESTION_BLOCK_WITH_MUSHROOM);
    this->objects->insert(Object_Item::STRING_QUESTION_BLOCK_WITH_COIN, Object_Item::QUESTION_BLOCK_WITH_COIN);
    this->objects->insert(Object_Item::STRING_HIDDEN_BLOCK_WITH_COIN, Object_Item::HIDDEN_BLOCK_WITH_COIN);
    this->objects->insert(Object_Item::STRING_HIDDEN_BLOCK_WITH_1UP, Object_Item::HIDDEN_BLOCK_WITH_1UP);
    this->objects->insert(Object_Item::STRING_BRICK_WITH_MUSHROOM, Object_Item::BRICK_WITH_MUSHROOM);
    this->objects->insert(Object_Item::STRING_BRICK_WITH_VINE, Object_Item::BRICK_WITH_VINE);
    this->objects->insert(Object_Item::STRING_BRICK_WITH_STAR, Object_Item::BRICK_WITH_STAR);
    this->objects->insert(Object_Item::STRING_BRICK_WITH_10_COINS, Object_Item::BRICK_WITH_10_COINS);
    this->objects->insert(Object_Item::STRING_BRICK_WITH_1UP, Object_Item::BRICK_WITH_1UP);
    this->objects->insert(Object_Item::STRING_UNDERWATER_SIDEWAYS_PIPE, Object_Item::UNDERWATER_SIDEWAYS_PIPE);
    this->objects->insert(Object_Item::STRING_USED_BLOCK, Object_Item::USED_BLOCK);
    this->objects->insert(Object_Item::STRING_TRAMPOLINE, Object_Item::TRAMPOLINE);
    this->objects->insert(Object_Item::STRING_BULLET_BILL_TURRET, Object_Item::CANNON);
    this->objects->insert(Object_Item::STRING_ISLAND, Object_Item::ISLAND);
    this->objects->insert(Object_Item::STRING_HORIZONTAL_BRICKS, Object_Item::HORIZONTAL_BRICKS);
    this->objects->insert(Object_Item::STRING_HORIZONTAL_BLOCKS, Object_Item::HORIZONTAL_BLOCKS);
    this->objects->insert(Object_Item::STRING_HORIZONTAL_COINS, Object_Item::HORIZONTAL_COINS);
    this->objects->insert(Object_Item::STRING_VERTICAL_BRICKS, Object_Item::VERTICAL_BRICKS);
    this->objects->insert(Object_Item::STRING_VERTICAL_BLOCKS, Object_Item::VERTICAL_BLOCKS);
    this->objects->insert(Object_Item::STRING_CORRAL, Object_Item::CORRAL);
    this->objects->insert(Object_Item::STRING_PIPE, Object_Item::PIPE);
    this->objects->insert(Object_Item::STRING_ENTERABLE_PIPE, Object_Item::ENTERABLE_PIPE);
    this->objects->insert(Object_Item::STRING_HOLE, Object_Item::HOLE);
    this->objects->insert(Object_Item::STRING_HOLE_WITH_WATER, Object_Item::HOLE_WITH_WATER);
    this->objects->insert(Object_Item::STRING_BALANCE__LIFT_VERTICAL_ROPE, Object_Item::BALANCE_LIFT_VERTICAL_ROPE);
    this->objects->insert(Object_Item::STRING_BRIDGE, Object_Item::BRIDGE);
    this->objects->insert(Object_Item::STRING_HORIZONTAL_QUESTION_BLOCKS_WITH_COINS, Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS);
    this->objects->insert(Object_Item::STRING_PAGE_CHANGE, Object_Item::PAGE_CHANGE);
    this->objects->insert(Object_Item::STRING_REVERSE_L_PIPE, Object_Item::REVERSE_L_PIPE);
    this->objects->insert(Object_Item::STRING_FLAGPOLE, Object_Item::FLAGPOLE);
    this->objects->insert(Object_Item::STRING_CASTLE, Object_Item::CASTLE);
    this->objects->insert(Object_Item::STRING_BIG_CASTLE, Object_Item::BIG_CASTLE);
    this->objects->insert(Object_Item::STRING_AXE, Object_Item::AXE);
    this->objects->insert(Object_Item::STRING_AXE_ROPE, Object_Item::AXE_ROPE);
    this->objects->insert(Object_Item::STRING_BOWSER_BRIDGE, Object_Item::BOWSER_BRIDGE);
    this->objects->insert(Object_Item::STRING_SCROLL_STOP, Object_Item::SCROLL_STOP);
    this->objects->insert(Object_Item::STRING_SCROLL_STOP_WARP_ZONE, Object_Item::SCROLL_STOP_WARP_ZONE);
    this->objects->insert(Object_Item::STRING_FLYING_CHEEP_CHEEP_SPAWNER, Object_Item::FLYING_CHEEP_CHEEP_SPAWNER);
    this->objects->insert(Object_Item::STRING_SWIMMING_CHEEP_CHEEP_SPAWNER, Object_Item::SWIMMING_CHEEP_CHEEP_SPAWNER);
    this->objects->insert(Object_Item::STRING_BULLET_BILL_SPAWNER, Object_Item::BULLET_BILL_SPAWNER);
    this->objects->insert(Object_Item::STRING_CANCEL_SPAWNER, Object_Item::CANCEL_SPAWNER);
    this->objects->insert(Object_Item::STRING_LOOP_COMMAND, Object_Item::LOOP_COMMAND);
    this->objects->insert(Object_Item::STRING_CHANGE_BRICK_AND_SCENERY, Object_Item::CHANGE_BRICK_AND_SCENERY);
    this->objects->insert(Object_Item::STRING_CHANGE_BACKGROUND, Object_Item::CHANGE_BACKGROUND);
    this->objects->insert(Object_Item::STRING_LIFT_ROPE, Object_Item::LIFT_ROPE);
    this->objects->insert(Object_Item::STRING_BALANCE_LIFT_HORIZONTAL_ROPE, Object_Item::BALANCE_LIFT_HORIZONTAL_ROPE);
    this->objects->insert(Object_Item::STRING_STEPS, Object_Item::STEPS);
    this->objects->insert(Object_Item::STRING_END_STEPS, Object_Item::END_STEPS);
    this->objects->insert(Object_Item::STRING_TALL_REVERSE_L_PIPE, Object_Item::TALL_REVERSE_L_PIPE);
    this->objects->insert(Object_Item::STRING_PIPE_WALL, Object_Item::PIPE_WALL);
    this->objects->insert(Object_Item::STRING_NOTHING, Object_Item::NOTHING);
}

void SMB1_Compliance_Map::Populate_Enemy_Map() {
    this->enemies->clear();
    this->enemies->insert(Enemy_Item::STRING_GREEN_KOOPA, Enemy_Item::GREEN_KOOPA);
    this->enemies->insert(Enemy_Item::STRING_RED_KOOPA, Enemy_Item::RED_KOOPA);
    this->enemies->insert(Enemy_Item::STRING_BUZZY_BEETLE, Enemy_Item::BUZZY_BEETLE);
    this->enemies->insert(Enemy_Item::STRING_HAMMER_BRO, Enemy_Item::HAMMER_BRO);
    this->enemies->insert(Enemy_Item::STRING_GOOMBA, Enemy_Item::GOOMBA);
    this->enemies->insert(Enemy_Item::STRING_BLOOPER, Enemy_Item::BLOOPER);
    this->enemies->insert(Enemy_Item::STRING_BULLET_BILL, Enemy_Item::BULLET_BILL);
    this->enemies->insert(Enemy_Item::STRING_GREEN_PARATROOPA, Enemy_Item::GREEN_PARATROOPA);
    this->enemies->insert(Enemy_Item::STRING_RED_PARATROOPA, Enemy_Item::RED_PARATROOPA);
    this->enemies->insert(Enemy_Item::STRING_GREEN_CHEEP_CHEEP, Enemy_Item::GREEN_CHEEP_CHEEP);
    this->enemies->insert(Enemy_Item::STRING_RED_CHEEP_CHEEP, Enemy_Item::RED_CHEEP_CHEEP);
    this->enemies->insert(Enemy_Item::STRING_PODOBOO, Enemy_Item::PODOBOO);
    this->enemies->insert(Enemy_Item::STRING_PIRANA_PLANT, Enemy_Item::PIRANA_PLANT);
    this->enemies->insert(Enemy_Item::STRING_LAKITU, Enemy_Item::LAKITU);
    this->enemies->insert(Enemy_Item::STRING_SPINY, Enemy_Item::SPINY);
    this->enemies->insert(Enemy_Item::STRING_BOWSER_FIRE_SPAWNER, Enemy_Item::BOWSER_FIRE_SPAWNER);
    this->enemies->insert(Enemy_Item::STRING_CHEEP_CHEEP_SPAWNER, Enemy_Item::CHEEP_CHEEP_SPAWNER);
    this->enemies->insert(Enemy_Item::STRING_BULLET_BILL_SPAWNER, Enemy_Item::BULLET_BILL_SPAWNER);
    this->enemies->insert(Enemy_Item::STRING_FIRE_BAR, Enemy_Item::FIRE_BAR);
    this->enemies->insert(Enemy_Item::STRING_LARGE_FIRE_BAR, Enemy_Item::LARGE_FIRE_BAR);
    this->enemies->insert(Enemy_Item::STRING_LIFT, Enemy_Item::LIFT);
    this->enemies->insert(Enemy_Item::STRING_FALLING_LIFT, Enemy_Item::FALLING_LIFT);
    this->enemies->insert(Enemy_Item::STRING_BALANCE_LIFT, Enemy_Item::BALANCE_LIFT);
    this->enemies->insert(Enemy_Item::STRING_SURFING_LIFT, Enemy_Item::SURFING_LIFT);
    this->enemies->insert(Enemy_Item::STRING_LIFT_SPAWNER, Enemy_Item::LIFT_SPAWNER);
    this->enemies->insert(Enemy_Item::STRING_BOWSER, Enemy_Item::BOWSER);
    this->enemies->insert(Enemy_Item::STRING_WARP_ZONE, Enemy_Item::WARP_ZONE);
    this->enemies->insert(Enemy_Item::STRING_PIPE_POINTER, Enemy_Item::PIPE_POINTER);
    this->enemies->insert(Enemy_Item::STRING_TOAD, Enemy_Item::TOAD);
    this->enemies->insert(Enemy_Item::STRING_GOOMBA_GROUP, Enemy_Item::GOOMBA_GROUP);
    this->enemies->insert(Enemy_Item::STRING_KOOPA_GROUP, Enemy_Item::KOOPA_GROUP);
    this->enemies->insert(Enemy_Item::STRING_PAGE_CHANGE, Enemy_Item::PAGE_CHANGE);
}
