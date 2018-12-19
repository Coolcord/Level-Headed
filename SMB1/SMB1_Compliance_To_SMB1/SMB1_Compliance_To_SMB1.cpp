#include "SMB1_Compliance_To_SMB1.h"
#include "../../Level-Headed/Common_Strings.h"
#include "SMB1_Compliance_To_SMB1_Strings.h"
#include "../SMB1_Compliance_Generator/SMB1_Compliance_Generator_Arguments.h"
#include "../SMB1_Writer/ROM_Filename.h"
#include "CLI_Parser.h"
#include "Configure_Settings_Form.h"
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
    if (this->applicationLocation.isEmpty() || !this->Load_Plugins()) {
        this->Shutdown();
        //TODO: Update this error
        this->Show_Message("Something went wrong. Check debug info...", true);
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
        Difficulty_Level_Configurations().Random(&this->pluginSettings, !this->pluginSettings.baseROM.startsWith(ROM_Filename::STRING_FULL_SUPPORT));
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
        //TODO: Show an error here
        return 1;
    }
    Configure_Settings_Form form(this->parent, this->applicationLocation, this->writerPlugin, &this->pluginSettings);
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
    QFile file(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG + "/" + Common_Strings::STRING_PLUGIN_SETTINGS_FILENAME);
    if (file.exists() && !file.remove()) return false;
    if (!file.open(QIODevice::ReadWrite)) return false;
    QTextStream stream(&file);
    stream << "Last_Tab=" << this->pluginSettings.tab << Common_Strings::STRING_NEW_LINE;
    stream << "Base_ROM=" << this->pluginSettings.baseROM << Common_Strings::STRING_NEW_LINE;
    stream << "Output_ROM_Location=" << this->pluginSettings.outputROMLocation << Common_Strings::STRING_NEW_LINE;
    stream << "Overwrite_Output_ROM=" << this->pluginSettings.overwriteOuputROM << Common_Strings::STRING_NEW_LINE;
    stream << "Random_Number_Of_Worlds=" << this->pluginSettings.randomNumWorlds << Common_Strings::STRING_NEW_LINE;
    stream << "Number_Of_Worlds=" << this->pluginSettings.numWorlds << Common_Strings::STRING_NEW_LINE;
    stream << "Numer_Of_Levels_Per_World=" << this->pluginSettings.numLevelsPerWorld << Common_Strings::STRING_NEW_LINE;
    stream << "Generate_New_Levels=" << this->pluginSettings.generateNewLevels << Common_Strings::STRING_NEW_LINE;
    stream << "Level_Scripts=" << this->pluginSettings.levelScripts << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty=" << this->pluginSettings.difficultyComboIndex << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Auto_Scroll=" << this->pluginSettings.difficultyAutoScroll << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Bullet_Time=" << this->pluginSettings.difficultyBulletTime << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Hammer_Time=" << this->pluginSettings.difficultyHammerTime << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Walking_Hammer_Bros=" << this->pluginSettings.difficultyWalkingHammerBros << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Buzzy_Beetles_Replace_Lone_Goombas=" << this->pluginSettings.difficultyBuzzyBeetlesReplaceLoneGoombas << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Bridge_Flying_Cheep_Cheeps=" << this->pluginSettings.difficultyBridgeFlyingCheepCheeps << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Bridge_Lakitus=" << this->pluginSettings.difficultyBridgeLakitus << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Bridge_Offscreen_Bullet_Bills=" << this->pluginSettings.difficultyBridgeOffscreenBulletBills << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Castle_Fire_Bars=" << this->pluginSettings.difficultyCastleFireBars << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Castle_Flying_Cheep_Cheeps=" << this->pluginSettings.difficultyCastleFlyingCheepCheeps << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Castle_Lakitus=" << this->pluginSettings.difficultyCastleLakitus << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Castle_Offscreen_Bullet_Bills=" << this->pluginSettings.difficultyCastleOffscreenBulletBills << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Island_Flying_Cheep_Cheeps=" << this->pluginSettings.difficultyIslandFlyingCheepCheeps << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Island_Lakitus=" << this->pluginSettings.difficultyIslandLakitus << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Island_Offscreen_Bullet_Bills=" << this->pluginSettings.difficultyIslandOffscreenBulletBills << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Underground_Flying_Cheep_Cheeps=" << this->pluginSettings.difficultyUndergroundFlyingCheepCheeps << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Underground_Lakitus=" << this->pluginSettings.difficultyUndergroundLakitus << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Underground_Offscreen_Bullet_Bills=" << this->pluginSettings.difficultyUndergroundOffscreenBulletBills << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Underwater_Bloopers=" << this->pluginSettings.difficultyUnderwaterBloopers << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Underwater_Flying_Cheep_Cheeps=" << this->pluginSettings.difficultyUnderwaterFlyingCheepCheeps << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Underwater_Lakitus=" << this->pluginSettings.difficultyUnderwaterLakitus << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Underwater_Swimming_Cheep_Cheeps=" << this->pluginSettings.difficultyUnderwaterSwimmingCheepCheeps << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Standard_Overworld_Flying_Cheep_Cheeps=" << this->pluginSettings.difficultyStandardOverworldFlyingCheepCheeps << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Standard_Overworld_Lakitus=" << this->pluginSettings.difficultyStandardOverworldLakitus << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Standard_Overworld_Offscreen_Bullet_Bills=" << this->pluginSettings.difficultyStandardOverworldOffscreenBulletBills << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_No_Enemies=" << this->pluginSettings.difficultyNoEnemies << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Start_With_Fire_Flower_On_Room_Change=" << this->pluginSettings.difficultyStartWithFireFlowerOnRoomChange << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Hammer_Time_Intensity=" << this->pluginSettings.difficultyHammerTimeIntensity << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Replace_Castle_Loops=" << this->pluginSettings.difficultyReplaceCastleLoops << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Auto_Scroll_Chance_Per_Level=" << this->pluginSettings.difficultyAutoScrollChancePerLevel << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Lakitu_Spawn_Chance_Per_Level=" << this->pluginSettings.difficultyLakituSpawnChancePerLevel << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Lakitu_Respawn_Speed=" << this->pluginSettings.difficultyLakituRespawnSpeed << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Spiny_Egg_Behavior=" << this->pluginSettings.difficultySpinyEggBehavior << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Disable_All_Other_Enemies_When_A_Lakitu_Spawns=" << this->pluginSettings.difficultyDisableAllOtherEnemiesWhenALakituSpawns << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Spawner_Priority=" << this->pluginSettings.difficultySpawnerPriority << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Piranha_Plant_Type=" << this->pluginSettings.difficultyPiranhaPlantType << Common_Strings::STRING_NEW_LINE;
    stream << "Standard_Overworld_Chance=" << this->pluginSettings.standardOverworldChance << Common_Strings::STRING_NEW_LINE;
    stream << "Underground_Chance=" << this->pluginSettings.undergroundChance << Common_Strings::STRING_NEW_LINE;
    stream << "Underwater_Chance=" << this->pluginSettings.underwaterChance << Common_Strings::STRING_NEW_LINE;
    stream << "Bridge_Chance=" << this->pluginSettings.bridgeChance << Common_Strings::STRING_NEW_LINE;
    stream << "Island_Chance=" << this->pluginSettings.islandChance << Common_Strings::STRING_NEW_LINE;
    stream << "Music=" << this->pluginSettings.music << Common_Strings::STRING_NEW_LINE;
    stream << "Combine_Music_With_Other_Packs=" << this->pluginSettings.combineMusicWithOtherPacks << Common_Strings::STRING_NEW_LINE;
    stream << "Tone_Color=" << this->pluginSettings.toneColor << Common_Strings::STRING_NEW_LINE;
    stream << "Graphics=" << this->pluginSettings.graphics << Common_Strings::STRING_NEW_LINE;
    stream << "Infinite_Lives=" << this->pluginSettings.infiniteLives << Common_Strings::STRING_NEW_LINE;
    stream << "Permadeath=" << this->pluginSettings.permadeath << Common_Strings::STRING_NEW_LINE;
    stream << "Number_Of_Lives=" << this->pluginSettings.numLives << Common_Strings::STRING_NEW_LINE;
    stream << "God_Mode=" << this->pluginSettings.godMode << Common_Strings::STRING_NEW_LINE;
    stream << "Add_Luigi_Game=" << this->pluginSettings.addLuigiGame << Common_Strings::STRING_NEW_LINE;
    stream << "Super_Mario_On_Damage=" << this->pluginSettings.superMarioOnDamage << Common_Strings::STRING_NEW_LINE;
    stream << "Lakitu_Throw_Arc=" << this->pluginSettings.lakituThrowArc << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Basic_Enemy_Speed=" << this->pluginSettings.difficultyBasicEnemySpeed << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Bullet_Bill_Speed=" << this->pluginSettings.difficultyBulletBillSpeed << Common_Strings::STRING_NEW_LINE;
    stream << "Difficulty_Speedy_Objects_And_Enemies=" << this->pluginSettings.difficultySpeedyObjectsAndEnemies << Common_Strings::STRING_NEW_LINE;
    stream << "Powerup=" << this->pluginSettings.powerup << Common_Strings::STRING_NEW_LINE;
    stream << "Secondary_Mushroom=" << this->pluginSettings.secondaryMushroom << Common_Strings::STRING_NEW_LINE;
    stream.flush();
    file.close();
    return true;
}

bool SMB1_Compliance_To_SMB1::Load_Plugin_Settings() {
    QFile file(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG + "/" + Common_Strings::STRING_PLUGIN_SETTINGS_FILENAME);
    if (!file.exists()) return true; //first time running the plugin
    if (!file.open(QIODevice::ReadWrite)) return false;
    bool valid = true;
    this->pluginSettings.tab = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.baseROM = this->Get_Setting_From_Line(file);
    this->pluginSettings.outputROMLocation = this->Get_Setting_From_Line(file);
    this->pluginSettings.overwriteOuputROM = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.randomNumWorlds = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.numWorlds = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.numLevelsPerWorld = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.generateNewLevels = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.levelScripts = this->Get_Setting_From_Line(file);
    this->pluginSettings.difficultyComboIndex = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyAutoScroll = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyBulletTime = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyHammerTime = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyWalkingHammerBros = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyBuzzyBeetlesReplaceLoneGoombas = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyBridgeFlyingCheepCheeps = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyBridgeLakitus = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyBridgeOffscreenBulletBills = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyCastleFireBars = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyCastleFlyingCheepCheeps = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyCastleLakitus = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyCastleOffscreenBulletBills = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyIslandFlyingCheepCheeps = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyIslandLakitus = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyIslandOffscreenBulletBills = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyUndergroundFlyingCheepCheeps = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyUndergroundLakitus = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyUndergroundOffscreenBulletBills = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyUnderwaterBloopers = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyUnderwaterFlyingCheepCheeps = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyUnderwaterLakitus = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyUnderwaterSwimmingCheepCheeps = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyStandardOverworldFlyingCheepCheeps = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyStandardOverworldLakitus = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyStandardOverworldOffscreenBulletBills = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyNoEnemies = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyStartWithFireFlowerOnRoomChange = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyHammerTimeIntensity = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyReplaceCastleLoops = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyAutoScrollChancePerLevel = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyLakituSpawnChancePerLevel = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyLakituRespawnSpeed = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultySpinyEggBehavior = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyDisableAllOtherEnemiesWhenALakituSpawns = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultySpawnerPriority = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyPiranhaPlantType = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.standardOverworldChance = this->Get_Setting_From_Line(file);
    this->pluginSettings.undergroundChance = this->Get_Setting_From_Line(file);
    this->pluginSettings.underwaterChance = this->Get_Setting_From_Line(file);
    this->pluginSettings.bridgeChance = this->Get_Setting_From_Line(file);
    this->pluginSettings.islandChance = this->Get_Setting_From_Line(file);
    this->pluginSettings.music = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.combineMusicWithOtherPacks = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.toneColor = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.graphics = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.infiniteLives = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.permadeath = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.numLives = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.godMode = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.addLuigiGame = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.superMarioOnDamage = static_cast<bool>(this->Get_Setting_From_Line(file).toInt(&valid)); if (!valid) return false;
    this->pluginSettings.lakituThrowArc = static_cast<bool>(this->Get_Setting_From_Line(file).toInt(&valid)); if (!valid) return false;
    this->pluginSettings.difficultyBasicEnemySpeed = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultyBulletBillSpeed = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.difficultySpeedyObjectsAndEnemies = static_cast<bool>(this->Get_Setting_From_Line(file).toInt(&valid)); if (!valid) return false;
    this->pluginSettings.powerup = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
    this->pluginSettings.secondaryMushroom = this->Get_Setting_From_Line(file).toInt(&valid); if (!valid) return false;
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
    this->pluginSettings.randomSeed = static_cast<unsigned int>(QTime::currentTime().msecsSinceStartOfDay());
    this->pluginSettings.randomNumWorlds = true;
    this->pluginSettings.music = 0;
    this->pluginSettings.combineMusicWithOtherPacks = true;
    this->pluginSettings.toneColor = 0;
    this->pluginSettings.graphics = 0;
    this->pluginSettings.infiniteLives = false;
    this->pluginSettings.permadeath = false;
    this->pluginSettings.numLives = 7;
    this->pluginSettings.godMode = false;
    this->pluginSettings.addLuigiGame = true;
    this->pluginSettings.superMarioOnDamage = true;
    this->pluginSettings.lakituThrowArc = true;
    this->pluginSettings.difficultyBasicEnemySpeed = 1;
    this->pluginSettings.difficultyBasicEnemySpeed = 2;
    this->pluginSettings.difficultySpeedyObjectsAndEnemies = false;
    this->pluginSettings.powerup = 0;
    this->pluginSettings.secondaryMushroom = 0;
    this->pluginSettings.difficultyComboIndex = 4;
    Difficulty_Level_Configurations().Normal(&this->pluginSettings, false);
}

QString SMB1_Compliance_To_SMB1::Get_Setting_From_Line(QFile &file) {
    QString setting = file.readLine().trimmed();
    int index = setting.indexOf('=');
    if (index == -1) return setting;
    return setting.remove(0, setting.indexOf('=')+1);
}

void SMB1_Compliance_To_SMB1::Update_ROM_Output_Location() {
    if (!this->pluginSettings.overwriteOuputROM) {
        while (QFileInfo(this->pluginSettings.outputROMLocation).exists()) {
            this->pluginSettings.outputROMLocation = this->Append_Number_To_FileName(this->pluginSettings.outputROMLocation);
        }
    }
}

void SMB1_Compliance_To_SMB1::Show_Message(const QString &message, bool error) {
    if (this->cliMode) {
        qDebug().noquote() << message;
    } else {
        if (error) QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED, message, Common_Strings::STRING_OK);
        else QMessageBox::information(this->parent, Common_Strings::STRING_LEVEL_HEADED, message, Common_Strings::STRING_OK);
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
