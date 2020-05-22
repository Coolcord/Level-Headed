#ifndef SMB1_COMPLIANCE_PARSER_ARGUMENTS_H
#define SMB1_COMPLIANCE_PARSER_ARGUMENTS_H

#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Level_Attribute.h"
#include "../Common_SMB1_Files/Level_Compliment.h"
#include "../Common_SMB1_Files/Scenery.h"
#include "Enemy_Buffer.h"
#include "Object_Buffer.h"
#include <QByteArray>

struct SMB1_Compliance_Parser_Arguments {
    QByteArray levelScriptBytes;
    Background::Background headerBackground;
    Brick::Brick headerBrick;
    Scenery::Scenery headerScenery;
    Level_Attribute::Level_Attribute levelAttribute;
    Level_Compliment::Level_Compliment levelCompliment;
    Level_Attribute::Level_Attribute staringPosition;
    bool autowalk;
    int time;
    int midpoint;
    Enemy_Buffer *enemyBuffer;
    Object_Buffer *objectBuffer;
    int lineNum;
};

#endif // SMB1_COMPLIANCE_PARSER_ARGUMENTS_H
