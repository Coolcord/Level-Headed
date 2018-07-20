#ifndef OBJECT_HANDLER_H
#define OBJECT_HANDLER_H

#include "Item_Handler.h"
#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Scenery.h"
#include <QMap>

class Object_Handler : public Item_Handler
{
public:
    Object_Handler(SMB1_Writer_Interface *writerPlugin);
    ~Object_Handler();

    bool Question_Block_With_Mushroom(const QString &line, int &errorCode);
    bool Question_Block_With_Coin(const QString &line, int &errorCode);
    bool Hidden_Block_With_Coin(const QString &line, int &errorCode);
    bool Hidden_Block_With_1up(const QString &line, int &errorCode);
    bool Brick_With_Mushroom(const QString &line, int &errorCode);
    bool Brick_With_Vine(const QString &line, int &errorCode);
    bool Brick_With_Star(const QString &line, int &errorCode);
    bool Brick_With_10_Coins(const QString &line, int &errorCode);
    bool Brick_With_1up(const QString &line, int &errorCode);
    bool Underwater_Sideways_Pipe(const QString &line, int &errorCode);
    bool Used_Block(const QString &line, int &errorCode);
    bool Trampoline(const QString &line, int &errorCode);
    bool Cannon(const QString &line, int &errorCode);
    bool Island(const QString &line, int &errorCode);
    bool Horizontal_Bricks(const QString &line, int &errorCode);
    bool Horizontal_Blocks(const QString &line, int &errorCode);
    bool Horizontal_Coins(const QString &line, int &errorCode);
    bool Vertical_Bricks(const QString &line, int &errorCode);
    bool Vertical_Blocks(const QString &line, int &errorCode);
    bool Corral(const QString &line, int &errorCode);
    bool Pipe(const QString &line, bool enterable, int &errorCode);
    bool Hole(const QString &line, bool filledWithWater, int &errorCode);
    bool Bridge(const QString &line, int &errorCode);
    bool Horizontal_Question_Blocks_With_Coins(const QString &line, int &errorCode);
    bool Page_Change(const QString &line, int &errorCode);
    bool Reverse_L_Pipe(const QString &line, int &errorCode);
    bool Flagpole(const QString &line, int &errorCode);
    bool Castle(const QString &line, int &errorCode);
    bool Big_Castle(const QString &line, int &errorCode);
    bool Axe(const QString &line, int &errorCode);
    bool Axe_Rope(const QString &line, int &errorCode);
    bool Bowser_Bridge(const QString &line, int &errorCode);
    bool Scroll_Stop(const QString &line, bool warpZone, int &errorCode);
    bool Toggle_Auto_Scroll(const QString &line, int &errorCode);
    bool Flying_Cheep_Cheep_Spawner(const QString &line, int &errorCode);
    bool Swimming_Cheep_Cheep_Spawner(const QString &line, int &errorCode);
    bool Bullet_Bill_Spawner(const QString &line, int &errorCode);
    bool Cancel_Spawner(const QString &line, int &errorCode);
    bool Loop_Command(const QString &line, int &errorCode);
    bool Change_Brick_And_Scenery(const QString &line, int &errorCode);
    bool Change_Background(const QString &line, int &errorCode);
    bool Lift_Rope(const QString &line, int &errorCode);
    bool Balance_Lift_Vertical_Rope(const QString &line, int &errorCode);
    bool Balance_Lift_Horizontal_Rope(const QString &line, int &errorCode);
    bool Steps(const QString &line, int &errorCode);
    bool End_Steps(const QString &line, int &errorCode);
    bool Tall_Reverse_L_Pipe(const QString &line, int &errorCode);
    bool Pipe_Wall(const QString &line, int &errorCode);
    bool Nothing(const QString &line, int &errorCode);

private:
    void Populate_Scenery_Map();
    void Populate_Brick_Map();
    void Populate_Background_Map();

    QMap<QString, Scenery::Scenery> *sceneries;
    QMap<QString, Brick::Brick> *bricks;
    QMap<QString, Background::Background> *backgrounds;
};

#endif // OBJECT_HANDLER_H
