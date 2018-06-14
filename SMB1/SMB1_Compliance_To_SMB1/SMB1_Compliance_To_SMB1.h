#ifndef SMB1_COMPLIANCE_TO_SMB1_H
#define SMB1_COMPLIANCE_TO_SMB1_H

#include "../../Level-Headed/Interpreter_Interface.h"
#include "../../SMB1/SMB1_Compliance_Generator/SMB1_Compliance_Generator_Interface.h"
#include "../../SMB1/SMB1_Writer/SMB1_Writer_Interface.h"
#include "../../../Hexagon/Hexagon/Hexagon_Interface.h"
#include "../../../Sequential_Archive/Sequential_Archive/Sequential_Archive_Interface.h"
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
    bool Save_Plugin_Settings();
    bool Load_Plugin_Settings();
    void Load_Plugin_Default_Settings();
    bool Load_Hexagon_Plugin();
    bool Load_Sequential_Archive_Plugin();
    void Show_Common_Plugin_Failed_To_Load_Error(const QString &pluginName, bool &errorDisplayed);
    void Update_ROM_Output_Location();
    QString Append_Number_To_FileName(const QString &oldFileName);
    QString Get_Four_Digit_Minimum_From_Int(int num);

    QString applicationLocation;
    QString pluginLocation;
    bool pluginsLoaded;
    SMB1_Compliance_Generator_Interface *generatorPlugin;
    SMB1_Writer_Interface *writerPlugin;
    Hexagon_Interface *hexagonPlugin;
    Sequential_Archive_Interface *sequentialArchivePlugin;
    QPluginLoader *generatorLoader;
    QPluginLoader *writerLoader;
    QPluginLoader *hexagonLoader;
    QPluginLoader *sequentialArchiveLoader;
    QWidget *parent;
    Plugin_Settings pluginSettings;
};

#endif // SMB1_COMPLIANCE_TO_SMB1_H
