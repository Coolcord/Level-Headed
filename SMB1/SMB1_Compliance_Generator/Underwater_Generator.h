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
    bool Brick_Pattern_Distraction(int x);
    bool Corral(int x);
};

#endif // UNDERWATER_GENERATOR_H
