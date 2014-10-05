#ifndef SMB1_COMPLIANCE_GENERATOR_INTERFACE_H
#define SMB1_COMPLIANCE_GENERATOR_INTERFACE_H

#include "../Common SMB1 Files/Level_Type.h"
#include <QObject>
#include <QtPlugin>
#include <QString>
#include <QWidget>

class SMB1_Compliance_Generator_Interface : public QObject {
public:
    virtual void Startup(QWidget *parent, QString location)=0;
    virtual void Shutdown()=0;
    virtual bool Generate_Level(const QString &fileName, int numObjectBytes, int numEnemyBytes, Level_Type::Level_Type type)=0;
};

Q_DECLARE_INTERFACE(SMB1_Compliance_Generator_Interface, "SMB1_Compliance_Generator_Interface")

#endif // SMB1_COMPLIANCE_GENERATOR_INTERFACE_H
