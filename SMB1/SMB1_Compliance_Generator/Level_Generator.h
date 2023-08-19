#ifndef LEVEL_GENERATOR_H
#define LEVEL_GENERATOR_H

#include "../Common_SMB1_Files/Level_Attribute.h"
#include "SMB1_Compliance_Generator_Arguments.h"
#include "Header_Writer.h"
#include "Object_Buffer.h"
#include "Enemy_Buffer.h"
#include "Pipe_Pointer_Buffer.h"
#include "Required_Enemy_Spawns.h"
#include "Enemy_Spawner.h"
#include "End_Spawner.h"
#include "Level_Crawler.h"
#include "Midpoint_Handler.h"
#include "Powerup_Distributor.h"
#include "First_Page_Handler.h"
#include <QFile>
#include <QTextStream>

class Continuous_Enemies_Spawner;

class Level_Generator {
public:
    Level_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args);
    virtual ~Level_Generator();
    virtual bool Generate_Level()=0;

protected:
    int Get_Random_X();
    int Get_Random_X(int min, bool firstPage = false);
    int Get_Safe_Random_X();
    void Handle_Auto_Scroll_Start(int &x);
    bool Write_Buffers_To_File();
    void Remove_Unsafe_Cannons_From_Level();

    Level_Generator(const Level_Generator&);
    Level_Generator& operator=(const Level_Generator&);
    QFile *file;
    Continuous_Enemies_Spawner *continuousEnemiesSpawner;
    Header_Writer *header;
    Object_Buffer *objects;
    Enemy_Buffer *enemies;
    Pipe_Pointer_Buffer *pipePointers;
    Required_Enemy_Spawns *requiredEnemySpawns;
    Enemy_Spawner *enemySpawner;
    End_Spawner *end;
    Level_Crawler *levelCrawler;
    Midpoint_Handler *midpointHandler;
    Powerup_Distributor *powerupDistributor;
    First_Page_Handler *firstPageHandler;
    SMB1_Compliance_Generator_Arguments *args;
};

#endif // LEVEL_GENERATOR_H
