#ifndef ENEMY_HANDLER_H
#define ENEMY_HANDLER_H

#include "Item_Handler.h"

class Enemy_Handler : public Item_Handler
{
public:
    Enemy_Handler(SMB1_Writer_Interface *writerPlugin) : Item_Handler(writerPlugin) {}

    bool Green_Koopa(const QString &line);
    bool Red_Koopa(const QString &line);
    bool Buzzy_Beetle(const QString &line);
    bool Hammer_Bro(const QString &line);
    bool Goomba(const QString &line);
    bool Blooper(const QString &line);
    bool Bullet_Bill(const QString &line);
    bool Green_Paratroopa(const QString &line);
    bool Red_Paratroopa(const QString &line);
    bool Green_Cheep_Cheep(const QString &line);
    bool Red_Cheep_Cheep(const QString &line);
    bool Podoboo(const QString &line);
    bool Pirana_Plant(const QString &line);
    bool Lakitu(const QString &line);
    bool Spiny(const QString &line);
    bool Bowser_Fire_Spawner(const QString &line);
    bool Cheep_Cheep_Spawner(const QString &line);
    bool Bullet_Bill_Spawner(const QString &line);
    bool Fire_Bar(const QString &line);
    bool Large_Fire_Bar(const QString &line);
    bool Lift(const QString &line);
    bool Falling_Lift(const QString &line);
    bool Balance_Lift(const QString &line);
    bool Surfing_Lift(const QString &line);
    bool Lift_Spawner(const QString &line);
    bool Bowser(const QString &line);
    bool Warp_Zone(const QString &line);
    bool Pipe_Pointer(const QString &line);
    bool Toad(const QString &line);
    bool Goomba_Group(const QString &line);
    bool Koopa_Group(const QString &line);
    bool Page_Change(const QString &line);
    bool Nothing(const QString &line);

private:
    bool Parse_Difficulty(const QString &value, bool &onlyHardMode);
    SMB1_Writer_Interface *writerPlugin;
};

#endif // ENEMY_HANDLER_H
