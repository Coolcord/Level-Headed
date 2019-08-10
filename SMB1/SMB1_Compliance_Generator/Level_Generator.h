#ifndef LEVEL_GENERATOR_H
#define LEVEL_GENERATOR_H

#include "../Common_SMB1_Files/Level_Attribute.h"
#include "SMB1_Compliance_Generator_Arguments.h"
#include "Header_Writer.h"
#include "Object_Writer.h"
#include "Enemy_Writer.h"
#include "Pipe_Pointer_Writer.h"
#include "Required_Enemy_Spawns.h"
#include "Enemy_Spawner.h"
#include "End_Spawner.h"
#include "First_Page_Handler.h"
#include <QFile>
#include <QTextStream>

class Continuous_Enemies_Spawner;

class Level_Generator
{
public:
    Level_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args);
    ~Level_Generator();
    virtual bool Generate_Level()=0;

protected:
    int Get_Random_X();
    int Get_Random_X(int min, bool firstPage = false);
    int Get_Safe_Random_X();
    void Handle_Auto_Scroll_Start(int &x);

    Level_Generator(const Level_Generator&);
    Level_Generator& operator=(const Level_Generator&);
    QTextStream *stream;
    Continuous_Enemies_Spawner *continuousEnemiesSpawner;
    Header_Writer *header;
    Object_Writer *object;
    Enemy_Writer *enemy;
    Pipe_Pointer_Writer *pipePointer;
    Required_Enemy_Spawns *requiredEnemySpawns;
    Enemy_Spawner *enemySpawner;
    End_Spawner *end;
    First_Page_Handler *firstPageHandler;
    SMB1_Compliance_Generator_Arguments *args;
};

#endif // LEVEL_GENERATOR_H
