#ifndef SMB1_COMPLIANCE_GENERATOR_H
#define SMB1_COMPLIANCE_GENERATOR_H

#include "../Common SMB1 Files/Level_Type.h"
#include <QString>
#include <QFile>
#include <QTextStream>

class SMB1_Compliance_Generator
{
public:
    SMB1_Compliance_Generator();
    bool Generate_Level(const QString &fileName, int numObjectBytes, int numEnemyBytes, Level_Type::Level_Type type);

private:
    bool Generate_Standard_Overworld_Level(QFile *file, int numObjectBytes, int numEnemyBytes);
};

#endif // SMB1_COMPLIANCE_GENERATOR_H
