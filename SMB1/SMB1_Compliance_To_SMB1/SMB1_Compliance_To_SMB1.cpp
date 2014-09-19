#include "SMB1_Compliance_To_SMB1.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../Common SMB1 Files/Level_Type.h"
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
    qDebug() << "Phase 1";
    if (this->applicationLocation.isEmpty()) return false;
    qDebug() << "Phase 2";
    if (!this->Load_Plugins()) return false;

    qDebug() << "Loading a ROM...";
    if (!this->writerPlugin->Load_ROM("C:/Users/Cord/Desktop/Level-Headed Test Files/Super Mario Bros..nes")) return false;
    qDebug() << "Allocating buffers for a new level...";
    if (!this->writerPlugin->New_Level(Level::WORLD_1_LEVEL_1)) return false;

    qDebug() << "Attempting to generate a new level...";

    //Generate the level
    QString fileName = this->applicationLocation + "/Level_1_1.lvl";
    if (!this->generatorPlugin->Generate_Level(fileName, this->writerPlugin->Get_Num_Object_Bytes(),
                                               this->writerPlugin->Get_Num_Enemy_Bytes(), Level_Type::STANDARD_OVERWORLD)) {
        qDebug() << "Looks like the generator blew up";
        return false;
    }

    qDebug() << "Done!";

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
