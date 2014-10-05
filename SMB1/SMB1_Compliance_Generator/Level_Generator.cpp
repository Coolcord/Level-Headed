#include "Level_Generator.h"
#include <assert.h>

Level_Generator::Level_Generator(QFile *file, int numObjectBytes, int numEnemyBytes) {
    assert(file);
    this->header = new Header_Writer(file);
    this->stream = new QTextStream(file);
    this->object = new Object_Writer(this->stream, numObjectBytes);
    this->enemy = new Enemy_Writer(this->stream, numEnemyBytes);
    this->pipePointer = new Pipe_Pointer_Writer(this->object, this->enemy);
    this->enemySpawner = new Enemy_Spawner(file, this->stream, this->enemy, numEnemyBytes);
    this->simpleObjectSpawner = new Simple_Object_Spawner(this->object);
    this->commonPatternSpawner = new Common_Pattern_Spawner(this->object);
    this->end = new End_Spawner(this->object);
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
}
