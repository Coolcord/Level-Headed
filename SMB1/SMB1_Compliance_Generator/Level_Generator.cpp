#include "Level_Generator.h"
#include <assert.h>

Level_Generator::Level_Generator(QFile *file, int numObjectBytes, int numEnemyBytes)
{
    assert(file);
    this->header = new Header_Writer(file);
    this->stream = new QTextStream(file);
    this->object = new Object_Writer(this->stream, numObjectBytes);
    this->enemy = new Enemy_Writer(this->stream, numEnemyBytes);
}

Level_Generator::~Level_Generator() {
    delete this->header;
    delete this->object;
    delete this->enemy;
    delete this->stream;
}
