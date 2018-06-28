#include "SMB1_Compliance_To_SMB1.h"
#include "../../Level-Headed/Common_Strings.h"
#include "SMB1_Compliance_To_SMB1_Strings.h"
#include "../SMB1_Compliance_Generator/SMB1_Compliance_Generator_Arguments.h"
#include "Configure_Base_Form.h"
#include "Configure_Level_Form.h"
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
    this->generatorPlugin = NULL;
    this->writerPlugin = NULL;
    this->generatorLoader = NULL;
    this->writerLoader = NULL;
    this->applicationLocation = QString();
    this->pluginsLoaded = false;
    this->Load_Plugin_Default_Settings();
}

void SMB1_Compliance_To_SMB1::Startup(QWidget *parent, const QString &location) {
    assert(parent);
    this->parent = parent;
    this->applicationLocation = location;
    if (!this->Load_Plugin_Settings()) this->Load_Plugin_Default_Settings();
}

bool SMB1_Compliance_To_SMB1::Run() {
    if (this->applicationLocation.isEmpty() || !this->Load_Plugins()) {
        this->Shutdown();
        //TODO: Update this error
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                              "Something went wrong. Check debug info...", Common_Strings::STRING_OK);
        return false;
    }

    //Get a new output ROM location if necessary
    this->Update_ROM_Output_Location();
    this->writerPlugin->Set_Output_ROM_Location(this->pluginSettings.outputROMLocation);

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

    //Handle the Difficulty Settings
    if (this->pluginSettings.difficultyComboIndex == 1) {
        Difficulty_Level_Configurations().Random(&this->pluginSettings);
    }

    //Generate the levels
    Level_Generator levelGenerator(this->applicationLocation, this->parent, &this->pluginSettings, this->generatorPlugin, this->writerPlugin);
    bool success = false;
    if (this->pluginSettings.generateNewLevels) success = levelGenerator.Generate_Levels();
    else success = levelGenerator.Parse_Level_Map();

    //Save the next output ROM location for later
    if (success) {
        this->pluginSettings.outputROMLocation = this->writerPlugin->Get_Output_ROM_Location();
        this->Update_ROM_Output_Location();
    }

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
    this->Save_Plugin_Settings();
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

bool SMB1_Compliance_To_SMB1::Save_Plugin_Settings() {
    if (!QDir().mkpath(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG)) return false;
    QFile file(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG + "/" + Common_Strings::STRING_PLUGIN_SETTINGS_FILENAME);
    if (file.exists() && !file.remove()) return false;
    if (!file.open(QIODevice::ReadWrite)) return false;
    QTextStream stream(&file);
    stream << this->pluginSettings.baseROM << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.outputROMLocation << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.overwriteOuputROM << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.randomNumWorlds << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.numWorlds << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.numLevelsPerWorld << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.generateNewLevels << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.levelScripts << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyComboIndex << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyBulletTime << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyHammerTime << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyBuzzyBeetlesReplaceLoneGoombas << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyBridgeFlyingCheepCheeps << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyBridgeLakitus << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyBridgeOffscreenBulletBills << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyCastleFlyingCheepCheeps << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyCastleLakitus << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyCastleOffscreenBulletBills << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyIslandFlyingCheepCheeps << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyIslandLakitus << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyIslandOffscreenBulletBills << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyUndergroundFlyingCheepCheeps << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyUndergroundLakitus << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyUndergroundOffscreenBulletBills << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyUnderwaterBloopers << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyUnderwaterFlyingCheepCheeps << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyUnderwaterLakitus << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyUnderwaterSwimmingCheepCheeps << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyStandardOverworldFlyingCheepCheeps << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyStandardOverworldLakitus << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyStandardOverworldOffscreenBulletBills << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyNoEnemies << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyHammerTimeIntensity << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyLakituSpawnChancePerLevel << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultyDisableAllOtherEnemiesWhenALakituSpawns << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.difficultySpawnerPriority << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.standardOverworldChance << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.undergroundChance << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.underwaterChance << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.bridgeChance << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.islandChance << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.music << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.combineMusicWithOtherPacks << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.toneColor << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.graphics << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.infiniteLives << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.numLives << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.godMode << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.addLuigiGame << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.superMarioOnDamage << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.lakituThrowArc << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.autoscroll << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.replaceFireFlowerWithHammerSuit << Common_Strings::STRING_NEW_LINE;
    stream << this->pluginSettings.enemySpeed << Common_Strings::STRING_NEW_LINE;
    stream.flush();
    file.close();
    return true;
}

bool SMB1_Compliance_To_SMB1::Load_Plugin_Settings() {
    QFile file(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG + "/" + Common_Strings::STRING_PLUGIN_SETTINGS_FILENAME);
    if (!file.exists()) return true; //first time running the plugin
    if (!file.open(QIODevice::ReadWrite)) return false;
    bool valid = true;
    this->pluginSettings.baseROM = file.readLine().trimmed();
    this->pluginSettings.outputROMLocation = file.readLine().trimmed();
    this->pluginSettings.overwriteOuputROM = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.randomNumWorlds = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.numWorlds = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.numLevelsPerWorld = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.generateNewLevels = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.levelScripts = file.readLine().trimmed();
    this->pluginSettings.difficultyComboIndex = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyBulletTime = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyHammerTime = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyBuzzyBeetlesReplaceLoneGoombas = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyBridgeFlyingCheepCheeps = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyBridgeLakitus = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyBridgeOffscreenBulletBills = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyCastleFlyingCheepCheeps = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyCastleLakitus = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyCastleOffscreenBulletBills = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyIslandFlyingCheepCheeps = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyIslandLakitus = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyIslandOffscreenBulletBills = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyUndergroundFlyingCheepCheeps = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyUndergroundLakitus = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyUndergroundOffscreenBulletBills = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyUnderwaterBloopers = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyUnderwaterFlyingCheepCheeps = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyUnderwaterLakitus = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyUnderwaterSwimmingCheepCheeps = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyStandardOverworldFlyingCheepCheeps = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyStandardOverworldLakitus = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyStandardOverworldOffscreenBulletBills = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyNoEnemies = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyHammerTimeIntensity = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyLakituSpawnChancePerLevel = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyDisableAllOtherEnemiesWhenALakituSpawns = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultySpawnerPriority = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.standardOverworldChance = file.readLine().trimmed();
    this->pluginSettings.undergroundChance = file.readLine().trimmed();
    this->pluginSettings.underwaterChance = file.readLine().trimmed();
    this->pluginSettings.bridgeChance = file.readLine().trimmed();
    this->pluginSettings.islandChance = file.readLine().trimmed();
    this->pluginSettings.music = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.combineMusicWithOtherPacks = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.toneColor = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.graphics = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.infiniteLives = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.numLives = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.godMode = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.addLuigiGame = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    this->pluginSettings.superMarioOnDamage = static_cast<Qt::CheckState>(file.readLine().trimmed().toInt(&valid)); if (!valid) return false;
    this->pluginSettings.lakituThrowArc = static_cast<Qt::CheckState>(file.readLine().trimmed().toInt(&valid)); if (!valid) return false;
    this->pluginSettings.autoscroll = static_cast<Qt::CheckState>(file.readLine().trimmed().toInt(&valid)); if (!valid) return false;
    this->pluginSettings.replaceFireFlowerWithHammerSuit = static_cast<Qt::CheckState>(file.readLine().trimmed().toInt(&valid)); if (!valid) return false;
    this->pluginSettings.enemySpeed = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    return true;
}

void SMB1_Compliance_To_SMB1::Load_Plugin_Default_Settings() {
    this->pluginSettings.numWorlds = 5;
    this->pluginSettings.numLevelsPerWorld = 4;
    this->pluginSettings.baseROM = "";
    this->pluginSettings.outputROMLocation = QString();
    this->pluginSettings.overwriteOuputROM = true;
    this->pluginSettings.generateNewLevels = true;
    this->pluginSettings.levelScripts = "";
    this->pluginSettings.standardOverworldChance = STRING_VERY_COMMON;
    this->pluginSettings.undergroundChance = STRING_COMMON;
    this->pluginSettings.underwaterChance = STRING_UNCOMMON;
    this->pluginSettings.bridgeChance = STRING_COMMON;
    this->pluginSettings.islandChance = STRING_COMMON;
    this->pluginSettings.randomSeed = QTime::currentTime().msecsSinceStartOfDay();
    this->pluginSettings.randomNumWorlds = true;
    this->pluginSettings.music = 0;
    this->pluginSettings.combineMusicWithOtherPacks = true;
    this->pluginSettings.toneColor = 0;
    this->pluginSettings.graphics = 0;
    this->pluginSettings.infiniteLives = false;
    this->pluginSettings.numLives = 5;
    this->pluginSettings.godMode = false;
    this->pluginSettings.addLuigiGame = true;
    this->pluginSettings.superMarioOnDamage = Qt::Unchecked;
    this->pluginSettings.lakituThrowArc = Qt::Checked;
    this->pluginSettings.autoscroll = Qt::Unchecked;
    this->pluginSettings.replaceFireFlowerWithHammerSuit = Qt::PartiallyChecked;
    this->pluginSettings.enemySpeed = 6;
    this->pluginSettings.difficultyComboIndex = 4;
    Difficulty_Level_Configurations().Normal(&this->pluginSettings);
}

void SMB1_Compliance_To_SMB1::Update_ROM_Output_Location() {
    if (!this->pluginSettings.overwriteOuputROM) {
        while (QFileInfo(this->pluginSettings.outputROMLocation).exists()) {
            this->pluginSettings.outputROMLocation = this->Append_Number_To_FileName(this->pluginSettings.outputROMLocation);
        }
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
