#ifndef SMB1_COMPLIANCE_TO_SMB1_H
#define SMB1_COMPLIANCE_TO_SMB1_H

#include "../../Level-Headed/Interpreter_Interface.h"

class SMB1_Compliance_To_SMB1 : public Interpreter_Interface {

    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "Interpreter_Interface" FILE "SMB1_Compliance_To_SMB1.json")
    Q_INTERFACES(Interpreter_Interface)
public:
    SMB1_Compliance_To_SMB1();
    bool Run();
    bool Configure_Generator();
    bool Configure_Writer();
};

#endif // SMB1_COMPLIANCE_TO_SMB1_H
