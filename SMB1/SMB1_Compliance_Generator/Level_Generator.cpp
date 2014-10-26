#include "Level_Generator.h"
#include <assert.h>

Level_Generator::Level_Generator(QFile *file, const SMB1_Compliance_Generator_Arguments &args) {
    assert(file);
    this->header = new Header_Writer(file);
    this->stream = new QTextStream(file);
    this->object = new Object_Writer(this->stream, args.numObjectBytes);
    this->enemy = new Enemy_Writer(this->stream, args.numEnemyBytes);
    this->pipePointer = new Pipe_Pointer_Writer(this->object, this->enemy);
    this->enemySpawner = new Enemy_Spawner(file, this->stream, this->enemy);
    this->simpleObjectSpawner = new Simple_Object_Spawner(this->object);
    this->commonPatternSpawner = new Common_Pattern_Spawner(this->object);
    this->end = new End_Spawner(this->object);
    this->midpointHandler = new Midpoint_Handler(this->object);
    this->firstPageHandler = new First_Page_Handler(this->object, args.headerBackground, args.startCastle);
}

Level_Generator::~Level_Generator() {
    delete this->stream;
    delete this->header;
    delete this->object;
    delete this->enemy;
    delete this->pipePointer;
    delete this->enemySpawner;
    delete this->simpleObjectSpawner;
    delete this->commonPatternSpawner;
    delete this->end;
    delete this->midpointHandler;
    delete this->firstPageHandler;
}

int Level_Generator::Get_Random_X() {
    //Aim for a lower value... but allow higher values to be possible
    switch (qrand() % 4) {
    case 0:     return (qrand() % 0x08);
    case 1:     return (qrand() % 0x0A);
    case 2:     return (qrand() % 0x0D);
    case 3:     return (qrand() % 0x10);
    default:    assert(false); return 0;
    }
}

int Level_Generator::Get_Random_X(int min) {
    int x = this->Get_Random_X();
    x += min;
    if (x > 0x10) x = 0x10;
    return x;
}

int Level_Generator::Get_Safe_Random_X() {
    return this->Get_Random_X(this->object->Get_Last_Object_Length());
}
