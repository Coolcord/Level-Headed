#include "SMB1_Compliance_To_SMB1.h"
#include "SMB1_Compliance_To_SMB1_Strings.h"
#include "../../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.h"
#include "../../Level-Headed/Common_Strings.h"
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
    Readable_Config_File configFile;
    if (!configFile.Open_Without_Loading(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG + "/" + Common_Strings::STRING_PLUGIN_SETTINGS_FILENAME)) return false;
    if (!configFile.Set_Value("Last_Tab", this->pluginSettings.tab)) return false;
    if (!configFile.Set_Value("Base_ROM", this->pluginSettings.baseROM)) return false;
    if (!configFile.Set_Value("Output_ROM_Location", this->pluginSettings.outputROMLocation)) return false;
    if (!configFile.Set_Value("Overwrite_Output_ROM", this->pluginSettings.overwriteOuputROM)) return false;
    if (!configFile.Set_Value("Random_Number_Of_Worlds", this->pluginSettings.randomNumWorlds)) return false;
    if (!configFile.Set_Value("Number_Of_Worlds", this->pluginSettings.numWorlds)) return false;
    if (!configFile.Set_Value("Numer_Of_Levels_Per_World", this->pluginSettings.numLevelsPerWorld)) return false;
    if (!configFile.Set_Value("Generate_New_Levels", this->pluginSettings.generateNewLevels)) return false;
    if (!configFile.Set_Value("Level_Scripts", this->pluginSettings.levelScripts)) return false;
    if (!configFile.Set_Value("Difficulty", this->pluginSettings.difficultyComboIndex)) return false;
    if (!configFile.Set_Value("Difficulty_Auto_Scroll", this->pluginSettings.difficultyAutoScroll)) return false;
    if (!configFile.Set_Value("Difficulty_Bullet_Time", this->pluginSettings.difficultyBulletTime)) return false;
    if (!configFile.Set_Value("Difficulty_Hammer_Time", this->pluginSettings.difficultyHammerTime)) return false;
    if (!configFile.Set_Value("Difficulty_Walking_Hammer_Bros", this->pluginSettings.difficultyWalkingHammerBros)) return false;
    if (!configFile.Set_Value("Difficulty_Buzzy_Beetles_Replace_Lone_Goombas", this->pluginSettings.difficultyBuzzyBeetlesReplaceLoneGoombas)) return false;
    if (!configFile.Set_Value("Difficulty_Bridge_Flying_Cheep_Cheeps", this->pluginSettings.difficultyBridgeFlyingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Bridge_Lakitus", this->pluginSettings.difficultyBridgeLakitus)) return false;
    if (!configFile.Set_Value("Difficulty_Bridge_Offscreen_Bullet_Bills", this->pluginSettings.difficultyBridgeOffscreenBulletBills)) return false;
    if (!configFile.Set_Value("Difficulty_Castle_Fire_Bars", this->pluginSettings.difficultyCastleFireBars)) return false;
    if (!configFile.Set_Value("Difficulty_Castle_Flying_Cheep_Cheeps", this->pluginSettings.difficultyCastleFlyingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Castle_Lakitus", this->pluginSettings.difficultyCastleLakitus)) return false;
    if (!configFile.Set_Value("Difficulty_Castle_Offscreen_Bullet_Bills", this->pluginSettings.difficultyCastleOffscreenBulletBills)) return false;
    if (!configFile.Set_Value("Difficulty_Island_Flying_Cheep_Cheeps", this->pluginSettings.difficultyIslandFlyingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Island_Lakitus", this->pluginSettings.difficultyIslandLakitus)) return false;
    if (!configFile.Set_Value("Difficulty_Island_Offscreen_Bullet_Bills", this->pluginSettings.difficultyIslandOffscreenBulletBills)) return false;
    if (!configFile.Set_Value("Difficulty_Underground_Flying_Cheep_Cheeps", this->pluginSettings.difficultyUndergroundFlyingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Underground_Lakitus", this->pluginSettings.difficultyUndergroundLakitus)) return false;
    if (!configFile.Set_Value("Difficulty_Underground_Offscreen_Bullet_Bills", this->pluginSettings.difficultyUndergroundOffscreenBulletBills)) return false;
    if (!configFile.Set_Value("Difficulty_Underwater_Bloopers", this->pluginSettings.difficultyUnderwaterBloopers)) return false;
    if (!configFile.Set_Value("Difficulty_Underwater_Flying_Cheep_Cheeps", this->pluginSettings.difficultyUnderwaterFlyingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Underwater_Lakitus", this->pluginSettings.difficultyUnderwaterLakitus)) return false;
    if (!configFile.Set_Value("Difficulty_Underwater_Swimming_Cheep_Cheeps", this->pluginSettings.difficultyUnderwaterSwimmingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Standard_Overworld_Flying_Cheep_Cheeps", this->pluginSettings.difficultyStandardOverworldFlyingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Standard_Overworld_Lakitus", this->pluginSettings.difficultyStandardOverworldLakitus)) return false;
    if (!configFile.Set_Value("Difficulty_Standard_Overworld_Offscreen_Bullet_Bills", this->pluginSettings.difficultyStandardOverworldOffscreenBulletBills)) return false;
    if (!configFile.Set_Value("Difficulty_No_Enemies", this->pluginSettings.difficultyNoEnemies)) return false;
    if (!configFile.Set_Value("Difficulty_Start_With_Fire_Flower_On_Room_Change", this->pluginSettings.difficultyStartWithFireFlowerOnRoomChange)) return false;
    if (!configFile.Set_Value("Difficulty_Hammer_Time_Intensity", this->pluginSettings.difficultyHammerTimeIntensity)) return false;
    if (!configFile.Set_Value("Difficulty_Replace_Castle_Loops", this->pluginSettings.difficultyReplaceCastleLoops)) return false;
    if (!configFile.Set_Value("Difficulty_Auto_Scroll_Chance_Per_Level", this->pluginSettings.difficultyAutoScrollChancePerLevel)) return false;
    if (!configFile.Set_Value("Difficulty_Lakitu_Spawn_Chance_Per_Level", this->pluginSettings.difficultyLakituSpawnChancePerLevel)) return false;
    if (!configFile.Set_Value("Difficulty_Lakitu_Respawn_Speed", this->pluginSettings.difficultyLakituRespawnSpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Spiny_Egg_Behavior", this->pluginSettings.difficultySpinyEggBehavior)) return false;
    if (!configFile.Set_Value("Difficulty_Disable_All_Other_Enemies_When_A_Lakitu_Spawns", this->pluginSettings.difficultyDisableAllOtherEnemiesWhenALakituSpawns)) return false;
    if (!configFile.Set_Value("Difficulty_Spawner_Priority", this->pluginSettings.difficultySpawnerPriority)) return false;
    if (!configFile.Set_Value("Difficulty_Piranha_Plant_Type", this->pluginSettings.difficultyPiranhaPlantType)) return false;
    if (!configFile.Set_Value("Standard_Overworld_Chance", this->pluginSettings.standardOverworldChance)) return false;
    if (!configFile.Set_Value("Underground_Chance", this->pluginSettings.undergroundChance)) return false;
    if (!configFile.Set_Value("Underwater_Chance", this->pluginSettings.underwaterChance)) return false;
    if (!configFile.Set_Value("Bridge_Chance", this->pluginSettings.bridgeChance)) return false;
    if (!configFile.Set_Value("Island_Chance", this->pluginSettings.islandChance)) return false;
    if (!configFile.Set_Value("Music", this->pluginSettings.music)) return false;
    if (!configFile.Set_Value("Combine_Music_With_Other_Packs", this->pluginSettings.combineMusicWithOtherPacks)) return false;
    if (!configFile.Set_Value("Tone_Color", this->pluginSettings.toneColor)) return false;
    if (!configFile.Set_Value("Random_Sound_Effects", this->pluginSettings.randomSounds)) return false;
    if (!configFile.Set_Value("Graphics", this->pluginSettings.graphics)) return false;
    if (!configFile.Set_Value("Infinite_Lives", this->pluginSettings.infiniteLives)) return false;
    if (!configFile.Set_Value("Permadeath", this->pluginSettings.permadeath)) return false;
    if (!configFile.Set_Value("Number_Of_Lives", this->pluginSettings.numLives)) return false;
    if (!configFile.Set_Value("God_Mode", this->pluginSettings.godMode)) return false;
    if (!configFile.Set_Value("Add_Luigi_Game", this->pluginSettings.addLuigiGame)) return false;
    if (!configFile.Set_Value("Super_Mario_On_Damage", this->pluginSettings.superMarioOnDamage)) return false;
    if (!configFile.Set_Value("Lakitu_Throw_Arc", this->pluginSettings.lakituThrowArc)) return false;
    if (!configFile.Set_Value("Difficulty_Basic_Enemy_Speed", this->pluginSettings.difficultyBasicEnemySpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Bullet_Bill_Speed", this->pluginSettings.difficultyBulletBillSpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Speedy_Objects_And_Enemies", this->pluginSettings.difficultySpeedyObjectsAndEnemies)) return false;
    if (!configFile.Set_Value("Powerup", this->pluginSettings.powerup)) return false;
    if (!configFile.Set_Value("Secondary_Mushroom", this->pluginSettings.secondaryMushroom)) return false;
    return configFile.Save_And_Close();
}

bool SMB1_Compliance_To_SMB1::Load_Plugin_Settings() {
    Readable_Config_File configFile;
    if (!configFile.Open(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG + "/" + Common_Strings::STRING_PLUGIN_SETTINGS_FILENAME)) return false;
    configFile.Get_Value("Last_Tab", this->pluginSettings.tab);
    configFile.Get_Value("Base_ROM", this->pluginSettings.baseROM);
    configFile.Get_Value("Output_ROM_Location", this->pluginSettings.outputROMLocation);
    configFile.Get_Value("Overwrite_Output_ROM", this->pluginSettings.overwriteOuputROM);
    configFile.Get_Value("Random_Number_Of_Worlds", this->pluginSettings.randomNumWorlds);
    configFile.Get_Value("Number_Of_Worlds", this->pluginSettings.numWorlds);
    configFile.Get_Value("Numer_Of_Levels_Per_World", this->pluginSettings.numLevelsPerWorld);
    configFile.Get_Value("Generate_New_Levels", this->pluginSettings.generateNewLevels);
    configFile.Get_Value("Level_Scripts", this->pluginSettings.levelScripts);
    configFile.Get_Value("Difficulty", this->pluginSettings.difficultyComboIndex);
    configFile.Get_Value("Difficulty_Auto_Scroll", this->pluginSettings.difficultyAutoScroll);
    configFile.Get_Value("Difficulty_Bullet_Time", this->pluginSettings.difficultyBulletTime);
    configFile.Get_Value("Difficulty_Hammer_Time", this->pluginSettings.difficultyHammerTime);
    configFile.Get_Value("Difficulty_Walking_Hammer_Bros", this->pluginSettings.difficultyWalkingHammerBros);
    configFile.Get_Value("Difficulty_Buzzy_Beetles_Replace_Lone_Goombas", this->pluginSettings.difficultyBuzzyBeetlesReplaceLoneGoombas);
    configFile.Get_Value("Difficulty_Bridge_Flying_Cheep_Cheeps", this->pluginSettings.difficultyBridgeFlyingCheepCheeps);
    configFile.Get_Value("Difficulty_Bridge_Lakitus", this->pluginSettings.difficultyBridgeLakitus);
    configFile.Get_Value("Difficulty_Bridge_Offscreen_Bullet_Bills", this->pluginSettings.difficultyBridgeOffscreenBulletBills);
    configFile.Get_Value("Difficulty_Castle_Fire_Bars", this->pluginSettings.difficultyCastleFireBars);
    configFile.Get_Value("Difficulty_Castle_Flying_Cheep_Cheeps", this->pluginSettings.difficultyCastleFlyingCheepCheeps);
    configFile.Get_Value("Difficulty_Castle_Lakitus", this->pluginSettings.difficultyCastleLakitus);
    configFile.Get_Value("Difficulty_Castle_Offscreen_Bullet_Bills", this->pluginSettings.difficultyCastleOffscreenBulletBills);
    configFile.Get_Value("Difficulty_Island_Flying_Cheep_Cheeps", this->pluginSettings.difficultyIslandFlyingCheepCheeps);
    configFile.Get_Value("Difficulty_Island_Lakitus", this->pluginSettings.difficultyIslandLakitus);
    configFile.Get_Value("Difficulty_Island_Offscreen_Bullet_Bills", this->pluginSettings.difficultyIslandOffscreenBulletBills);
    configFile.Get_Value("Difficulty_Underground_Flying_Cheep_Cheeps", this->pluginSettings.difficultyUndergroundFlyingCheepCheeps);
    configFile.Get_Value("Difficulty_Underground_Lakitus", this->pluginSettings.difficultyUndergroundLakitus);
    configFile.Get_Value("Difficulty_Underground_Offscreen_Bullet_Bills", this->pluginSettings.difficultyUndergroundOffscreenBulletBills);
    configFile.Get_Value("Difficulty_Underwater_Bloopers", this->pluginSettings.difficultyUnderwaterBloopers);
    configFile.Get_Value("Difficulty_Underwater_Flying_Cheep_Cheeps", this->pluginSettings.difficultyUnderwaterFlyingCheepCheeps);
    configFile.Get_Value("Difficulty_Underwater_Lakitus", this->pluginSettings.difficultyUnderwaterLakitus);
    configFile.Get_Value("Difficulty_Underwater_Swimming_Cheep_Cheeps", this->pluginSettings.difficultyUnderwaterSwimmingCheepCheeps);
    configFile.Get_Value("Difficulty_Standard_Overworld_Flying_Cheep_Cheeps", this->pluginSettings.difficultyStandardOverworldFlyingCheepCheeps);
    configFile.Get_Value("Difficulty_Standard_Overworld_Lakitus", this->pluginSettings.difficultyStandardOverworldLakitus);
    configFile.Get_Value("Difficulty_Standard_Overworld_Offscreen_Bullet_Bills", this->pluginSettings.difficultyStandardOverworldOffscreenBulletBills);
    configFile.Get_Value("Difficulty_No_Enemies", this->pluginSettings.difficultyNoEnemies);
    configFile.Get_Value("Difficulty_Start_With_Fire_Flower_On_Room_Change", this->pluginSettings.difficultyStartWithFireFlowerOnRoomChange);
    configFile.Get_Value("Difficulty_Hammer_Time_Intensity", this->pluginSettings.difficultyHammerTimeIntensity);
    configFile.Get_Value("Difficulty_Replace_Castle_Loops", this->pluginSettings.difficultyReplaceCastleLoops);
    configFile.Get_Value("Difficulty_Auto_Scroll_Chance_Per_Level", this->pluginSettings.difficultyAutoScrollChancePerLevel);
    configFile.Get_Value("Difficulty_Lakitu_Spawn_Chance_Per_Level", this->pluginSettings.difficultyLakituSpawnChancePerLevel);
    configFile.Get_Value("Difficulty_Lakitu_Respawn_Speed", this->pluginSettings.difficultyLakituRespawnSpeed);
    configFile.Get_Value("Difficulty_Spiny_Egg_Behavior", this->pluginSettings.difficultySpinyEggBehavior);
    configFile.Get_Value("Difficulty_Disable_All_Other_Enemies_When_A_Lakitu_Spawns", this->pluginSettings.difficultyDisableAllOtherEnemiesWhenALakituSpawns);
    configFile.Get_Value("Difficulty_Spawner_Priority", this->pluginSettings.difficultySpawnerPriority);
    configFile.Get_Value("Difficulty_Piranha_Plant_Type", this->pluginSettings.difficultyPiranhaPlantType);
    configFile.Get_Value("Standard_Overworld_Chance", this->pluginSettings.standardOverworldChance);
    configFile.Get_Value("Underground_Chance", this->pluginSettings.undergroundChance);
    configFile.Get_Value("Underwater_Chance", this->pluginSettings.underwaterChance);
    configFile.Get_Value("Bridge_Chance", this->pluginSettings.bridgeChance);
    configFile.Get_Value("Island_Chance", this->pluginSettings.islandChance);
    configFile.Get_Value("Music", this->pluginSettings.music);
    configFile.Get_Value("Combine_Music_With_Other_Packs", this->pluginSettings.combineMusicWithOtherPacks);
    configFile.Get_Value("Tone_Color", this->pluginSettings.toneColor);
    configFile.Get_Value("Random_Sound_Effects", this->pluginSettings.randomSounds);
    configFile.Get_Value("Graphics", this->pluginSettings.graphics);
    configFile.Get_Value("Infinite_Lives", this->pluginSettings.infiniteLives);
    configFile.Get_Value("Permadeath", this->pluginSettings.permadeath);
    configFile.Get_Value("Number_Of_Lives", this->pluginSettings.numLives);
    configFile.Get_Value("God_Mode", this->pluginSettings.godMode);
    configFile.Get_Value("Add_Luigi_Game", this->pluginSettings.addLuigiGame);
    configFile.Get_Value("Super_Mario_On_Damage", this->pluginSettings.superMarioOnDamage);
    configFile.Get_Value("Lakitu_Throw_Arc", this->pluginSettings.lakituThrowArc);
    configFile.Get_Value("Difficulty_Basic_Enemy_Speed", this->pluginSettings.difficultyBasicEnemySpeed);
    configFile.Get_Value("Difficulty_Bullet_Bill_Speed", this->pluginSettings.difficultyBulletBillSpeed);
    configFile.Get_Value("Difficulty_Speedy_Objects_And_Enemies", this->pluginSettings.difficultySpeedyObjectsAndEnemies);
    configFile.Get_Value("Powerup", this->pluginSettings.powerup);
    configFile.Get_Value("Secondary_Mushroom", this->pluginSettings.secondaryMushroom);
    configFile.Discard_And_Close();
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
    this->pluginSettings.randomSounds = true;
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
