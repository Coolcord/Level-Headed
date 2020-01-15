#ifndef ISLAND_GENERATOR_H
#define ISLAND_GENERATOR_H

#include "Level_Generator.h"

class Item_Spawner;

class Island_Generator : public Level_Generator {
public:
    Island_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args);
    ~Island_Generator();
    bool Generate_Level();
private:
    Island_Generator(const Island_Generator&);
    Island_Generator& operator=(const Island_Generator&);
    int Get_Island_Length(int min = 3);
    int Get_Safe_Jump_Distance(int min);
    int Get_Island_Y();
    bool Spawn_Intro(int &x);
    bool Spawn_Basic_Island(int x);
    bool Spawn_Two_Islands(int x);

    Item_Spawner *itemSpawner;
};

#endif // ISLAND_GENERATOR_H
