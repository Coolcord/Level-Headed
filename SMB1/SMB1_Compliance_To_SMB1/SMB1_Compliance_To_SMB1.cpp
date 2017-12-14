#include "SMB1_Compliance_To_SMB1.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../SMB1_Compliance_Generator/SMB1_Compliance_Generator_Arguments.h"
#include "Configure_Base_Form.h"
#include "Configure_Level_Form.h"
#include "Level_Generator.h"
#include "Hacks_Handler.h"
#include <QFile>
#include <QMessageBox>
#include <QTime>
#include <QDebug>
#include <assert.h>

SMB1_Compliance_To_SMB1::SMB1_Compliance_To_SMB1() {
    this->generatorPlugin = NULL;
    this->writerPlugin = NULL;
    this->applicationLocation = QString();
    this->pluginsLoaded = false;
    this->pluginSettings.numWorlds = 5;
    this->pluginSettings.numLevelsPerWorld = 4;
    this->pluginSettings.baseROM = "";
    this->pluginSettings.generateNewLevels = true;
    this->pluginSettings.levelScripts = "";
    this->pluginSettings.standardOverworldChance = STRING_VERY_COMMON;
    this->pluginSettings.undergroundChance = STRING_COMMON;
    this->pluginSettings.underwaterChance = STRING_UNCOMMON;
    this->pluginSettings.bridgeChance = STRING_UNCOMMON;
    this->pluginSettings.islandChance = STRING_UNCOMMON;
    this->pluginSettings.randomSeed = QTime::currentTime().msecsSinceStartOfDay();
    this->pluginSettings.randomNumWorlds = true;
    this->pluginSettings.hammerTime = false;
    this->pluginSettings.music = 0;
    this->pluginSettings.graphics = 0;
    this->pluginSettings.infiniteLives = false;
    this->pluginSettings.numLives = 5;
    this->pluginSettings.godMode = false;
    this->pluginSettings.addLuigiGame = true;
    this->pluginSettings.superMarioOnDamage = Qt::PartiallyChecked;
    this->pluginSettings.lakituThrowArc = Qt::PartiallyChecked;
    this->pluginSettings.enemySpeed = 6;
    this->outputROMLocation = QString();
}

void SMB1_Compliance_To_SMB1::Startup(QWidget *parent, const QString &location) {
    assert(parent);
    this->parent = parent;
    this->applicationLocation = location;
}

bool SMB1_Compliance_To_SMB1::Run() {
    if (this->applicationLocation.isEmpty() || !this->Load_Plugins()) {
        this->Shutdown();
        //TODO: Update this error
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                              "Something went wrong. Check debug info...", Common_Strings::STRING_OK);
        return false;
    }

    //Set the output ROM location if a previous generation was performed
    if (!this->outputROMLocation.isEmpty()) {
        assert(this->writerPlugin->Set_Output_ROM_Location(this->outputROMLocation));
    }

    //Load a ROM into the Writer Plugin
    bool loaded = false;
    if (this->pluginSettings.baseROM.isEmpty()) {
        loaded = this->writerPlugin->Load_ROM();
    } else {
        loaded = this->writerPlugin->Load_ROM(this->pluginSettings.baseROM);
    }
    if (!loaded) {
        qDebug() << "Failed to load the ROM!";
        return false;
    }

    //Apply Hacks
    assert(Hacks_Handler(this->writerPlugin, &this->pluginSettings).Write_Hacks());

    //Generate the levels
    Level_Generator levelGenerator(this->applicationLocation, this->parent, &this->pluginSettings, this->generatorPlugin, this->writerPlugin);
    bool success = false;
    if (this->pluginSettings.generateNewLevels) success = levelGenerator.Generate_Levels();
    else success = levelGenerator.Parse_Level_Map();

    //Save the output ROM location for later
    if (success) this->outputROMLocation = this->writerPlugin->Get_Output_ROM_Location();

    //Unload plugins
    this->Shutdown();
    if (success) {
        QMessageBox::information(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                 "Game successfully generated!", Common_Strings::STRING_OK);
    }
    return success;
}

int SMB1_Compliance_To_SMB1::Configure_Generator() {
    if (!this->Load_Plugins()) {
        //TODO: Show an error here
        return 1;
    }
    Configure_Level_Form form(this->parent, &this->pluginSettings, this->applicationLocation);
    return form.exec();
}

int SMB1_Compliance_To_SMB1::Configure_Writer() {
    if (!this->Load_Plugins()) {
        //TODO: Show an error here
        return 1;
    }
    Configure_Base_Form form(this->parent, &this->pluginSettings, this->writerPlugin, this->applicationLocation);
    return form.exec();
}

void SMB1_Compliance_To_SMB1::Shutdown() {
    if (this->generatorPlugin) this->generatorPlugin->Shutdown();
    if (this->writerPlugin) this->writerPlugin->Shutdown();
    if (this->generatorLoader) this->generatorLoader->unload();
    if (this->writerLoader) this->writerLoader->unload();
    delete this->generatorLoader;
    delete this->writerLoader;
    this->generatorLoader = NULL;
    this->writerLoader = NULL;
    this->generatorPlugin = NULL;
    this->writerPlugin = NULL;
    this->pluginsLoaded = false;
}

bool SMB1_Compliance_To_SMB1::Load_Plugins() {
    if (this->pluginsLoaded) return true;
    QString pluginLocation = this->applicationLocation + "/" + Common_Strings::STRING_PLUGINS + "/";
    QString generatorLocation = pluginLocation + Common_Strings::STRING_GENERATORS + "/SMB1_Compliance_Generator" + Common_Strings::STRING_PLUGIN_EXTENSION;
    QString writerLocation = pluginLocation + Common_Strings::STRING_WRITERS + "/SMB1_Writer" + Common_Strings::STRING_PLUGIN_EXTENSION;

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
