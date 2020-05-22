#include "Object_Buffer.h"
#include "../Common_SMB1_Files/Background_String.h"
#include "../Common_SMB1_Files/Brick_String.h"
#include "../Common_SMB1_Files/Scenery_String.h"
#include "../Common_SMB1_Files/Object_Item_String.h"
#include "Block_Data.h"
#include "Enemy_Spawner.h"
#include "Buffer_Data.h"
#include "Physics.h"
#include <QTextStream>
#include <assert.h>

Object_Buffer::Object_Buffer(int nbl, SMB1_Compliance_Generator_Arguments *args) : Item_Buffer(nbl) {
    assert(args);
    this->args = args;
    this->objectsAtXCoordinates = nullptr;
    this->questionBlocks = nullptr;
    this->brickBlocks = nullptr;
    this->Clear_Buffer();
}

Object_Buffer::~Object_Buffer() {
    delete this->objectsAtXCoordinates;
    delete this->questionBlocks;
    delete this->brickBlocks;
}

void Object_Buffer::Clear_Buffer() {
    this->Initialize_Buffer();
    this->lastObjectLength = 0;
    this->coinBlockZone = 0;
    this->powerupZone = 0;
    this->endObjectCount = Physics::MIN_END_OBJECTS;
    this->reservedObjectCount = 0;
    this->firstPageSafety = false;
    this->autoScrollActive = false;
    this->wereFlyingCheepCheepsSpawned = false;
    this->wasAutoScrollUsed = false;
    this->exceededVerticalObjectLimit = false;
    this->cancelSpawnerX = -1;
    delete this->objectsAtXCoordinates;
    delete this->questionBlocks;
    delete this->brickBlocks;
    this->objectsAtXCoordinates = new QVector<int>(0x400, 0); //holds up to 40 pages of coordinates
    this->questionBlocks = new QMap<QString, Block_Data>();
    this->brickBlocks = new QMap<QString, Block_Data>();
}

bool Object_Buffer::Write_Buffer_To_File(QFile *file) {
    QTextStream stream(file);
    for (QLinkedList<Buffer_Data>::iterator iter = this->itemBuffer->begin(); iter != this->itemBuffer->end(); ++iter) {
        Buffer_Data data = *iter;
        switch (data.objectItem) {
        case Object_Item::REVERSE_L_PIPE:
        case Object_Item::FLAGPOLE:
        case Object_Item::CASTLE:
        case Object_Item::BIG_CASTLE:
        case Object_Item::AXE:
        case Object_Item::AXE_ROPE:
        case Object_Item::BOWSER_BRIDGE:
        case Object_Item::SCROLL_STOP:
        case Object_Item::SCROLL_STOP_WARP_ZONE:
        case Object_Item::TOGGLE_AUTO_SCROLL:
        case Object_Item::FLYING_CHEEP_CHEEP_SPAWNER:
        case Object_Item::SWIMMING_CHEEP_CHEEP_SPAWNER:
        case Object_Item::BULLET_BILL_SPAWNER:
        case Object_Item::CANCEL_SPAWNER:
        case Object_Item::LOOP_COMMAND:
        case Object_Item::PIPE_WALL:
        case Object_Item::LIFT_ROPE:
        case Object_Item::END_STEPS: //x
            stream << this->Get_String_From_Object_Item(data.objectItem) << " " << QString::number(data.x) << endl;
            break;
        case Object_Item::BALANCE_LIFT_VERTICAL_ROPE:
        case Object_Item::BALANCE_LIFT_HORIZONTAL_ROPE:
        case Object_Item::HOLE:
        case Object_Item::HOLE_WITH_WATER:
        case Object_Item::STEPS: //x length
            stream << this->Get_String_From_Object_Item(data.objectItem) << " " << QString::number(data.x) << " " << QString::number(data.length) << endl;
            break;
        case Object_Item::QUESTION_BLOCK_WITH_MUSHROOM:
        case Object_Item::QUESTION_BLOCK_WITH_COIN:
        case Object_Item::HIDDEN_BLOCK_WITH_COIN:
        case Object_Item::HIDDEN_BLOCK_WITH_1UP:
        case Object_Item::BRICK_WITH_MUSHROOM:
        case Object_Item::BRICK_WITH_VINE:
        case Object_Item::BRICK_WITH_STAR:
        case Object_Item::BRICK_WITH_10_COINS:
        case Object_Item::BRICK_WITH_1UP:
        case Object_Item::UNDERWATER_SIDEWAYS_PIPE:
        case Object_Item::USED_BLOCK:
        case Object_Item::TRAMPOLINE:
        case Object_Item::TALL_REVERSE_L_PIPE: //x y
            stream << this->Get_String_From_Object_Item(data.objectItem) << " " << QString::number(data.x) << " " << QString::number(data.y) << endl;
            break;
        case Object_Item::BULLET_BILL_TURRET:
        case Object_Item::VERTICAL_BRICKS:
        case Object_Item::VERTICAL_BLOCKS:
        case Object_Item::CORAL:
        case Object_Item::PIPE:
        case Object_Item::ENTERABLE_PIPE: //x y height
            stream << this->Get_String_From_Object_Item(data.objectItem) << " " << QString::number(data.x) << " " << QString::number(data.y) << " " << QString::number(data.height) << endl;
            break;
        case Object_Item::BRIDGE:
        case Object_Item::ISLAND:
        case Object_Item::HORIZONTAL_BRICKS:
        case Object_Item::HORIZONTAL_BLOCKS:
        case Object_Item::HORIZONTAL_COINS:
        case Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS: //x y length
            stream << this->Get_String_From_Object_Item(data.objectItem) << " " << QString::number(data.x) << " " << QString::number(data.y) << " " << QString::number(data.length) << endl;
            break;
        case Object_Item::PAGE_CHANGE: //page
            stream << this->Get_String_From_Object_Item(data.objectItem) << " " << QString::number(data.page) << endl;
            break;
        case Object_Item::CHANGE_BRICK_AND_SCENERY: //x brick scenery
            stream << this->Get_String_From_Object_Item(data.objectItem) << " " << QString::number(data.x) << " " << this->Get_String_From_Brick(data.brick) << " " << this->Get_String_From_Scenery(data.scenery) << endl;
            break;
        case Object_Item::CHANGE_BACKGROUND: //x background
            stream << this->Get_String_From_Object_Item(data.objectItem) << " " << QString::number(data.x) << " " << this->Get_String_From_Background(data.background) << endl;
            break;
        case Object_Item::NOTHING: //no arguments
            stream << this->Get_String_From_Object_Item(data.objectItem) << " 0" << endl;
            break;
        }
        if (stream.status() != QTextStream::Ok) return false;
    }
    stream.flush();
    return stream.status() == QTextStream::Ok;
}

bool Object_Buffer::Did_Level_Exceed_Vertical_Object_Limit() {
    return this->exceededVerticalObjectLimit;
}

int Object_Buffer::Get_Last_Object_Length() {
    return this->lastObjectLength;
}

void Object_Buffer::Increment_Last_Object_Length(int value) {
    this->lastObjectLength += value;
}

void Object_Buffer::Set_Last_Object_Length(int value) {
    this->lastObjectLength = value;
}

bool Object_Buffer::Was_Last_Object_A_Platform() {
    return this->lastObjectIsPlatform;
}

int Object_Buffer::Get_Num_Objects_Left() {
    return (this->Get_Num_Bytes_Left()/2);
}

int Object_Buffer::Get_Num_Objects_Available() {
    return (this->Get_Num_Objects_Left()-this->endObjectCount)-this->reservedObjectCount;
}

bool Object_Buffer::Reserve_Objects(int amount) {
    if (this->Get_Num_Objects_Available() < amount) return false;
    this->reservedObjectCount += amount;
    return true;
}

bool Object_Buffer::Free_Reserved_Objects(int amount) {
    if (this->reservedObjectCount < amount) { this->reservedObjectCount = 0; return false; }
    this->reservedObjectCount -= amount;
    return true;
}

bool Object_Buffer::Were_Flying_Cheep_Cheeps_Spawned() {
    return this->wereFlyingCheepCheepsSpawned;
}

bool Object_Buffer::Is_Auto_Scroll_Active() {
    return this->autoScrollActive;
}

bool Object_Buffer::Was_Auto_Scroll_Used() {
    return this->wasAutoScrollUsed;
}

int Object_Buffer::Get_Cancel_Spawner_X() {
    return this->cancelSpawnerX;
}

bool Object_Buffer::Is_Midpoint_Ready() {
    bool halfOfObjectsUsed = ((this->Get_Num_Objects_Left()-Physics::MIN_END_OBJECTS) < (this->totalBytes/4) || this->currentPage > 0xA);
    if (this->args->maxLevelLength == 0) {
        return halfOfObjectsUsed;
    } else {
        return halfOfObjectsUsed || (this->levelLength >= (this->args->maxLevelLength/2));
    }
}

bool Object_Buffer::Get_First_Page_Safety() {
    return this->firstPageSafety;
}

void Object_Buffer::Set_First_Page_Safety(bool firstPageSafety) {
    this->firstPageSafety = firstPageSafety;
}

void Object_Buffer::Set_End_Object_Count(int value) {
    this->endObjectCount = value;
}

int Object_Buffer::Get_Relative_X_From_Absolute_X(int absoluteX) {
    int x = absoluteX-this->currentAbsoluteX;
    assert(x >= 0);
    return x;
}

bool Object_Buffer::Decrement_Vertical_Object_Count_At_X(int x) {
    if (x < 0 || x >= this->objectsAtXCoordinates->size()) return false;
    int tmp = this->objectsAtXCoordinates->at(x);
    if (tmp <= 0) return false;
    this->objectsAtXCoordinates->data()[x] = tmp-1;
    return true;
}

bool Object_Buffer::Decrement_Vertical_Object_Count_Starting_At_X(int x, int length) {
    assert(length > 0);
    if (x < 0 || x+length >= this->objectsAtXCoordinates->size()) return false;
    for (int i = x; i < x+length; ++i) {
        int tmp = this->objectsAtXCoordinates->at(i);
        --tmp;
        if (tmp < 0) return false;
        this->objectsAtXCoordinates->data()[i] = tmp;
    }
    return true;
}

int Object_Buffer::Get_Num_Vertical_Objects_At_X(int x) {
    if (x < 0 || x >= this->objectsAtXCoordinates->size()) return 0;
    return this->objectsAtXCoordinates->at(x);
}

QString Object_Buffer::Get_Coordinate_Key(int x, int y) {
    assert(x >= 0);
    assert(y >= 0 && y < 0xB);
    return QString(QString::number(x)+"x"+QString::number(y));
}

QMap<QString, Block_Data> *Object_Buffer::Get_Question_Blocks() {
    return this->questionBlocks;
}

QMap<QString, Block_Data> *Object_Buffer::Get_Brick_Blocks() {
    return this->brickBlocks;
}

bool Object_Buffer::Write_Object(Object_Item::Object_Item objectItem, bool platform, int x, int length) {
    this->Update_Last_Page_Change_For_Next_X(x);
    assert(!this->coordinateSafety || this->Is_Coordinate_Valid(x));
    assert(this->Is_Safe_To_Write_Item());
    Buffer_Data objectBufferData;
    objectBufferData.objectItem = objectItem;
    objectBufferData.platform = platform;
    objectBufferData.x = x;
    objectBufferData.length = length;
    objectBufferData.absoluteX = this->currentAbsoluteX+x;
    this->Insert_Into_Buffer(objectBufferData);
    this->Update_Level_Stats(x);
    this->lastObjectIsPlatform = platform;
    this->Handle_Zones(x);
    this->lastObjectLength = length;
    this->Check_Vertical_Object_Limit(length);
    return true;
}

bool Object_Buffer::Write_Object(Object_Item::Object_Item objectItem, bool platform, int x, int y, int length) {
    if (!this->Is_Y_Valid(y)) return false;
    this->Update_Last_Page_Change_For_Next_X(x);
    assert(!this->coordinateSafety || this->Is_Coordinate_Valid(x));
    assert(this->Is_Safe_To_Write_Item());
    Buffer_Data objectBufferData;
    objectBufferData.objectItem = objectItem;
    objectBufferData.platform = platform;
    objectBufferData.x = x;
    objectBufferData.y = y;
    objectBufferData.length = length;
    objectBufferData.absoluteX = this->currentAbsoluteX+x;
    this->Insert_Into_Buffer(objectBufferData);
    this->Update_Level_Stats(x);
    this->lastObjectIsPlatform = platform;
    this->Handle_Zones(x);
    this->currentY = y;
    this->lastObjectLength = length;
    this->Check_Vertical_Object_Limit(length);
    return true;
}

bool Object_Buffer::Write_Object(Object_Item::Object_Item objectItem, bool platform, int x, int y, int height, int length) {
    if (!this->Is_Y_Valid(y)) return false;
    this->Update_Last_Page_Change_For_Next_X(x);
    assert(!this->coordinateSafety || this->Is_Coordinate_Valid(x));
    assert(this->Is_Safe_To_Write_Item());
    Buffer_Data objectBufferData;
    objectBufferData.objectItem = objectItem;
    objectBufferData.platform = platform;
    objectBufferData.x = x;
    objectBufferData.y = y;
    objectBufferData.height = height;
    objectBufferData.absoluteX = this->currentAbsoluteX+x;
    this->Insert_Into_Buffer(objectBufferData);
    this->Update_Level_Stats(x);
    this->lastObjectIsPlatform = platform;
    this->Handle_Zones(x);
    this->currentY = y;
    this->lastObjectLength = length;
    this->Check_Vertical_Object_Limit(length);
    return true;
}

bool Object_Buffer::Write_Object(int x, Background::Background background) {
    this->Update_Last_Page_Change_For_Next_X(x);
    assert(!this->coordinateSafety || this->Is_Coordinate_Valid(x));
    assert(this->Is_Safe_To_Write_Item());
    Buffer_Data objectBufferData;
    objectBufferData.objectItem = Object_Item::CHANGE_BACKGROUND;
    objectBufferData.x = x;
    objectBufferData.background = background;
    objectBufferData.absoluteX = this->currentAbsoluteX+x;
    this->Insert_Into_Buffer(objectBufferData);
    this->Update_Level_Stats(x);
    this->Handle_Zones(x);
    this->Check_Vertical_Object_Limit(1);
    return true;
}

bool Object_Buffer::Write_Object(int x, Brick::Brick brick, Scenery::Scenery scenery) {
    this->Update_Last_Page_Change_For_Next_X(x);
    assert(!this->coordinateSafety || this->Is_Coordinate_Valid(x));
    assert(this->Is_Safe_To_Write_Item());
    Buffer_Data objectBufferData;
    objectBufferData.objectItem = Object_Item::CHANGE_BRICK_AND_SCENERY;
    objectBufferData.x = x;
    objectBufferData.brick = brick;
    objectBufferData.scenery = scenery;
    objectBufferData.absoluteX = this->currentAbsoluteX+x;
    this->Insert_Into_Buffer(objectBufferData);
    this->Update_Level_Stats(x);
    this->Handle_Zones(x);
    this->Check_Vertical_Object_Limit(1);
    return true;
}

bool Object_Buffer::Write_Object(int page) {
    if (page < 0x00 || page > 0x3F) return false;
    if (this->currentPage == page) return true; //nothing to do. We are already on the requested page
    int relativeX = (0x10*page)-this->levelLength;
    if (relativeX < 0) return false;
    if (this->Is_Last_Item_A_Page_Change()) { //modify the previous page change if there is one
        this->Decrement_Vertical_Object_Count_At_X(this->itemBuffer->last().absoluteX);
        this->numBytesLeft += 2; //temporarily restore 2 bytes. This will be decremented again in Update_Level_Stats()
        if (!this->Handle_Level_Length_On_Page_Change(page)) return false;
        Buffer_Data *objectBufferData = this->Get_Current_For_Modification();
        objectBufferData->page = page;
        objectBufferData->absoluteX = this->currentAbsoluteX; //don't add x here
    } else { //write a new page change
        assert(this->Is_Safe_To_Write_Item());
        if (!this->Handle_Level_Length_On_Page_Change(page)) return false;
        Buffer_Data objectBufferData;
        objectBufferData.objectItem = Object_Item::PAGE_CHANGE;
        objectBufferData.page = page;
        objectBufferData.absoluteX = this->currentAbsoluteX; //don't add x here
        this->Insert_Into_Buffer(objectBufferData);
    }
    this->Update_Level_Stats(0); //this must be 0 for page change, since most of it was updated in Handle_Level_Length_On_Page_Change()
    this->Handle_Zones(relativeX);
    this->Check_Vertical_Object_Limit(1);
    return true;
}

void Object_Buffer::Check_Vertical_Object_Limit(int length) {
    for (int i = this->currentAbsoluteX; i < this->currentAbsoluteX+length; ++i) {
        int tmp = this->objectsAtXCoordinates->at(i);
        ++tmp;
        if (tmp > 3) this->exceededVerticalObjectLimit = true;
        this->objectsAtXCoordinates->data()[i] = tmp;
    }
}

QString Object_Buffer::Get_String_From_Object_Item(Object_Item::Object_Item objectItem) {
    switch (objectItem) {
    case Object_Item::QUESTION_BLOCK_WITH_MUSHROOM:                 return Object_Item::STRING_QUESTION_BLOCK_WITH_MUSHROOM;
        case Object_Item::QUESTION_BLOCK_WITH_COIN:                 return Object_Item::STRING_QUESTION_BLOCK_WITH_COIN;
        case Object_Item::HIDDEN_BLOCK_WITH_COIN:                   return Object_Item::STRING_HIDDEN_BLOCK_WITH_COIN;
        case Object_Item::HIDDEN_BLOCK_WITH_1UP:                    return Object_Item::STRING_HIDDEN_BLOCK_WITH_1UP;
        case Object_Item::BRICK_WITH_MUSHROOM:                      return Object_Item::STRING_BRICK_WITH_MUSHROOM;
        case Object_Item::BRICK_WITH_VINE:                          return Object_Item::STRING_BRICK_WITH_VINE;
        case Object_Item::BRICK_WITH_STAR:                          return Object_Item::STRING_BRICK_WITH_STAR;
        case Object_Item::BRICK_WITH_10_COINS:                      return Object_Item::STRING_BRICK_WITH_10_COINS;
        case Object_Item::BRICK_WITH_1UP:                           return Object_Item::STRING_BRICK_WITH_1UP;
        case Object_Item::UNDERWATER_SIDEWAYS_PIPE:                 return Object_Item::STRING_UNDERWATER_SIDEWAYS_PIPE;
        case Object_Item::USED_BLOCK:                               return Object_Item::STRING_USED_BLOCK;
        case Object_Item::TRAMPOLINE:                               return Object_Item::STRING_TRAMPOLINE;
        case Object_Item::BULLET_BILL_TURRET:                       return Object_Item::STRING_BULLET_BILL_TURRET;
        case Object_Item::ISLAND:                                   return Object_Item::STRING_ISLAND;
        case Object_Item::HORIZONTAL_BRICKS:                        return Object_Item::STRING_HORIZONTAL_BRICKS;
        case Object_Item::HORIZONTAL_BLOCKS:                        return Object_Item::STRING_HORIZONTAL_BLOCKS;
        case Object_Item::HORIZONTAL_COINS:                         return Object_Item::STRING_HORIZONTAL_COINS;
        case Object_Item::VERTICAL_BRICKS:                          return Object_Item::STRING_VERTICAL_BRICKS;
        case Object_Item::VERTICAL_BLOCKS:                          return Object_Item::STRING_VERTICAL_BLOCKS;
        case Object_Item::CORAL:                                    return Object_Item::STRING_CORAL;
        case Object_Item::PIPE:                                     return Object_Item::STRING_PIPE;
        case Object_Item::ENTERABLE_PIPE:                           return Object_Item::STRING_ENTERABLE_PIPE;
        case Object_Item::HOLE:                                     return Object_Item::STRING_HOLE;
        case Object_Item::HOLE_WITH_WATER:                          return Object_Item::STRING_HOLE_WITH_WATER;
        case Object_Item::BRIDGE:                                   return Object_Item::STRING_BRIDGE;
        case Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS:    return Object_Item::STRING_HORIZONTAL_QUESTION_BLOCKS_WITH_COINS;
        case Object_Item::PAGE_CHANGE:                              return Object_Item::STRING_PAGE_CHANGE;
        case Object_Item::REVERSE_L_PIPE:                           return Object_Item::STRING_REVERSE_L_PIPE;
        case Object_Item::FLAGPOLE:                                 return Object_Item::STRING_FLAGPOLE;
        case Object_Item::CASTLE:                                   return Object_Item::STRING_CASTLE;
        case Object_Item::BIG_CASTLE:                               return Object_Item::STRING_BIG_CASTLE;
        case Object_Item::AXE:                                      return Object_Item::STRING_AXE;
        case Object_Item::AXE_ROPE:                                 return Object_Item::STRING_AXE_ROPE;
        case Object_Item::BOWSER_BRIDGE:                            return Object_Item::STRING_BOWSER_BRIDGE;
        case Object_Item::SCROLL_STOP:                              return Object_Item::STRING_SCROLL_STOP;
        case Object_Item::SCROLL_STOP_WARP_ZONE:                    return Object_Item::STRING_SCROLL_STOP_WARP_ZONE;
        case Object_Item::TOGGLE_AUTO_SCROLL:                       return Object_Item::STRING_TOGGLE_AUTO_SCROLL;
        case Object_Item::FLYING_CHEEP_CHEEP_SPAWNER:               return Object_Item::STRING_FLYING_CHEEP_CHEEP_SPAWNER;
        case Object_Item::SWIMMING_CHEEP_CHEEP_SPAWNER:             return Object_Item::STRING_SWIMMING_CHEEP_CHEEP_SPAWNER;
        case Object_Item::BULLET_BILL_SPAWNER:                      return Object_Item::STRING_BULLET_BILL_SPAWNER;
        case Object_Item::CANCEL_SPAWNER:                           return Object_Item::STRING_CANCEL_SPAWNER;
        case Object_Item::LOOP_COMMAND:                             return Object_Item::STRING_LOOP_COMMAND;
        case Object_Item::CHANGE_BRICK_AND_SCENERY:                 return Object_Item::STRING_CHANGE_BRICK_AND_SCENERY;
        case Object_Item::CHANGE_BACKGROUND:                        return Object_Item::STRING_CHANGE_BACKGROUND;
        case Object_Item::LIFT_ROPE:                                return Object_Item::STRING_LIFT_ROPE;
        case Object_Item::BALANCE_LIFT_VERTICAL_ROPE:               return Object_Item::STRING_BALANCE_LIFT_VERTICAL_ROPE;
        case Object_Item::BALANCE_LIFT_HORIZONTAL_ROPE:             return Object_Item::STRING_BALANCE_LIFT_HORIZONTAL_ROPE;
        case Object_Item::STEPS:                                    return Object_Item::STRING_STEPS;
        case Object_Item::END_STEPS:                                return Object_Item::STRING_END_STEPS;
        case Object_Item::TALL_REVERSE_L_PIPE:                      return Object_Item::STRING_TALL_REVERSE_L_PIPE;
        case Object_Item::PIPE_WALL:                                return Object_Item::STRING_PIPE_WALL;
        case Object_Item::NOTHING:                                  return Object_Item::STRING_NOTHING;
    }
    assert(false); return QString();
}

QString Object_Buffer::Get_String_From_Background(Background::Background background) {
    switch (background) {
    case Background::BLANK_BACKGROUND:  return Background::STRING_BLANK_BACKGROUND;
    case Background::IN_WATER:          return Background::STRING_IN_WATER;
    case Background::CASTLE_WALL:       return Background::STRING_CASTLE_WALL;
    case Background::OVER_WATER:        return Background::STRING_OVER_WATER;
    case Background::NIGHT:             return Background::STRING_NIGHT;
    case Background::SNOW:              return Background::STRING_SNOW;
    case Background::NIGHT_AND_SNOW:    return Background::STRING_NIGHT_AND_SNOW;
    case Background::NIGHT_AND_FREEZE:  return Background::STRING_NIGHT_AND_FREEZE;
    }
    assert(false); return QString();
}

QString Object_Buffer::Get_String_From_Brick(Brick::Brick brick) {
    switch (brick) {
    case Brick::NO_BRICKS:                          return Brick::STRING_NO_BRICKS;
    case Brick::SURFACE:                            return Brick::STRING_SURFACE;
    case Brick::SURFACE_AND_CEILING:                return Brick::STRING_SURFACE_AND_CEILING;
    case Brick::SURFACE_AND_CEILING_3:              return Brick::STRING_SURFACE_AND_CEILING_3;
    case Brick::SURFACE_AND_CEILING_4:              return Brick::STRING_SURFACE_AND_CEILING_4;
    case Brick::SURFACE_AND_CEILING_8:              return Brick::STRING_SURFACE_AND_CEILING_8;
    case Brick::SURFACE_4_AND_CEILING:              return Brick::STRING_SURFACE_4_AND_CEILING;
    case Brick::SURFACE_4_AND_CEILING_3:            return Brick::STRING_SURFACE_4_AND_CEILING_3;
    case Brick::SURFACE_4_AND_CEILING_4:            return Brick::STRING_SURFACE_4_AND_CEILING_4;
    case Brick::SURFACE_5_AND_CEILING:              return Brick::STRING_SURFACE_5_AND_CEILING;
    case Brick::CEILING:                            return Brick::STRING_CEILING;
    case Brick::SURFACE_5_AND_CEILING_4:            return Brick::STRING_SURFACE_5_AND_CEILING_4;
    case Brick::SURFACE_8_AND_CEILING:              return Brick::STRING_SURFACE_8_AND_CEILING;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_5:   return Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_5;
    case Brick::SURFACE_AND_CEILING_AND_MIDDLE_4:   return Brick::STRING_SURFACE_AND_CEILING_AND_MIDDLE_4;
    case Brick::ALL:                                return Brick::STRING_ALL;
    }
    assert(false); return QString();
}

QString Object_Buffer::Get_String_From_Scenery(Scenery::Scenery scenery) {
    switch (scenery) {
    case Scenery::NO_SCENERY:   return Scenery::STRING_NO_SCENERY;
    case Scenery::ONLY_CLOUDS:  return Scenery::STRING_ONLY_CLOUDS;
    case Scenery::MOUNTAINS:    return Scenery::STRING_MOUNTAINS;
    case Scenery::FENCES:       return Scenery::STRING_FENCES;
    }
    assert(false); return QString();
}

void Object_Buffer::Handle_Zones(int x) {
    if (this->coinBlockZone > 0) this->coinBlockZone -= x;
    if (this->powerupZone > 0) this->powerupZone -= x;
    if (this->coinBlockZone < 0) this->coinBlockZone = 0;
    if (this->powerupZone < 0) this->powerupZone = 0;
}

bool Object_Buffer::Is_Y_Valid(int y) {
    return (y >= 0x0 && y <= 0xF);
}

bool Object_Buffer::Is_Coordinate_Valid(int &coordinate) {
    if (coordinate < 0) return false;
    if (this->firstPageSafety) {
        this->firstPageSafety = false;
        return (coordinate >= 0x00 && coordinate <= 0x1F);
    }
    if (this->Is_Last_Item_A_Page_Change() && coordinate >= 0x10) {
        assert(this->Page_Change(((this->levelLength+coordinate)/16)+1));
        while (coordinate >= 0x10) coordinate -= 0x10;
        return true;
    } else {
        return coordinate <= 0x10;
    }
}

void Object_Buffer::Update_Last_Page_Change_For_Next_X(int &x) {
    if (!this->Is_Last_Item_A_Page_Change()) return; //nothing to do
    assert(this->Page_Change((this->levelLength+x)/16));
    while (x > 0x0F) x -= 0x10;
}

void Object_Buffer::Insert_Into_Block_Map(Object_Item::Object_Item objectItem, int y, int length, bool questionBlock) {
    QMap<QString, Block_Data> *blocks = this->brickBlocks;
    if (questionBlock) blocks = this->questionBlocks;
    Block_Data data;
    data.objectItem = objectItem;
    data.x = this->currentAbsoluteX;
    data.y = y;
    data.groupX = this->currentAbsoluteX;
    data.groupY = y;
    data.groupLength = length;
    data.hittable = false;
    data.safeForMushroom = false;
    data.safeForStar = false;
    if (objectItem == Object_Item::VERTICAL_BRICKS) {
        for (int i = 0; i < length && y+i < 0xB; ++i) {
            data.y = y+i;

            //Only insert if it doesn't exist
            QString key = this->Get_Coordinate_Key(data.x, data.y);
            QMap<QString, Block_Data>::iterator iter = blocks->find(key);
            if (iter == blocks->end()) blocks->insert(key, data);
            if (questionBlock) this->brickBlocks->remove(key);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            data.x = this->currentAbsoluteX+i;

            //Only insert if it doesn't exist or if it is a single block
            QString key = this->Get_Coordinate_Key(data.x, data.y);
            QMap<QString, Block_Data>::iterator iter = blocks->find(key);
            if (iter == blocks->end() || iter->groupLength == 1) blocks->insert(key, data);
            if (questionBlock) this->brickBlocks->remove(key);
        }
    }
}

bool Object_Buffer::Question_Block_With_Mushroom(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(Object_Item::QUESTION_BLOCK_WITH_MUSHROOM, true, x, y, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Question_Block_With_Coin(int x, int y) {
    if (y > 0xB) return false;
    if (!this->Write_Object(Object_Item::QUESTION_BLOCK_WITH_COIN, true, x, y, Physics::MIN_OBJECT_LENGTH)) return false;
    this->Insert_Into_Block_Map(Object_Item::QUESTION_BLOCK_WITH_COIN, y, Physics::MIN_OBJECT_LENGTH, true);
    return true;
}

bool Object_Buffer::Hidden_Block_With_Coin(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(Object_Item::HIDDEN_BLOCK_WITH_COIN, true, x, y, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Hidden_Block_With_1up(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(Object_Item::HIDDEN_BLOCK_WITH_1UP, true, x, y, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Brick_With_Mushroom(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(Object_Item::BRICK_WITH_MUSHROOM, true, x, y, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Brick_With_Star(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(Object_Item::BRICK_WITH_STAR, true, x, y, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Brick_With_10_Coins(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(Object_Item::BRICK_WITH_10_COINS, true, x, y, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Brick_With_1up(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(Object_Item::BRICK_WITH_1UP, true, x, y, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Brick_With_Vine_Without_Pointer(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(Object_Item::BRICK_WITH_VINE, true, x, y, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Used_Block(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(Object_Item::USED_BLOCK, true, x, y, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Trampoline(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(Object_Item::TRAMPOLINE, true, x, y, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Bullet_Bill_Turret(int x, int y, int height) {
    if (y > 0xB) return false;
    if (height < 1 || height > 16) return false;
    return this->Write_Object(Object_Item::BULLET_BILL_TURRET, true, x, y, height, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Island(int x, int y, int length) {
    if (y > 0xB) return false;
    if (length < 1 || length > 16) return false;
    return this->Write_Object(Object_Item::ISLAND, true, x, y, length);
}

bool Object_Buffer::Horizontal_Bricks(int x, int y, int length) {
    if (y > 0xB) return false;
    if (length < 1 || length > 16) return false;
    if (!this->Write_Object(Object_Item::HORIZONTAL_BRICKS, true, x, y, length)) return false;
    this->Insert_Into_Block_Map(Object_Item::HORIZONTAL_BRICKS, y, length, false);
    return true;
}

bool Object_Buffer::Horizontal_Blocks(int x, int y, int length) {
    if (y > 0xB) return false;
    if (length < 1 || length > 16) return false;
    return this->Write_Object(Object_Item::HORIZONTAL_BLOCKS, true, x, y, length);
}

bool Object_Buffer::Horizontal_Coins(int x, int y, int length) {
    if (y > 0xB) return false;
    if (length < 1 || length > 16) return false;
    return this->Write_Object(Object_Item::HORIZONTAL_COINS, false, x, y, length);
}

bool Object_Buffer::Vertical_Bricks(int x, int y, int height) {
    if (y > 0xB) return false;
    if (height < 1 || height > 16) return false;
    if (!this->Write_Object(Object_Item::VERTICAL_BRICKS, true, x, y, height, Physics::MIN_OBJECT_LENGTH)) return false;
    this->Insert_Into_Block_Map(Object_Item::VERTICAL_BRICKS, y, height, false);
    return true;
}

bool Object_Buffer::Vertical_Blocks(int x, int y, int height) {
    if (y > 0xB) return false;
    if (height < 1 || height > 16) return false;

    //Bullet Time Mod
    if (this->args->difficulty >= this->args->difficultyBulletTime && this->args->levelCompliment == Level_Compliment::BULLET_BILL_TURRETS) {
        return this->Bullet_Bill_Turret(x, y, height);
    }
    return this->Write_Object(Object_Item::VERTICAL_BLOCKS, true, x, y, height, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Coral(int x, int y, int height) {
    if (y > 0xB) return false;
    if (height < 1 || height > 16) return false;
    return this->Write_Object(Object_Item::CORAL, true, x, y, height, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Underwater_Sideways_Pipe_Without_Pointer(int x, int y) {
    if (y > 0xB) return false;
    return this->Write_Object(Object_Item::UNDERWATER_SIDEWAYS_PIPE, true, x, y, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Pipe(int x, int y, int height) {
    if (y > 0xB) return false;
    if (height < 2 || height > 8) return false;
    return this->Write_Object(Object_Item::PIPE, true, x, y, height, Physics::PIPE_LENGTH);
}

bool Object_Buffer::Enterable_Pipe_Without_Pointer(int x, int y, int height) {
    if (y > 0xB) return false;
    if (height < 2 || height > 8) return false;
    return this->Write_Object(Object_Item::ENTERABLE_PIPE, true, x, y, height, Physics::PIPE_LENGTH);
}

bool Object_Buffer::Tall_Reverse_L_Pipe_Without_Pointer(int x, int y) {
    if (y < 0x2 || y > 0xA) return false;
    return this->Write_Object(Object_Item::TALL_REVERSE_L_PIPE, true, x, y, Physics::REVERSE_L_PIPE_LENGTH);
}

bool Object_Buffer::Hole(int x, int length, bool filledWithWater) {
    if (length < 1 || length > 16) return false;
    if (filledWithWater) {
        return this->Write_Object(Object_Item::HOLE_WITH_WATER, false, x, length);
    } else {
        return this->Write_Object(Object_Item::HOLE, false, x, length);
    }
}

bool Object_Buffer::Bridge(int x, int yPlacement, int length) {
    if (length < 1 || length > 16) return false;
    if (yPlacement == 0x7 || yPlacement == 0x8 || yPlacement == 0xA) {
        return this->Write_Object(Object_Item::BRIDGE, true, x, yPlacement, length);
    } else {
        return false;
    }
}

bool Object_Buffer::Horizontal_Question_Blocks_With_Coins(int x, int yPlacement, int length) {
    if (length < 1 || length > 16) return false;
    if (yPlacement == 0x3 || yPlacement == 0x7) {
        if (!this->Write_Object(Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS, true, x, yPlacement, length)) return false;
        this->Insert_Into_Block_Map(Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS, yPlacement, length, true);
        return true;
    } else {
        return false;
    }
}

bool Object_Buffer::Page_Change(int page) {
    return this->Write_Object(page);
}

bool Object_Buffer::Flagpole(int x) {
    return this->Write_Object(Object_Item::FLAGPOLE, false, x, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Castle(int x) {
    return this->Write_Object(Object_Item::CASTLE, false, x, Physics::CASTLE_LENGTH);
}

bool Object_Buffer::Big_Castle(int x) {
    return this->Write_Object(Object_Item::BIG_CASTLE, false, x, Physics::CASTLE_LENGTH);
}

bool Object_Buffer::Axe(int x) {
    if (this->Get_Page_Relative_Absolute_X(x) == 0xF) return false;
    return this->Write_Object(Object_Item::AXE, false, x, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Axe_Rope(int x) {
    if (this->Get_Page_Relative_Absolute_X(x) == 0xF) return false;
    return this->Write_Object(Object_Item::AXE_ROPE, false, x, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Bowser_Bridge(int x) {
    if (this->Get_Page_Relative_Absolute_X(x) == 0xF) return false;
    return this->Write_Object(Object_Item::BOWSER_BRIDGE, false, x, Physics::BOWSER_BRIDGE_LENGTH);
}

bool Object_Buffer::Scroll_Stop(int x, bool warpZone) {
    if (this->Get_Page_Relative_Absolute_X(x) == 0xF) return false;
    if (warpZone) {
        return this->Write_Object(Object_Item::SCROLL_STOP_WARP_ZONE, false, x, Physics::MIN_OBJECT_LENGTH);
    } else {
        return this->Write_Object(Object_Item::SCROLL_STOP, false, x, Physics::MIN_OBJECT_LENGTH);
    }
}

bool Object_Buffer::Toggle_Auto_Scroll(int x) {
    if (this->Get_Page_Relative_Absolute_X(x) == 0xF) return false;
    if (this->Write_Object(Object_Item::TOGGLE_AUTO_SCROLL, false, x, Physics::MIN_OBJECT_LENGTH)) {
        this->wasAutoScrollUsed = true;
        this->autoScrollActive = !this->autoScrollActive;
        return true;
    } else {
        return false;
    }
}

bool Object_Buffer::Flying_Cheep_Cheep_Spawner(int x) {
    if (this->Get_Page_Relative_Absolute_X(x) == 0xF) return false;
    if (!this->Write_Object(Object_Item::FLYING_CHEEP_CHEEP_SPAWNER, false, x, Physics::MIN_OBJECT_LENGTH)) return false;
    this->wereFlyingCheepCheepsSpawned = true;
    return true;
}

bool Object_Buffer::Swimming_Cheep_Cheep_Spawner(int x) {
    if (this->Get_Page_Relative_Absolute_X(x) == 0xF) return false;
    return this->Write_Object(Object_Item::SWIMMING_CHEEP_CHEEP_SPAWNER, false, x, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Bullet_Bill_Spawner(int x) {
    if (this->Get_Page_Relative_Absolute_X(x) == 0xF) return false;
    return this->Write_Object(Object_Item::BULLET_BILL_SPAWNER, false, x, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Cancel_Spawner(int x) {
    if (this->Get_Page_Relative_Absolute_X(x) == 0xF) return false;
    if (this->Write_Object(Object_Item::CANCEL_SPAWNER, false, x, Physics::MIN_OBJECT_LENGTH)) {
        this->cancelSpawnerX = this->levelLength;
        return true;
    } else {
        return false;
    }
}

bool Object_Buffer::Loop_Command(int x) {
    if (this->Get_Page_Relative_Absolute_X(x) == 0xF) return false;
    return this->Write_Object(Object_Item::LOOP_COMMAND, false, x, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Change_Brick_And_Scenery(int x, Brick::Brick brick, Scenery::Scenery scenery) {
    return this->Write_Object(x, brick, scenery);
}

bool Object_Buffer::Change_Background(int x, Background::Background background) {
    return this->Write_Object(x, background);
}

bool Object_Buffer::Lift_Rope(int x) {
    return this->Write_Object(Object_Item::LIFT_ROPE, false, x, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Balance_Lift_Vertical_Rope(int x, int length) {
    if (length < 1 || length > 16) return false;
    return this->Write_Object(Object_Item::BALANCE_LIFT_VERTICAL_ROPE, false, x, 1, length, Physics::MIN_OBJECT_LENGTH);
}

bool Object_Buffer::Balance_Lift_Horizontal_Rope(int x, int length) {
    if (length < 3 || length > 16) return false;
    return this->Write_Object(Object_Item::BALANCE_LIFT_HORIZONTAL_ROPE, false, x, length);
}

bool Object_Buffer::Steps(int x, int width) {
    if (width < 1 || width > 8) return false;
    if (!this->Write_Object(Object_Item::STEPS, true, x, width)) return false;
    this->currentY = 11-width;
    return true;
}

bool Object_Buffer::End_Steps(int x) {
    if (!this->Write_Object(Object_Item::END_STEPS, true, x, Physics::END_STAIRS_LENGTH)) return false;
    this->currentY = 3;
    return true;
}

bool Object_Buffer::Reverse_L_Pipe_Without_Pointer(int x) {
    return this->Write_Object(Object_Item::REVERSE_L_PIPE, true, x, Physics::REVERSE_L_PIPE_LENGTH);
}

bool Object_Buffer::Pipe_Wall(int x) {
    return this->Write_Object(Object_Item::PIPE_WALL, true, x, Physics::REVERSE_L_PIPE_LENGTH);
}

bool Object_Buffer::Nothing() {
    return this->Write_Object(Object_Item::NOTHING, false, 0, Physics::MIN_OBJECT_LENGTH);
}
