#include "Item_Buffer.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Scenery.h"
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
    this->currentAbsoluteX = 0;
    this->currentY = Physics::GROUND_Y+1;
    this->coordinateSafety = true;
    this->itemBuffer = new QLinkedList<Buffer_Data>();
    this->itemBufferIter = this->itemBuffer->begin();
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

int Item_Buffer::Get_Absolute_X() {
    return this->currentAbsoluteX;
}

int Item_Buffer::Get_Page_Relative_Absolute_X(int x) {
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

void Item_Buffer::Insert_Into_Buffer(const Buffer_Data &data) {
    if (this->itemBuffer->isEmpty()) {
        this->itemBuffer->append(data);
        this->itemBufferIter = this->itemBuffer->begin();
    } else {
        this->itemBufferIter = this->itemBuffer->insert(this->itemBufferIter+1, data);
    }
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
    this->currentAbsoluteX = page*(0x10);
    this->levelLength = this->currentAbsoluteX;
    return true;
}

void Item_Buffer::Update_Level_Stats(int x) {
    this->numBytesLeft -= 2;
    this->levelLength += x;
    this->currentX += x;
    this->currentAbsoluteX += x;
    while (this->currentX > 0xF) {
        this->currentX -= 0x10;
        ++this->currentPage;
    }
    ++this->numItems;
}

bool Item_Buffer::Is_Empty() {
    return this->itemBuffer->isEmpty();
}

bool Item_Buffer::At_Beginning() {
    return this->itemBufferIter == this->itemBuffer->begin();
}

bool Item_Buffer::At_End() {
    return this->itemBufferIter == this->itemBuffer->end();
}

bool Item_Buffer::Seek_To_Object_Item(int absoluteX, int y, Object_Item::Object_Item objectItem) {
    if (!this->Seek_To_Absolute_X(absoluteX)) return false;
    bool beginningChecked = false;
    while (!beginningChecked) {
        Buffer_Data data = *this->itemBufferIter;
        if (data.y == y && data.objectItem == objectItem) return true;
        if (this->At_Beginning()) beginningChecked = true;
        else this->Seek_To_Previous();
    }
    return false;
}

bool Item_Buffer::Seek_To_Enemy_Item(int absoluteX, int y, Enemy_Item::Enemy_Item enemyItem) {
    if (!this->Seek_To_Absolute_X(absoluteX)) return false;
    bool beginningChecked = false;
    while (!beginningChecked) {
        Buffer_Data data = *this->itemBufferIter;
        if (data.y == y && data.enemyItem == enemyItem) return true;
        if (this->At_Beginning()) beginningChecked = true;
        else this->Seek_To_Previous();
    }
    return false;
}

bool Item_Buffer::Seek_To_Absolute_X(int absoluteX) {
    if (absoluteX < 0) return false; //out of range!
    if (absoluteX == 0) { this->Seek_To_First_Item(); return true; }
    if (absoluteX > this->levelLength+31) return false; //out of range!
    if (absoluteX > this->levelLength) { this->Seek_To_End(); return true; }

    //Compare against the current x to make sure that
    if (absoluteX == this->currentAbsoluteX) return true; //nothing to do
    if (currentAbsoluteX < absoluteX) {
        if (this->At_End()) return true; //nothing to do

        //Check if this is the correct absolute x
        this->Seek_To_Next();
        int tmpX = this->Get_Absolute_X();
        this->Seek_To_Previous();
        if (tmpX > absoluteX) return true; //nothing to do
    }

    //Seek through the level to find the coordinate
    bool found = false;
    if (absoluteX < levelLength/2) { //start from the beginning
        this->Seek_To_First_Item();
        while (!this->At_End() && !found) {
            Buffer_Data data = *this->itemBufferIter;
            if (data.absoluteX > absoluteX) found = true;
            if (found) this->Seek_To_Previous(); //roll back one, as we have passed the absolute X
            else this->Seek_To_Next();
        }
    } else { //start from the end
        this->Seek_To_End();
        bool beginningChecked = false;
        while (!beginningChecked && !found) {
            Buffer_Data data = *this->itemBufferIter;
            if (data.absoluteX <= absoluteX) found = true;
            if (!found) {
                if (this->At_Beginning()) beginningChecked = true;
                else this->Seek_To_Previous();
            }
        }
    }
    assert(found);
    return true;
}

void Item_Buffer::Seek_To_First_Item() {
    this->itemBufferIter = this->itemBuffer->begin();
    Buffer_Data data = *this->itemBufferIter;
    this->currentAbsoluteX = data.absoluteX;
    this->currentX = this->currentAbsoluteX%0x10;
    this->currentPage = this->currentAbsoluteX/0x10;
}

void Item_Buffer::Seek_To_Next() {
    if (!this->At_End()) {
        ++this->itemBufferIter;
        Buffer_Data data = *this->itemBufferIter;
        this->currentAbsoluteX = data.absoluteX;
        this->currentX = this->currentAbsoluteX%0x10;
        this->currentPage = this->currentAbsoluteX/0x10;
    }
}

void Item_Buffer::Seek_To_Previous() {
    if (!this->At_Beginning()) {
        --this->itemBufferIter;
        Buffer_Data data = *this->itemBufferIter;
        this->currentAbsoluteX = data.absoluteX;
        this->currentX = this->currentAbsoluteX%0x10;
        this->currentPage = this->currentAbsoluteX/0x10;
    }
}

void Item_Buffer::Seek_To_End() {
    this->itemBufferIter = this->itemBuffer->end();
    this->currentAbsoluteX = this->levelLength;
    this->currentX = this->currentAbsoluteX%0x10;
    this->currentPage = this->currentAbsoluteX/0x10;
}

Buffer_Data Item_Buffer::Get_Current() {
    assert(!this->At_End());
    return *this->itemBufferIter;
}

Buffer_Data *Item_Buffer::Get_Current_For_Modification() {
    assert(!this->At_End());
    return &(*this->itemBufferIter);
}
