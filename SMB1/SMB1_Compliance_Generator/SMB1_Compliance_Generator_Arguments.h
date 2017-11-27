#ifndef SMB1_COMPLIANCE_GENERATOR_ARGUMENTS_H
#define SMB1_COMPLIANCE_GENERATOR_ARGUMENTS_H

#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Scenery.h"
#include "../Common_SMB1_Files/Level_Compliment.h"
#include "../Common_SMB1_Files/Castle.h"
#include "../Common_SMB1_Files/Level_Type.h"
#include <QString>

struct SMB1_Compliance_Generator_Arguments {
    QString fileName;
    int numObjectBytes;
    int numEnemyBytes;
    Background::Background headerBackground;
    Scenery::Scenery headerScenery;
    Level_Compliment::Level_Compliment levelCompliment;
    Castle::Castle startCastle;
    Castle::Castle endCastle;
    Level_Type::Level_Type levelType;
};

#endif // SMB1_COMPLIANCE_GENERATOR_ARGUMENTS_H
