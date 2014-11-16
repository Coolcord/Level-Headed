#include "Level_Generator.h"
#include <assert.h>

Level_Generator::Level_Generator(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    assert(file);
    assert(args);
    this->args = args;
    this->header = new Header_Writer(file);
    this->stream = new QTextStream(file);
    this->object = new Object_Writer(this->stream, this->args->numObjectBytes);
    this->enemy = new Enemy_Writer(this->stream, this->args->numEnemyBytes);
    this->pipePointer = new Pipe_Pointer_Writer(this->object, this->enemy);
    this->requiredEnemySpawns = new Required_Enemy_Spawns(this->object, this->enemy, this->pipePointer);
    this->enemySpawner = new Enemy_Spawner(file, this->stream, this->enemy, this->requiredEnemySpawns, this->args->levelType);
    this->end = new End_Spawner(this->object, this->args);
    this->midpointHandler = new Midpoint_Handler(this->object, this->args->levelType);
    this->firstPageHandler = new First_Page_Handler(this->object, this->args->headerBackground, this->args->startCastle);
}

Level_Generator::~Level_Generator() {
    delete this->stream;
    delete this->header;
    delete this->object;
    delete this->enemy;
    delete this->pipePointer;
    delete this->enemySpawner;
    delete this->requiredEnemySpawns;
    delete this->end;
    delete this->midpointHandler;
    delete this->firstPageHandler;
}

int Level_Generator::Get_Random_X() {
    //Aim for a lower value... but allow higher values to be possible
    int random = qrand()%20;
    if (random < 10) {
        return (qrand() % 0x06);
    } else if (random < 16) {
        return (qrand() % 0x08);
    } else if (random < 19) {
        return (qrand() % 0x0A);
    } else if (random < 20) {
        return (qrand() % 0x10);
    } else {
        assert(false);
        return 0;
    }
}

int Level_Generator::Get_Random_X(int min, bool firstPage) {
    int x = this->Get_Random_X();
    x += min;
    if (!firstPage && x > 0x10) x = 0x10;
    else if (firstPage && x > 0x1F) x = 0x1F;
    return x;
}

int Level_Generator::Get_Safe_Random_X() {
    return this->Get_Random_X(this->object->Get_Last_Object_Length());
}
