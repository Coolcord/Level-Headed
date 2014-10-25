#ifndef BRIDGE_GENERATOR_H
#define BRIDGE_GENERATOR_H

#include "Level_Generator.h"

class Bridge_Generator : public Level_Generator
{
public:
    Bridge_Generator(QFile *file, int numObjectBytes, int numEnemyBytes) : Level_Generator(file, numObjectBytes, numEnemyBytes) {}
    ~Bridge_Generator() {}
    bool Generate_Level();
private:
    Bridge_Generator(const Bridge_Generator&);
    Bridge_Generator& operator=(const Bridge_Generator&);
};

#endif // BRIDGE_GENERATOR_H
