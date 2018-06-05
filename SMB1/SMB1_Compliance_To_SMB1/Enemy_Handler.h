#ifndef ENEMY_HANDLER_H
#define ENEMY_HANDLER_H

#include "Item_Handler.h"

class Enemy_Handler : public Item_Handler
{
public:
    Enemy_Handler(SMB1_Writer_Interface *writerPlugin);

    bool Green_Koopa(const QString &line, int &errorCode);
    bool Red_Koopa(const QString &line, int &errorCode);
    bool Buzzy_Beetle(const QString &line, int &errorCode);
    bool Hammer_Bro(const QString &line, int &errorCode);
    bool Goomba(const QString &line, int &errorCode);
    bool Blooper(const QString &line, int &errorCode);
    bool Bullet_Bill(const QString &line, int &errorCode);
    bool Green_Paratroopa(const QString &line, int &errorCode);
    bool Red_Paratroopa(const QString &line, int &errorCode);
    bool Green_Cheep_Cheep(const QString &line, int &errorCode);
    bool Red_Cheep_Cheep(const QString &line, int &errorCode);
    bool Podoboo(const QString &line, int &errorCode);
    bool Pirana_Plant(const QString &line, int &errorCode);
    bool Lakitu(const QString &line, int &errorCode);
    bool Spiny(const QString &line, int &errorCode);
    bool Bowser_Fire_Spawner(const QString &line, int &errorCode);
    bool Swimming_Cheep_Cheep_Spawner(const QString &line, int &errorCode);
    bool Bullet_Bill_Spawner(const QString &line, int &errorCode);
    bool Fire_Bar(const QString &line, int &errorCode);
    bool Large_Fire_Bar(const QString &line, int &errorCode);
    bool Lift(const QString &line, int &errorCode);
    bool Falling_Lift(const QString &line, int &errorCode);
    bool Balance_Lift(const QString &line, int &errorCode);
    bool Surfing_Lift(const QString &line, int &errorCode);
    bool Lift_Spawner(const QString &line, int &errorCode);
    bool Bowser(const QString &line, int &errorCode);
    bool Warp_Zone(const QString &line, int &errorCode);
    bool Pipe_Pointer(const QString &line, int &errorCode);
    bool Toad(const QString &line, int &errorCode);
    bool Goomba_Group(const QString &line, int &errorCode);
    bool Koopa_Group(const QString &line, int &errorCode);
    bool Page_Change(const QString &line, int &errorCode);

private:
    bool Parse_Difficulty(const QString &value, bool &onlyHardMode);
};

#endif // ENEMY_HANDLER_H
