#ifndef CASTLE_GENERATOR_H
#define CASTLE_GENERATOR_H

#include "Level_Generator.h"
#include "Item_Spawner.h"

class Castle_Generator : public Level_Generator {
public:
    Castle_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args);
    ~Castle_Generator();
    bool Generate_Level();

private:
    Castle_Generator(const Castle_Generator&);
    Castle_Generator& operator=(const Castle_Generator&);
    Brick::Brick brick;
    Item_Spawner *itemSpawner;
    bool bowserFireReadyToSpawn;
    bool bowserFireSpawned;
    int Get_Lowest_Y_From_Brick(Brick::Brick brick);
    int Get_Random_Y();
    bool Spawn_Firebar(int x, int y);

    bool Spawn_Intro(int &x);
    void Handle_Bowser_Fire();
    bool Room_With_Single_Firebar_Pillar(int x);
    bool Room_With_Platforms_And_Firebars(int x);
    bool Drop_Down_And_Climb_Up_U_Shape(int x);
    bool Two_Object_Hole(int x);
    bool Coin_Tease(int x);
    bool Item_Tease(int x);
    bool Defense_Hole(int x);
};

#endif // CASTLE_GENERATOR_H
