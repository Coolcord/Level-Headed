#ifndef SMB1_COMPLIANCE_GENERATOR_INTERFACE_H
#define SMB1_COMPLIANCE_GENERATOR_INTERFACE_H

#include "SMB1_Compliance_Generator_Arguments.h"
#include <QObject>
#include <QtPlugin>
#include <QString>
#include <QWidget>

class SMB1_Compliance_Generator_Interface : public QObject {
public:
    virtual void Startup(QWidget *parent, const QString &location)=0;
    virtual void Shutdown()=0;
    virtual bool Generate_Level(SMB1_Compliance_Generator_Arguments args)=0;
};

Q_DECLARE_INTERFACE(SMB1_Compliance_Generator_Interface, "SMB1_Compliance_Generator_Interface")

#endif // SMB1_COMPLIANCE_GENERATOR_INTERFACE_H
