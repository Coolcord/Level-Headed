#include "SMB1_Compliance_Generator.h"
#include "Standard_Overworld_Generator.h"
#include "Bridge_Generator.h"
#include <assert.h>
#include <QTime>

SMB1_Compliance_Generator::SMB1_Compliance_Generator() {
    this->applicationLocation = QString();
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
}

void SMB1_Compliance_Generator::Startup(QWidget *parent, QString location) {
    assert(parent);
    this->parent = parent;
    this->applicationLocation = location;
}

void SMB1_Compliance_Generator::Shutdown() {

}

bool SMB1_Compliance_Generator::Generate_Level(SMB1_Compliance_Generator_Arguments args) {
    if (this->applicationLocation.isEmpty()) return false;

    //Create a new file for writing
    QFile file(args.fileName);
    if (!file.open(QFile::ReadWrite | QFile::Truncate)) {
        return false;
    }

    //Determine which level generator to use
    bool success = false;
    switch (args.levelType) {
    case Level_Type::STANDARD_OVERWORLD:
        success = this->Generate_Standard_Overworld_Level(&file, args);
        break;
    case Level_Type::UNDERGROUND:
        success = false;
        break; //TODO: Implement this...
    case Level_Type::UNDERWATER:
        success = false;
        break; //TODO: Implement this...
    case Level_Type::CASTLE:
        success = false;
        break; //TODO: Implement this...
    case Level_Type::BRIDGE:
        success = this->Generate_Bridge_Level(&file, args);
        break;
    case Level_Type::ISLAND:
        success = false;
        break; //TODO: Implement this...
    default:
        assert(false);
    }

    file.close();
    return success;
}

bool SMB1_Compliance_Generator::Generate_Standard_Overworld_Level(QFile *file, const SMB1_Compliance_Generator_Arguments &args) {
    Standard_Overworld_Generator levelGenerator(file, args);
    return levelGenerator.Generate_Level();
}

bool SMB1_Compliance_Generator::Generate_Bridge_Level(QFile *file, const SMB1_Compliance_Generator_Arguments &args) {
    Bridge_Generator levelGenerator(file, args);
    return levelGenerator.Generate_Level();
}
