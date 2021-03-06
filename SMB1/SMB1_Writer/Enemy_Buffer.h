#ifndef ENEMY_BUFFER_H
#define ENEMY_BUFFER_H

#include "Item_Buffer.h"
#include "../Common_SMB1_Files/Level.h"
#include <QMap>

class Enemy_Buffer : public Item_Buffer {
public:
    Enemy_Buffer(QByteArray *buffer, Header_Writer *headerWriter, Room_ID_Handler *roomIDHandler);
    ~Enemy_Buffer();
    void Populate_Level_Slots();
    bool Fill_Buffer();
    bool Random_Continous_Enemy_Spawner(int x, bool underwater, bool onlyHardMode);
    bool Random_Enemy(int x, int y, bool onlyHardMode, bool allowHammerBros, bool allowLakitus, bool allowContinousEnemySpawners);
    bool Random_Enemy_Group(int x, int y, int num, bool onlyHardMode);
    bool Random_Flying_Enemy(int x, bool onlyHardMode);
    bool Random_Fire_Bar(int x, int y, bool onlyHardMode);
    bool Random_Underwater_Enemy(int x, int y, bool onlyHardMode, bool allowHammerBros);

    bool Green_Koopa(int x, int y, bool moving = true, bool onlyHardMode = false);
    bool Red_Koopa(int x, int y, bool onlyHardMode = false);
    bool Buzzy_Beetle(int x, int y, bool onlyHardMode = false);
    bool Hammer_Bro(int x, int y, bool onlyHardMode = false);
    bool Goomba(int x, int y, bool onlyHardMode = false);
    bool Blooper(int x, int y, bool onlyHardMode = false);
    bool Bullet_Bill(int x, int y, bool onlyHardMode = false);
    bool Green_Paratroopa(int x, int y, bool moving = true, bool leaping = true, bool onlyHardMode = false);
    bool Red_Paratroopa(int x, int y, bool onlyHardMode = false);
    bool Green_Cheep_Cheep(int x, int y, bool onlyHardMode = false);
    bool Red_Cheep_Cheep(int x, int y, bool onlyHardMode = false);
    bool Podoboo(int x, bool onlyHardMode = false);
    bool Piranha_Plant(int x, int y, bool onlyHardMode = false);
    bool Lakitu(int x, int y, bool onlyHardMode = false);
    bool Spiny(int x, int y, bool onlyHardMode = false);
    bool Bowser_Fire_Spawner(int x, bool onlyHardMode = false);
    bool Swimming_Cheep_Cheep_Spawner(int x, bool leaping = false, bool onlyHardMode = false);
    bool Bullet_Bill_Spawner(int x, bool onlyHardMode = false);
    bool Fire_Bar(int x, int y, bool clockwise = true, bool fast = false, bool onlyHardMode = false);
    bool Large_Fire_Bar(int x, int y, bool onlyHardMode = false);
    bool Lift(int x, int y, bool vertical = true, bool onlyHardMode = false);
    bool Falling_Lift(int x, int y, bool onlyHardMode = false);
    bool Balance_Lift(int x, int y, bool onlyHardMode = false);
    bool Surfing_Lift(int x, int y, bool onlyHardMode = false);
    bool Lift_Spawner(int x, int y, bool up = true, bool small = false, bool onlyHardMode = false);
    bool Bowser(int x, bool onlyHardMode = false);
    bool Warp_Zone(int x);
    bool Toad(int x, bool onlyHardMode = false);
    bool Goomba_Group(int x, int y, int num, bool onlyHardMode = false);
    bool Koopa_Group(int x, int y, int num, bool onlyHardMode = false);
    bool Page_Change(int page);
    bool Pipe_Pointer(int x, int room, int world, int page);
    bool Pipe_Pointer(int x, const QString &levelSlot, int world, int page);

private:
    Enemy_Buffer(const Enemy_Buffer&);
    Enemy_Buffer& operator=(const Enemy_Buffer&);
    bool Write_Enemy(int x, int y, int enemyByte, bool onlyHardMode);
    bool Write_Group(int x, int y, int enemyByte, bool onlyHardMode);
    int Handle_Group_Page_Flag(int x);
    int Get_Random_Air_Y();

    QMap<QString, Level::Level> *levelSlots;
    bool groupPageFlag;
};

#endif // ENEMY_BUFFER_H
