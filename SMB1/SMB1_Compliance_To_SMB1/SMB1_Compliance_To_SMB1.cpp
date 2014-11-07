#include "SMB1_Compliance_To_SMB1.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../SMB1_Compliance_Generator/SMB1_Compliance_Generator_Arguments.h"
#include "SMB1_Compliance_Parser.h"
#include "Configure_Base_Form.h"
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <assert.h>

SMB1_Compliance_To_SMB1::SMB1_Compliance_To_SMB1() {
    this->generatorPlugin = NULL;
    this->writerPlugin = NULL;
    this->applicationLocation = QString();
    this->pluginsLoaded = false;
    this->baseGameSettings.numWorlds = 8;
    this->baseGameSettings.noDuplicates = false;
    this->baseGameSettings.baseROM = "";
}

void SMB1_Compliance_To_SMB1::Startup(QWidget *parent, QString location) {
    assert(parent);
    this->parent = parent;
    this->applicationLocation = location;
}

bool SMB1_Compliance_To_SMB1::Run() {
    if (this->applicationLocation.isEmpty() || !this->Load_Plugins()) {
        this->Shutdown();
        return false;
    }

    //Set up the Parser
    this->parser = new SMB1_Compliance_Parser(this->writerPlugin);

    qDebug() << "Loading a ROM...";
    bool loaded = false;
    if (this->baseGameSettings.baseROM.isEmpty()) {
        loaded = this->writerPlugin->Load_ROM();
    } else {
        loaded = this->writerPlugin->Load_ROM(this->baseGameSettings.baseROM);
    }
    if (!loaded) {
        qDebug() << "Failed to load the ROM!";
        this->Shutdown();
        return false;
    }

    qDebug() << "Attempting to generate a new level...";

    assert(this->writerPlugin->Room_Table_Set_Number_Of_Worlds(this->baseGameSettings.numWorlds));
    assert(this->writerPlugin->Room_Table_Set_Next_Level(Level::WORLD_1_LEVEL_1));

    //Allocate Buffers for a New Level
    if (!this->writerPlugin->New_Level(Level::WORLD_1_LEVEL_1)) {
        this->Shutdown();
        return false;
    }

    //Generate the level
    SMB1_Compliance_Generator_Arguments args;
    args.fileName = this->applicationLocation + "/Level_1_1.lvl";
    args.headerBackground = Background::NIGHT;
    args.headerScenery = Scenery::ONLY_CLOUDS;
    args.levelCompliment = Level_Compliment::MUSHROOMS;
    args.numObjectBytes = this->writerPlugin->Get_Num_Object_Bytes();
    args.numEnemyBytes = this->writerPlugin->Get_Num_Enemy_Bytes();
    args.startCastle = Castle::NONE;
    args.endCastle = Castle::BIG;
    args.levelType = Level_Type::BRIDGE;
    if (!this->generatorPlugin->Generate_Level(args)) {
        qDebug() << "Looks like the generator blew up";
        this->Shutdown();
        return false;
    }

    //Parse the level
    qDebug() << "Parsing the level...";
    int lineNum = 0;
    int errorCode = this->parser->Parse_Level(args.fileName, lineNum);
    switch (errorCode) {
    case -1: //An error occurred and was handled within the parser
        this->Shutdown();
        return false;
    case 0: break; //Parser ran fine
    case 1: //Unable to open the file
        QMessageBox::critical(this->parent, Common_Strings::LEVEL_HEADED,
                              "Unable to open " + args.fileName + "!", Common_Strings::OK);
        this->Shutdown();
        return false;
    case 2: //Syntax error
        QMessageBox::critical(this->parent, Common_Strings::LEVEL_HEADED,
                              "Syntax error on line " + QString::number(lineNum) + "!", Common_Strings::OK);
        this->Shutdown();
        return false;
    case 3: //Writer was unable to write an item
        QMessageBox::critical(this->parent, Common_Strings::LEVEL_HEADED,
                              "The writer plugin failed to write item on line " + QString::number(lineNum) + "!", Common_Strings::OK);
        this->Shutdown();
        return false;
    default:
        assert(false);
    }

    //Write the Level
    qDebug() << "Writing to the ROM...";
    if (!this->writerPlugin->Write_Level()) {
        qDebug() << "Looks like the writer plugin blew up";
        this->Shutdown();
        return false;
    }

    qDebug() << "Done!";

    //Unload plugins
    this->Shutdown();
    return true;
}

int SMB1_Compliance_To_SMB1::Configure_Generator() {
    qDebug() << "Configure Generator Called!";
    return 0;
}

int SMB1_Compliance_To_SMB1::Configure_Writer() {
    if (!this->Load_Plugins()) {
        //TODO: Show an error here
        return 1;
    }
    Configure_Writer_Form form(this->parent, &this->baseGameSettings, this->writerPlugin);
    return form.exec();
}

void SMB1_Compliance_To_SMB1::Shutdown() {
    if (this->generatorPlugin) this->generatorPlugin->Shutdown();
    if (this->writerPlugin) this->writerPlugin->Shutdown();
    if (this->generatorLoader) this->generatorLoader->unload();
    if (this->writerLoader) this->writerLoader->unload();
    delete this->generatorLoader;
    delete this->writerLoader;
    delete this->parser;
    this->generatorLoader = NULL;
    this->writerLoader = NULL;
    this->generatorPlugin = NULL;
    this->writerPlugin = NULL;
    this->parser = NULL;
    this->pluginsLoaded = false;
}

bool SMB1_Compliance_To_SMB1::Load_Plugins() {
    if (this->pluginsLoaded) return true;
    QString pluginLocation = this->applicationLocation + "/" + Common_Strings::PLUGINS + "/";
    QString generatorLocation = pluginLocation + Common_Strings::GENERATORS + "/SMB1_Compliance_Generator" + Common_Strings::PLUGIN_EXTENSION;
    QString writerLocation = pluginLocation + Common_Strings::WRITERS + "/SMB1_Writer" + Common_Strings::PLUGIN_EXTENSION;

    if (!QFile(generatorLocation).exists()) return false; //TODO: Throw an error here
    if (!QFile(writerLocation).exists()) return false; //TODO: Throw an error here

    //Load the Level Generator Plugin
    this->generatorLoader = new QPluginLoader(generatorLocation);
    QObject *validPlugin = this->generatorLoader->instance();
    if (!validPlugin) return false; //TODO: Throw an error here
    this->generatorPlugin = qobject_cast<SMB1_Compliance_Generator_Interface*>(validPlugin);
    if (!this->generatorPlugin) return false; //TODO: Throw an error here

    //Load the Writer Plugin
    this->writerLoader = new QPluginLoader(writerLocation);
    validPlugin = this->writerLoader->instance();
    if (!validPlugin) return false; //TODO: Throw an error here
    this->writerPlugin = qobject_cast<SMB1_Writer_Interface*>(validPlugin);
    if (!this->writerPlugin) return false; //TODO: Throw an error here

    //Set the application locations
    this->generatorPlugin->Startup(this->parent, this->applicationLocation);
    this->writerPlugin->Startup(this->parent, this->applicationLocation);

    this->pluginsLoaded = true;
    return true;
}
