#ifndef BRIDGE_GENERATOR_H
#define BRIDGE_GENERATOR_H

#include "Level_Generator.h"

class Bridge_Generator : public Level_Generator
{
public:
    Bridge_Generator(QFile *file, const SMB1_Compliance_Generator_Arguments &args) : Level_Generator(file, args) {}
    ~Bridge_Generator() {}
    bool Generate_Level();
private:
    Bridge_Generator(const Bridge_Generator&);
    Bridge_Generator& operator=(const Bridge_Generator&);
    int Get_Y_From_Height(int height);
    int Get_Height_From_Y(int y);
    int Get_Bridge_Y();
    int Get_Bridge_Length();
    int Get_Safe_Jump_Distance(int min = 0);
    bool Spawn_Intro(int &x);
    bool Spawn_Simple_Bridge(int x, int y = 0, bool ignoreFirstSupport = false);
    bool Spawn_Multi_Bridge(int x, int y = 0, bool ignoreFirstSupport = false);
    bool Spawn_Lone_Bridge(int x, int y = 0, int length = 0);
    bool Spawn_Lone_Bridge_Series(int x);
};

#endif // BRIDGE_GENERATOR_H
