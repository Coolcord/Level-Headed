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
    ~SMB1_Compliance_To_SMB1();
    void Startup(QWidget *parent, const QString &location, const QStringList &args);
    bool Run();
    bool Run_CLI();
    int Configure_Settings();

private:
    void Shutdown();
    bool Load_Plugins();
    bool Save_Plugin_Settings();
    bool Load_Plugin_Settings();
    void Load_Plugin_Default_Settings();
    void Update_ROM_Output_Location();
    void Show_Message(const QString &message, bool error);
    QString Append_Number_To_FileName(const QString &oldFileName);
    QString Get_Four_Digit_Minimum_From_Int(int num);

    QString applicationLocation;
    QStringList *args;
    bool pluginsLoaded;
    SMB1_Compliance_Generator_Interface *generatorPlugin;
    SMB1_Writer_Interface *writerPlugin;
    QPluginLoader *generatorLoader;
    QPluginLoader *writerLoader;
    QWidget *parent;
    Plugin_Settings pluginSettings;
    bool cliMode;
};

#endif // SMB1_COMPLIANCE_TO_SMB1_H
