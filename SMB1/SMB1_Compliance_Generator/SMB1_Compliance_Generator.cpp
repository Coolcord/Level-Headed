#include "SMB1_Compliance_Generator.h"
#include "Standard_Overworld_Generator.h"
#include <assert.h>
#include <QTime>

SMB1_Compliance_Generator::SMB1_Compliance_Generator() {
    this->applicationLocation = QString();

    //Seed the random number generator
    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));
}

void SMB1_Compliance_Generator::Set_Application_Directory(QString location) {
    this->applicationLocation = location;
}

bool SMB1_Compliance_Generator::Generate_Level(const QString &fileName, int numObjectBytes, int numEnemyBytes, Level_Type::Level_Type type) {
    if (this->applicationLocation.isEmpty()) return false;

    //Create a new file for writing
    QFile file(fileName);
    if (!file.open(QFile::ReadWrite | QFile::Truncate)) {
        return false;
    }

    //Determine which level generator to use
    switch (type) {
    case Level_Type::STANDARD_OVERWORLD:    return this->Generate_Standard_Overworld_Level(&file, numObjectBytes, numEnemyBytes);
    case Level_Type::ISLAND:                return false; //TODO: Implement this...
    default:                    return false;
    }
}

bool SMB1_Compliance_Generator::Generate_Standard_Overworld_Level(QFile *file, int numObjectBytes, int numEnemyBytes) {
    Standard_Overworld_Generator levelGenerator(file, numObjectBytes, numEnemyBytes);
    return levelGenerator.Generate_Level();
}
