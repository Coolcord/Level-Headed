#include "Room_ID_Handler.h"
#include "Room_Order_Writer.h"
#include "Room_Address_Writer.h"
#include "Level_Offset.h"
#include "Enemy_Bytes_Tracker.h"
#include "Binary_Manipulator.h"
#include <assert.h>

Room_ID_Handler::Room_ID_Handler(QFile *file, Level_Offset *levelOffset) {
    assert(file);
    assert(levelOffset);
    this->file = file;
    this->currentLevel = Level::WORLD_1_LEVEL_1;
    this->levelOffset = levelOffset;
    this->enemyBytesTracker = nullptr;
    this->roomIDs = new QMap<Level::Level, unsigned char>();
    this->roomOrderWriter = nullptr;
    this->roomAddressWriter = nullptr;
    this->enemyBytesTracker = nullptr;
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
    assert(roomOrderWriter);
    this->roomOrderWriter = roomOrderWriter;
}

void Room_ID_Handler::Set_Room_Address_Writer(Room_Address_Writer *roomAddressWriter) {
    assert(roomAddressWriter);
    this->roomAddressWriter = roomAddressWriter;
}

void Room_ID_Handler::Set_Enemy_Bytes_Tracker(Enemy_Bytes_Tracker *enemyBytesTracker) {
    assert(enemyBytesTracker);
    this->enemyBytesTracker = enemyBytesTracker;
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

bool Room_ID_Handler::Get_Level_From_Room_ID(unsigned char id, Level::Level &level) {
    foreach (unsigned char value, this->roomIDs->values()) {
        if (value == id) {
            level = this->roomIDs->key(value);
            return true;
        }
    }
    return false;
}

QVector<unsigned char> *Room_ID_Handler::Get_Midpoint_Indexes_From_Room_ID(unsigned char id) {
    QMap<unsigned char, QVector<unsigned char>*>::iterator iter = this->midpointIndexes->find(id);
    if (iter == this->midpointIndexes->end()) return nullptr; //not found
    return iter.value();
}

QVector<unsigned char> *Room_ID_Handler::Get_Midpoint_Indexes_From_Current_Level() {
    return this->Get_Midpoint_Indexes_From_Level(this->currentLevel);
}

QVector<unsigned char> *Room_ID_Handler::Get_Midpoint_Indexes_From_Level(Level::Level level) {
    unsigned char roomID = 0;
    if (!this->Get_Room_ID_From_Level(level, roomID)) return nullptr;
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
    QMap<unsigned char, Level::Level> oldRoomIDs;
    foreach (unsigned char value, this->roomIDs->values()) {
        Level::Level level = this->roomIDs->key(value);
        oldRoomIDs.insert(value, level);
    }

    //Copy the Room Order
    QVector<Level::Level> oldRoomOrder;
    for (i = 0; i < 36; ++i) {
        assert(oldRoomIDs.contains(static_cast<unsigned char>(this->roomOrderWriter->buffer->data()[i])));
        oldRoomOrder.append(oldRoomIDs[static_cast<unsigned char>(this->roomOrderWriter->buffer->data()[i])]);
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
        if ((static_cast<unsigned char>(this->roomAddressWriter->enemiesHeaderBuffer->data()[i]) > static_cast<unsigned char>(this->roomAddressWriter->enemiesHeaderBuffer->data()[newAttribute]))
            && (addressLength > static_cast<unsigned char>(this->roomAddressWriter->enemiesHeaderBuffer->data()[i]))) {
            addressLength = static_cast<unsigned char>(this->roomAddressWriter->enemiesHeaderBuffer->data()[i]);
        }
    }
    unsigned char newRoomNum = addressLength-static_cast<unsigned char>(this->roomAddressWriter->enemiesHeaderBuffer->data()[newAttribute])-1;

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
        if (((id>>5)&0x3) == oldAttribute) {
            if (oldRoomNum == (id&0x1F)) {
                this->roomOrderWriter->buffer->data()[i] = static_cast<char>((newAttribute<<5)|newRoomNum);
            } else if (oldRoomNum < (id&0x1F)) {
                this->roomOrderWriter->buffer->data()[i] = static_cast<char>(id-1);
            }
        }
    }

    //Fix the Room IDs
    this->Update_Room_IDs(oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->roomOrderWriter->Populate_Midpoint_Indexes_In_Handler();

    //Fix Pipe Pointers
    if (!this->Update_Pipe_Pointers(oldRoomIDs)) return false;

    return true;
}

unsigned char Room_ID_Handler::Get_Value_From_Attribute(Level_Attribute::Level_Attribute attribute) {
    switch (attribute) {
    case Level_Attribute::UNDERWATER:   return 0;
    case Level_Attribute::OVERWORLD:    return 1;
    case Level_Attribute::UNDERGROUND:  return 2;
    case Level_Attribute::CASTLE:       return 3;
    }
    assert(false); return 0;
}

bool Room_ID_Handler::Send_Object_Bytes_From_One_Level_To_Another(Level::Level fromLevel, Level::Level toLevel, int numBytes) {
    assert(this->file);
    assert(fromLevel != toLevel);

    //Get the Level Offsets
    qint64 fromOffset = this->levelOffset->Get_Level_Object_Offset(fromLevel);
    qint64 toOffset = this->levelOffset->Get_Level_Object_Offset(toLevel);

    return this->Send_Bytes_From_One_Offset_To_Another(fromOffset, toOffset, numBytes, false);
}

bool Room_ID_Handler::Send_Enemy_Bytes_From_One_Level_To_Another(Level::Level fromLevel, Level::Level toLevel, int numBytes) {
    assert(this->file);
    assert(fromLevel != toLevel);
    assert(this->enemyBytesTracker);

    //Get the previous byte counts
    int oldFromBytes = this->enemyBytesTracker->Get_Enemy_Byte_Count_In_Level(fromLevel);
    if (oldFromBytes - numBytes < 0) return false;
    int oldToBytes = this->enemyBytesTracker->Get_Enemy_Byte_Count_In_Level(toLevel);

    //Get the Level Offsets
    qint64 fromOffset = this->levelOffset->Get_Level_Enemy_Offset(fromLevel);
    qint64 toOffset = this->levelOffset->Get_Level_Enemy_Offset(toLevel);

    //Move the bytes
    bool success = this->Send_Bytes_From_One_Offset_To_Another(fromOffset, toOffset, numBytes, true);
    if (success) { //update the enemy bytes tracker with the updated values if the move is successful
        this->enemyBytesTracker->Set_Enemy_Byte_Count_In_Level(fromLevel, oldFromBytes-numBytes);
        this->enemyBytesTracker->Set_Enemy_Byte_Count_In_Level(toLevel, oldToBytes+numBytes);
    }
    return success;
}

bool Room_ID_Handler::Send_Bytes_From_One_Offset_To_Another(qint64 fromOffset, qint64 toOffset, int numBytes, bool enemies) {
    assert(this->file);
    assert(fromOffset != toOffset);

    //Calculate the Buffer Size and Buffer Read Offset
    qint64 offset = 0;
    int bufferSize = 0;
    bool fromFirst = false;
    if (toOffset > fromOffset) {
        bufferSize = static_cast<int>(toOffset - fromOffset);
        offset = fromOffset;
        fromFirst = true;
    } else { //toLevel comes first in the binary
        bufferSize = static_cast<int>((fromOffset+numBytes) - toOffset);
        offset = toOffset;
    }

    //Determine if the first byte is valid
    unsigned char firstByte = 0;
    if (!this->Read_First_Byte(offset, firstByte)) return false;

    //Read into the Buffer
    QByteArray byteBuffer = QByteArray(bufferSize, ' ');
    if (!this->file->seek(offset)) return false;
    qint64 ret = this->file->read(byteBuffer.data(), bufferSize);
    if (ret != bufferSize || byteBuffer.size() != bufferSize) return false;

    //Send the Bytes to the according level
    assert(numBytes < bufferSize);
    QByteArray data = QByteArray(numBytes, static_cast<char>(0xFF));
    if (fromFirst) {
        if (!enemies) {
            for (int i = 0; i < numBytes; ++i) {
                data.data()[i] = byteBuffer.at(i);
            }
        }
        byteBuffer = byteBuffer.remove(0, numBytes);
        byteBuffer.append(data);
    } else {
        if (!enemies) {
            for (int i = 0; i < numBytes; ++i) {
                data.data()[i] = byteBuffer.at((bufferSize-numBytes)+i);
            }
        }
        byteBuffer = byteBuffer.remove(bufferSize-numBytes, numBytes);
        byteBuffer.prepend(data);
    }

    //Write the Buffer back to the ROM
    if (!this->file->seek(offset)) return false;
    if (this->file->write(byteBuffer.data(), byteBuffer.length()) != byteBuffer.length()) return false;
    return this->roomAddressWriter->Fix_Level_Addresses(fromOffset, toOffset, numBytes);
}

bool Room_ID_Handler::Read_First_Byte(qint64 offset, unsigned char &c) {
    assert(this->file);
    QByteArray buffer(1, ' ');
    if (!this->file->seek(offset)) return false;
    qint64 ret = this->file->read(buffer.data(), 1);
    if (ret != 1 || buffer.data() == nullptr) return false;
    unsigned char firstByte = static_cast<unsigned char>(buffer.data()[0]);
    if (firstByte == 0xFD) return false; //end of the level
    c = firstByte;
    return true;
}

void Room_ID_Handler::Update_Room_IDs(unsigned char oldRoomNum, unsigned char newRoomNum, unsigned char oldAttribute, unsigned char newAttribute) {
    this->Update_Room_ID(Level::WORLD_1_LEVEL_1, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_1_LEVEL_2, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_1_LEVEL_3, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_1_LEVEL_4, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_2_LEVEL_1, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_2_LEVEL_2, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_2_LEVEL_3, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_2_LEVEL_4, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_3_LEVEL_1, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_3_LEVEL_2, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_3_LEVEL_3, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_3_LEVEL_4, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_4_LEVEL_1, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_4_LEVEL_2, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_4_LEVEL_3, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_4_LEVEL_4, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_5_LEVEL_1, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_5_LEVEL_2, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_6_LEVEL_1, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_6_LEVEL_2, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_6_LEVEL_3, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_7_LEVEL_1, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_7_LEVEL_4, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_8_LEVEL_1, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_8_LEVEL_2, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_8_LEVEL_3, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WORLD_8_LEVEL_4, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::PIPE_INTRO, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::UNDERWATER_BONUS, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::UNDERWATER_CASTLE, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::CLOUD_BONUS_1, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::CLOUD_BONUS_2, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::UNDERGROUND_BONUS, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
    this->Update_Room_ID(Level::WARP_ZONE, oldRoomNum, newRoomNum, oldAttribute, newAttribute);
}

void Room_ID_Handler::Update_Room_ID(Level::Level level, unsigned char oldRoomNum, unsigned char newRoomNum, unsigned char oldAttribute, unsigned char newAttribute) {
    assert(this->roomIDs->contains(level));
    unsigned char value = this->roomIDs->value(level);
    if (((value>>5)&0x3) == oldAttribute) {
        if (oldRoomNum == (value&0x1F)) {
            value = static_cast<unsigned char>((newAttribute<<5)|newRoomNum);
        } else if (oldRoomNum < (value&0x1F)) {
            --value;
        }
    }
    assert(this->roomIDs->remove(level) != 0);
    this->roomIDs->insert(level, value);
}

bool Room_ID_Handler::Update_Pipe_Pointers(const QMap<unsigned char, Level::Level> &oldRoomIDs) {
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_1_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_1_LEVEL_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_1_LEVEL_3)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_1_LEVEL_4)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_2_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_2_LEVEL_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_2_LEVEL_3)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_2_LEVEL_4)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_3_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_3_LEVEL_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_3_LEVEL_3)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_3_LEVEL_4)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_4_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_4_LEVEL_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_4_LEVEL_3)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_4_LEVEL_4)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_5_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_5_LEVEL_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_6_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_6_LEVEL_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_6_LEVEL_3)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_7_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_7_LEVEL_4)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_8_LEVEL_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_8_LEVEL_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_8_LEVEL_3)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::WORLD_8_LEVEL_4)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::UNDERWATER_BONUS)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::UNDERWATER_CASTLE)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::CLOUD_BONUS_1)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::CLOUD_BONUS_2)) return false;
    if (!this->Update_Pipe_Pointers_At_Level(oldRoomIDs, Level::UNDERGROUND_BONUS)) return false;
    return true;
}

bool Room_ID_Handler::Update_Pipe_Pointers_At_Level(const QMap<unsigned char, Level::Level> &oldRoomIDs, Level::Level level) {
    if (this->currentLevel == level) return true; //don't bother messing with the current level, as it will be written later
    qint64 offset = this->levelOffset->Get_Level_Enemy_Offset(level);
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
            bool pageFlag = false;
            if (!oldRoomIDs.contains(roomID)) {
                roomID = (roomID^0x80); //try the other value
                if (!oldRoomIDs.contains(roomID)) continue; //unknown room ID... just ignore it
            }
            if (roomID > 0x80) pageFlag = true;
            Level::Level key = oldRoomIDs.value(roomID);
            if (this->roomIDs->contains(key)) {
                unsigned char properValue = this->roomIDs->value(key);
                if (pageFlag) {
                    if (properValue < 0x80) properValue += 0x80;
                } else {
                    if (properValue > 0x80) properValue -= 0x80;
                }
                buffer.data()[1] = static_cast<char>(properValue);
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
    this->roomIDs->insert(Level::WORLD_6_LEVEL_1, 0x2E);
    this->roomIDs->insert(Level::WORLD_6_LEVEL_2, 0x23);
    this->roomIDs->insert(Level::WORLD_6_LEVEL_3, 0x2D);
    this->roomIDs->insert(Level::WORLD_7_LEVEL_1, 0x33);
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
