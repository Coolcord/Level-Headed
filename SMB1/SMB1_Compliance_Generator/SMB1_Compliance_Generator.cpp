#include "SMB1_Compliance_Generator.h"
#include "Standard_Overworld_Generator.h"
#include "Underground_Generator.h"
#include "Underwater_Generator.h"
#include "Castle_Generator.h"
#include "Bridge_Generator.h"
#include "Island_Generator.h"
#include "Pipe_Exit_Generator.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include <assert.h>
#include <QDebug>
#include <QTime>

SMB1_Compliance_Generator::SMB1_Compliance_Generator() {
    this->applicationLocation = QString();
}

void SMB1_Compliance_Generator::Startup(QWidget *parent, const QString &location) {
    this->parent = parent;
    this->applicationLocation = location;
}

void SMB1_Compliance_Generator::Shutdown() {

}

void SMB1_Compliance_Generator::Seed_Random_Number_Generator_Instance(const QString &seed) {
    Random::Get_Instance().Seed(seed, 3);
}

bool SMB1_Compliance_Generator::Generate_Level(SMB1_Compliance_Generator_Arguments &args) {
    if (this->applicationLocation.isEmpty()) return false;

    //Create a new file for writing
    QFile file(args.fileName);
    if (!file.open(QFile::ReadWrite | QFile::Truncate)) {
        return false;
    }

    //Determine which level generator to use
    bool success = false;
    switch (args.levelType) {
    default:                                assert(false); return false;
    case Level_Type::STANDARD_OVERWORLD:    success = this->Generate_Standard_Overworld_Level(&file, &args); break;
    case Level_Type::UNDERGROUND:           success = this->Generate_Underground_Level(&file, &args); break;
    case Level_Type::UNDERWATER:            success = this->Generate_Underwater_Level(&file, &args); break;
    case Level_Type::CASTLE:                success = this->Generate_Castle_Level(&file, &args); break;
    case Level_Type::BRIDGE:                success = this->Generate_Bridge_Level(&file, &args); break;
    case Level_Type::ISLAND:                success = this->Generate_Island_Level(&file, &args); break;
    case Level_Type::PIPE_EXIT:             success = this->Generate_Pipe_Exit_Level(&file, &args); break;
    }

    file.close();
    return success;
}

bool SMB1_Compliance_Generator::Generate_Standard_Overworld_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    Standard_Overworld_Generator levelGenerator(file, args);
    return levelGenerator.Generate_Level();
}

bool SMB1_Compliance_Generator::Generate_Underground_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    Underground_Generator levelGenerator(file, args);
    return levelGenerator.Generate_Level();
}

bool SMB1_Compliance_Generator::Generate_Underwater_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    Underwater_Generator levelGenerator(file, args);
    return levelGenerator.Generate_Level();
}

bool SMB1_Compliance_Generator::Generate_Castle_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    Castle_Generator levelGenerator(file, args);
    return levelGenerator.Generate_Level();
}

bool SMB1_Compliance_Generator::Generate_Bridge_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    Bridge_Generator levelGenerator(file, args);
    return levelGenerator.Generate_Level();
}

bool SMB1_Compliance_Generator::Generate_Island_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    Island_Generator levelGenerator(file, args);
    return levelGenerator.Generate_Level();
}

bool SMB1_Compliance_Generator::Generate_Pipe_Exit_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    Pipe_Exit_Generator pipeExitGenerator(file, args);
    return pipeExitGenerator.Generate_Level();
}
