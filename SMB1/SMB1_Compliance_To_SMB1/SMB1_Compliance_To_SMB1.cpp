#include "SMB1_Compliance_To_SMB1.h"
#include "SMB1_Compliance_To_SMB1_Strings.h"
#include "../../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "../../Common_Files/Version.h"
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
    Readable_Config_File configFile;
    if (!configFile.Open_Without_Loading(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG + "/" + Common_Strings::STRING_PLUGIN_SETTINGS_FILENAME)) return false;
    if (!configFile.Set_Value("Version", Version::VERSION)) return false;
    if (!configFile.Set_Value("Last_Tab", this->pluginSettings.tab)) return false;
    if (!configFile.Set_Value("Base_ROM", this->pluginSettings.baseROM)) return false;
    if (!configFile.Set_Value("Output_ROM_Location", this->pluginSettings.outputROMLocation)) return false;
    if (!configFile.Set_Value("Overwrite_Output_ROM", this->pluginSettings.overwriteOuputROM)) return false;
    if (!configFile.Set_Value("Random_Number_Of_Worlds", this->pluginSettings.randomNumWorlds)) return false;
    if (!configFile.Set_Value("Number_Of_Worlds", this->pluginSettings.numWorlds)) return false;
    if (!configFile.Set_Value("Numer_Of_Levels_Per_World", this->pluginSettings.numLevelsPerWorld)) return false;
    if (!configFile.Set_Value("Generate_New_Levels", this->pluginSettings.generateNewLevels)) return false;
    if (!configFile.Set_Value("Redistribute_Powerups", this->pluginSettings.redistributePowerups)) return false;
    if (!configFile.Set_Value("Randomize_Warp_Zones", this->pluginSettings.randomizeWarpZones)) return false;
    if (!configFile.Set_Value("Level_Script_Enemies", this->pluginSettings.levelScriptEnemies)) return false;
    if (!configFile.Set_Value("No_Auto_Scrolling_Levels", this->pluginSettings.noAutoScrollingLevels)) return false;
    if (!configFile.Set_Value("Standard_Level_Distribution", this->pluginSettings.standardLevelDistribution)) return false;
    if (!configFile.Set_Value("Include_Standard_Overworld_Levels_In_Random_Distribution", this->pluginSettings.includeStandardOverworldLevelsInRandomDistribution)) return false;
    if (!configFile.Set_Value("Include_Underground_Levels_In_Random_Distribution", this->pluginSettings.includeUndergroundLevelsInRandomDistribution)) return false;
    if (!configFile.Set_Value("Include_Underwater_Levels_In_Random_Distribution", this->pluginSettings.includeUnderwaterLevelsInRandomDistribution)) return false;
    if (!configFile.Set_Value("Include_Bridge_Levels_In_Random_Distribution", this->pluginSettings.includeBridgeLevelsInRandomDistribution)) return false;
    if (!configFile.Set_Value("Include_Island_Levels_In_Random_Distribution", this->pluginSettings.includeIslandLevelsInRandomDistribution)) return false;
    if (!configFile.Set_Value("SMB_Utility_Compatibility", this->pluginSettings.smbUtilityCompatibility)) return false;
    if (!configFile.Set_Value("Level_Scripts", this->pluginSettings.levelScripts)) return false;
    if (!configFile.Set_Value("Difficulty", this->pluginSettings.difficultyComboIndex)) return false;
    if (!configFile.Set_Value("Difficulty_Auto_Scroll", this->pluginSettings.difficultyAutoScroll)) return false;
    if (!configFile.Set_Value("Difficulty_Auto_Scroll_With_Flying_Cheep_Cheeps", this->pluginSettings.difficultyAutoScrollWithFlyingCheepCheeps)) return false;
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
    if (!configFile.Set_Value("Difficulty_Underwater_Hammer_Bros", this->pluginSettings.difficultyUnderwaterHammerBros)) return false;
    if (!configFile.Set_Value("Difficulty_Underwater_Lakitus", this->pluginSettings.difficultyUnderwaterLakitus)) return false;
    if (!configFile.Set_Value("Difficulty_Underwater_Swimming_Cheep_Cheeps", this->pluginSettings.difficultyUnderwaterSwimmingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Standard_Overworld_Flying_Cheep_Cheeps", this->pluginSettings.difficultyStandardOverworldFlyingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Standard_Overworld_Lakitus", this->pluginSettings.difficultyStandardOverworldLakitus)) return false;
    if (!configFile.Set_Value("Difficulty_Standard_Overworld_Offscreen_Bullet_Bills", this->pluginSettings.difficultyStandardOverworldOffscreenBulletBills)) return false;
    if (!configFile.Set_Value("Difficulty_Minimum_Enemy_Distance", this->pluginSettings.difficultyMinimumEnemyDistance)) return false;
    if (!configFile.Set_Value("Difficulty_Minimum_Underwater_Enemy_Distance", this->pluginSettings.difficultyMinimumUnderwaterEnemyDistance)) return false;
    if (!configFile.Set_Value("Difficulty_No_Enemies", this->pluginSettings.difficultyNoEnemies)) return false;
    if (!configFile.Set_Value("Difficulty_Unlimited_Time", this->pluginSettings.difficultyUnlimitedTime)) return false;
    if (!configFile.Set_Value("Difficulty_Hammer_Time_Intensity", this->pluginSettings.difficultyHammerTimeIntensity)) return false;
    if (!configFile.Set_Value("Difficulty_Replace_Castle_Loops", this->pluginSettings.difficultyReplaceCastleLoops)) return false;
    if (!configFile.Set_Value("Difficulty_Max_Level_Length", this->pluginSettings.difficultyMaxLevelLength)) return false;
    if (!configFile.Set_Value("Difficulty_Auto_Scroll_Chance_Per_Level", this->pluginSettings.difficultyAutoScrollChancePerLevel)) return false;
    if (!configFile.Set_Value("Difficulty_Lakitu_Spawn_Chance_Per_Level", this->pluginSettings.difficultyLakituSpawnChancePerLevel)) return false;
    if (!configFile.Set_Value("Difficulty_Lakitu_Respawn_Speed", this->pluginSettings.difficultyLakituRespawnSpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Spiny_Egg_Behavior", this->pluginSettings.difficultySpinyEggBehavior)) return false;
    if (!configFile.Set_Value("Difficulty_Disable_All_Other_Enemies_When_A_Lakitu_Spawns", this->pluginSettings.difficultyDisableAllOtherEnemiesWhenALakituSpawns)) return false;
    if (!configFile.Set_Value("Difficulty_Disable_All_Other_Enemies_When_Flying_Cheep_Cheeps_Spawn", this->pluginSettings.difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn)) return false;
    if (!configFile.Set_Value("Difficulty_Spawner_Priority", this->pluginSettings.difficultySpawnerPriority)) return false;
    if (!configFile.Set_Value("Difficulty_Secondary_Mushroom", this->pluginSettings.difficultySecondaryMushroom)) return false;
    if (!configFile.Set_Value("Difficulty_Surfing_Lift_Speed", this->pluginSettings.difficultySurfingLiftSpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Piranha_Plant_Type", this->pluginSettings.difficultyPiranhaPlantType)) return false;
    if (!configFile.Set_Value("Difficulty_Max_Number_Of_Piranha_Plants", this->pluginSettings.difficultyMaxNumberOfPiranhaPlants)) return false;
    if (!configFile.Set_Value("Standard_Overworld_Chance", this->pluginSettings.standardOverworldChance)) return false;
    if (!configFile.Set_Value("Underground_Chance", this->pluginSettings.undergroundChance)) return false;
    if (!configFile.Set_Value("Underwater_Chance", this->pluginSettings.underwaterChance)) return false;
    if (!configFile.Set_Value("Bridge_Chance", this->pluginSettings.bridgeChance)) return false;
    if (!configFile.Set_Value("Island_Chance", this->pluginSettings.islandChance)) return false;
    if (!configFile.Set_Value("Music", this->pluginSettings.music)) return false;
    if (!configFile.Set_Value("Combine_Music_With_Other_Packs", this->pluginSettings.combineMusicWithOtherPacks)) return false;
    if (!configFile.Set_Value("Tone_Color", this->pluginSettings.toneColor)) return false;
    if (!configFile.Set_Value("Random_Sound_Effects", this->pluginSettings.randomSounds)) return false;
    if (!configFile.Set_Value("Random_Text", this->pluginSettings.randomText)) return false;
    if (!configFile.Set_Value("Randomize_Some_Animations", this->pluginSettings.randomizeSomeAnimations)) return false;
    if (!configFile.Set_Value("Graphics", this->pluginSettings.graphics)) return false;
    if (!configFile.Set_Value("Combine_Graphics_With_Other_Packs", this->pluginSettings.combineGraphicsWithOtherPacks)) return false;
    if (!configFile.Set_Value("Palette", this->pluginSettings.palette)) return false;
    if (!configFile.Set_Value("Mario_Sprite", this->pluginSettings.marioSprite)) return false;
    if (!configFile.Set_Value("Mario_Sprite_Palette", this->pluginSettings.marioSpritePalette)) return false;
    if (!configFile.Set_Value("Infinite_Lives", this->pluginSettings.infiniteLives)) return false;
    if (!configFile.Set_Value("Permadeath", this->pluginSettings.permadeath)) return false;
    if (!configFile.Set_Value("Number_Of_Lives", this->pluginSettings.numLives)) return false;
    if (!configFile.Set_Value("God_Mode", this->pluginSettings.godMode)) return false;
    if (!configFile.Set_Value("Add_Luigi_Game", this->pluginSettings.addLuigiGame)) return false;
    if (!configFile.Set_Value("Super_Mario_On_Damage", this->pluginSettings.superMarioOnDamage)) return false;
    if (!configFile.Set_Value("Difficulty_European_Blooper_Swim_Height", this->pluginSettings.difficultyEuropeanBlooperSwimHeight)) return false;
    if (!configFile.Set_Value("Lakitu_Throw_Arc", this->pluginSettings.lakituThrowArc)) return false;
    if (!configFile.Set_Value("Difficulty_Basic_Enemy_Speed", this->pluginSettings.difficultyBasicEnemySpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Bowser_Flame_Frequency", this->pluginSettings.difficultyBowserFlameFrequency)) return false;
    if (!configFile.Set_Value("Difficulty_Bullet_Bill_Firing_Rate", this->pluginSettings.difficultyBulletBillFiringRate)) return false;
    if (!configFile.Set_Value("Difficulty_Bullet_Bill_Speed", this->pluginSettings.difficultyBulletBillSpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Leaping_Paratroopa_Speed", this->pluginSettings.difficultyLeapingParatroopaSpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Enemy_Revival_Speed", this->pluginSettings.difficultyEnemyRevivalSpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Fire_Bar_Length", this->pluginSettings.difficultyFireBarLength)) return false;
    if (!configFile.Set_Value("Difficulty_Long_Fire_Bar_Length", this->pluginSettings.difficultyLongFireBarLength)) return false;
    if (!configFile.Set_Value("Difficulty_Flying_Cheep_Cheep_Jump_Height", this->pluginSettings.difficultyFlyingCheepCheepJumpHeight)) return false;
    if (!configFile.Set_Value("Difficulty_Hammer_Bros_Throw_Rate", this->pluginSettings.difficultyHammerBrosThrowRate)) return false;
    if (!configFile.Set_Value("Difficulty_Hammer_Bros_Never_Jump", this->pluginSettings.difficultyHammerBrosNeverJump)) return false;
    if (!configFile.Set_Value("Difficulty_Fire_Bowser_Throws_A_Ludicrous_Amount_Of_Fireballs", this->pluginSettings.difficultyFireBowserThrowsALudicrousAmountOfFireballs)) return false;
    if (!configFile.Set_Value("Difficulty_Speedy_Objects_And_Enemies", this->pluginSettings.difficultySpeedyObjectsAndEnemies)) return false;
    if (!configFile.Set_Value("Difficulty_Piranha_Plants_On_First_Level", this->pluginSettings.difficultyPiranhaPlantsOnFirstLevel)) return false;
    if (!configFile.Set_Value("Difficulty_Prevent_The_First_Level_From_Being_Underwater", this->pluginSettings.difficultyPreventTheFirstLevelFromBeingUnderwater)) return false;
    if (!configFile.Set_Value("Difficulty_Prevent_Hammer_Time_From_Spawning_Two_Hammer_Bros_In_A_Row", this->pluginSettings.difficultyPreventHammerTimeFromSpawningTwoHammerBrosInARow)) return false;
    if (!configFile.Set_Value("Difficulty_Allow_Hammer_Bros_When_Randomizing_Enemies_In_Level_Scripts", this->pluginSettings.difficultyAllowHammerBrosWhenRandomizingEnemiesInLevelScripts)) return false;
    if (!configFile.Set_Value("Difficulty_Allow_Hammer_Bros_Groups_When_Randomizing_Enemies_In_Level_Scripts", this->pluginSettings.difficultyAllowHammerBrosGroupsWhenRandomizingEnemiesInLevelScripts)) return false;
    if (!configFile.Set_Value("Difficulty_Allow_Lakitus_When_Randomizing_Enemies_In_Level_Scripts", this->pluginSettings.difficultyAllowLakitusWhenRandomizingEnemiesInLevelScripts)) return false;
    if (!configFile.Set_Value("Difficulty_Allow_Bullet_Bill_And_Cheep_Cheep_Spawners_When_Randomizing_Enemies_In_Level_Scripts", this->pluginSettings.difficultyAllowBulletBillAndCheepCheepSpawnersWhenRandomizingEnemiesInLevelScripts)) return false;
    if (!configFile.Set_Value("Difficulty_Max_Powerups", this->pluginSettings.difficultyMaxPowerups)) return false;
    if (!configFile.Set_Value("Difficulty_Max_Hidden_Powerups", this->pluginSettings.difficultyMaxHiddenPowerups)) return false;
    if (!configFile.Set_Value("Difficulty_Max_One_Ups", this->pluginSettings.difficultyMaxOneUps)) return false;
    if (!configFile.Set_Value("Difficulty_Max_Ten_Coin_Blocks", this->pluginSettings.difficultyMaxTenCoinBlocks)) return false;
    if (!configFile.Set_Value("Difficulty_Max_Stars", this->pluginSettings.difficultyMaxStars)) return false;
    if (!configFile.Set_Value("Difficulty_Hidden_Powerup_Chance", this->pluginSettings.difficultyHiddenPowerupChance)) return false;
    if (!configFile.Set_Value("Difficulty_One_Up_Chance", this->pluginSettings.difficultyOneUpChance)) return false;
    if (!configFile.Set_Value("Difficulty_Ten_Coin_Block_Chance", this->pluginSettings.difficultyTenCoinBlockChance)) return false;
    if (!configFile.Set_Value("Difficulty_Star_Chance", this->pluginSettings.difficultyStarChance)) return false;
    if (!configFile.Set_Value("Powerup", this->pluginSettings.powerup)) return false;
    if (!configFile.Set_Value("Mario_Name", this->pluginSettings.marioName)) return false;
    if (!configFile.Set_Value("Luigi_Name", this->pluginSettings.luigiName)) return false;
    return configFile.Save_And_Close();
}

bool SMB1_Compliance_To_SMB1::Load_Plugin_Settings() {
    Readable_Config_File configFile;
    if (!configFile.Open(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG + "/" + Common_Strings::STRING_PLUGIN_SETTINGS_FILENAME)) return false;
    this->pluginSettings.fireFlowerBouncesLikeAStar = false;
    configFile.Get_Value("Last_Tab", this->pluginSettings.tab);
    configFile.Get_Value("Base_ROM", this->pluginSettings.baseROM);
    configFile.Get_Value("Output_ROM_Location", this->pluginSettings.outputROMLocation);
    configFile.Get_Value("Overwrite_Output_ROM", this->pluginSettings.overwriteOuputROM);
    configFile.Get_Value("Random_Number_Of_Worlds", this->pluginSettings.randomNumWorlds);
    configFile.Get_Value("Number_Of_Worlds", this->pluginSettings.numWorlds);
    configFile.Get_Value("Numer_Of_Levels_Per_World", this->pluginSettings.numLevelsPerWorld);
    configFile.Get_Value("Generate_New_Levels", this->pluginSettings.generateNewLevels);
    configFile.Get_Value("Redistribute_Powerups", this->pluginSettings.redistributePowerups);
    configFile.Get_Value("Randomize_Warp_Zones", this->pluginSettings.randomizeWarpZones);
    configFile.Get_Value("Level_Script_Enemies", this->pluginSettings.levelScriptEnemies);
    configFile.Get_Value("No_Auto_Scrolling_Levels", this->pluginSettings.noAutoScrollingLevels);
    configFile.Get_Value("Standard_Level_Distribution", this->pluginSettings.standardLevelDistribution);
    configFile.Get_Value("Include_Standard_Overworld_Levels_In_Random_Distribution", this->pluginSettings.includeStandardOverworldLevelsInRandomDistribution);
    configFile.Get_Value("Include_Underground_Levels_In_Random_Distribution", this->pluginSettings.includeUndergroundLevelsInRandomDistribution);
    configFile.Get_Value("Include_Underwater_Levels_In_Random_Distribution", this->pluginSettings.includeUnderwaterLevelsInRandomDistribution);
    configFile.Get_Value("Include_Bridge_Levels_In_Random_Distribution", this->pluginSettings.includeBridgeLevelsInRandomDistribution);
    configFile.Get_Value("Include_Island_Levels_In_Random_Distribution", this->pluginSettings.includeIslandLevelsInRandomDistribution);
    configFile.Get_Value("SMB_Utility_Compatibility", this->pluginSettings.smbUtilityCompatibility);
    configFile.Get_Value("Level_Scripts", this->pluginSettings.levelScripts);
    configFile.Get_Value("Difficulty", this->pluginSettings.difficultyComboIndex);
    configFile.Get_Value("Difficulty_Auto_Scroll", this->pluginSettings.difficultyAutoScroll);
    configFile.Get_Value("Difficulty_Auto_Scroll_With_Flying_Cheep_Cheeps", this->pluginSettings.difficultyAutoScrollWithFlyingCheepCheeps);
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
    configFile.Get_Value("Difficulty_Underwater_Hammer_Bros", this->pluginSettings.difficultyUnderwaterHammerBros);
    configFile.Get_Value("Difficulty_Underwater_Lakitus", this->pluginSettings.difficultyUnderwaterLakitus);
    configFile.Get_Value("Difficulty_Underwater_Swimming_Cheep_Cheeps", this->pluginSettings.difficultyUnderwaterSwimmingCheepCheeps);
    configFile.Get_Value("Difficulty_Standard_Overworld_Flying_Cheep_Cheeps", this->pluginSettings.difficultyStandardOverworldFlyingCheepCheeps);
    configFile.Get_Value("Difficulty_Standard_Overworld_Lakitus", this->pluginSettings.difficultyStandardOverworldLakitus);
    configFile.Get_Value("Difficulty_Standard_Overworld_Offscreen_Bullet_Bills", this->pluginSettings.difficultyStandardOverworldOffscreenBulletBills);
    configFile.Get_Value("Difficulty_Minimum_Enemy_Distance", this->pluginSettings.difficultyMinimumEnemyDistance);
    configFile.Get_Value("Difficulty_Minimum_Underwater_Enemy_Distance", this->pluginSettings.difficultyMinimumUnderwaterEnemyDistance);
    configFile.Get_Value("Difficulty_No_Enemies", this->pluginSettings.difficultyNoEnemies);
    configFile.Get_Value("Difficulty_Unlimited_Time", this->pluginSettings.difficultyUnlimitedTime);
    configFile.Get_Value("Difficulty_Hammer_Time_Intensity", this->pluginSettings.difficultyHammerTimeIntensity);
    configFile.Get_Value("Difficulty_Replace_Castle_Loops", this->pluginSettings.difficultyReplaceCastleLoops);
    configFile.Get_Value("Difficulty_Max_Level_Length", this->pluginSettings.difficultyMaxLevelLength);
    configFile.Get_Value("Difficulty_Auto_Scroll_Chance_Per_Level", this->pluginSettings.difficultyAutoScrollChancePerLevel);
    configFile.Get_Value("Difficulty_Lakitu_Spawn_Chance_Per_Level", this->pluginSettings.difficultyLakituSpawnChancePerLevel);
    configFile.Get_Value("Difficulty_Lakitu_Respawn_Speed", this->pluginSettings.difficultyLakituRespawnSpeed);
    configFile.Get_Value("Difficulty_Spiny_Egg_Behavior", this->pluginSettings.difficultySpinyEggBehavior);
    configFile.Get_Value("Difficulty_Disable_All_Other_Enemies_When_A_Lakitu_Spawns", this->pluginSettings.difficultyDisableAllOtherEnemiesWhenALakituSpawns);
    configFile.Get_Value("Difficulty_Disable_All_Other_Enemies_When_Flying_Cheep_Cheeps_Spawn", this->pluginSettings.difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn);
    configFile.Get_Value("Difficulty_Spawner_Priority", this->pluginSettings.difficultySpawnerPriority);
    configFile.Get_Value("Difficulty_Secondary_Mushroom", this->pluginSettings.difficultySecondaryMushroom);
    configFile.Get_Value("Difficulty_Surfing_Lift_Speed", this->pluginSettings.difficultySurfingLiftSpeed);
    configFile.Get_Value("Difficulty_Piranha_Plant_Type", this->pluginSettings.difficultyPiranhaPlantType);
    configFile.Get_Value("Difficulty_Max_Number_Of_Piranha_Plants", this->pluginSettings.difficultyMaxNumberOfPiranhaPlants);
    configFile.Get_Value("Standard_Overworld_Chance", this->pluginSettings.standardOverworldChance);
    configFile.Get_Value("Underground_Chance", this->pluginSettings.undergroundChance);
    configFile.Get_Value("Underwater_Chance", this->pluginSettings.underwaterChance);
    configFile.Get_Value("Bridge_Chance", this->pluginSettings.bridgeChance);
    configFile.Get_Value("Island_Chance", this->pluginSettings.islandChance);
    configFile.Get_Value("Music", this->pluginSettings.music);
    configFile.Get_Value("Combine_Music_With_Other_Packs", this->pluginSettings.combineMusicWithOtherPacks);
    configFile.Get_Value("Tone_Color", this->pluginSettings.toneColor);
    configFile.Get_Value("Random_Sound_Effects", this->pluginSettings.randomSounds);
    configFile.Get_Value("Random_Text", this->pluginSettings.randomText);
    configFile.Get_Value("Randomize_Some_Animations", this->pluginSettings.randomizeSomeAnimations);
    configFile.Get_Value("Graphics", this->pluginSettings.graphics);
    configFile.Get_Value("Combine_Graphics_With_Other_Packs", this->pluginSettings.combineGraphicsWithOtherPacks);
    configFile.Get_Value("Palette", this->pluginSettings.palette);
    configFile.Get_Value("Mario_Sprite", this->pluginSettings.marioSprite);
    configFile.Get_Value("Mario_Sprite_Palette", this->pluginSettings.marioSpritePalette);
    configFile.Get_Value("Infinite_Lives", this->pluginSettings.infiniteLives);
    configFile.Get_Value("Permadeath", this->pluginSettings.permadeath);
    configFile.Get_Value("Number_Of_Lives", this->pluginSettings.numLives);
    configFile.Get_Value("God_Mode", this->pluginSettings.godMode);
    configFile.Get_Value("Add_Luigi_Game", this->pluginSettings.addLuigiGame);
    configFile.Get_Value("Super_Mario_On_Damage", this->pluginSettings.superMarioOnDamage);
    configFile.Get_Value("Difficulty_European_Blooper_Swim_Height", this->pluginSettings.difficultyEuropeanBlooperSwimHeight);
    configFile.Get_Value("Lakitu_Throw_Arc", this->pluginSettings.lakituThrowArc);
    configFile.Get_Value("Difficulty_Basic_Enemy_Speed", this->pluginSettings.difficultyBasicEnemySpeed);
    configFile.Get_Value("Difficulty_Bowser_Flame_Frequency", this->pluginSettings.difficultyBowserFlameFrequency);
    configFile.Get_Value("Difficulty_Bullet_Bill_Firing_Rate", this->pluginSettings.difficultyBulletBillFiringRate);
    configFile.Get_Value("Difficulty_Bullet_Bill_Speed", this->pluginSettings.difficultyBulletBillSpeed);
    configFile.Get_Value("Difficulty_Leaping_Paratroopa_Speed", this->pluginSettings.difficultyLeapingParatroopaSpeed);
    configFile.Get_Value("Difficulty_Enemy_Revival_Speed", this->pluginSettings.difficultyEnemyRevivalSpeed);
    configFile.Get_Value("Difficulty_Fire_Bar_Length", this->pluginSettings.difficultyFireBarLength);
    configFile.Get_Value("Difficulty_Long_Fire_Bar_Length", this->pluginSettings.difficultyLongFireBarLength);
    configFile.Get_Value("Difficulty_Flying_Cheep_Cheep_Jump_Height", this->pluginSettings.difficultyFlyingCheepCheepJumpHeight);
    configFile.Get_Value("Difficulty_Hammer_Bros_Throw_Rate", this->pluginSettings.difficultyHammerBrosThrowRate);
    configFile.Get_Value("Difficulty_Hammer_Bros_Never_Jump", this->pluginSettings.difficultyHammerBrosNeverJump);
    configFile.Get_Value("Difficulty_Fire_Bowser_Throws_A_Ludicrous_Amount_Of_Fireballs", this->pluginSettings.difficultyFireBowserThrowsALudicrousAmountOfFireballs);
    configFile.Get_Value("Difficulty_Speedy_Objects_And_Enemies", this->pluginSettings.difficultySpeedyObjectsAndEnemies);
    configFile.Get_Value("Difficulty_Piranha_Plants_On_First_Level", this->pluginSettings.difficultyPiranhaPlantsOnFirstLevel);
    configFile.Get_Value("Difficulty_Prevent_The_First_Level_From_Being_Underwater", this->pluginSettings.difficultyPreventTheFirstLevelFromBeingUnderwater);
    configFile.Get_Value("Difficulty_Prevent_Hammer_Time_From_Spawning_Two_Hammer_Bros_In_A_Row", this->pluginSettings.difficultyPreventHammerTimeFromSpawningTwoHammerBrosInARow);
    configFile.Get_Value("Difficulty_Allow_Hammer_Bros_When_Randomizing_Enemies_In_Level_Scripts", this->pluginSettings.difficultyAllowHammerBrosWhenRandomizingEnemiesInLevelScripts);
    configFile.Get_Value("Difficulty_Allow_Hammer_Bros_Groups_When_Randomizing_Enemies_In_Level_Scripts", this->pluginSettings.difficultyAllowHammerBrosGroupsWhenRandomizingEnemiesInLevelScripts);
    configFile.Get_Value("Difficulty_Allow_Lakitus_When_Randomizing_Enemies_In_Level_Scripts", this->pluginSettings.difficultyAllowLakitusWhenRandomizingEnemiesInLevelScripts);
    configFile.Get_Value("Difficulty_Allow_Bullet_Bill_And_Cheep_Cheep_Spawners_When_Randomizing_Enemies_In_Level_Scripts", this->pluginSettings.difficultyAllowBulletBillAndCheepCheepSpawnersWhenRandomizingEnemiesInLevelScripts);
    configFile.Get_Value("Difficulty_Max_Powerups", this->pluginSettings.difficultyMaxPowerups);
    configFile.Get_Value("Difficulty_Max_Hidden_Powerups", this->pluginSettings.difficultyMaxHiddenPowerups);
    configFile.Get_Value("Difficulty_Max_One_Ups", this->pluginSettings.difficultyMaxOneUps);
    configFile.Get_Value("Difficulty_Max_Ten_Coin_Blocks", this->pluginSettings.difficultyMaxTenCoinBlocks);
    configFile.Get_Value("Difficulty_Max_Stars", this->pluginSettings.difficultyMaxStars);
    configFile.Get_Value("Difficulty_Hidden_Powerup_Chance", this->pluginSettings.difficultyHiddenPowerupChance);
    configFile.Get_Value("Difficulty_One_Up_Chance", this->pluginSettings.difficultyOneUpChance);
    configFile.Get_Value("Difficulty_Ten_Coin_Block_Chance", this->pluginSettings.difficultyTenCoinBlockChance);
    configFile.Get_Value("Difficulty_Star_Chance", this->pluginSettings.difficultyStarChance);
    configFile.Get_Value("Powerup", this->pluginSettings.powerup);
    configFile.Get_Value("Mario_Name", this->pluginSettings.marioName);
    configFile.Get_Value("Luigi_Name", this->pluginSettings.luigiName);
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
