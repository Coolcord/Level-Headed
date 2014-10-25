#include "SMB1_Compliance_To_SMB1.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../Common SMB1 Files/Level_Type.h"
#include "SMB1_Compliance_Parser.h"
#include <QPluginLoader>
#include <QFile>
#include <QDebug>
#include <assert.h>

SMB1_Compliance_To_SMB1::SMB1_Compliance_To_SMB1() {
    this->generatorPlugin = NULL;
    this->writerPlugin = NULL;
    this->applicationLocation = QString();
}

void SMB1_Compliance_To_SMB1::Startup(QWidget *parent, QString location) {
    assert(parent);
    this->parent = parent;
    this->applicationLocation = location;
}

bool SMB1_Compliance_To_SMB1::Run() {
    this->generatorPlugin = NULL;
    this->writerPlugin = NULL;

    if (this->applicationLocation.isEmpty()) return false;
    if (!this->Load_Plugins()) return false;

    //Set up the Parser
    this->parser = new SMB1_Compliance_Parser(this->writerPlugin);

    qDebug() << "Loading a ROM...";
    if (!this->writerPlugin->Load_ROM()) {
        qDebug() << "Failed to load the ROM!";
        return false;
    }

    qDebug() << "Attempting to generate a new level...";

    //Allocate Buffers for a New Level
    if (!this->writerPlugin->New_Level(Level::WORLD_1_LEVEL_1)) return false;

    //Generate the level
    QString fileName = this->applicationLocation + "/Level_1_1.lvl";
    //QString fileName = "C:/Users/Cord/Desktop/Level_1_1.lvl";

    if (!this->generatorPlugin->Generate_Level(fileName, this->writerPlugin->Get_Num_Object_Bytes(),
                                               this->writerPlugin->Get_Num_Enemy_Bytes(), Level_Type::STANDARD_OVERWORLD)) {
        qDebug() << "Looks like the generator blew up";
        return false;
    }


    //Parse the level
    qDebug() << "Parsing the level...";
    if (!this->parser->Parse_Level(fileName)) {
        qDebug() << "Looks like the interpreter blew up";
        return false;
    }

    //Write the Level
    qDebug() << "Writing to the ROM...";
    if (!this->writerPlugin->Write_Level()) {
        qDebug() << "Looks like the writer plugin blew up";
        return false;
    }

    qDebug() << "Done!";

    //Unload plugins
    this->generatorPlugin->Shutdown();
    this->writerPlugin->Shutdown();
    delete this->parser;
    delete this->generatorPlugin;
    delete this->writerPlugin;
    this->parser = NULL;
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

    //Set the application locations
    this->generatorPlugin->Startup(this->parent, this->applicationLocation);
    this->writerPlugin->Startup(this->parent, this->applicationLocation);

    return true;
}
