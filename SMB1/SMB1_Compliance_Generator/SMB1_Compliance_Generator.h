#ifndef SMB1_COMPLIANCE_GENERATOR_H
#define SMB1_COMPLIANCE_GENERATOR_H

#include "SMB1_Compliance_Generator_Interface.h"
#include <QFile>
#include <QTextStream>
#include <QString>

class SMB1_Compliance_Generator : public SMB1_Compliance_Generator_Interface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "SMB1_Compliance_Generator_Interface")
    Q_INTERFACES(SMB1_Compliance_Generator_Interface)

public:
    SMB1_Compliance_Generator();
    void Startup(QWidget *parent, const QString &location);
    void Shutdown();
    void Seed_Random_Number_Generator_Instance(const QString &seed);
    bool Generate_Level(SMB1_Compliance_Generator_Arguments &args);

    //Level Script Functions
    bool Parse_Level_Script(SMB1_Compliance_Parser_Arguments &args);
    bool Perform_Enemy_Chaotic_Swap(Enemy_Buffer *enemyBuffer, Level_Attribute::Level_Attribute levelAttribute, bool allowHammerBros, bool allowLakitus, bool allowContinousEnemySpawners);
    bool Redistribute_Enemies(SMB1_Compliance_Generator_Arguments &args, SMB1_Compliance_Parser_Arguments &parserArgs);
    bool Redistribute_Powerups(SMB1_Compliance_Generator_Arguments &args, SMB1_Compliance_Parser_Arguments &parserArgs);

    //Argument Helpers
    SMB1_Compliance_Generator_Arguments Get_Empty_SMB1_Compliance_Generator_Arguments();
    SMB1_Compliance_Parser_Arguments Get_Empty_SMB1_Compliance_Parser_Arguments();

private:
    SMB1_Compliance_Generator(const SMB1_Compliance_Generator&);
    SMB1_Compliance_Generator& operator=(const SMB1_Compliance_Generator&);
    bool Generate_Standard_Overworld_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args);
    bool Generate_Underground_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args);
    bool Generate_Underwater_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args);
    bool Generate_Castle_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args);
    bool Generate_Bridge_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args);
    bool Generate_Island_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args);
    bool Generate_Pipe_Exit_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args);
    bool Generate_Pipe_Intro_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args);
    bool Generate_Underground_Bonus_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args);
    QString applicationLocation;
    QWidget *parent;
};

#endif // SMB1_COMPLIANCE_GENERATOR_H
