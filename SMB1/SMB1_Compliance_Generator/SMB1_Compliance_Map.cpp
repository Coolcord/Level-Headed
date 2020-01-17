#include "SMB1_Compliance_Map.h"
#include "../Common_SMB1_Files/Object_Item_String.h"
#include "../Common_SMB1_Files/Enemy_Item_String.h"

SMB1_Compliance_Map::SMB1_Compliance_Map() {
    //Build the map for the Objects
    this->objectsMap = new QMap<QString, Object_Item::Object_Item>();
    this->Populate_Object_Map();

    //Build the map for the Enemies
    this->enemiesMap = new QMap<QString, Enemy_Item::Enemy_Item>();
    this->Populate_Enemy_Map();
}

SMB1_Compliance_Map::~SMB1_Compliance_Map() {
    delete this->objectsMap;
    delete this->enemiesMap;
}

void SMB1_Compliance_Map::Populate_Object_Map() {
    this->objectsMap->clear();
    this->objectsMap->insert(Object_Item::STRING_QUESTION_BLOCK_WITH_MUSHROOM, Object_Item::QUESTION_BLOCK_WITH_MUSHROOM);
    this->objectsMap->insert(Object_Item::STRING_QUESTION_BLOCK_WITH_COIN, Object_Item::QUESTION_BLOCK_WITH_COIN);
    this->objectsMap->insert(Object_Item::STRING_HIDDEN_BLOCK_WITH_COIN, Object_Item::HIDDEN_BLOCK_WITH_COIN);
    this->objectsMap->insert(Object_Item::STRING_HIDDEN_BLOCK_WITH_1UP, Object_Item::HIDDEN_BLOCK_WITH_1UP);
    this->objectsMap->insert(Object_Item::STRING_BRICK_WITH_MUSHROOM, Object_Item::BRICK_WITH_MUSHROOM);
    this->objectsMap->insert(Object_Item::STRING_BRICK_WITH_VINE, Object_Item::BRICK_WITH_VINE);
    this->objectsMap->insert(Object_Item::STRING_BRICK_WITH_STAR, Object_Item::BRICK_WITH_STAR);
    this->objectsMap->insert(Object_Item::STRING_BRICK_WITH_10_COINS, Object_Item::BRICK_WITH_10_COINS);
    this->objectsMap->insert(Object_Item::STRING_BRICK_WITH_1UP, Object_Item::BRICK_WITH_1UP);
    this->objectsMap->insert(Object_Item::STRING_UNDERWATER_SIDEWAYS_PIPE, Object_Item::UNDERWATER_SIDEWAYS_PIPE);
    this->objectsMap->insert(Object_Item::STRING_USED_BLOCK, Object_Item::USED_BLOCK);
    this->objectsMap->insert(Object_Item::STRING_TRAMPOLINE, Object_Item::TRAMPOLINE);
    this->objectsMap->insert(Object_Item::STRING_BULLET_BILL_TURRET, Object_Item::CANNON);
    this->objectsMap->insert(Object_Item::STRING_ISLAND, Object_Item::ISLAND);
    this->objectsMap->insert(Object_Item::STRING_HORIZONTAL_BRICKS, Object_Item::HORIZONTAL_BRICKS);
    this->objectsMap->insert(Object_Item::STRING_HORIZONTAL_BLOCKS, Object_Item::HORIZONTAL_BLOCKS);
    this->objectsMap->insert(Object_Item::STRING_HORIZONTAL_COINS, Object_Item::HORIZONTAL_COINS);
    this->objectsMap->insert(Object_Item::STRING_VERTICAL_BRICKS, Object_Item::VERTICAL_BRICKS);
    this->objectsMap->insert(Object_Item::STRING_VERTICAL_BLOCKS, Object_Item::VERTICAL_BLOCKS);
    this->objectsMap->insert(Object_Item::STRING_CORAL, Object_Item::CORRAL);
    this->objectsMap->insert(Object_Item::STRING_PIPE, Object_Item::PIPE);
    this->objectsMap->insert(Object_Item::STRING_ENTERABLE_PIPE, Object_Item::ENTERABLE_PIPE);
    this->objectsMap->insert(Object_Item::STRING_HOLE, Object_Item::HOLE);
    this->objectsMap->insert(Object_Item::STRING_HOLE_WITH_WATER, Object_Item::HOLE_WITH_WATER);
    this->objectsMap->insert(Object_Item::STRING_BALANCE_LIFT_VERTICAL_ROPE, Object_Item::BALANCE_LIFT_VERTICAL_ROPE);
    this->objectsMap->insert(Object_Item::STRING_BRIDGE, Object_Item::BRIDGE);
    this->objectsMap->insert(Object_Item::STRING_HORIZONTAL_QUESTION_BLOCKS_WITH_COINS, Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS);
    this->objectsMap->insert(Object_Item::STRING_PAGE_CHANGE, Object_Item::PAGE_CHANGE);
    this->objectsMap->insert(Object_Item::STRING_REVERSE_L_PIPE, Object_Item::REVERSE_L_PIPE);
    this->objectsMap->insert(Object_Item::STRING_FLAGPOLE, Object_Item::FLAGPOLE);
    this->objectsMap->insert(Object_Item::STRING_CASTLE, Object_Item::CASTLE);
    this->objectsMap->insert(Object_Item::STRING_BIG_CASTLE, Object_Item::BIG_CASTLE);
    this->objectsMap->insert(Object_Item::STRING_AXE, Object_Item::AXE);
    this->objectsMap->insert(Object_Item::STRING_AXE_ROPE, Object_Item::AXE_ROPE);
    this->objectsMap->insert(Object_Item::STRING_BOWSER_BRIDGE, Object_Item::BOWSER_BRIDGE);
    this->objectsMap->insert(Object_Item::STRING_SCROLL_STOP, Object_Item::SCROLL_STOP);
    this->objectsMap->insert(Object_Item::STRING_SCROLL_STOP_WARP_ZONE, Object_Item::SCROLL_STOP_WARP_ZONE);
    this->objectsMap->insert(Object_Item::STRING_TOGGLE_AUTO_SCROLL, Object_Item::TOGGLE_AUTO_SCROLL);
    this->objectsMap->insert(Object_Item::STRING_FLYING_CHEEP_CHEEP_SPAWNER, Object_Item::FLYING_CHEEP_CHEEP_SPAWNER);
    this->objectsMap->insert(Object_Item::STRING_SWIMMING_CHEEP_CHEEP_SPAWNER, Object_Item::SWIMMING_CHEEP_CHEEP_SPAWNER);
    this->objectsMap->insert(Object_Item::STRING_BULLET_BILL_SPAWNER, Object_Item::BULLET_BILL_SPAWNER);
    this->objectsMap->insert(Object_Item::STRING_CANCEL_SPAWNER, Object_Item::CANCEL_SPAWNER);
    this->objectsMap->insert(Object_Item::STRING_LOOP_COMMAND, Object_Item::LOOP_COMMAND);
    this->objectsMap->insert(Object_Item::STRING_CHANGE_BRICK_AND_SCENERY, Object_Item::CHANGE_BRICK_AND_SCENERY);
    this->objectsMap->insert(Object_Item::STRING_CHANGE_BACKGROUND, Object_Item::CHANGE_BACKGROUND);
    this->objectsMap->insert(Object_Item::STRING_LIFT_ROPE, Object_Item::LIFT_ROPE);
    this->objectsMap->insert(Object_Item::STRING_BALANCE_LIFT_HORIZONTAL_ROPE, Object_Item::BALANCE_LIFT_HORIZONTAL_ROPE);
    this->objectsMap->insert(Object_Item::STRING_STEPS, Object_Item::STEPS);
    this->objectsMap->insert(Object_Item::STRING_END_STEPS, Object_Item::END_STEPS);
    this->objectsMap->insert(Object_Item::STRING_TALL_REVERSE_L_PIPE, Object_Item::TALL_REVERSE_L_PIPE);
    this->objectsMap->insert(Object_Item::STRING_PIPE_WALL, Object_Item::PIPE_WALL);
    this->objectsMap->insert(Object_Item::STRING_NOTHING, Object_Item::NOTHING);
}

void SMB1_Compliance_Map::Populate_Enemy_Map() {
    this->enemiesMap->clear();
    this->enemiesMap->insert(Enemy_Item::STRING_GREEN_KOOPA, Enemy_Item::GREEN_KOOPA);
    this->enemiesMap->insert(Enemy_Item::STRING_RED_KOOPA, Enemy_Item::RED_KOOPA);
    this->enemiesMap->insert(Enemy_Item::STRING_BUZZY_BEETLE, Enemy_Item::BUZZY_BEETLE);
    this->enemiesMap->insert(Enemy_Item::STRING_HAMMER_BRO, Enemy_Item::HAMMER_BRO);
    this->enemiesMap->insert(Enemy_Item::STRING_GOOMBA, Enemy_Item::GOOMBA);
    this->enemiesMap->insert(Enemy_Item::STRING_BLOOPER, Enemy_Item::BLOOPER);
    this->enemiesMap->insert(Enemy_Item::STRING_BULLET_BILL, Enemy_Item::BULLET_BILL);
    this->enemiesMap->insert(Enemy_Item::STRING_GREEN_PARATROOPA, Enemy_Item::GREEN_PARATROOPA);
    this->enemiesMap->insert(Enemy_Item::STRING_RED_PARATROOPA, Enemy_Item::RED_PARATROOPA);
    this->enemiesMap->insert(Enemy_Item::STRING_GREEN_CHEEP_CHEEP, Enemy_Item::GREEN_CHEEP_CHEEP);
    this->enemiesMap->insert(Enemy_Item::STRING_RED_CHEEP_CHEEP, Enemy_Item::RED_CHEEP_CHEEP);
    this->enemiesMap->insert(Enemy_Item::STRING_PODOBOO, Enemy_Item::PODOBOO);
    this->enemiesMap->insert(Enemy_Item::STRING_PIRANHA_PLANT, Enemy_Item::PIRANHA_PLANT);
    this->enemiesMap->insert(Enemy_Item::STRING_LAKITU, Enemy_Item::LAKITU);
    this->enemiesMap->insert(Enemy_Item::STRING_SPINY, Enemy_Item::SPINY);
    this->enemiesMap->insert(Enemy_Item::STRING_BOWSER_FIRE_SPAWNER, Enemy_Item::BOWSER_FIRE_SPAWNER);
    this->enemiesMap->insert(Enemy_Item::STRING_CHEEP_CHEEP_SPAWNER, Enemy_Item::CHEEP_CHEEP_SPAWNER);
    this->enemiesMap->insert(Enemy_Item::STRING_BULLET_BILL_SPAWNER, Enemy_Item::BULLET_BILL_SPAWNER);
    this->enemiesMap->insert(Enemy_Item::STRING_FIRE_BAR, Enemy_Item::FIRE_BAR);
    this->enemiesMap->insert(Enemy_Item::STRING_LARGE_FIRE_BAR, Enemy_Item::LARGE_FIRE_BAR);
    this->enemiesMap->insert(Enemy_Item::STRING_LIFT, Enemy_Item::LIFT);
    this->enemiesMap->insert(Enemy_Item::STRING_FALLING_LIFT, Enemy_Item::FALLING_LIFT);
    this->enemiesMap->insert(Enemy_Item::STRING_BALANCE_LIFT, Enemy_Item::BALANCE_LIFT);
    this->enemiesMap->insert(Enemy_Item::STRING_SURFING_LIFT, Enemy_Item::SURFING_LIFT);
    this->enemiesMap->insert(Enemy_Item::STRING_LIFT_SPAWNER, Enemy_Item::LIFT_SPAWNER);
    this->enemiesMap->insert(Enemy_Item::STRING_BOWSER, Enemy_Item::BOWSER);
    this->enemiesMap->insert(Enemy_Item::STRING_WARP_ZONE, Enemy_Item::WARP_ZONE);
    this->enemiesMap->insert(Enemy_Item::STRING_PIPE_POINTER, Enemy_Item::PIPE_POINTER);
    this->enemiesMap->insert(Enemy_Item::STRING_TOAD, Enemy_Item::TOAD);
    this->enemiesMap->insert(Enemy_Item::STRING_GOOMBA_GROUP, Enemy_Item::GOOMBA_GROUP);
    this->enemiesMap->insert(Enemy_Item::STRING_KOOPA_GROUP, Enemy_Item::KOOPA_GROUP);
    this->enemiesMap->insert(Enemy_Item::STRING_PAGE_CHANGE, Enemy_Item::PAGE_CHANGE);
}
