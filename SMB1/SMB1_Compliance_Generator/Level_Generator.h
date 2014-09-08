#ifndef LEVEL_GENERATOR_H
#define LEVEL_GENERATOR_H

#include "../Common SMB1 Files/Level_Attribute.h"
#include "../Common SMB1 Files/Level_Compliment.h"
#include "../Common SMB1 Files/Level_Type.h"
#include "Header_Writer.h"
#include "Object_Writer.h"
#include "Enemy_Writer.h"
#include <QFile>
#include <QTextStream>

class Level_Generator
{
public:
    Level_Generator(QFile *file, int numObjectBytes, int numEnemyBytes);
    ~Level_Generator();
    virtual bool Generate_Level()=0;

protected:
    QTextStream *stream;
    Header_Writer *header;
    Object_Writer *object;
    Enemy_Writer *enemy;
};

#endif // LEVEL_GENERATOR_H
