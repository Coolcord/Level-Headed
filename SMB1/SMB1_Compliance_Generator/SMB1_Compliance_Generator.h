#ifndef SMB1_COMPLIANCE_GENERATOR_H
#define SMB1_COMPLIANCE_GENERATOR_H

#include "SMB1_Compliance_Generator_Interface.h"
#include <QFile>
#include <QTextStream>

class SMB1_Compliance_Generator : public SMB1_Compliance_Generator_Interface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "SMB1_Compliance_Generator_Interface")
    Q_INTERFACES(SMB1_Compliance_Generator_Interface)

public:
    SMB1_Compliance_Generator();
    bool Generate_Level(const QString &fileName, int numObjectBytes, int numEnemyBytes, Level_Type::Level_Type type);

private:
    SMB1_Compliance_Generator(const SMB1_Compliance_Generator&);
    SMB1_Compliance_Generator& operator=(const SMB1_Compliance_Generator&);
    bool Generate_Standard_Overworld_Level(QFile *file, int numObjectBytes, int numEnemyBytes);
};

#endif // SMB1_COMPLIANCE_GENERATOR_H
