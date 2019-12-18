#ifndef STANDARD_OVERWORLD_GENERATOR_H
#define STANDARD_OVERWORLD_GENERATOR_H

#include "Level_Generator.h"

class Simple_Object_Spawner;
class Common_Pattern_Spawner;

class Standard_Overworld_Generator : public Level_Generator {
public:
    Standard_Overworld_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args) : Level_Generator(file, args) {}
    ~Standard_Overworld_Generator();
    bool Generate_Level();
private:
    void Spawn_Intro(int &x);

    Standard_Overworld_Generator(const Standard_Overworld_Generator&);
    Standard_Overworld_Generator& operator=(const Standard_Overworld_Generator&);
    Simple_Object_Spawner *simpleObjectSpawner;
    Common_Pattern_Spawner *commonPatternSpawner;
};

#endif // STANDARD_OVERWORLD_GENERATOR_H
