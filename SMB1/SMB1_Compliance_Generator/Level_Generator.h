#ifndef LEVEL_GENERATOR_H
#define LEVEL_GENERATOR_H

#include "../Common SMB1 Files/Level_Attribute.h"
#include "SMB1_Compliance_Generator_Arguments.h"
#include "Header_Writer.h"
#include "Object_Writer.h"
#include "Enemy_Writer.h"
#include "Pipe_Pointer_Writer.h"
#include "Enemy_Spawner.h"
#include "Simple_Object_Spawner.h"
#include "Common_Pattern_Spawner.h"
#include "End_Spawner.h"
#include "Midpoint_Handler.h"
#include "First_Page_Handler.h"
#include <QFile>
#include <QTextStream>

class Level_Generator
{
public:
    Level_Generator(QFile *file, const SMB1_Compliance_Generator_Arguments &args);
    ~Level_Generator();
    virtual bool Generate_Level()=0;

protected:
    int Get_Random_X();
    int Get_Random_X(int min);
    int Get_Safe_Random_X();

    Level_Generator(const Level_Generator&);
    Level_Generator& operator=(const Level_Generator&);
    QTextStream *stream;
    Header_Writer *header;
    Object_Writer *object;
    Enemy_Writer *enemy;
    Pipe_Pointer_Writer *pipePointer;
    Enemy_Spawner *enemySpawner;
    Simple_Object_Spawner *simpleObjectSpawner;
    Common_Pattern_Spawner *commonPatternSpawner;
    End_Spawner *end;
    Midpoint_Handler *midpointHandler;
    First_Page_Handler *firstPageHandler;
};

#endif // LEVEL_GENERATOR_H
