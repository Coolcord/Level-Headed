#include "Room_ID_Handler.h"
#include "Room_Order_Writer.h"
#include "Room_Address_Writer.h"
#include "Level_Offset.h"
#include "Binary_Manipulator.h"
#include <assert.h>

Room_ID_Handler::Room_ID_Handler(QFile *file, Level_Offset *levelOffset) {
    assert(file);
    assert(levelOffset);
    this->file = file;
    this->currentLevel = Level::WORLD_1_LEVEL_1;
    this->levelOffset = levelOffset;
    this->roomIDs = new QMap<Level::Level, unsigned char>();
    this->roomOrderWriter = NULL;
    this->roomAddressWriter = NULL;
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

void Room_ID_Handler::Set_Room_Order_Writer(Room_Order_Writer *roomOrderWriter) {
    this->roomOrderWriter = roomOrderWriter;
}

void Room_ID_Handler::Set_Room_Address_Writer(Room_Address_Writer *roomAddressWriter) {
    this->roomAddressWriter = roomAddressWriter;
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

bool Room_ID_Handler::Change_Current_Level_Attribute(Level_Attribute::Level_Attribute attribute) {
    return this->Change_Level_Attribute(this->currentLevel, attribute);
}

bool Room_ID_Handler::Change_Level_Attribute(Level::Level level, Level_Attribute::Level_Attribute attribute) {
    unsigned char id = 0;
    if (!this->Get_Room_ID_From_Level(level, id)) return false;
    return this->Change_Room_Attribute(id, attribute);
}

bool Room_ID_Handler::Change_Room_Attribute(unsigned char oldRoomID, Level_Attribute::Level_Attribute attribute) {
    //This function requires access to the Room Order Writer
    if (!this->roomOrderWriter) return false;
    //This function requires access to the Room Address Writer
    if (!this->roomAddressWriter) return false;

    //Get the values of the attributes
    unsigned char newAttribute = this->Get_Value_From_Attribute(attribute);
    unsigned char oldAttribute = ((oldRoomID>>5)&0x3);
    unsigned char oldRoomNum = (oldRoomID&0x1F);
    unsigned char i = 0;
    if (newAttribute == oldAttribute) return true; //nothing to do

    //Copy the Room IDs
    QMap<unsigned char, Level::Level> oldRoomIDs = *(this->roomIDs);

    //Copy the Room Order
    QVector<Level::Level> oldRoomOrder;
    for (i = 0; i < 36; ++i) {
        assert(oldRoomIDs.contains(this->roomOrderWriter->buffer->data()[i]));
        oldRoomOrder.append(oldRoomIDs[this->roomOrderWriter->buffer->data()[i]]);
    }

    //Copy the Header Address Tables
    QByteArray oldObjectsHeaderBuffer = *(this->roomAddressWriter->objectsHeaderBuffer);
    QByteArray oldEnemiesHeaderBuffer = *(this->roomAddressWriter->enemiesHeaderBuffer);

    //Fix the Enemy Header
    for (i = 0; i < 4; ++i) {
        if (static_cast<unsigned char>(oldEnemiesHeaderBuffer.data()[i]) > static_cast<unsigned char>(oldEnemiesHeaderBuffer.data()[newAttribute])) {
            ++(this->roomAddressWriter->enemiesHeaderBuffer->data()[i]);
        }
    }
    for (i = 0; i < 4; ++i) {
        if (static_cast<unsigned char>(oldEnemiesHeaderBuffer.data()[i]) > static_cast<unsigned char>(oldEnemiesHeaderBuffer.data()[oldAttribute])) {
            --(this->roomAddressWriter->enemiesHeaderBuffer->data()[i]);
        }
    }

    //Fix the Object Header
    for (i = 0; i < 4; ++i) {
        if (static_cast<unsigned char>(oldObjectsHeaderBuffer.data()[i]) > static_cast<unsigned char>(oldObjectsHeaderBuffer.data()[newAttribute])) {
            ++(this->roomAddressWriter->objectsHeaderBuffer->data()[i]);
        }
    }
    for (i = 0; i < 4; ++i) {
        if (static_cast<unsigned char>(oldObjectsHeaderBuffer.data()[i]) > static_cast<unsigned char>(oldObjectsHeaderBuffer.data()[oldAttribute])) {
            --(this->roomAddressWriter->objectsHeaderBuffer->data()[i]);
        }
    }

    //Get a new room ID
    unsigned char addressLength = 34;
    for (i = 0; i < 4; ++i) {
        if (static_cast<unsigned char>(this->roomAddressWriter->enemiesHeaderBuffer->data()[i]) > static_cast<unsigned char>(this->roomAddressWriter->enemiesHeaderBuffer->data()[newAttribute])
            && addressLength > static_cast<unsigned char>(this->roomAddressWriter->enemiesHeaderBuffer->data()[i])) {
            addressLength = static_cast<unsigned char>(this->roomAddressWriter->enemiesHeaderBuffer->data()[i]);
        }
    }
    unsigned char newRoomNum = addressLength-static_cast<unsigned char>(oldEnemiesHeaderBuffer.data()[newAttribute])-1;

    //Fix the enemy addresses
    unsigned char tmpLowByte = static_cast<unsigned char>(this->roomAddressWriter->lowEnemyBuffer->data()[static_cast<unsigned char>(oldEnemiesHeaderBuffer.data()[oldAttribute])+oldRoomNum]);
    unsigned char tmpHighByte = static_cast<unsigned char>(this->roomAddressWriter->highEnemyBuffer->data()[static_cast<unsigned char>(oldEnemiesHeaderBuffer.data()[oldAttribute])+oldRoomNum]);
    for (i = static_cast<unsigned char>(oldEnemiesHeaderBuffer.data()[oldAttribute])+oldRoomNum; i < 33; ++i) {
        this->roomAddressWriter->lowEnemyBuffer->data()[i] = this->roomAddressWriter->lowEnemyBuffer->data()[i+1];
        this->roomAddressWriter->highEnemyBuffer->data()[i] = this->roomAddressWriter->highEnemyBuffer->data()[i+1];
    }
    for (i = 33; i > static_cast<unsigned char>(this->roomAddressWriter->enemiesHeaderBuffer->data()[newAttribute])+newRoomNum; --i) {
        this->roomAddressWriter->lowEnemyBuffer->data()[i] = this->roomAddressWriter->lowEnemyBuffer->data()[i-1];
        this->roomAddressWriter->highEnemyBuffer->data()[i] = this->roomAddressWriter->highEnemyBuffer->data()[i-1];
    }
    this->roomAddressWriter->lowEnemyBuffer->data()[i] = static_cast<char>(tmpLowByte);
    this->roomAddressWriter->highEnemyBuffer->data()[i] = static_cast<char>(tmpHighByte);


    //Fix the object addresses
    tmpLowByte = static_cast<unsigned char>(this->roomAddressWriter->lowObjectBuffer->data()[static_cast<unsigned char>(oldObjectsHeaderBuffer.data()[oldAttribute])+oldRoomNum]);
    tmpHighByte = static_cast<unsigned char>(this->roomAddressWriter->highObjectBuffer->data()[static_cast<unsigned char>(oldObjectsHeaderBuffer.data()[oldAttribute])+oldRoomNum]);
    for (i = static_cast<unsigned char>(oldObjectsHeaderBuffer.data()[oldAttribute])+oldRoomNum; i < 33; ++i) {
        this->roomAddressWriter->lowObjectBuffer->data()[i] = this->roomAddressWriter->lowObjectBuffer->data()[i+1];
        this->roomAddressWriter->highObjectBuffer->data()[i] = this->roomAddressWriter->highObjectBuffer->data()[i+1];
    }
    for (i = 33; i > static_cast<unsigned char>(this->roomAddressWriter->objectsHeaderBuffer->data()[newAttribute])+newRoomNum; --i) {
        this->roomAddressWriter->lowObjectBuffer->data()[i] = this->roomAddressWriter->lowObjectBuffer->data()[i-1];
        this->roomAddressWriter->highObjectBuffer->data()[i] = this->roomAddressWriter->highObjectBuffer->data()[i-1];
    }
    this->roomAddressWriter->lowObjectBuffer->data()[i] = static_cast<char>(tmpLowByte);
    this->roomAddressWriter->highObjectBuffer->data()[i] = static_cast<char>(tmpHighByte);

    //Fix the Room Order Table
    for (i = 0; i < 36; ++i) {
        unsigned char id = static_cast<unsigned char>(this->roomOrderWriter->buffer->data()[i]);
        if (oldRoomNum == (id&0x1F)) {
            this->roomOrderWriter->buffer->data()[i] = static_cast<char>((newAttribute<<5)|newRoomNum);
        } else if (newRoomNum < (id&0x1F)) {
            --id;
            this->roomOrderWriter->buffer->data()[i] = static_cast<char>(id);
        }
    }

    //Fix the Room IDs
    //foreach (unsigned char value, this->roomIDs)

    //Run through the Room Order Table and update the Room IDs that have changed
    QMap<Level::Level, unsigned char> newRoomLevels = oldRoomLevels;
    for (i = 0; i < 36; ++i) {
        newRoomLevels.insert(oldRoomOrder.at(i), this->roomOrderWriter->buffer->data()[i]);
    }
    this->Update_Room_IDs(newRoomLevels);
    this->roomOrderWriter->Populate_Midpoint_Indexes_In_Handler();

    //Fix Pipe Pointers
    if (!this->Update_Pipe_Pointers(oldRoomIDs, newRoomLevels)) return false;

    return true;
}

unsigned char Room_ID_Handler::Get_Value_From_Attribute(Level_Attribute::Level_Attribute attribute) {
    switch (attribute) {
    case Level_Attribute::UNDERWATER:   return 0;
    case Level_Attribute::OVERWORLD:    return 1;
    case Level_Attribute::UNDERGROUND:  return 2;
    case Level_Attribute::CASTLE:       return 3;
    default:                            assert(false); return 0;
    }
}

void Room_ID_Handler::Update_Room_IDs(const QMap<Level::Level, unsigned char> &newRoomLevels) {
    this->Update_Room_ID(newRoomLevels, Level::WORLD_1_LEVEL_1);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_1_LEVEL_2);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_1_LEVEL_3);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_1_LEVEL_4);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_2_LEVEL_1);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_2_LEVEL_2);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_2_LEVEL_3);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_2_LEVEL_4);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_3_LEVEL_1);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_3_LEVEL_2);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_3_LEVEL_3);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_3_LEVEL_4);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_4_LEVEL_1);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_4_LEVEL_2);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_4_LEVEL_3);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_4_LEVEL_4);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_5_LEVEL_1);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_5_LEVEL_2);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_5_LEVEL_3);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_5_LEVEL_4);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_6_LEVEL_1);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_6_LEVEL_2);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_6_LEVEL_3);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_6_LEVEL_4);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_7_LEVEL_1);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_7_LEVEL_2);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_7_LEVEL_3);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_7_LEVEL_4);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_8_LEVEL_1);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_8_LEVEL_2);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_8_LEVEL_3);
    this->Update_Room_ID(newRoomLevels, Level::WORLD_8_LEVEL_4);
    this->Update_Room_ID(newRoomLevels, Level::PIPE_INTRO);
    this->Update_Room_ID(newRoomLevels, Level::UNDERWATER_BONUS);
    this->Update_Room_ID(newRoomLevels, Level::UNDERWATER_CASTLE);
    this->Update_Room_ID(newRoomLevels, Level::CLOUD_BONUS_1);
    this->Update_Room_ID(newRoomLevels, Level::CLOUD_BONUS_2);
    this->Update_Room_ID(newRoomLevels, Level::UNDERGROUND_BONUS);
    this->Update_Room_ID(newRoomLevels, Level::WARP_ZONE);
}

void Room_ID_Handler::Update_Room_ID(const QMap<Level::Level, unsigned char> &newRoomLevels, Level::Level level) {
    Level::Level baseLevel = this->Get_Base_Level(level);
    if (newRoomLevels.contains(baseLevel)) {
        this->roomIDs->remove(level);
        this->roomIDs->insert(level, newRoomLevels.value(baseLevel));
    }
}

bool Room_ID_Handler::Update_Pipe_Pointers(const QMap<unsigned char, Level::Level> &oldRoomIDs, const QMap<Level::Level, unsigned char> &newRoomLevels) {
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_1_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_1_LEVEL_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_1_LEVEL_3)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_1_LEVEL_4)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_2_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_2_LEVEL_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_2_LEVEL_3)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_2_LEVEL_4)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_3_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_3_LEVEL_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_3_LEVEL_3)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_3_LEVEL_4)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_4_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_4_LEVEL_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_4_LEVEL_3)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_4_LEVEL_4)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_5_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_5_LEVEL_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_6_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_6_LEVEL_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_6_LEVEL_3)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_7_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_7_LEVEL_4)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_8_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_8_LEVEL_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_8_LEVEL_3)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::WORLD_8_LEVEL_4)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::UNDERWATER_BONUS)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::UNDERWATER_CASTLE)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::CLOUD_BONUS_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::CLOUD_BONUS_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, newRoomLevels, Level::UNDERGROUND_BONUS)) return false;
    return true;
}

bool Room_ID_Handler::Update_Pipe_Pointers_At_Level(const QMap<unsigned char, Level::Level> &oldRoomIDs, const QMap<Level::Level, unsigned char> &newRoomLevels, Level::Level level) {
    if (this->currentLevel == level) return true; //don't bother messing with the current level, as it will be written later
    int offset = this->levelOffset->Get_Level_Enemy_Offset(level);
    if (offset == BAD_OFFSET) return false;
    if (offset == 0) return true; //nothing to read
    if (!this->file->seek(offset)) return false;

    //Read the enemies from the level, fixing Pipe Pointers along the way
    QByteArray enemiesBuffer;
    for (QByteArray coordinate = this->file->peek(1); !coordinate.isEmpty() &&
         static_cast<unsigned char>(coordinate.data()[0]) != 0xFF; coordinate = this->file->peek(1)) {
        if (Binary_Manipulator::Get_Second_Digit_From_Hex(static_cast<unsigned char>(coordinate[0])) == 0xE) { //pipe pointer
            QByteArray buffer(3, ' ');
            if (this->file->read(buffer.data(), 3) != 3) return false; //something went wrong...
            unsigned char roomID = static_cast<unsigned char>(buffer.data()[1]);
            if (!oldRoomIDs.contains(roomID)) {
                roomID = (roomID!=0x80); //try the other value
                if (!oldRoomIDs.contains(roomID)) return false;
            }
            Level::Level key = oldRoomIDs.value(roomID);
            if (newRoomLevels.contains(key)) {
                buffer.data()[1] = static_cast<char>(newRoomLevels.value(key));
            }
            enemiesBuffer.append(buffer);
        } else { //typical enemy
            QByteArray buffer(2, ' ');
            if (this->file->read(buffer.data(), 2) != 2) return false; //something went wrong...
            enemiesBuffer.append(buffer);
        }
    }

    //Write the enemies back to the ROM
    if (enemiesBuffer.isEmpty()) return true; //nothing to write
    if (!this->file->seek(offset)) return false;
    return this->file->write(enemiesBuffer.data(), enemiesBuffer.length()) == enemiesBuffer.length();
}

Level::Level Room_ID_Handler::Get_Base_Level(Level::Level level) {
    switch (level) {
    case Level::WORLD_1_LEVEL_2:   return Level::WORLD_1_LEVEL_2;
    case Level::WORLD_1_LEVEL_3:
    case Level::WORLD_5_LEVEL_3:   return Level::WORLD_1_LEVEL_3;
    case Level::WORLD_1_LEVEL_4:
    case Level::WORLD_6_LEVEL_4:   return Level::WORLD_1_LEVEL_4;
    case Level::WORLD_2_LEVEL_2:
    case Level::WORLD_7_LEVEL_2:   return Level::WORLD_2_LEVEL_2;
    case Level::WORLD_2_LEVEL_3:
    case Level::WORLD_7_LEVEL_3:   return Level::WORLD_2_LEVEL_3;
    case Level::WORLD_2_LEVEL_4:
    case Level::WORLD_5_LEVEL_4:   return Level::WORLD_2_LEVEL_4;
    default:                       return level;
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
    this->roomIDs->insert(Level::UNDERGROUND_BONUS, 0xC2);
    this->roomIDs->insert(Level::WARP_ZONE, 0x2F);
}
