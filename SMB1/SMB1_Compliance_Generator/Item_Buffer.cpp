#include "Item_Buffer.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Scenery.h"
#include "Enemy_Buffer_Data.h"
#include "Object_Buffer_Data.h"
#include "Physics.h"
#include <assert.h>
#include <QDebug>

Item_Buffer::Item_Buffer(int numBytesLeft) {
    assert(numBytesLeft >= 0);
    this->numItems = 0;
    this->levelLength = 0;
    this->numBytesLeft = numBytesLeft;
    this->currentPage = 0;
    this->currentX = 0;
    this->currentY = Physics::GROUND_Y+1;
    this->coordinateSafety = true;
}

int Item_Buffer::Get_Num_Bytes_Left() {
    return this->numBytesLeft;
}

int Item_Buffer::Get_Num_Items() {
    return this->numItems;
}

int Item_Buffer::Get_Level_Length() {
    return this->levelLength;
}

int Item_Buffer::Get_Current_Y() {
    return this->currentY;
}

void Item_Buffer::Set_Current_Y(int value) {
    this->currentY = value;
}

int Item_Buffer::Get_Absolute_X(int x) {
    return ((this->currentX+x)%0x10);
}

int Item_Buffer::Get_Current_Page() {
    return this->currentPage;
}

void Item_Buffer::Set_Coordinate_Safety(bool value) {
    this->coordinateSafety = value;
}

bool Item_Buffer::Will_Page_Flag_Be_Tripped(int x) {
    int simulatedCurrentX = this->currentX + x;
    return (simulatedCurrentX > 0xF);
}

bool Item_Buffer::Is_Byte_Valid(int byte) {
    return (byte >= 0x00 && byte <= 0xFF);
}

bool Item_Buffer::Is_Safe_To_Write_Item(int numBytes) {
    return (this->numBytesLeft >= numBytes);
}

bool Item_Buffer::Handle_Level_Length_On_Page_Change(int page) {
    if (this->currentPage >= page) {
        qDebug() << "Current Page: " << this->currentPage;
        qDebug() << "Page attempting to change to: " << page;
        return false;
    }
    this->currentPage = page;
    this->currentX = 0;
    this->levelLength = page*(0x10);
    return true;
}

Object_Buffer_Data Item_Buffer::Get_Empty_Object_Buffer_Data() {
    Object_Buffer_Data emptyBufferData;
    emptyBufferData.objectItem = Object_Item::NOTHING;
    emptyBufferData.x = 0; emptyBufferData.y = 0; emptyBufferData.length = 1; emptyBufferData.platform = false;
    emptyBufferData.height = 1; emptyBufferData.page = 0;
    emptyBufferData.brick = Brick::NO_BRICKS; emptyBufferData.scenery = Scenery::NO_SCENERY; emptyBufferData.background = Background::BLANK_BACKGROUND;
    return emptyBufferData;
}

Enemy_Buffer_Data Item_Buffer::Get_Empty_Enemy_Buffer_Data() {
    Enemy_Buffer_Data emptyBufferData;
    emptyBufferData.x = 0; emptyBufferData.y = 0;
    emptyBufferData.enemyItem = Enemy_Item::NOTHING; emptyBufferData.args = this->Get_Empty_Enemy_Args();
    return emptyBufferData;
}

Extra_Enemy_Args Item_Buffer::Get_Empty_Enemy_Args() {
    return this->Get_Empty_Enemy_Args(false);
}

Extra_Enemy_Args Item_Buffer::Get_Empty_Enemy_Args(bool onlyHardMode) {
    Extra_Enemy_Args args;
    args.onlyHardMode = onlyHardMode; args.moving = false; args.leaping = false; args.clockwise = false;
    args.fast = false; args.small = false; args.vertical = false; args.up = false;
    args.allowSpawnAfterCancelSpawner = true;
    args.level = Level::WORLD_1_LEVEL_1;
    args.world = 0; args.page = 0; args.num = 0;
    return args;
}

void Item_Buffer::Update_Level_Stats(int x) {
    this->numBytesLeft -= 2;
    this->levelLength += x;
    this->currentX += x;
    while (this->currentX > 0xF) {
        this->currentX -= 0x10;
        ++this->currentPage;
    }
    ++this->numItems;
}
