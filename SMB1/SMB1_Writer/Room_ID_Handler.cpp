#include "Room_ID_Handler.h"
#include <assert.h>

Room_ID_Handler::Room_ID_Handler() {
    this->currentLevel = Level::WORLD_1_LEVEL_1;
    this->roomIDs = new QMap<Level::Level, unsigned char>();
    this->midpointIndexes = new QMap<unsigned char, QVector<unsigned char>*>();
    this->Populate_Room_IDs();
}

Room_ID_Handler::~Room_ID_Handler() {
    delete this->roomIDs;

    //Deallocate all of the QVectors
    foreach (QVector<unsigned char> *vector, this->midpointIndexes->values()) {
        delete vector;
    }
    delete this->midpointIndexes;
}

Level::Level Room_ID_Handler::Get_Current_Level() {
    return this->currentLevel;
}

void Room_ID_Handler::Set_Current_Level(Level::Level level) {
    this->currentLevel = level;
}

bool Room_ID_Handler::Get_Room_ID_From_Level(Level::Level level, unsigned char &id) {
    QMap<Level::Level, unsigned char>::iterator iter = this->roomIDs->find(level);
    if (iter == this->roomIDs->end()) return false; //not found
    id = iter.value();
    return true;
}

QVector<unsigned char> *Room_ID_Handler::Get_Midpoint_Indexes_From_Room_ID(unsigned char id) {
    QMap<unsigned char, QVector<unsigned char>*>::iterator iter = this->midpointIndexes->find(id);
    if (iter == this->midpointIndexes->end()) return NULL; //not found
    return iter.value();
}

QVector<unsigned char> *Room_ID_Handler::Get_Midpoint_Indexes_From_Current_Level() {
    return this->Get_Midpoint_Indexes_From_Level(this->currentLevel);
}

QVector<unsigned char> *Room_ID_Handler::Get_Midpoint_Indexes_From_Level(Level::Level level) {
    unsigned char roomID = 0;
    if (!this->Get_Room_ID_From_Level(level, roomID)) return NULL;
    return this->Get_Midpoint_Indexes_From_Room_ID(roomID);
}

Level_Attribute::Level_Attribute Room_ID_Handler::Get_Level_Attribute_From_Current_Level() {
    return this->Get_Level_Attribute_From_Level(this->currentLevel);
}

Level_Attribute::Level_Attribute Room_ID_Handler::Get_Level_Attribute_From_Level(Level::Level level) {
    unsigned char id = 0;
    assert(this->Get_Room_ID_From_Level(level, id));
    return this->Get_Level_Attribute_From_ID(id);
}

Level_Attribute::Level_Attribute Room_ID_Handler::Get_Level_Attribute_From_ID(unsigned char id) {
    unsigned char attribute = ((id >> 5) & 0x3);
    switch (attribute) {
    case 0: return Level_Attribute::UNDERWATER;
    case 1: return Level_Attribute::OVERWORLD;
    case 2: return Level_Attribute::UNDERGROUND;
    case 3: return Level_Attribute::CASTLE;
    default: assert(false); return Level_Attribute::OVERWORLD;
    }
}

void Room_ID_Handler::Populate_Room_IDs() {
    this->roomIDs->clear();
    this->roomIDs->insert(Level::WORLD_1_LEVEL_1, 0x25);
    this->roomIDs->insert(Level::WORLD_1_LEVEL_2, 0xC0);
    this->roomIDs->insert(Level::WORLD_1_LEVEL_3, 0x26);
    this->roomIDs->insert(Level::WORLD_1_LEVEL_4, 0x60);
    this->roomIDs->insert(Level::WORLD_2_LEVEL_1, 0x28);
    this->roomIDs->insert(Level::WORLD_2_LEVEL_2, 0x01);
    this->roomIDs->insert(Level::WORLD_2_LEVEL_3, 0x27);
    this->roomIDs->insert(Level::WORLD_2_LEVEL_4, 0x62);
    this->roomIDs->insert(Level::WORLD_3_LEVEL_1, 0x24);
    this->roomIDs->insert(Level::WORLD_3_LEVEL_2, 0x35);
    this->roomIDs->insert(Level::WORLD_3_LEVEL_3, 0x20);
    this->roomIDs->insert(Level::WORLD_3_LEVEL_4, 0x63);
    this->roomIDs->insert(Level::WORLD_4_LEVEL_1, 0x22);
    this->roomIDs->insert(Level::WORLD_4_LEVEL_2, 0x41);
    this->roomIDs->insert(Level::WORLD_4_LEVEL_3, 0x2C);
    this->roomIDs->insert(Level::WORLD_4_LEVEL_4, 0x61);
    this->roomIDs->insert(Level::WORLD_5_LEVEL_1, 0x2A);
    this->roomIDs->insert(Level::WORLD_5_LEVEL_2, 0x31);
    this->roomIDs->insert(Level::WORLD_5_LEVEL_3, 0x26);
    this->roomIDs->insert(Level::WORLD_5_LEVEL_4, 0x62);
    this->roomIDs->insert(Level::WORLD_6_LEVEL_1, 0x2E);
    this->roomIDs->insert(Level::WORLD_6_LEVEL_2, 0x23);
    this->roomIDs->insert(Level::WORLD_6_LEVEL_3, 0x2D);
    this->roomIDs->insert(Level::WORLD_6_LEVEL_4, 0x60);
    this->roomIDs->insert(Level::WORLD_7_LEVEL_1, 0x33);
    this->roomIDs->insert(Level::WORLD_7_LEVEL_2, 0x01);
    this->roomIDs->insert(Level::WORLD_7_LEVEL_3, 0x27);
    this->roomIDs->insert(Level::WORLD_7_LEVEL_4, 0x64);
    this->roomIDs->insert(Level::WORLD_8_LEVEL_1, 0x30);
    this->roomIDs->insert(Level::WORLD_8_LEVEL_2, 0x32);
    this->roomIDs->insert(Level::WORLD_8_LEVEL_3, 0x21);
    this->roomIDs->insert(Level::WORLD_8_LEVEL_4, 0x65);
    this->roomIDs->insert(Level::PIPE_INTRO, 0x29);
    this->roomIDs->insert(Level::UNDERWATER_BONUS, 0x00);
    this->roomIDs->insert(Level::UNDERWATER_CASTLE, 0x02);
    this->roomIDs->insert(Level::CLOUD_BONUS_1, 0x2B);
    this->roomIDs->insert(Level::CLOUD_BONUS_2, 0x34);
    this->roomIDs->insert(Level::UNDERGROUND_BONUS, 0x42);
}
