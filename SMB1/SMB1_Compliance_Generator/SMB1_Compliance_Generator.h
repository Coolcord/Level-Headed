#ifndef SMB1_COMPLIANCE_GENERATOR_H
#define SMB1_COMPLIANCE_GENERATOR_H

#include "Level_Type.h"
#include <QString>
#include <QFile>

class SMB1_Compliance_Generator
{

public:
    SMB1_Compliance_Generator();
    bool Generate_Level(const QString &fileName, int numObjects, int numEnemies, Level_Type type);

private:
    QFile *file;
};

#endif // SMB1_COMPLIANCE_GENERATOR_H
