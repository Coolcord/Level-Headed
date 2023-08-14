#include "SMB1_Compliance_To_SMB1.h"
#include "SMB1_Compliance_To_SMB1_Strings.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "../../Common_Files/Version.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../SMB1_Compliance_Generator/SMB1_Compliance_Generator_Arguments.h"
#include "../SMB1_Writer/ROM_Filename.h"
#include "CLI_Parser.h"
#include "Configure_Settings_Form.h"
#include "Config_File_Handler.h"
#include "Difficulty_Level_Configurations.h"
#include "Level_Generator.h"
#include "Hacks_Handler.h"
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QTime>
#include <QTextStream>
#include <QDebug>
#include <assert.h>

SMB1_Compliance_To_SMB1::SMB1_Compliance_To_SMB1() {
    this->generatorPlugin = nullptr;
    this->writerPlugin = nullptr;
    this->generatorLoader = nullptr;
    this->writerLoader = nullptr;
    this->applicationLocation = QString();
    this->args = nullptr;
    this->pluginsLoaded = false;
    this->cliMode = false;
    this->Load_Plugin_Default_Settings();
}

SMB1_Compliance_To_SMB1::~SMB1_Compliance_To_SMB1() {
    delete this->args;
}

void SMB1_Compliance_To_SMB1::Startup(QWidget *parent, const QString &location, const QStringList &args) {
    this->parent = parent;
    this->applicationLocation = location;
    if (!args.isEmpty()) this->args = new QStringList(args);
    if (!this->Load_Plugin_Settings()) this->Load_Plugin_Default_Settings();
}

bool SMB1_Compliance_To_SMB1::Run() {
    qDebug().noquote() << "Seed: " << this->pluginSettings.randomSeed;
    Random::Get_Instance().Seed(this->pluginSettings.randomSeed, 4);
    if (this->applicationLocation.isEmpty() || !this->Load_Plugins()) {
        this->Shutdown();
        this->Show_Message("Unable to load plugins!", true);
        return false;
    }
    this->writerPlugin->Seed_Random_Number_Generator_Instance(this->pluginSettings.randomSeed);
    this->generatorPlugin->Seed_Random_Number_Generator_Instance(this->pluginSettings.randomSeed);

    //Get a new output ROM location if necessary
    this->Update_ROM_Output_Location();
    this->writerPlugin->Set_Output_ROM_Location(this->pluginSettings.outputROMLocation);

    //Load a ROM into the Writer Plugin
    bool loaded = false;
    if (this->pluginSettings.baseROM.isEmpty()) loaded = this->writerPlugin->Load_ROM_First_Time(this->pluginSettings.baseROM);
    else loaded = this->writerPlugin->Load_ROM(this->pluginSettings.baseROM);
    if (!loaded) {
        this->Shutdown();
        this->Show_Message("Failed to load the ROM!", true);
        return false;
    }

    //Handle the Difficulty Settings
    if (this->pluginSettings.difficultyComboIndex == 1) { //Random Difficulty
        Difficulty_Level_Configurations().Random(&this->pluginSettings, this->pluginSettings.baseROM.startsWith(ROM_Filename::STRING_PARTIAL_SUPPORT));
    } else { //get the expected values for the specified present
        Difficulty_Level_Configurations().Update_Plugin_Settings_For_Difficulty_Present(this->pluginSettings.difficultyComboIndex, &this->pluginSettings, this->pluginSettings.baseROM.startsWith(ROM_Filename::STRING_PARTIAL_SUPPORT));
    }

    //Apply Hacks
    if (!Hacks_Handler(this->writerPlugin, &this->pluginSettings).Write_Hacks()) {
        this->Shutdown();
        this->Show_Message("Failed to hack the ROM!", true);
        return false;
    }

    //Generate the levels
    bool success = Level_Generator(this->applicationLocation, this->parent, &this->pluginSettings, this->generatorPlugin, this->writerPlugin).Run_Level_Generator();

    //Save the next output ROM location for later
    if (success) {
        this->pluginSettings.outputROMLocation = this->writerPlugin->Get_Output_ROM_Location();
        this->Update_ROM_Output_Location();
    }

    //Get a new seed for the next generation, since this one was successful
    this->pluginSettings.randomSeed = QString::number(QTime::currentTime().msecsSinceStartOfDay());

    //Unload plugins
    this->Shutdown();
    if (success) this->Show_Message("Game successfully generated!", false);
    else qDebug() << "Generation failed!"; //only show in command line to prevent flooding the user with message boxes
    return success;
}

bool SMB1_Compliance_To_SMB1::Run_CLI() {
    this->cliMode = true;
    CLI_Parser cliParser(this->args, &this->pluginSettings);
    bool parseSuccess = cliParser.Parse_Args();
    if (cliParser.Was_Help_Requested()) {
        cliParser.Show_Help();
        return parseSuccess;
    }
    if (!parseSuccess) return false;
    return this->Run();
}

int SMB1_Compliance_To_SMB1::Configure_Settings() {
    if (!this->Load_Plugins()) {
        this->Shutdown();
        this->Show_Message("Unable to load plugins!", true);
        return 1;
    }
    Configure_Settings_Form form(this->parent, this->applicationLocation, this->writerPlugin, &this->pluginSettings);
    return form.exec();
}

QString SMB1_Compliance_To_SMB1::Get_Seed() {
    return this->pluginSettings.randomSeed;
}

void SMB1_Compliance_To_SMB1::Shutdown() {
    this->Save_Plugin_Settings();
    if (this->generatorPlugin) this->generatorPlugin->Shutdown();
    if (this->writerPlugin) this->writerPlugin->Shutdown();
    if (this->generatorLoader) this->generatorLoader->unload();
    if (this->writerLoader) this->writerLoader->unload();
    delete this->generatorLoader;
    delete this->writerLoader;
    this->generatorLoader = nullptr;
    this->writerLoader = nullptr;
    this->generatorPlugin = nullptr;
    this->writerPlugin = nullptr;
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

bool SMB1_Compliance_To_SMB1::Save_Plugin_Settings() {
    if (!QDir().mkpath(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG)) return false;
    QString configFileLocation = this->applicationLocation + "/" + Common_Strings::STRING_CONFIG + "/" + Common_Strings::STRING_PLUGIN_SETTINGS_FILENAME;
    return Config_File_Handler(this->parent, this->applicationLocation).Save_Plugin_Settings(&this->pluginSettings, configFileLocation, true);
}

bool SMB1_Compliance_To_SMB1::Load_Plugin_Settings() {
    QString configFileLocation = this->applicationLocation + "/" + Common_Strings::STRING_CONFIG + "/" + Common_Strings::STRING_PLUGIN_SETTINGS_FILENAME;
    return Config_File_Handler(this->parent, this->applicationLocation).Load_Plugin_Settings(&this->pluginSettings, configFileLocation, true);
}

void SMB1_Compliance_To_SMB1::Load_Plugin_Default_Settings() {
    this->pluginSettings.numWorlds = 5;
    this->pluginSettings.numLevelsPerWorld = 4;
    this->pluginSettings.baseROM = "";
    this->pluginSettings.outputROMLocation = QString();
    this->pluginSettings.lastExternalConfigLocation = this->applicationLocation;
    this->pluginSettings.overwriteOuputROM = true;
    this->pluginSettings.generateNewLevels = true;
    this->pluginSettings.redistributePowerups = true;
    this->pluginSettings.randomizeWarpZones = true;
    this->pluginSettings.redistributeEnemies = false;
    this->pluginSettings.performChaoticSwapOnEnemies = false;
    this->pluginSettings.levelScriptEnemies = 3;
    this->pluginSettings.noAutoScrollingLevels = false;
    this->pluginSettings.standardLevelDistribution = true;
    this->pluginSettings.includeStandardOverworldLevelsInRandomDistribution = true;
    this->pluginSettings.includeUndergroundLevelsInRandomDistribution = true;
    this->pluginSettings.includeUnderwaterLevelsInRandomDistribution = true;
    this->pluginSettings.includeBridgeLevelsInRandomDistribution = true;
    this->pluginSettings.includeIslandLevelsInRandomDistribution = true;
    this->pluginSettings.smbUtilityCompatibility = false;
    this->pluginSettings.levelScripts = "";
    this->pluginSettings.standardOverworldChance = STRING_VERY_COMMON;
    this->pluginSettings.undergroundChance = STRING_COMMON;
    this->pluginSettings.underwaterChance = STRING_UNCOMMON;
    this->pluginSettings.bridgeChance = STRING_COMMON;
    this->pluginSettings.islandChance = STRING_COMMON;
    this->pluginSettings.randomSeed = QString::number(QTime::currentTime().msecsSinceStartOfDay());
    this->pluginSettings.randomNumWorlds = true;
    this->pluginSettings.music = 0;
    this->pluginSettings.combineMusicWithOtherPacks = true;
    this->pluginSettings.toneColor = 0;
    this->pluginSettings.randomSounds = true;
    this->pluginSettings.randomText = true;
    this->pluginSettings.randomizeSomeAnimations = true;
    this->pluginSettings.graphics = 0;
    this->pluginSettings.combineGraphicsWithOtherPacks = true;
    this->pluginSettings.palette = 0;
    this->pluginSettings.marioSprite = 0;
    this->pluginSettings.marioSpritePalette = 0;
    this->pluginSettings.infiniteLives = false;
    this->pluginSettings.permadeath = false;
    this->pluginSettings.alwaysRestartFromCurrentWorldAfterGameOver = false;
    this->pluginSettings.numLives = 7;
    this->pluginSettings.godMode = false;
    this->pluginSettings.addLuigiGame = true;
    this->pluginSettings.superMarioOnDamage = true;
    this->pluginSettings.lakituThrowArc = true;
    this->pluginSettings.powerup = 0;
    this->pluginSettings.fireFlowerBouncesLikeAStar = false;
    this->pluginSettings.marioName = "MARIO";
    this->pluginSettings.luigiName = "LUIGI";
    this->pluginSettings.difficultyComboIndex = 4;
    Difficulty_Level_Configurations().Normal(&this->pluginSettings, false);
}

void SMB1_Compliance_To_SMB1::Update_ROM_Output_Location() {
    if (!this->pluginSettings.overwriteOuputROM) {
        while (QFileInfo::exists(this->pluginSettings.outputROMLocation)) {
            this->pluginSettings.outputROMLocation = this->Append_Number_To_FileName(this->pluginSettings.outputROMLocation);
        }
    }
}

void SMB1_Compliance_To_SMB1::Show_Message(const QString &message, bool error) {
    if (this->cliMode) {
        qDebug().noquote() << message;
    } else {
        if (error) QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED, message, QMessageBox::Ok);
        else QMessageBox::information(this->parent, Common_Strings::STRING_LEVEL_HEADED, message, QMessageBox::Ok);
    }
}

QString SMB1_Compliance_To_SMB1::Append_Number_To_FileName(const QString &oldFileName) {
    bool hasExtension = false;
    if (oldFileName.contains(".")) hasExtension = true;
    QString newFileName = QString();

    //Check to see if a number was previously appended
    if (oldFileName.contains(" ")) {
        QStringList strings = oldFileName.split(" ");
        assert(!strings.isEmpty());
        QStringList extensionStrings = strings.last().split(".");
        if (extensionStrings.isEmpty()) hasExtension = false;
        for (int i = 0; i < strings.size()-1; ++i) {
            newFileName += strings.at(i)+" ";
        }
        if (hasExtension) {
            assert(extensionStrings.size() >= 2);
            for (int i = 0 ; i < extensionStrings.size()-2; ++i) {
                newFileName += extensionStrings.at(i)+".";
            }
            bool valid = false;
            int generationNumber = extensionStrings.at(extensionStrings.size()-2).toInt(&valid);
            if (valid) {
                newFileName += this->Get_Four_Digit_Minimum_From_Int(generationNumber+1) + ".";
            } else {
                newFileName += extensionStrings.at(extensionStrings.size()-2);
                newFileName += " 0001.";
            }
            newFileName.append(extensionStrings.last());
        } else { //no extension
            bool valid = false;
            int generationNumber = strings.last().toInt(&valid);
            if (valid) newFileName += this->Get_Four_Digit_Minimum_From_Int(generationNumber+1);
            else newFileName += "0001";
        }
    } else {
        if (hasExtension) {
            QStringList strings = oldFileName.split(".");
            assert(!strings.isEmpty());
            for (int i = 0 ; i < strings.size()-2; ++i) {
                newFileName += strings.at(i)+".";
            }
            newFileName += strings.at(strings.size()-2);
            newFileName += " 0001.";
            newFileName += strings.last();
        } else { //no extension
            newFileName += " 0001";
        }
    }
    return newFileName;
}

QString SMB1_Compliance_To_SMB1::Get_Four_Digit_Minimum_From_Int(int num) {
    QString numString = QString();
    if (num <= 999) numString.append("0");
    if (num <= 99) numString.append("0");
    if (num <= 9) numString.append("0");
    numString.append(QString::number(num));
    return numString;
}
