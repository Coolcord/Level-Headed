#ifndef OBJECT_HANDLER_H
#define OBJECT_HANDLER_H

#include "Item_Handler.h"

class Object_Handler : public Item_Handler
{
public:
    Object_Handler(SMB1_Writer_Interface *writerPlugin) : Item_Handler(writerPlugin) {}

    bool Question_Block_With_Mushroom(const QString &line);
    bool Question_Block_With_Coin(const QString &line);
    bool Hidden_Block_With_Coin(const QString &line);
    bool Hidden_Block_With_1up(const QString &line);
    bool Brick_With_Mushroom(const QString &line);
    bool Brick_With_Vine(const QString &line);
    bool Brick_With_Star(const QString &line);
    bool Brick_With_10_Coins(const QString &line);
    bool Brick_With_1up(const QString &line);
    bool Underwater_Sideways_Pipe(const QString &line);
    bool Used_Block(const QString &line);
    bool Trampoline(const QString &line);
    bool Cannon(const QString &line);
    bool Island(const QString &line);
    bool Horizontal_Bricks(const QString &line);
    bool Horizontal_Blocks(const QString &line);
    bool Horizontal_Coins(const QString &line);
    bool Vertical_Bricks(const QString &line);
    bool Vertical_Blocks(const QString &line);
    bool Pipe(const QString &line, bool enterable);
    bool Hole(const QString &line, bool filledWithWater);
    bool Balance_Rope(const QString &line);
    bool Bridge(const QString &line);
    bool Horizontal_Question_Blocks_With_Coins(const QString &line);
    bool Page_Change(const QString &line);
    bool Reverse_L_Pipe(const QString &line);
    bool Flagpole(const QString &line);
    bool Castle(const QString &line);
    bool Big_Castle(const QString &line);
    bool Axe(const QString &line);
    bool Axe_Rope(const QString &line);
    bool Bowser_Bridge(const QString &line);
    bool Scroll_Stop(const QString &line, bool warpZone);
    bool Flying_Cheep_Cheep_Spawner(const QString &line);
    bool Swimming_Cheep_Cheep_Spawner(const QString &line);
    bool Bullet_Bill_Spawner(const QString &line);
    bool Cancel_Spawner(const QString &line);
    bool Loop_Command(const QString &line);
    bool Change_Brick_And_Scenery(const QString &line);
    bool Change_Background(const QString &line);
    bool Lift_Rope(const QString &line);
    bool Balance_Lift_Rope(const QString &line);
    bool Steps(const QString &line);
    bool End_Steps(const QString &line);
    bool Tall_Reverse_L_Pipe(const QString &line);
    bool Pipe_Wall(const QString &line);
    bool Nothing(const QString &line);

private:
    SMB1_Writer_Interface *writerPlugin;
};

#endif // OBJECT_HANDLER_H
