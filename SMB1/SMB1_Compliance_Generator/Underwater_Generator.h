#ifndef UNDERWATER_GENERATOR_H
#define UNDERWATER_GENERATOR_H

#include "Level_Generator.h"

class Underwater_Generator : public Level_Generator
{
public:
    Underwater_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args) : Level_Generator(file, args) {}
    ~Underwater_Generator() {}
    bool Generate_Level();
private:
    Underwater_Generator(const Underwater_Generator&);
    Underwater_Generator& operator=(const Underwater_Generator&);
    int Get_Underwater_X(int min);
    bool Spawn_Intro(int &x);
    bool Brick_Pattern_Distraction(int x);
    bool Corral(int x);
    bool Corral_Series(int x);
    bool Corral_On_Blocks(int x);
    bool Hole(int x);
};

#endif // UNDERWATER_GENERATOR_H
