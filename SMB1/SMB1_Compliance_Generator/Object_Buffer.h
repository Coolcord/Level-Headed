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
#include <QVector>

struct Object_Buffer_Data;

class Object_Buffer : public Item_Buffer {
public:
    Object_Buffer(int numBytesLeft, SMB1_Compliance_Generator_Arguments *args);
    ~Object_Buffer();
    bool Write_Buffer_To_File(QFile *file);
    bool Did_Level_Exceed_Vertical_Object_Limit();
    int Get_Last_Object_Length();
    void Increment_Last_Object_Length(int value);
    void Set_Last_Object_Length(int value);
    bool Was_Last_Object_A_Platform();
    int Get_Num_Objects_Left();
    int Get_Num_Objects_Available();
    bool Were_Flying_Cheep_Cheeps_Spawned();
    bool Is_Auto_Scroll_Active();
    bool Was_Auto_Scroll_Used();
    int Get_Cancel_Spawner_X();
    bool Is_Midpoint_Ready();
    bool Get_First_Page_Safety();
    void Set_First_Page_Safety(bool firstPageSafety);
    void Set_End_Object_Count(int value);

    bool Question_Block_With_Mushroom(int x, int y);
    bool Question_Block_With_Mushroom_Only(int x, int y);
    bool Question_Block_With_Coin(int x, int y);
    bool Hidden_Block_With_Coin(int x, int y);
    bool Hidden_Block_With_1up(int x, int y);
    bool Hidden_Block_With_1up_Only(int x, int y);
    bool Brick_With_Mushroom(int x, int y);
    bool Brick_With_Mushroom_Only(int x, int y);
    bool Brick_With_Star(int x, int y);
    bool Brick_With_Star_Only(int x, int y);
    bool Brick_With_10_Coins(int x, int y);
    bool Brick_With_10_Coins_Only(int x, int y);
    bool Brick_With_1up(int x, int y);
    bool Brick_With_1up_Only(int x, int y);
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

    //Buffer Navigation
    bool Is_Empty();
    bool At_Beginning();
    bool At_End();
    void Seek_To_Beginning();
    void Seek_To_Next();
    void Seek_To_Previous();
    void Seek_To_End();
    Object_Buffer_Data Get_Current();

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
    bool Is_Coordinate_Valid(int coordinate);

    const static int MAX_COIN_BLOCK_ZONE = 24;
    const static int MAX_POWERUP_ZONE = 24;

    SMB1_Compliance_Generator_Arguments *args;
    QLinkedList<Object_Buffer_Data> *objectBuffer;
    QLinkedList<Object_Buffer_Data>::iterator objectBufferIter;
    QVector<int> *objectsAtXCoordinates;
    bool exceededVerticalObjectLimit;
    int lastObjectLength;
    bool lastObjectIsPlatform;
    int coinBlockZone;
    int powerupZone;
    int endObjectCount;
    int totalBytes;
    bool firstPageSafety;
    bool autoScrollActive;
    bool wereFlyingCheepCheepsSpawned;
    bool wasAutoScrollUsed;
    int cancelSpawnerX;

    friend class Pipe_Pointer_Buffer;
};

#endif // OBJECT_BUFFER_H
