#include "Enemy_Bytes_Tracker.h"
#include "Level_Offset.h"
#include "Binary_Manipulator.h"
#include <assert.h>

Enemy_Bytes_Tracker::Enemy_Bytes_Tracker(QFile *file, Level_Offset *levelOffset) {
    assert(file);
    assert(levelOffset);
    this->file = file;
    this->levelOffset = levelOffset;
    this->enemyByteCount = new QMap<Level::Level, int>;
    this->Initialize_Enemy_Byte_Counts();
}

Enemy_Bytes_Tracker::~Enemy_Bytes_Tracker() {
    delete this->enemyByteCount;
    this->enemyByteCount = nullptr;
    this->file = nullptr;
    this->levelOffset = nullptr;
}

int Enemy_Bytes_Tracker::Get_Enemy_Byte_Count_In_Level(Level::Level level) {
    assert(this->enemyByteCount);
    const QMap<Level::Level, int>::const_iterator iter = this->enemyByteCount->find(level);
    assert(iter != this->enemyByteCount->end());
    return iter.value();
}

void Enemy_Bytes_Tracker::Set_Enemy_Byte_Count_In_Level(Level::Level level, int numBytes) {
    assert(this->enemyByteCount);
    assert(numBytes >= 0);
    this->enemyByteCount->insert(level, numBytes);
}

void Enemy_Bytes_Tracker::Initialize_Enemy_Byte_Counts() {
    assert(this->enemyByteCount);
    this->enemyByteCount->insert(Level::WORLD_1_LEVEL_1, 0);
    this->enemyByteCount->insert(Level::WORLD_1_LEVEL_2, 0);
    this->enemyByteCount->insert(Level::WORLD_1_LEVEL_3, 0);
    this->enemyByteCount->insert(Level::WORLD_1_LEVEL_4, 0);
    this->enemyByteCount->insert(Level::WORLD_2_LEVEL_1, 0);
    this->enemyByteCount->insert(Level::WORLD_2_LEVEL_2, 0);
    this->enemyByteCount->insert(Level::WORLD_2_LEVEL_3, 0);
    this->enemyByteCount->insert(Level::WORLD_2_LEVEL_4, 0);
    this->enemyByteCount->insert(Level::WORLD_3_LEVEL_1, 0);
    this->enemyByteCount->insert(Level::WORLD_3_LEVEL_2, 0);
    this->enemyByteCount->insert(Level::WORLD_3_LEVEL_3, 0);
    this->enemyByteCount->insert(Level::WORLD_3_LEVEL_4, 0);
    this->enemyByteCount->insert(Level::WORLD_4_LEVEL_1, 0);
    this->enemyByteCount->insert(Level::WORLD_4_LEVEL_2, 0);
    this->enemyByteCount->insert(Level::WORLD_4_LEVEL_3, 0);
    this->enemyByteCount->insert(Level::WORLD_4_LEVEL_4, 0);
    this->enemyByteCount->insert(Level::WORLD_5_LEVEL_1, 0);
    this->enemyByteCount->insert(Level::WORLD_5_LEVEL_2, 0);
    this->enemyByteCount->insert(Level::WORLD_6_LEVEL_1, 0);
    this->enemyByteCount->insert(Level::WORLD_6_LEVEL_2, 0);
    this->enemyByteCount->insert(Level::WORLD_6_LEVEL_3, 0);
    this->enemyByteCount->insert(Level::WORLD_7_LEVEL_1, 0);
    this->enemyByteCount->insert(Level::WORLD_7_LEVEL_4, 0);
    this->enemyByteCount->insert(Level::WORLD_8_LEVEL_1, 0);
    this->enemyByteCount->insert(Level::WORLD_8_LEVEL_2, 0);
    this->enemyByteCount->insert(Level::WORLD_8_LEVEL_3, 0);
    this->enemyByteCount->insert(Level::WORLD_8_LEVEL_4, 0);
    this->enemyByteCount->insert(Level::UNDERGROUND_BONUS, 0);
    this->enemyByteCount->insert(Level::CLOUD_BONUS_1, 0);
    this->enemyByteCount->insert(Level::CLOUD_BONUS_2, 0);
    this->enemyByteCount->insert(Level::UNDERWATER_BONUS, 0);
    this->enemyByteCount->insert(Level::UNDERWATER_CASTLE, 0);
    this->enemyByteCount->insert(Level::PIPE_INTRO, 0);
    this->enemyByteCount->insert(Level::WARP_ZONE, 0);
}

bool Enemy_Bytes_Tracker::Calculate_Enemy_Bytes_In_All_Levels() {
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_1_LEVEL_1)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_1_LEVEL_2)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_1_LEVEL_3)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_1_LEVEL_4)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_2_LEVEL_1)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_2_LEVEL_2)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_2_LEVEL_3)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_2_LEVEL_4)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_3_LEVEL_1)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_3_LEVEL_2)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_3_LEVEL_3)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_3_LEVEL_4)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_4_LEVEL_1)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_4_LEVEL_2)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_4_LEVEL_3)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_4_LEVEL_4)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_5_LEVEL_1)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_5_LEVEL_2)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_6_LEVEL_1)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_6_LEVEL_2)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_6_LEVEL_3)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_7_LEVEL_1)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_7_LEVEL_4)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_8_LEVEL_1)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_8_LEVEL_2)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_8_LEVEL_3)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::WORLD_8_LEVEL_4)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::UNDERGROUND_BONUS)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::CLOUD_BONUS_1)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::CLOUD_BONUS_2)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::UNDERWATER_BONUS)) return false;
    if (!this->Calculate_Enemy_Bytes_In_Level(Level::UNDERWATER_CASTLE)) return false;
    return true;
}

bool Enemy_Bytes_Tracker::Calculate_Enemy_Bytes_In_Level(Level::Level level) {
    assert(this->file);
    assert(this->file->isOpen() && this->file->isReadable());
    assert(this->enemyByteCount);
    qint64 offset = this->levelOffset->Get_Level_Enemy_Offset(level);
    assert(offset != BAD_OFFSET);
    if (!this->file->seek(offset)) return false;

    //Read the enemies from the level
    int numEnemyBytes = 0;
    for (QByteArray coordinate = this->file->peek(1); !coordinate.isEmpty() &&
         static_cast<unsigned char>(coordinate.data()[0]) != 0xFF; coordinate = this->file->peek(1)) {
        if (Binary_Manipulator::Get_Second_Digit_From_Hex(static_cast<unsigned char>(coordinate[0])) == 0xE) { //pipe pointer
            QByteArray buffer(3, ' ');
            if (this->file->read(buffer.data(), 3) != 3) return false; //something went wrong...
            numEnemyBytes += 3;
        } else { //typical enemy
            QByteArray buffer(2, ' ');
            if (this->file->read(buffer.data(), 2) != 2) return false; //something went wrong...
            numEnemyBytes += 2;
        }
    }

    this->Set_Enemy_Byte_Count_In_Level(level, numEnemyBytes);
    return true;
}
