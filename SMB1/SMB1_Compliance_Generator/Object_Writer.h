#ifndef OBJECT_WRITER_H
#define OBJECT_WRITER_H

#include "../Common SMB1 Files/Background.h"
#include "../Common SMB1 Files/Brick.h"
#include "../Common SMB1 Files/Scenery.h"
#include "Item_Writer.h"

class Object_Writer : public Item_Writer
{
public:
    Object_Writer(QTextStream *stream, int numBytesLeft);
    ~Object_Writer() {}
    int Get_Last_Object_Length();
    void Increment_Last_Object_Length(int value);
    bool Was_Last_Object_A_Platform();
    int Get_Num_Objects_Left();
    int Get_Num_Objects_Available();
    bool Is_Midpoint_Ready();
    bool Get_First_Page_Safety();
    void Set_First_Page_Safety(bool firstPageSafety);
    void Set_End_Object_Count(int value);

    bool Question_Block_With_Mushroom(int x, int y);
    bool Question_Block_With_Mushroom_Only(int x, int y);
    bool Question_Block_With_Coin(int x, int y);
    bool Hidden_Block_With_Coin(int x, int y);
    bool Hidden_Block_With_1up(int x, int y);
    bool Brick_With_Mushroom(int x, int y);
    bool Brick_With_Mushroom_Only(int x, int y);
    bool Brick_With_Star(int x, int y);
    bool Brick_With_Star_Only(int x, int y);
    bool Brick_With_10_Coins(int x, int y);
    bool Brick_With_10_Coins_Only(int x, int y);
    bool Brick_With_1up(int x, int y);
    bool Brick_With_1up_Only(int x, int y);
    bool Used_Block(int x, int y);
    bool Trampoline(int x, int y);
    bool Cannon(int x, int y, int height);
    bool Island(int x, int y, int length);
    bool Horizontal_Bricks(int x, int y, int length);
    bool Horizontal_Blocks(int x, int y, int length);
    bool Horizontal_Coins(int x, int y, int length);
    bool Vertical_Bricks(int x, int y, int height);
    bool Vertical_Blocks(int x, int y, int height);
    bool Corral(int x, int y, int height);
    bool Pipe(int x, int y, int height);
    bool Hole(int x, int length, bool filledWithWater = false);
    bool Balance_Rope(int x, int length);
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
    bool Flying_Cheep_Cheep_Spawner(int x);
    bool Swimming_Cheep_Cheep_Spawner(int x);
    bool Bullet_Bill_Spawner(int x);
    bool Cancel_Spawner(int x);
    bool Loop_Command(int x);
    bool Change_Brick_And_Scenery(int x, Brick::Brick brick, Scenery::Scenery scenery);
    bool Change_Background(int x, Background::Background background);
    bool Lift_Rope(int x);
    bool Balance_Lift_Rope(int x, int length);
    bool Steps(int x, int width);
    bool End_Steps(int x);
    bool Pipe_Wall(int x);
    bool Nothing();


private:
    Object_Writer(const Object_Writer&);
    Object_Writer& operator=(const Object_Writer&);
    bool Write_Object(int x, const QString &object, bool platform);
    bool Write_Object(int x, const QString &object, int length, bool platform);
    bool Write_Object(int x, const QString &object, const QString &parameters, bool platform);
    bool Write_Object(int x, const QString &object, const QString &parameters, int length, bool platform);
    bool Write_Object(int x, int y, const QString &object, int length, bool platform);
    bool Write_Object(int x, int y, const QString &object, const QString &parameters, int length, bool platform);
    void Handle_Zones(int x);
    bool Is_Y_Valid(int y);
    bool Is_Coordinate_Valid(int coordinate);

    const static int MAX_COIN_BLOCK_ZONE = 24;
    const static int MAX_POWERUP_ZONE = 56;

    int lastObjectLength;
    bool lastObjectIsPlatform;
    int coinBlockZone;
    int powerupZone;
    int endObjectCount;
    int totalBytes;
    bool firstPageSafety;

    friend class Pipe_Pointer_Writer;
};

#endif // OBJECT_WRITER_H
