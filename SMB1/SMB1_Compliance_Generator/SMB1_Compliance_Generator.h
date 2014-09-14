#ifndef SMB1_COMPLIANCE_GENERATOR_H
#define SMB1_COMPLIANCE_GENERATOR_H

#include "../Common SMB1 Files/Level_Type.h"
#include <QObject>
#include <QtPlugin>
#include <QString>
#include <QFile>
#include <QTextStream>

class SMB1_Compliance_Generator : public QObject
{
    Q_PLUGIN_METADATA(IID  "SMB1_Compliance_Generator" FILE "SMB1_Compliance_Generator.json")
    Q_INTERFACES(SMB1_Compliance_Generator)

public:
    SMB1_Compliance_Generator();
    bool Generate_Level(const QString &fileName, int numObjectBytes, int numEnemyBytes, Level_Type::Level_Type type);

private:
    bool Generate_Standard_Overworld_Level(QFile *file, int numObjectBytes, int numEnemyBytes);
};

Q_DECLARE_INTERFACE(SMB1_Compliance_Generator, "SMB1_Compliance_Generator")

#endif // SMB1_COMPLIANCE_GENERATOR_H
