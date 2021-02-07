#include "Enemy_Buffer.h"
#include "../Common_SMB1_Files/Enemy_Item_String.h"
#include "../Common_SMB1_Files/Level_String.h"
#include "Buffer_Data.h"
#include <QTextStream>
#include <assert.h>

const static int MAX_LARGE_FIRE_BAR_ZONE = 24;

Enemy_Buffer::Enemy_Buffer(int nbl) : Item_Buffer(nbl) {
    this->Clear_Buffer();
}

void Enemy_Buffer::Clear_Buffer() {
    this->Initialize_Buffer();
    this->firstEnemy = true;
    this->lakituIsActive = false;
    this->wasLakituSpawned = false;
    this->lastEnemyWasHammerBro = false;
    this->largeFirebarZone = 0;
}

bool Enemy_Buffer::Write_Buffer_To_File(QFile *file) {
    QTextStream stream(file);
    for (QLinkedList<Buffer_Data>::iterator iter = this->itemBuffer->begin(); iter != this->itemBuffer->end(); ++iter) {
        Buffer_Data data = *iter;
        QString hardMode = data.onlyHardMode ? Enemy_Item::STRING_HARD : Enemy_Item::STRING_NORMAL;
        switch (data.enemyItem) {
        case Enemy_Item::WARP_ZONE: //x
            stream << this->Get_String_From_Enemy_Item(data.enemyItem) << " " << QString::number(data.x) << Qt::endl;
            break;
        case Enemy_Item::PODOBOO:
        case Enemy_Item::BOWSER_FIRE_SPAWNER:
        case Enemy_Item::CHEEP_CHEEP_SPAWNER:
        case Enemy_Item::BULLET_BILL_SPAWNER:
        case Enemy_Item::BOWSER:
        case Enemy_Item::TOAD: //x hardMode
            stream << this->Get_String_From_Enemy_Item(data.enemyItem) << " " << QString::number(data.x) << " " << hardMode << Qt::endl;
            break;
        case Enemy_Item::RED_KOOPA:
        case Enemy_Item::BUZZY_BEETLE:
        case Enemy_Item::HAMMER_BRO:
        case Enemy_Item::GOOMBA:
        case Enemy_Item::BLOOPER:
        case Enemy_Item::BULLET_BILL:
        case Enemy_Item::RED_PARATROOPA:
        case Enemy_Item::GREEN_CHEEP_CHEEP:
        case Enemy_Item::RED_CHEEP_CHEEP:
        case Enemy_Item::PIRANHA_PLANT:
        case Enemy_Item::LAKITU:
        case Enemy_Item::SPINY:
        case Enemy_Item::LARGE_FIRE_BAR:
        case Enemy_Item::FALLING_LIFT:
        case Enemy_Item::BALANCE_LIFT:
        case Enemy_Item::SURFING_LIFT: //x y hardMode
            stream << this->Get_String_From_Enemy_Item(data.enemyItem) << " " << QString::number(data.x) << " " << QString::number(data.y) << " " << hardMode << Qt::endl;
            break;
        case Enemy_Item::GREEN_KOOPA: //x y moving hardMode
            stream << this->Get_String_From_Enemy_Item(data.enemyItem) << " " << QString::number(data.x) << " " << QString::number(data.y)
                   << " " << (data.moving ? Enemy_Item::STRING_MOVING : Enemy_Item::STRING_STOPPED) << " " << hardMode << Qt::endl;
            break;
        case Enemy_Item::GREEN_PARATROOPA: //x y leapingMovement hardMode
            if (!data.moving) {
                stream << this->Get_String_From_Enemy_Item(data.enemyItem) << " " << QString::number(data.x) << " " << QString::number(data.y)
                       << " " << Enemy_Item::STRING_STOPPED << " " << hardMode << Qt::endl;
            } else {
                stream << this->Get_String_From_Enemy_Item(data.enemyItem) << " " << QString::number(data.x) << " " << QString::number(data.y)
                       << " " << (data.leaping ? Enemy_Item::STRING_LEAPING : Enemy_Item::STRING_FLYING) << " " << hardMode << Qt::endl;
            }
            break;
        case Enemy_Item::FIRE_BAR: //x y clockwise fast hardMode
            stream << this->Get_String_From_Enemy_Item(data.enemyItem) << " " << QString::number(data.x) << " " << QString::number(data.y)
                   << " " << (data.clockwise ? Enemy_Item::STRING_CLOCKWISE : Enemy_Item::STRING_COUNTER_CLOCKWISE) << " " << (data.fast ? Enemy_Item::STRING_FAST : Enemy_Item::STRING_SLOW) << " " << hardMode << Qt::endl;
            break;
        case Enemy_Item::LIFT: //x y vertical hardMode
            stream << this->Get_String_From_Enemy_Item(data.enemyItem) << " " << QString::number(data.x) << " " << QString::number(data.y)
                   << " " << (data.vertical ? Enemy_Item::STRING_VERTICAL : Enemy_Item::STRING_HORIZONTAL) << " " << hardMode << Qt::endl;
            break;
        case Enemy_Item::LIFT_SPAWNER: //x y up small hardMode
            stream << this->Get_String_From_Enemy_Item(data.enemyItem) << " " << QString::number(data.x) << " " << QString::number(data.y)
                   << " " << (data.up ? Enemy_Item::STRING_UP : Enemy_Item::STRING_DOWN) << " " << (data.small ? Enemy_Item::STRING_SMALL : Enemy_Item::STRING_LARGE) << " " << hardMode << Qt::endl;
            break;
        case Enemy_Item::GOOMBA_GROUP:
        case Enemy_Item::KOOPA_GROUP: //x y num hardMode
            stream << this->Get_String_From_Enemy_Item(data.enemyItem) << " " << QString::number(data.x) << " " << QString::number(data.y) << " " << QString::number(data.num) << " " << hardMode << Qt::endl;
            break;
        case Enemy_Item::PIPE_POINTER: //x level world page
            stream << this->Get_String_From_Enemy_Item(data.enemyItem) << " " << QString::number(data.x) << " " << this->Get_String_From_Level(data.level)
                   << " " << QString::number(data.world) << " " << QString::number(data.page) << Qt::endl;
            break;
        case Enemy_Item::PAGE_CHANGE: //page
            stream << this->Get_String_From_Enemy_Item(data.enemyItem) << " " << QString::number(data.page) << Qt::endl;
            break;
        case Enemy_Item::NOTHING:
            assert(false); return false;
        }
        if (stream.status() != QTextStream::Ok) return false;
    }
    stream.flush();
    return stream.status() == QTextStream::Ok;
}

bool Enemy_Buffer::Get_First_Enemy() {
    return this->firstEnemy;
}

void Enemy_Buffer::Set_First_Enemy(bool value) {
    this->firstEnemy = value;
}

bool Enemy_Buffer::Is_Lakitu_Active() {
    return this->lakituIsActive;
}

bool Enemy_Buffer::Was_Lakitu_Spawned() {
    return this->wasLakituSpawned;
}

bool Enemy_Buffer::Was_Last_Enemy_A_Hammer_Bro() {
    return this->lastEnemyWasHammerBro;
}

bool Enemy_Buffer::Write_Enemy(int page) {
    if (page < 0x00 || page > 0x3F) return false;
    if (this->currentPage == page) return true; //nothing to do. We are already on the requested page
    int relativeX = (0x10*page)-this->levelLength;
    if (this->Is_Last_Item_A_Page_Change()) { //modify the previous page change if there is one
        this->numBytesLeft += 2; //temporarily restore 2 bytes. This will be decremented again in Update_Level_Stats()
        if (!this->Handle_Level_Length_On_Page_Change(page)) return false;
        Buffer_Data *enemyBufferData = this->Get_Current_For_Modification();
        enemyBufferData->page = page;
        enemyBufferData->absoluteX = this->currentAbsoluteX; //don't add x here
        enemyBufferData->lineNum = this->currentLineNum;
    } else { //write a new page change
        assert(this->Is_Safe_To_Write_Item());
        if (!this->Handle_Level_Length_On_Page_Change(page)) return false;
        Buffer_Data enemyBufferData;
        enemyBufferData.enemyItem = Enemy_Item::PAGE_CHANGE;
        enemyBufferData.page = page;
        enemyBufferData.absoluteX = this->currentAbsoluteX; //don't add x here
        enemyBufferData.lineNum = this->currentLineNum;
        this->Insert_Into_Buffer(enemyBufferData);
    }
    this->Update_Level_Stats(0); //this must be 0 for page change, since most of it was updated in Handle_Level_Length_On_Page_Change()
    this->Handle_Zones(relativeX);
    this->firstEnemy = false;
    return true;
}

bool Enemy_Buffer::Write_Enemy(int x, Level::Level level, int world, int page) {
    if (world < 1 || world > 8) return false;
    if (page < 0x00 || page > 0x3F) return false;
    if (this->numBytesLeft < 3) return false;
    if (this->firstEnemy && x < 16) x += 16;
    this->Update_Last_Page_Change_For_Next_X(x);
    assert(!this->coordinateSafety || this->Is_Coordinate_Valid(x));
    assert(this->Is_Safe_To_Write_Item());
    Buffer_Data enemyBufferData;
    enemyBufferData.enemyItem = Enemy_Item::PIPE_POINTER;
    enemyBufferData.x = x;
    enemyBufferData.level = level;
    enemyBufferData.world = world;
    enemyBufferData.page = page;
    enemyBufferData.absoluteX = this->currentAbsoluteX+x;
    enemyBufferData.lineNum = this->currentLineNum;
    this->Insert_Into_Buffer(enemyBufferData);
    this->Update_Level_Stats(x);
    --this->numBytesLeft; //pipe pointers use 3 bytes instead of 2
    this->Handle_Zones(x);
    this->firstEnemy = false;
    return true;
}

bool Enemy_Buffer::Write_Enemy(Enemy_Item::Enemy_Item enemyItem, Buffer_Data &args, int x) {
    if (this->firstEnemy && x < 16) x += 16;
    this->Update_Last_Page_Change_For_Next_X(x);
    assert(!this->coordinateSafety || this->Is_Coordinate_Valid(x));
    assert(this->Is_Safe_To_Write_Item());
    args.enemyItem = enemyItem;
    args.x = x;
    args.absoluteX = this->currentAbsoluteX+x;
    args.lineNum = this->currentLineNum;
    this->Insert_Into_Buffer(args);
    this->Update_Level_Stats(x);
    this->Handle_Zones(x);
    this->lastEnemyWasHammerBro = enemyItem == Enemy_Item::HAMMER_BRO;
    this->firstEnemy = false;
    return true;
}

bool Enemy_Buffer::Write_Enemy(Enemy_Item::Enemy_Item enemyItem, Buffer_Data &args, int x, int y) {
    if (y > 0xD) return false;
    if (this->firstEnemy && x < 16) x += 16;
    this->Update_Last_Page_Change_For_Next_X(x);
    assert(!this->coordinateSafety || this->Is_Coordinate_Valid(x));
    assert(this->Is_Safe_To_Write_Item());
    args.enemyItem = enemyItem;
    args.x = x;
    args.y = y;
    args.absoluteX = this->currentAbsoluteX+x;
    args.lineNum = this->currentLineNum;
    this->Insert_Into_Buffer(args);
    this->Update_Level_Stats(x);
    this->Handle_Zones(x);
    this->lastEnemyWasHammerBro = enemyItem == Enemy_Item::HAMMER_BRO;
    this->firstEnemy = false;
    this->currentY = y;
    return true;
}

bool Enemy_Buffer::Is_Coordinate_Valid(int &coordinate) {
    if (coordinate < 0) return false;
    if (this->firstEnemy) return (coordinate >= 0x10 && coordinate <= 0x1F);
    if (this->Is_Last_Item_A_Page_Change() && coordinate >= 0x10) {
        assert(this->Page_Change((this->levelLength+coordinate)/16));
        while (coordinate >= 0x10) coordinate -= 0x10;
        return true;
    } else {
        return coordinate <= 0x10;
    }
}

void Enemy_Buffer::Update_Last_Page_Change_For_Next_X(int &x) {
    if (!this->coordinateSafety || !this->Is_Last_Item_A_Page_Change()) return; //nothing to do
    assert(this->Page_Change(((this->levelLength+x)/16)));
    while (x > 0x0F) x -= 0x10;
}

QString Enemy_Buffer::Get_String_From_Enemy_Item(Enemy_Item::Enemy_Item enemyItem) {
    switch (enemyItem) {
    case Enemy_Item::GREEN_KOOPA:           return Enemy_Item::STRING_GREEN_KOOPA;
    case Enemy_Item::RED_KOOPA:             return Enemy_Item::STRING_RED_KOOPA;
    case Enemy_Item::BUZZY_BEETLE:          return Enemy_Item::STRING_BUZZY_BEETLE;
    case Enemy_Item::HAMMER_BRO:            return Enemy_Item::STRING_HAMMER_BRO;
    case Enemy_Item::GOOMBA:                return Enemy_Item::STRING_GOOMBA;
    case Enemy_Item::BLOOPER:               return Enemy_Item::STRING_BLOOPER;
    case Enemy_Item::BULLET_BILL:           return Enemy_Item::STRING_BULLET_BILL;
    case Enemy_Item::GREEN_PARATROOPA:      return Enemy_Item::STRING_GREEN_PARATROOPA;
    case Enemy_Item::RED_PARATROOPA:        return Enemy_Item::STRING_RED_PARATROOPA;
    case Enemy_Item::GREEN_CHEEP_CHEEP:     return Enemy_Item::STRING_GREEN_CHEEP_CHEEP;
    case Enemy_Item::RED_CHEEP_CHEEP:       return Enemy_Item::STRING_RED_CHEEP_CHEEP;
    case Enemy_Item::PODOBOO:               return Enemy_Item::STRING_PODOBOO;
    case Enemy_Item::PIRANHA_PLANT:         return Enemy_Item::STRING_PIRANHA_PLANT;
    case Enemy_Item::LAKITU:                return Enemy_Item::STRING_LAKITU;
    case Enemy_Item::SPINY:                 return Enemy_Item::STRING_SPINY;
    case Enemy_Item::BOWSER_FIRE_SPAWNER:   return Enemy_Item::STRING_BOWSER_FIRE_SPAWNER;
    case Enemy_Item::CHEEP_CHEEP_SPAWNER:   return Enemy_Item::STRING_CHEEP_CHEEP_SPAWNER;
    case Enemy_Item::BULLET_BILL_SPAWNER:   return Enemy_Item::STRING_BULLET_BILL_SPAWNER;
    case Enemy_Item::FIRE_BAR:              return Enemy_Item::STRING_FIRE_BAR;
    case Enemy_Item::LARGE_FIRE_BAR:        return Enemy_Item::STRING_LARGE_FIRE_BAR;
    case Enemy_Item::LIFT:                  return Enemy_Item::STRING_LIFT;
    case Enemy_Item::FALLING_LIFT:          return Enemy_Item::STRING_FALLING_LIFT;
    case Enemy_Item::BALANCE_LIFT:          return Enemy_Item::STRING_BALANCE_LIFT;
    case Enemy_Item::SURFING_LIFT:          return Enemy_Item::STRING_SURFING_LIFT;
    case Enemy_Item::LIFT_SPAWNER:          return Enemy_Item::STRING_LIFT_SPAWNER;
    case Enemy_Item::BOWSER:                return Enemy_Item::STRING_BOWSER;
    case Enemy_Item::WARP_ZONE:             return Enemy_Item::STRING_WARP_ZONE;
    case Enemy_Item::PIPE_POINTER:          return Enemy_Item::STRING_PIPE_POINTER;
    case Enemy_Item::TOAD:                  return Enemy_Item::STRING_TOAD;
    case Enemy_Item::GOOMBA_GROUP:          return Enemy_Item::STRING_GOOMBA_GROUP;
    case Enemy_Item::KOOPA_GROUP:           return Enemy_Item::STRING_KOOPA_GROUP;
    case Enemy_Item::PAGE_CHANGE:           return Enemy_Item::STRING_PAGE_CHANGE;
    case Enemy_Item::NOTHING:               return Enemy_Item::STRING_NOTHING;
    }
    assert(false); return QString();
}

QString Enemy_Buffer::Get_String_From_Level(Level::Level level) {
    switch (level) {
    case Level::WORLD_1_LEVEL_1:    return Level::STRING_WORLD_1_LEVEL_1;
    case Level::WORLD_1_LEVEL_2:    return Level::STRING_WORLD_1_LEVEL_2;
    case Level::WORLD_1_LEVEL_3:    return Level::STRING_WORLD_1_LEVEL_3;
    case Level::WORLD_1_LEVEL_4:    return Level::STRING_WORLD_1_LEVEL_4;
    case Level::WORLD_2_LEVEL_1:    return Level::STRING_WORLD_2_LEVEL_1;
    case Level::WORLD_2_LEVEL_2:    return Level::STRING_WORLD_2_LEVEL_2;
    case Level::WORLD_2_LEVEL_3:    return Level::STRING_WORLD_2_LEVEL_3;
    case Level::WORLD_2_LEVEL_4:    return Level::STRING_WORLD_2_LEVEL_4;
    case Level::WORLD_3_LEVEL_1:    return Level::STRING_WORLD_3_LEVEL_1;
    case Level::WORLD_3_LEVEL_2:    return Level::STRING_WORLD_3_LEVEL_2;
    case Level::WORLD_3_LEVEL_3:    return Level::STRING_WORLD_3_LEVEL_3;
    case Level::WORLD_3_LEVEL_4:    return Level::STRING_WORLD_3_LEVEL_4;
    case Level::WORLD_4_LEVEL_1:    return Level::STRING_WORLD_4_LEVEL_1;
    case Level::WORLD_4_LEVEL_2:    return Level::STRING_WORLD_4_LEVEL_2;
    case Level::WORLD_4_LEVEL_3:    return Level::STRING_WORLD_4_LEVEL_3;
    case Level::WORLD_4_LEVEL_4:    return Level::STRING_WORLD_4_LEVEL_4;
    case Level::WORLD_5_LEVEL_1:    return Level::STRING_WORLD_5_LEVEL_1;
    case Level::WORLD_5_LEVEL_2:    return Level::STRING_WORLD_5_LEVEL_2;
    case Level::WORLD_6_LEVEL_1:    return Level::STRING_WORLD_6_LEVEL_1;
    case Level::WORLD_6_LEVEL_2:    return Level::STRING_WORLD_6_LEVEL_2;
    case Level::WORLD_6_LEVEL_3:    return Level::STRING_WORLD_6_LEVEL_3;
    case Level::WORLD_7_LEVEL_1:    return Level::STRING_WORLD_7_LEVEL_1;
    case Level::WORLD_7_LEVEL_4:    return Level::STRING_WORLD_7_LEVEL_4;
    case Level::WORLD_8_LEVEL_1:    return Level::STRING_WORLD_8_LEVEL_1;
    case Level::WORLD_8_LEVEL_2:    return Level::STRING_WORLD_8_LEVEL_2;
    case Level::WORLD_8_LEVEL_3:    return Level::STRING_WORLD_8_LEVEL_3;
    case Level::WORLD_8_LEVEL_4:    return Level::STRING_WORLD_8_LEVEL_4;
    case Level::PIPE_INTRO:         return Level::STRING_PIPE_INTRO;
    case Level::UNDERGROUND_BONUS:  return Level::STRING_UNDERGROUND_BONUS;
    case Level::CLOUD_BONUS_1:      return Level::STRING_CLOUD_BONUS_1;
    case Level::CLOUD_BONUS_2:      return Level::STRING_CLOUD_BONUS_2;
    case Level::UNDERWATER_BONUS:   return Level::STRING_UNDERWATER_BONUS;
    case Level::WARP_ZONE:          return Level::STRING_WARP_ZONE;
    case Level::UNDERWATER_CASTLE:  return Level::STRING_UNDERWATER_CASTLE;
    }
    assert(false); return QString();
}

void Enemy_Buffer::Handle_Zones(int x) {
    if (this->largeFirebarZone > 0) this->largeFirebarZone -= x;
    if (this->largeFirebarZone < 0) this->largeFirebarZone = 0;
}

bool Enemy_Buffer::Green_Koopa(int x, int y, bool moving, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode; args.moving = moving;
    return this->Write_Enemy(Enemy_Item::GREEN_KOOPA, args, x, y);
}

bool Enemy_Buffer::Red_Koopa(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::RED_KOOPA, args, x, y);
}

bool Enemy_Buffer::Buzzy_Beetle(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::BUZZY_BEETLE, args, x, y);
}

bool Enemy_Buffer::Hammer_Bro(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::HAMMER_BRO, args, x, y);
}

bool Enemy_Buffer::Goomba(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::GOOMBA, args, x, y);
}

bool Enemy_Buffer::Blooper(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::BLOOPER, args, x, y);
}

bool Enemy_Buffer::Bullet_Bill(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::BULLET_BILL, args, x, y);
}

bool Enemy_Buffer::Green_Paratroopa(int x, int y, bool moving, bool leaping, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    args.moving = moving; args.leaping = leaping;
    return this->Write_Enemy(Enemy_Item::GREEN_PARATROOPA, args, x, y);
}

bool Enemy_Buffer::Red_Paratroopa(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::RED_PARATROOPA, args, x, y);
}

bool Enemy_Buffer::Green_Cheep_Cheep(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::GREEN_CHEEP_CHEEP, args, x, y);
}

bool Enemy_Buffer::Red_Cheep_Cheep(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::RED_CHEEP_CHEEP, args, x, y);
}

bool Enemy_Buffer::Podoboo(int x, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::PODOBOO, args, x);
}

bool Enemy_Buffer::Piranha_Plant(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::PIRANHA_PLANT, args, x, y);
}

bool Enemy_Buffer::Lakitu(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    bool success = this->Write_Enemy(Enemy_Item::LAKITU, args, x, y);
    if (success) {
        this->wasLakituSpawned = true;
        this->lakituIsActive = true;
    }
    return success;
}

bool Enemy_Buffer::Spiny(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::SPINY, args, x, y);
}

bool Enemy_Buffer::Bowser_Fire_Spawner(int x, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::BOWSER_FIRE_SPAWNER, args, x);
}

bool Enemy_Buffer::Swimming_Cheep_Cheep_Spawner(int x, bool leaping, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode; args.leaping = leaping;
    return this->Write_Enemy(Enemy_Item::CHEEP_CHEEP_SPAWNER, args, x);
}

bool Enemy_Buffer::Bullet_Bill_Spawner(int x, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::BULLET_BILL_SPAWNER, args, x);
}

bool Enemy_Buffer::Fire_Bar(int x, int y, bool clockwise, bool fast, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    args.clockwise = clockwise; args.fast = fast;
    return this->Write_Enemy(Enemy_Item::FIRE_BAR, args, x, y);
}

bool Enemy_Buffer::Large_Fire_Bar(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    if (this->largeFirebarZone == 0) {
        if (!this->Write_Enemy(Enemy_Item::LARGE_FIRE_BAR, args, x, y)) return false;
        this->largeFirebarZone = MAX_LARGE_FIRE_BAR_ZONE;
        return true;
    } else {
        return this->Fire_Bar(x, y, true, false, onlyHardMode);
    }
}

bool Enemy_Buffer::Lift(int x, int y, bool vertical, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode; args.vertical = vertical;
    return this->Write_Enemy(Enemy_Item::LIFT, args, x, y);
}

bool Enemy_Buffer::Falling_Lift(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::FALLING_LIFT, args, x, y);
}

bool Enemy_Buffer::Balance_Lift(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::BALANCE_LIFT, args, x, y);
}

bool Enemy_Buffer::Surfing_Lift(int x, int y, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::SURFING_LIFT, args, x, y);
}

bool Enemy_Buffer::Lift_Spawner(int x, int y, bool up, bool small, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    args.up = up; args.small = small;
    return this->Write_Enemy(Enemy_Item::LIFT_SPAWNER, args, x, y);
}

bool Enemy_Buffer::Bowser(int x, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::BOWSER, args, x);
}

bool Enemy_Buffer::Warp_Zone(int x) {
    Buffer_Data args;
    return this->Write_Enemy(Enemy_Item::WARP_ZONE, args, x);
}

bool Enemy_Buffer::Toad(int x, bool onlyHardMode) {
    Buffer_Data args; args.onlyHardMode = onlyHardMode;
    return this->Write_Enemy(Enemy_Item::TOAD, args, x);
}

bool Enemy_Buffer::Goomba_Group(int x, int y, int num, bool onlyHardMode) {
    if (y == 0x6 || y == 0xA) {
        if (num == 2 || num == 3) {
            Buffer_Data args; args.onlyHardMode = onlyHardMode; args.num = num;
            return this->Write_Enemy(Enemy_Item::GOOMBA_GROUP, args, x, y);
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool Enemy_Buffer::Koopa_Group(int x, int y, int num, bool onlyHardMode) {
    if (y == 0x6 || y == 0xA) {
        if (num == 2 || num == 3) {
            Buffer_Data args; args.onlyHardMode = onlyHardMode; args.num = num;
            return this->Write_Enemy(Enemy_Item::KOOPA_GROUP, args, x, y);
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool Enemy_Buffer::Page_Change(int page) {
    return this->Write_Enemy(page);
}

bool Enemy_Buffer::Pipe_Pointer(int x, Level::Level level, int world, int page) {
    return this->Write_Enemy(x, level, world, page);
}
