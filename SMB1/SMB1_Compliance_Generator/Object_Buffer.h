#ifndef OBJECT_BUFFER_H
#define OBJECT_BUFFER_H

#include "Item_Buffer.h"
#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Object_Item.h"
#include "../Common_SMB1_Files/Scenery.h"
#include "SMB1_Compliance_Generator_Arguments.h"
#include <QFile>
#include <QLinkedList>
#include <QMap>
#include <QVector>

struct Block_Data;

class Object_Buffer : public Item_Buffer {
public:
    Object_Buffer(int numBytesLeft, SMB1_Compliance_Generator_Arguments *args);
    ~Object_Buffer();
    void Clear_Buffer();
    bool Write_Buffer_To_File(QFile *file);
    bool Did_Level_Exceed_Vertical_Object_Limit();
    int Get_Last_Object_Length();
    void Increment_Last_Object_Length(int value);
    void Set_Last_Object_Length(int value);
    bool Was_Last_Object_A_Platform();
    int Get_Num_Objects_Left();
    int Get_Num_Objects_Available();
    bool Reserve_Objects(int amount);
    bool Free_Reserved_Objects(int amount);
    bool Were_Flying_Cheep_Cheeps_Spawned();
    bool Is_Auto_Scroll_Active();
    bool Was_Auto_Scroll_Used();
    int Get_Cancel_Spawner_X();
    bool Is_Midpoint_Ready();
    bool Get_First_Page_Safety();
    void Set_First_Page_Safety(bool firstPageSafety);
    void Set_End_Object_Count(int value);
    int Get_Relative_X_From_Absolute_X(int absoluteX);
    bool Decrement_Vertical_Object_Count_At_X(int x);
    bool Decrement_Vertical_Object_Count_Starting_At_X(int x, int length);
    int Get_Num_Vertical_Objects_At_X(int x);
    QString Get_Coordinate_Key(int x, int y);
    QMap<QString, Block_Data> *Get_Question_Blocks();
    QMap<QString, Block_Data> *Get_Brick_Blocks();

    //These insert after the current line
    //Note: There is no way to insert at the beginning after the first insertion!
    bool Question_Block_With_Mushroom(int x, int y);
    bool Question_Block_With_Coin(int x, int y);
    bool Hidden_Block_With_Coin(int x, int y);
    bool Hidden_Block_With_1up(int x, int y);
    bool Brick_With_Mushroom(int x, int y);
    bool Brick_With_Star(int x, int y);
    bool Brick_With_10_Coins(int x, int y);
    bool Brick_With_1up(int x, int y);
    bool Brick_With_Vine_Without_Pointer(int x, int y);
    bool Used_Block(int x, int y);
    bool Trampoline(int x, int y);
    bool Bullet_Bill_Turret(int x, int y, int height);
    bool Island(int x, int y, int length);
    bool Horizontal_Bricks(int x, int y, int length);
    bool Horizontal_Blocks(int x, int y, int length);
    bool Horizontal_Coins(int x, int y, int length);
    bool Vertical_Bricks(int x, int y, int height);
    bool Vertical_Blocks(int x, int y, int height);
    bool Coral(int x, int y, int height);
    bool Underwater_Sideways_Pipe_Without_Pointer(int x, int y);
    bool Pipe(int x, int y, int height);
    bool Enterable_Pipe_Without_Pointer(int x, int y, int height);
    bool Tall_Reverse_L_Pipe_Without_Pointer(int x, int y);
    bool Hole(int x, int length, bool filledWithWater = false);
    bool Bridge(int x, int yPlacement, int length);
    bool Horizontal_Question_Blocks_With_Coins(int x, int yPlacement, int length);
    bool Page_Change(int amount);
    bool Flagpole(int x);
    bool Castle(int x);
    bool Big_Castle(int x);
    bool Axe(int x);
    bool Axe_Rope(int x);
    bool Bowser_Bridge(int x);
    bool Scroll_Stop(int x, bool warpZone);
    bool Toggle_Auto_Scroll(int x);
    bool Flying_Cheep_Cheep_Spawner(int x);
    bool Swimming_Cheep_Cheep_Spawner(int x);
    bool Bullet_Bill_Spawner(int x);
    bool Cancel_Spawner(int x);
    bool Loop_Command(int x);
    bool Change_Brick_And_Scenery(int x, Brick::Brick brick, Scenery::Scenery scenery);
    bool Change_Background(int x, Background::Background background);
    bool Lift_Rope(int x);
    bool Balance_Lift_Vertical_Rope(int x, int length);
    bool Balance_Lift_Horizontal_Rope(int x, int length);
    bool Steps(int x, int width);
    bool End_Steps(int x);
    bool Reverse_L_Pipe_Without_Pointer(int x);
    bool Pipe_Wall(int x);
    bool Nothing();

private:
    Object_Buffer(const Object_Buffer&);
    Object_Buffer& operator=(const Object_Buffer&);
    bool Write_Object(Object_Item::Object_Item objectItem, bool platform, int x, int length);
    bool Write_Object(Object_Item::Object_Item objectItem, bool platform, int x, int y, int length);
    bool Write_Object(Object_Item::Object_Item objectItem, bool platform, int x, int y, int height, int length);
    bool Write_Object(int x, Background::Background background);
    bool Write_Object(int x, Brick::Brick brick, Scenery::Scenery scenery);
    bool Write_Object(int page);
    void Check_Vertical_Object_Limit(int length);
    QString Get_String_From_Object_Item(Object_Item::Object_Item objectItem);
    QString Get_String_From_Background(Background::Background background);
    QString Get_String_From_Brick(Brick::Brick brick);
    QString Get_String_From_Scenery(Scenery::Scenery scenery);
    void Handle_Zones(int x);
    bool Is_Y_Valid(int y);
    bool Is_Coordinate_Valid(int &coordinate);
    void Update_Last_Page_Change_For_Next_X(int &x);
    void Insert_Into_Block_Map(Object_Item::Object_Item objectItem, int y, int length, bool questionBlock);

    SMB1_Compliance_Generator_Arguments *args;
    QVector<int> *objectsAtXCoordinates;
    QMap<QString, Block_Data> *questionBlocks;
    QMap<QString, Block_Data> *brickBlocks;
    bool exceededVerticalObjectLimit;
    int lastObjectLength;
    bool lastObjectIsPlatform;
    int coinBlockZone;
    int powerupZone;
    int endObjectCount;
    int reservedObjectCount;
    int totalBytes;
    bool firstPageSafety;
    bool autoScrollActive;
    bool wereFlyingCheepCheepsSpawned;
    bool wasAutoScrollUsed;
    int cancelSpawnerX;

    friend class Pipe_Pointer_Buffer;
};

#endif // OBJECT_BUFFER_H
