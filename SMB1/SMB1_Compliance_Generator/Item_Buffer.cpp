#include "Item_Buffer.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Scenery.h"
#include "Physics.h"
#include <QDebug>
#include <assert.h>
#include <cmath>

Item_Buffer::Item_Buffer(int numBytesLeft) {
    assert(numBytesLeft >= 0);
    this->totalBytes = numBytesLeft;
    this->numBytesLeft = numBytesLeft;
    this->itemBuffer = new QLinkedList<Buffer_Data>();
    this->itemBufferIter = this->itemBuffer->begin();
    this->Initialize_Buffer();
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

void Item_Buffer::Set_Num_Bytes_Left_And_Total_Bytes(int value) {
    this->numBytesLeft = value;
    this->totalBytes = value;
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
    if (this->itemBuffer->isEmpty() || this->itemBufferIter == this->itemBuffer->end()) {
        this->itemBuffer->append(data);
        this->itemBufferIter = this->itemBuffer->end()-1;
    } else {
        this->itemBufferIter = this->itemBuffer->insert(this->itemBufferIter+1, data);
    }
}

void Item_Buffer::Initialize_Buffer() {
    this->numItems = 0;
    this->levelLength = 0;
    this->currentPage = 0;
    this->currentX = 0;
    this->currentAbsoluteX = 0;
    this->currentY = Physics::GROUND_Y+1;
    this->coordinateSafety = true;
    this->itemBuffer->clear();
    this->itemBufferIter = this->itemBuffer->end();
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
    this->currentAbsoluteX += x;
    this->currentX += x;
    while (this->currentX > 0xF) {
        this->currentX -= 0x10;
        ++this->currentPage;
    }
    ++this->numItems;

    //Fix X Values
    if (this->At_End() || this->itemBufferIter+1 == this->itemBuffer->end()) { //when appending, increment the level length
        this->levelLength += x;
    } else { //when inserting, fix the x value of the next object
        QLinkedList<Buffer_Data>::iterator iter = this->itemBufferIter+1;
        Buffer_Data *data = &(*iter);
        data->x -= x;
        assert(data->x >= 0);
    }
}

bool Item_Buffer::Is_Last_Item_A_Page_Change() {
    if (this->itemBuffer->isEmpty()) return false;
    return this->itemBuffer->last().objectItem == Object_Item::PAGE_CHANGE || this->itemBuffer->last().enemyItem == Enemy_Item::PAGE_CHANGE;
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
    assert(this->currentAbsoluteX <= this->levelLength);
    if (absoluteX < 0) return false; //out of range!
    if (absoluteX == 0) {
        this->Seek_To_First_Item();
        Buffer_Data data = *this->itemBufferIter;
        return data.x == 0;
    }
    if (absoluteX > this->levelLength+31) return false; //out of range!
    if (absoluteX > this->levelLength) { this->Seek_To_End(); return true; }

    //Potentially start from the beginning or the end to save time
    int distance = absoluteX-this->currentAbsoluteX;
    int absDistance = std::abs(distance);
    if (absDistance > this->levelLength/2) { //if more than half the level needs to be traversed
        assert(distance != 0);
        if (distance < 0) this->Seek_To_First_Item(); //start from the beginning
        else this->Seek_To_End(); //start from the end
    }

    //Determine which direction to seek
    if (this->currentAbsoluteX <= absoluteX) { //seek forwards
        while (!this->At_End()) {
            this->Seek_To_Next();
            if (this->currentAbsoluteX > absoluteX) {
                this->Seek_To_Previous(); //we've seeked too far, so go back by one
                return true;
            }
        }
        return true;
    } else { //seek backwards
        bool beginningChecked = false;
        while (!beginningChecked) {
            if (this->At_Beginning()) beginningChecked = true;
            if (this->currentAbsoluteX <= absoluteX) {
                return true;
            }
            this->Seek_To_Previous();
        }
        return false; //the item exists before the first item. It is impossible to seek here!
    }
}

void Item_Buffer::Seek_To_First_Item() {
    this->itemBufferIter = this->itemBuffer->begin();
    Buffer_Data data = *this->itemBufferIter;
    this->currentAbsoluteX = data.absoluteX;
    this->currentX = this->currentAbsoluteX%0x10;
    this->currentPage = this->currentAbsoluteX/0x10;
}

void Item_Buffer::Seek_To_Next() {
    if (this->At_End()) {
        this->Seek_To_End();
    } else {
        ++this->itemBufferIter;
        if (this->At_End()) {
            this->Seek_To_End();
        } else {
            Buffer_Data data = *this->itemBufferIter;
            this->currentAbsoluteX = data.absoluteX;
            this->currentX = this->currentAbsoluteX%0x10;
            this->currentPage = this->currentAbsoluteX/0x10;
        }
    }
}

void Item_Buffer::Seek_To_Previous() {
    if (this->At_Beginning()) {
        this->Seek_To_First_Item();
    } else {
        --this->itemBufferIter;
        if (this->At_Beginning()) {
            this->Seek_To_First_Item();
        } else {
            Buffer_Data data = *this->itemBufferIter;
            this->currentAbsoluteX = data.absoluteX;
            this->currentX = this->currentAbsoluteX%0x10;
            this->currentPage = this->currentAbsoluteX/0x10;
        }
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
