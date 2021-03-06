#ifndef SMB1_COMPLIANCE_GENERATOR_INTERFACE_H
#define SMB1_COMPLIANCE_GENERATOR_INTERFACE_H

#include "SMB1_Compliance_Generator_Arguments.h"
#include "SMB1_Compliance_Parser_Arguments.h"
#include <QObject>
#include <QtPlugin>
#include <QString>
#include <QWidget>

class SMB1_Compliance_Generator_Interface : public QObject {
public:
    virtual void Startup(QWidget *parent, const QString &location)=0;
    virtual void Shutdown()=0;
    virtual void Seed_Random_Number_Generator_Instance(const QString &seed)=0;
    virtual bool Generate_Level(SMB1_Compliance_Generator_Arguments &args)=0;

    //Level Script Functions
    virtual bool Parse_Level_Script(SMB1_Compliance_Parser_Arguments &args)=0;
    virtual bool Perform_Enemy_Chaotic_Swap(Enemy_Buffer *enemyBuffer, Level_Attribute::Level_Attribute levelAttribute, bool allowHammerBros, bool allowLakitus, bool allowContinousEnemySpawners)=0;
    virtual bool Redistribute_Enemies(SMB1_Compliance_Generator_Arguments &args, SMB1_Compliance_Parser_Arguments &parserArgs)=0;
    virtual bool Redistribute_Powerups(SMB1_Compliance_Generator_Arguments &args, SMB1_Compliance_Parser_Arguments &parserArgs)=0;

    //Argument Helpers
    virtual SMB1_Compliance_Generator_Arguments Get_Empty_SMB1_Compliance_Generator_Arguments()=0;
    virtual SMB1_Compliance_Parser_Arguments Get_Empty_SMB1_Compliance_Parser_Arguments()=0;
};

Q_DECLARE_INTERFACE(SMB1_Compliance_Generator_Interface, "SMB1_Compliance_Generator_Interface")

#endif // SMB1_COMPLIANCE_GENERATOR_INTERFACE_H
