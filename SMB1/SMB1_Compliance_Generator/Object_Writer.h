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

    bool Question_Block_With_Mushroom(int x, int y);
    bool Question_Block_With_Coin(int x, int y);
    bool Hidden_Block_With_Coin(int x, int y);
    bool Hidden_Block_With_1up(int x, int y);
    bool Brick_With_Mushroom(int x, int y);
    bool Brick_With_Star(int x, int y);
    bool Brick_With_10_Coins(int x, int y);
    bool Brick_With_1up(int x, int y);
    bool Underwater_Sideways_Pipe(int x, int y);
    bool Used_Block(int x, int y);
    bool Trampoline(int x, int y);
    bool Cannon(int x, int y, int height);
    bool Island(int x, int y, int length);
    bool Horizontal_Bricks(int x, int y, int length);
    bool Horizontal_Blocks(int x, int y, int length);
    bool Horizontal_Coins(int x, int y, int length);
    bool Vertical_Bricks(int x, int y, int height);
    bool Vertical_Blocks(int x, int y, int height);
    bool Pipe(int x, int y, int height);
    bool Hole(int x, int length, bool filledWithWater = false);
    bool Balance_Rope(int x, int length);
    bool Bridge(int x, int yPlacement, int length);
    bool Horizontal_Question_Blocks_With_Coins(int x, int yPlacement, int length);
    bool Page_Skip(int amount);
    bool Reverse_L_Pipe(int x);
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
    bool Write_Object(int x, const QString &object);
    bool Write_Object(int x, const QString &object, const QString &parameters);
    bool Write_Object(int x, int y, const QString &object);
    bool Write_Object(int x, int y, const QString &object, const QString &parameters);
    void Handle_Zones(int x);

    const static int MAX_COIN_BLOCK_ZONE = 24;
    const static int MAX_POWERUP_ZONE = 56;

    int coinBlockZone;
    int powerupZone;

    friend class Pipe_Pointer_Writer;
};

#endif // OBJECT_WRITER_H
