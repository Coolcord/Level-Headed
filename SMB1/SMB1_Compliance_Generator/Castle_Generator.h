#ifndef CASTLE_GENERATOR_H
#define CASTLE_GENERATOR_H

#include "Level_Generator.h"

class Castle_Generator : public Level_Generator
{
public:
    Castle_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args) : Level_Generator(file, args) {}
    ~Castle_Generator() {}
    bool Generate_Level();
private:
    Castle_Generator(const Castle_Generator&);
    Castle_Generator& operator=(const Castle_Generator&);
    Brick::Brick brick;
    int Get_Lowest_Y_From_Brick(Brick::Brick brick);
    bool Spawn_Intro(int &x);
    bool Room_With_Single_Firebar_Pillar(int x);
    bool Drop_Down_And_Climb_Up_U_Shape(int x);
    bool Two_Object_Hole(int x);
};

#endif // CASTLE_GENERATOR_H
