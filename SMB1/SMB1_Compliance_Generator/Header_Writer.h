#ifndef HEADER_WRITER_H
#define HEADER_WRITER_H

#include "../../../C_Common_Code/Qt/Text_Insertion_Buffer/Text_Insertion_Buffer.h"
#include "../Common_SMB1_Files/Level_Attribute.h"
#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Scenery.h"
#include "../Common_SMB1_Files/Level_Compliment.h"
#include "../Common_SMB1_Files/Level_Type.h"
#include <QFile>

class Header_Writer {
public:
    Header_Writer();
    ~Header_Writer();
    bool Write_Header_To_Buffer(Level_Type::Level_Type type, Level_Attribute::Level_Attribute attribute, Brick::Brick brick, Background::Background background, Scenery::Scenery scenery, Level_Compliment::Level_Compliment compliment,
                      int time, int halfwayPoint, int difficulty, int levelLength, int numObjects, int numEnemies, int numPipePointers);
    bool Write_Buffer_To_File(QFile *file);

private:
    Header_Writer(const Header_Writer&);
    Header_Writer& operator=(const Header_Writer&);
    Text_Insertion_Buffer *buffer;
};

#endif // HEADER_WRITER_H
