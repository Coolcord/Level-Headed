#include "SMB1_Compliance_To_SMB1.h"
#include <QDebug>

SMB1_Compliance_To_SMB1::SMB1_Compliance_To_SMB1()
{
    qDebug() << "Constructed";
}

bool SMB1_Compliance_To_SMB1::Run() {
    qDebug() << "Run() called!";
    return true;
}

bool SMB1_Compliance_To_SMB1::Configure_Generator() {
    qDebug() << "Configure Generator Called!";
    return true;
}

bool SMB1_Compliance_To_SMB1::Configure_Writer() {
    qDebug() << "Configure Writer Called!";
    return true;
}
