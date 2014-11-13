#ifndef UNDERGROUND_GENERATOR_H
#define UNDERGROUND_GENERATOR_H

#include "Level_Generator.h"

class Simple_Object_Spawner;
class Common_Pattern_Spawner;

class Underground_Generator : public Level_Generator
{
public:
    Underground_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args) : Level_Generator(file, args) {}
    ~Underground_Generator();
    bool Generate_Level();
private:
    Underground_Generator(const Underground_Generator&);
    Underground_Generator& operator=(const Underground_Generator&);
    bool Spawn_Intro(int &x);
    bool Brick_Pattern_Distraction(int x);

    Simple_Object_Spawner *simpleObjectSpawner;
    Common_Pattern_Spawner *commonPatternSpawner;
};

#endif // UNDERGROUND_GENERATOR_H
