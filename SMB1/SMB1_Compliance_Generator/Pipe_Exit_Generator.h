#ifndef PIPE_EXIT_GENERATOR_H
#define PIPE_EXIT_GENERATOR_H

#include "Level_Generator.h"

class Pipe_Exit_Generator : public Level_Generator {
public:
    Pipe_Exit_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args) : Level_Generator(file, args) {}
    bool Generate_Level();
};

#endif // PIPE_EXIT_GENERATOR_H
