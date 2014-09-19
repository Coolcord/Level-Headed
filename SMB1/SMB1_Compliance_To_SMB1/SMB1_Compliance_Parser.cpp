#include "SMB1_Compliance_Parser.h"
#include "../Common SMB1 Files/Object_Item.h"
#include "../Common SMB1 Files/Object_Item_String.h"
#include "../Common SMB1 Files/Enemy_Item.h"
#include "../Common SMB1 Files/Enemy_Item_String.h"

SMB1_Compliance_Parser::SMB1_Compliance_Parser() {
    //Build the map for the Objects
    this->objects = new QMap<QString, Object_Item::Object_Item>();
    this->Populate_Objects_Map();

    //Build the map for the enemies
    this->enemies = new QMap<QString, Enemy_Item::Enemy_Item>();
    this->Populate_Enemy_Map();

    //Build the map for enemy properties
    this->enemyProperties = new QMap<QString, Enemy_Item::Enemy_Item();




}

SMB1_Compliance_Parser::~SMB1_Compliance_Parser() {
    delete this->objects;
    delete this->enemies;
}

void SMB1_Compliance_Parser::Populate_Objects_Map() {
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
    this->objects->insert(Object_Item::STRING_CANNON, Object_Item::CANNON);
    this->objects->insert(Object_Item::STRING_ISLAND, Object_Item::ISLAND);
    this->objects->insert(Object_Item::STRING_HORIZONTAL_BRICKS, Object_Item::HORIZONTAL_BRICKS);
    this->objects->insert(Object_Item::STRING_HORIZONTAL_BLOCKS, Object_Item::HORIZONTAL_BLOCKS);
    this->objects->insert(Object_Item::STRING_HORIZONTAL_COINS, Object_Item::HORIZONTAL_COINS);
    this->objects->insert(Object_Item::STRING_VERTICAL_BRICKS, Object_Item::VERTICAL_BRICKS);
    this->objects->insert(Object_Item::STRING_VERTICAL_BLOCKS, Object_Item::VERTICAL_BLOCKS);
    this->objects->insert(Object_Item::STRING_PIPE, Object_Item::PIPE);
    this->objects->insert(Object_Item::STRING_ENTERABLE_PIPE, Object_Item::ENTERABLE_PIPE);
    this->objects->insert(Object_Item::STRING_HOLE, Object_Item::HOLE);
    this->objects->insert(Object_Item::STRING_HOLE_WITH_WATER, Object_Item::HOLE_WITH_WATER);
    this->objects->insert(Object_Item::STRING_BALANCE_ROPE, Object_Item::BALANCE_ROPE);
    this->objects->insert(Object_Item::STRING_BRIDGE, Object_Item::BRIDGE);
    this->objects->insert(Object_Item::STRING_HORIZONTAL_QUESTION_BLOCKS_WITH_COINS, Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS);
    this->objects->insert(Object_Item::STRING_PAGE_SKIP, Object_Item::PAGE_SKIP);
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
    this->objects->insert(Object_Item::STRING_BALANCE_LIFT_ROPE, Object_Item::BALANCE_LIFT_ROPE);
    this->objects->insert(Object_Item::STRING_STEPS, Object_Item::STEPS);
    this->objects->insert(Object_Item::STRING_END_STEPS, Object_Item::END_STEPS);
    this->objects->insert(Object_Item::STRING_TALL_REVERSE_L_PIPE, Object_Item::TALL_REVERSE_L_PIPE);
    this->objects->insert(Object_Item::STRING_PIPE_WALL, Object_Item::PIPE_WALL);
    this->objects->insert(Object_Item::STRING_NOTHING, Object_Item::NOTHING);
}

void SMB1_Compliance_Parser::Populate_Enemy_Map() {
    this->enemies->clear();

}

void SMB1_Compliance_Parser::Populate_Enemy_Properties_Map() {
    this->enemyProperties->clear();

}


