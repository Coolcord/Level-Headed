#ifndef HEADER_WRITER_H
#define HEADER_WRITER_H

#include "../Common SMB1 Files/Level_Attribute.h"
#include "../Common SMB1 Files/Level_Compliment.h"
#include "../Common SMB1 Files/Level_Type.h"
#include <QFile>

class Header_Writer
{
public:
    Header_Writer(QFile *file);
    bool Write_Header(Level_Type::Level_Type type, Level_Attribute::Level_Attribute attribute, Level_Compliment::Level_Compliment compliment,
                      int time, int halfwayPoint, int levelLength, int numObjects, int numEnemies, int numPipePointers);

private:
    QFile *file;
    bool written;
};

#endif // HEADER_WRITER_H
