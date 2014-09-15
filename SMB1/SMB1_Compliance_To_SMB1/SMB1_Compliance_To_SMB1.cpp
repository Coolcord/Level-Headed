#include "SMB1_Compliance_To_SMB1.h"
#include "../../Level-Headed/Common_Strings.h"
#include <QPluginLoader>
#include <QFile>
#include <QDebug>

SMB1_Compliance_To_SMB1::SMB1_Compliance_To_SMB1() {
    this->generatorPlugin = NULL;
    this->writerPlugin = NULL;
    this->applicationLocation = QString();
}

void SMB1_Compliance_To_SMB1::Set_Application_Directory(const QString &location) {
    this->applicationLocation = location;
}

bool SMB1_Compliance_To_SMB1::Run() {
    if (this->applicationLocation.isEmpty()) return false;
    qDebug() << this->Load_Plugins();


    //Unload plugins
    delete this->generatorPlugin;
    delete this->writerPlugin;
    this->generatorPlugin = NULL;
    this->writerPlugin = NULL;
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

bool SMB1_Compliance_To_SMB1::Load_Plugins() {
    QString pluginLocation = this->applicationLocation + "/" + Common_Strings::PLUGINS + "/";
    QString generatorLocation = pluginLocation + Common_Strings::GENERATORS + "/SMB1_Compliance_Generator" + Common_Strings::PLUGIN_EXTENSION;
    QString writerLocation = pluginLocation + Common_Strings::WRITERS + "/SMB1_Writer" + Common_Strings::PLUGIN_EXTENSION;

    if (!QFile(generatorLocation).exists()) return false; //TODO: Throw an error here
    if (!QFile(writerLocation).exists()) return false; //TODO: Throw an error here

    //Load the Level Generator Plugin
    QPluginLoader generatorLoader(generatorLocation);
    QObject *validPlugin = generatorLoader.instance();
    if (!validPlugin) return false; //TODO: Throw an error here
    this->generatorPlugin = qobject_cast<SMB1_Compliance_Generator_Interface*>(validPlugin);
    if (!this->generatorPlugin) return false; //TODO: Throw an error here

    //Load the Writer Plugin
    QPluginLoader writerLoader(writerLocation);
    validPlugin = writerLoader.instance();
    if (!validPlugin) return false; //TODO: Throw an error here
    this->writerPlugin = qobject_cast<SMB1_Writer_Interface*>(validPlugin);
    if (!this->writerPlugin) return false; //TODO: Throw an error here

    return true;
}
