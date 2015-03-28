#ifndef SMB1_COMPLIANCE_TO_SMB1_H
#define SMB1_COMPLIANCE_TO_SMB1_H

#include "../../Level-Headed/Interpreter_Interface.h"
#include "../../SMB1/SMB1_Compliance_Generator/SMB1_Compliance_Generator_Interface.h"
#include "../../SMB1/SMB1_Writer/SMB1_Writer_Interface.h"
#include "Plugin_Settings.h"
#include "QPluginLoader"

class SMB1_Compliance_To_SMB1 : public Interpreter_Interface {

    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "Interpreter_Interface")
    Q_INTERFACES(Interpreter_Interface)
public:
    SMB1_Compliance_To_SMB1();
    void Startup(QWidget *parent, const QString &location);
    bool Run();
    int Configure_Generator();
    int Configure_Writer();

private:
    void Shutdown();
    bool Load_Plugins();
    QString applicationLocation;

    bool pluginsLoaded;
    SMB1_Compliance_Generator_Interface *generatorPlugin;
    SMB1_Writer_Interface *writerPlugin;
    QPluginLoader *generatorLoader;
    QPluginLoader *writerLoader;
    QWidget *parent;
    Plugin_Settings pluginSettings;
};

#endif // SMB1_COMPLIANCE_TO_SMB1_H
