#ifndef STANDARD_OVERWORLD_GENERATOR_H
#define STANDARD_OVERWORLD_GENERATOR_H

#include "Level_Generator.h"

class Standard_Overworld_Generator : public Level_Generator
{
public:
    Standard_Overworld_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args) : Level_Generator(file, args) {}
    ~Standard_Overworld_Generator() {}
    bool Generate_Level();
private:
    Standard_Overworld_Generator(const Standard_Overworld_Generator&);
    Standard_Overworld_Generator& operator=(const Standard_Overworld_Generator&);
};

#endif // STANDARD_OVERWORLD_GENERATOR_H
