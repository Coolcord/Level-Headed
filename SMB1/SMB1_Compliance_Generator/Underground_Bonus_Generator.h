#ifndef UNDERGROUND_BONUS_GENERATOR_H
#define UNDERGROUND_BONUS_GENERATOR_H

#include "Level_Generator.h"

class Underground_Bonus_Generator : public Level_Generator {
public:
    Underground_Bonus_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args) : Level_Generator(file, args) {}
    bool Generate_Level();
};

#endif // UNDERGROUND_BONUS_GENERATOR_H
