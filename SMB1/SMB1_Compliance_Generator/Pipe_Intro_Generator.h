#ifndef PIPE_INTRO_GENERATOR_H
#define PIPE_INTRO_GENERATOR_H

#include "Level_Generator.h"

class Pipe_Intro_Generator : public Level_Generator {
public:
    Pipe_Intro_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args) : Level_Generator(file, args) {}
    bool Generate_Level();
};

#endif // PIPE_INTRO_GENERATOR_H
