#include "Config_File_Handler.h"
#include "../../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../../Common_Files/Version.h"
#include "../SMB1_Writer/SMB1_Writer_Strings.h"
#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

Config_File_Handler::Config_File_Handler(QWidget *parent, const QString &applicationLocation) {
    this->parent = parent;
    this->applicationLocation = applicationLocation;
}

bool Config_File_Handler::Save_Plugin_Settings(Plugin_Settings *ps, const QString &configFileLocation, bool internalConfig) {
    Readable_Config_File configFile(!internalConfig);
    if (!configFile.Open_Without_Loading(configFileLocation)) return false;
    if (!configFile.Set_Value("Level_Scripts", ps->levelScripts)) return false;
    if (internalConfig) {
        if (!configFile.Set_Value("Last_Tab", ps->tab)) return false;
        if (!configFile.Set_Value("Output_ROM_Location", ps->outputROMLocation)) return false;
        if (!configFile.Set_Value("Overwrite_Output_ROM", ps->overwriteOuputROM)) return false;
        if (!configFile.Set_Value("Last_External_Config_Location", ps->lastExternalConfigLocation)) return false;
    } else {
        if (!configFile.Set_Value("Intended_Generator_Plugin", QString("SMB1_Compliance_Generator"))) return false;
        if (!configFile.Set_Value("Intended_Writer_Plugin", QString("SMB1_Writer"))) return false;

        //Save the intended checksums to the external config file
        QString romsArchiveLocation = this->applicationLocation+"/"+Common_Strings::STRING_DATA+"/"+Common_Strings::STRING_GAME_NAME+"/ROMs.sa";
        QString graphicsPacksArchiveLocation = this->applicationLocation+"/"+Common_Strings::STRING_DATA+"/"+Common_Strings::STRING_GAME_NAME+"/Graphics.sa";
        QString musicPacksArchiveLocation = this->applicationLocation+"/"+Common_Strings::STRING_DATA+"/"+Common_Strings::STRING_GAME_NAME+"/Music.sa";
        QString textArchiveLocation = this->applicationLocation+"/"+Common_Strings::STRING_DATA+"/"+Common_Strings::STRING_GAME_NAME+"/Text.sa";
        QString levelScriptsLocation = this->applicationLocation+"/"+Common_Strings::STRING_LEVELS+"/"+Common_Strings::STRING_GAME_NAME+"/"+ps->levelScripts+Common_Strings::STRING_LEVELS_EXTENSION;
        QString romsArchiveChecksum = this->Get_Checksum(romsArchiveLocation);
        QString graphicsPacksArchiveChecksum = this->Get_Checksum(graphicsPacksArchiveLocation);
        QString musicPacksArchiveChecksum = this->Get_Checksum(musicPacksArchiveLocation);
        QString textArchiveChecksum = this->Get_Checksum(textArchiveLocation);
        QString levelScriptsChecksum = this->Get_Checksum(levelScriptsLocation);
        if (romsArchiveChecksum.isEmpty() || graphicsPacksArchiveChecksum.isEmpty() || musicPacksArchiveChecksum.isEmpty() || textArchiveChecksum.isEmpty() || (!ps->generateNewLevels && levelScriptsChecksum.isEmpty())) return false;
        if (!configFile.Set_Value("Intended_ROMs_Archive_Checksum", romsArchiveChecksum)) return false;
        if (!configFile.Set_Value("Intended_Graphics_Pack_Archive_Checksum", graphicsPacksArchiveChecksum)) return false;
        if (!configFile.Set_Value("Intended_Music_Pack_Archive_Checksum", musicPacksArchiveChecksum)) return false;
        if (!configFile.Set_Value("Intended_Text_Archive_Checksum", textArchiveChecksum)) return false;
        if (!configFile.Set_Value("Intended_Level_Scripts_Checksum", levelScriptsChecksum)) return false;
    }
    if (!configFile.Set_Value("Version", Version::VERSION)) return false;
    if (!configFile.Set_Value("Base_ROM", ps->baseROM)) return false;
    if (!configFile.Set_Value("Random_Number_Of_Worlds", ps->randomNumWorlds)) return false;
    if (!configFile.Set_Value("Number_Of_Worlds", ps->numWorlds)) return false;
    if (!configFile.Set_Value("Numer_Of_Levels_Per_World", ps->numLevelsPerWorld)) return false;
    if (!configFile.Set_Value("Generate_New_Levels", ps->generateNewLevels)) return false;
    if (!configFile.Set_Value("Redistribute_Powerups", ps->redistributePowerups)) return false;
    if (!configFile.Set_Value("Randomize_Warp_Zones", ps->randomizeWarpZones)) return false;
    if (!configFile.Set_Value("Level_Script_Enemies", ps->levelScriptEnemies)) return false;
    if (!configFile.Set_Value("No_Auto_Scrolling_Levels", ps->noAutoScrollingLevels)) return false;
    if (!configFile.Set_Value("Standard_Level_Distribution", ps->standardLevelDistribution)) return false;
    if (!configFile.Set_Value("Include_Standard_Overworld_Levels_In_Random_Distribution", ps->includeStandardOverworldLevelsInRandomDistribution)) return false;
    if (!configFile.Set_Value("Include_Underground_Levels_In_Random_Distribution", ps->includeUndergroundLevelsInRandomDistribution)) return false;
    if (!configFile.Set_Value("Include_Underwater_Levels_In_Random_Distribution", ps->includeUnderwaterLevelsInRandomDistribution)) return false;
    if (!configFile.Set_Value("Include_Bridge_Levels_In_Random_Distribution", ps->includeBridgeLevelsInRandomDistribution)) return false;
    if (!configFile.Set_Value("Include_Island_Levels_In_Random_Distribution", ps->includeIslandLevelsInRandomDistribution)) return false;
    if (!configFile.Set_Value("SMB_Utility_Compatibility", ps->smbUtilityCompatibility)) return false;
    if (!configFile.Set_Value("Difficulty", ps->difficultyComboIndex)) return false;
    if (!configFile.Set_Value("Difficulty_Auto_Scroll", ps->difficultyAutoScroll)) return false;
    if (!configFile.Set_Value("Difficulty_Auto_Scroll_With_Flying_Cheep_Cheeps", ps->difficultyAutoScrollWithFlyingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Bullet_Time", ps->difficultyBulletTime)) return false;
    if (!configFile.Set_Value("Difficulty_Hammer_Time", ps->difficultyHammerTime)) return false;
    if (!configFile.Set_Value("Difficulty_Walking_Hammer_Bros", ps->difficultyWalkingHammerBros)) return false;
    if (!configFile.Set_Value("Difficulty_Buzzy_Beetles_Replace_Lone_Goombas", ps->difficultyBuzzyBeetlesReplaceLoneGoombas)) return false;
    if (!configFile.Set_Value("Difficulty_Bridge_Flying_Cheep_Cheeps", ps->difficultyBridgeFlyingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Bridge_Lakitus", ps->difficultyBridgeLakitus)) return false;
    if (!configFile.Set_Value("Difficulty_Bridge_Offscreen_Bullet_Bills", ps->difficultyBridgeOffscreenBulletBills)) return false;
    if (!configFile.Set_Value("Difficulty_Castle_Fire_Bars", ps->difficultyCastleFireBars)) return false;
    if (!configFile.Set_Value("Difficulty_Castle_Flying_Cheep_Cheeps", ps->difficultyCastleFlyingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Castle_Lakitus", ps->difficultyCastleLakitus)) return false;
    if (!configFile.Set_Value("Difficulty_Castle_Offscreen_Bullet_Bills", ps->difficultyCastleOffscreenBulletBills)) return false;
    if (!configFile.Set_Value("Difficulty_Island_Flying_Cheep_Cheeps", ps->difficultyIslandFlyingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Island_Lakitus", ps->difficultyIslandLakitus)) return false;
    if (!configFile.Set_Value("Difficulty_Island_Offscreen_Bullet_Bills", ps->difficultyIslandOffscreenBulletBills)) return false;
    if (!configFile.Set_Value("Difficulty_Underground_Flying_Cheep_Cheeps", ps->difficultyUndergroundFlyingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Underground_Lakitus", ps->difficultyUndergroundLakitus)) return false;
    if (!configFile.Set_Value("Difficulty_Underground_Offscreen_Bullet_Bills", ps->difficultyUndergroundOffscreenBulletBills)) return false;
    if (!configFile.Set_Value("Difficulty_Underwater_Bloopers", ps->difficultyUnderwaterBloopers)) return false;
    if (!configFile.Set_Value("Difficulty_Underwater_Flying_Cheep_Cheeps", ps->difficultyUnderwaterFlyingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Underwater_Hammer_Bros", ps->difficultyUnderwaterHammerBros)) return false;
    if (!configFile.Set_Value("Difficulty_Underwater_Lakitus", ps->difficultyUnderwaterLakitus)) return false;
    if (!configFile.Set_Value("Difficulty_Underwater_Swimming_Cheep_Cheeps", ps->difficultyUnderwaterSwimmingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Standard_Overworld_Flying_Cheep_Cheeps", ps->difficultyStandardOverworldFlyingCheepCheeps)) return false;
    if (!configFile.Set_Value("Difficulty_Standard_Overworld_Lakitus", ps->difficultyStandardOverworldLakitus)) return false;
    if (!configFile.Set_Value("Difficulty_Standard_Overworld_Offscreen_Bullet_Bills", ps->difficultyStandardOverworldOffscreenBulletBills)) return false;
    if (!configFile.Set_Value("Difficulty_Minimum_Enemy_Distance", ps->difficultyMinimumEnemyDistance)) return false;
    if (!configFile.Set_Value("Difficulty_Minimum_Underwater_Enemy_Distance", ps->difficultyMinimumUnderwaterEnemyDistance)) return false;
    if (!configFile.Set_Value("Difficulty_No_Enemies", ps->difficultyNoEnemies)) return false;
    if (!configFile.Set_Value("Difficulty_Unlimited_Time", ps->difficultyUnlimitedTime)) return false;
    if (!configFile.Set_Value("Difficulty_Hammer_Time_Intensity", ps->difficultyHammerTimeIntensity)) return false;
    if (!configFile.Set_Value("Difficulty_Replace_Castle_Loops", ps->difficultyReplaceCastleLoops)) return false;
    if (!configFile.Set_Value("Difficulty_Max_Level_Length", ps->difficultyMaxLevelLength)) return false;
    if (!configFile.Set_Value("Difficulty_Auto_Scroll_Chance_Per_Level", ps->difficultyAutoScrollChancePerLevel)) return false;
    if (!configFile.Set_Value("Difficulty_Lakitu_Spawn_Chance_Per_Level", ps->difficultyLakituSpawnChancePerLevel)) return false;
    if (!configFile.Set_Value("Difficulty_Lakitu_Respawn_Speed", ps->difficultyLakituRespawnSpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Spiny_Egg_Behavior", ps->difficultySpinyEggBehavior)) return false;
    if (!configFile.Set_Value("Difficulty_Disable_All_Other_Enemies_When_A_Lakitu_Spawns", ps->difficultyDisableAllOtherEnemiesWhenALakituSpawns)) return false;
    if (!configFile.Set_Value("Difficulty_Disable_All_Other_Enemies_When_Flying_Cheep_Cheeps_Spawn", ps->difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn)) return false;
    if (!configFile.Set_Value("Difficulty_Spawner_Priority", ps->difficultySpawnerPriority)) return false;
    if (!configFile.Set_Value("Difficulty_Secondary_Mushroom", ps->difficultySecondaryMushroom)) return false;
    if (!configFile.Set_Value("Difficulty_Surfing_Lift_Speed", ps->difficultySurfingLiftSpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Piranha_Plant_Type", ps->difficultyPiranhaPlantType)) return false;
    if (!configFile.Set_Value("Difficulty_Max_Number_Of_Piranha_Plants", ps->difficultyMaxNumberOfPiranhaPlants)) return false;
    if (!configFile.Set_Value("Standard_Overworld_Chance", ps->standardOverworldChance)) return false;
    if (!configFile.Set_Value("Underground_Chance", ps->undergroundChance)) return false;
    if (!configFile.Set_Value("Underwater_Chance", ps->underwaterChance)) return false;
    if (!configFile.Set_Value("Bridge_Chance", ps->bridgeChance)) return false;
    if (!configFile.Set_Value("Island_Chance", ps->islandChance)) return false;
    if (!configFile.Set_Value("Music", ps->music)) return false;
    if (!configFile.Set_Value("Combine_Music_With_Other_Packs", ps->combineMusicWithOtherPacks)) return false;
    if (!configFile.Set_Value("Tone_Color", ps->toneColor)) return false;
    if (!configFile.Set_Value("Random_Sound_Effects", ps->randomSounds)) return false;
    if (!configFile.Set_Value("Random_Text", ps->randomText)) return false;
    if (!configFile.Set_Value("Randomize_Some_Animations", ps->randomizeSomeAnimations)) return false;
    if (!configFile.Set_Value("Graphics", ps->graphics)) return false;
    if (!configFile.Set_Value("Combine_Graphics_With_Other_Packs", ps->combineGraphicsWithOtherPacks)) return false;
    if (!configFile.Set_Value("Palette", ps->palette)) return false;
    if (!configFile.Set_Value("Mario_Sprite", ps->marioSprite)) return false;
    if (!configFile.Set_Value("Mario_Sprite_Palette", ps->marioSpritePalette)) return false;
    if (!configFile.Set_Value("Infinite_Lives", ps->infiniteLives)) return false;
    if (!configFile.Set_Value("Permadeath", ps->permadeath)) return false;
    if (!configFile.Set_Value("Always_Restart_From_Current_World_After_Game_Over", ps->alwaysRestartFromCurrentWorldAfterGameOver)) return false;
    if (!configFile.Set_Value("Number_Of_Lives", ps->numLives)) return false;
    if (!configFile.Set_Value("God_Mode", ps->godMode)) return false;
    if (!configFile.Set_Value("Add_Luigi_Game", ps->addLuigiGame)) return false;
    if (!configFile.Set_Value("Super_Mario_On_Damage", ps->superMarioOnDamage)) return false;
    if (!configFile.Set_Value("Difficulty_European_Blooper_Swim_Height", ps->difficultyEuropeanBlooperSwimHeight)) return false;
    if (!configFile.Set_Value("Lakitu_Throw_Arc", ps->lakituThrowArc)) return false;
    if (!configFile.Set_Value("Difficulty_Basic_Enemy_Speed", ps->difficultyBasicEnemySpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Bowser_Flame_Frequency", ps->difficultyBowserFlameFrequency)) return false;
    if (!configFile.Set_Value("Difficulty_Bullet_Bill_Firing_Rate", ps->difficultyBulletBillFiringRate)) return false;
    if (!configFile.Set_Value("Difficulty_Bullet_Bill_Speed", ps->difficultyBulletBillSpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Leaping_Paratroopa_Speed", ps->difficultyLeapingParatroopaSpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Enemy_Revival_Speed", ps->difficultyEnemyRevivalSpeed)) return false;
    if (!configFile.Set_Value("Difficulty_Fire_Bar_Length", ps->difficultyFireBarLength)) return false;
    if (!configFile.Set_Value("Difficulty_Long_Fire_Bar_Length", ps->difficultyLongFireBarLength)) return false;
    if (!configFile.Set_Value("Difficulty_Flying_Cheep_Cheep_Jump_Height", ps->difficultyFlyingCheepCheepJumpHeight)) return false;
    if (!configFile.Set_Value("Difficulty_Hammer_Bros_Throw_Rate", ps->difficultyHammerBrosThrowRate)) return false;
    if (!configFile.Set_Value("Difficulty_Hammer_Bros_Never_Jump", ps->difficultyHammerBrosNeverJump)) return false;
    if (!configFile.Set_Value("Difficulty_Fire_Bowser_Throws_A_Ludicrous_Amount_Of_Fireballs", ps->difficultyFireBowserThrowsALudicrousAmountOfFireballs)) return false;
    if (!configFile.Set_Value("Difficulty_Speedy_Objects_And_Enemies", ps->difficultySpeedyObjectsAndEnemies)) return false;
    if (!configFile.Set_Value("Difficulty_Piranha_Plants_On_First_Level", ps->difficultyPiranhaPlantsOnFirstLevel)) return false;
    if (!configFile.Set_Value("Difficulty_Prevent_The_First_Level_From_Being_Underwater", ps->difficultyPreventTheFirstLevelFromBeingUnderwater)) return false;
    if (!configFile.Set_Value("Difficulty_Prevent_Hammer_Time_From_Spawning_Two_Hammer_Bros_In_A_Row", ps->difficultyPreventHammerTimeFromSpawningTwoHammerBrosInARow)) return false;
    if (!configFile.Set_Value("Difficulty_Allow_Hammer_Bros_When_Randomizing_Enemies_In_Level_Scripts", ps->difficultyAllowHammerBrosWhenRandomizingEnemiesInLevelScripts)) return false;
    if (!configFile.Set_Value("Difficulty_Allow_Hammer_Bros_Groups_When_Randomizing_Enemies_In_Level_Scripts", ps->difficultyAllowHammerBrosGroupsWhenRandomizingEnemiesInLevelScripts)) return false;
    if (!configFile.Set_Value("Difficulty_Allow_Lakitus_When_Randomizing_Enemies_In_Level_Scripts", ps->difficultyAllowLakitusWhenRandomizingEnemiesInLevelScripts)) return false;
    if (!configFile.Set_Value("Difficulty_Allow_Bullet_Bill_And_Cheep_Cheep_Spawners_When_Randomizing_Enemies_In_Level_Scripts", ps->difficultyAllowBulletBillAndCheepCheepSpawnersWhenRandomizingEnemiesInLevelScripts)) return false;
    if (!configFile.Set_Value("Difficulty_Max_Powerups", ps->difficultyMaxPowerups)) return false;
    if (!configFile.Set_Value("Difficulty_Max_Hidden_Powerups", ps->difficultyMaxHiddenPowerups)) return false;
    if (!configFile.Set_Value("Difficulty_Max_One_Ups", ps->difficultyMaxOneUps)) return false;
    if (!configFile.Set_Value("Difficulty_Max_Ten_Coin_Blocks", ps->difficultyMaxTenCoinBlocks)) return false;
    if (!configFile.Set_Value("Difficulty_Max_Stars", ps->difficultyMaxStars)) return false;
    if (!configFile.Set_Value("Difficulty_Hidden_Powerup_Chance", ps->difficultyHiddenPowerupChance)) return false;
    if (!configFile.Set_Value("Difficulty_One_Up_Chance", ps->difficultyOneUpChance)) return false;
    if (!configFile.Set_Value("Difficulty_Ten_Coin_Block_Chance", ps->difficultyTenCoinBlockChance)) return false;
    if (!configFile.Set_Value("Difficulty_Star_Chance", ps->difficultyStarChance)) return false;
    if (!configFile.Set_Value("Powerup", ps->powerup)) return false;
    if (!configFile.Set_Value("Mario_Name", ps->marioName)) return false;
    if (!configFile.Set_Value("Luigi_Name", ps->luigiName)) return false;
    return configFile.Save_And_Close();
}

bool Config_File_Handler::Load_Plugin_Settings(Plugin_Settings *ps, const QString &configFileLocation, bool internalConfig) {
    bool messageShown = false;
    return this->Load_Plugin_Settings(ps, configFileLocation, internalConfig, messageShown);
}

bool Config_File_Handler::Load_Plugin_Settings(Plugin_Settings *ps, const QString &configFileLocation, bool internalConfig, bool &messageShown) {
    messageShown = false;
    Readable_Config_File configFile(!internalConfig);
    if (!configFile.Open(configFileLocation)) return false;
    ps->fireFlowerBouncesLikeAStar = false;
    QString levelScripts = "", baseROM = "";
    bool generateNewLevels = true;
    configFile.Get_Value("Level_Scripts", levelScripts);
    configFile.Get_Value("Base_ROM", baseROM);
    configFile.Get_Value("Generate_New_Levels", generateNewLevels);
    if (internalConfig) {
        configFile.Get_Value("Last_Tab", ps->tab);
        configFile.Get_Value("Output_ROM_Location", ps->outputROMLocation);
        configFile.Get_Value("Overwrite_Output_ROM", ps->overwriteOuputROM);
        configFile.Get_Value("Last_External_Config_Location", ps->lastExternalConfigLocation);
    } else {
        QString version = "", generatorPlugin = "", writerPlugin = "";
        configFile.Get_Value("Version", version);
        configFile.Get_Value("Intended_Generator_Plugin", generatorPlugin);
        configFile.Get_Value("Intended_Writer_Plugin", writerPlugin);

        //Make sure the intended plugins are correct
        if (generatorPlugin != "SMB1_Compliance_Generator") {
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED, "This config file is not intended for this level generator plugin!");
            messageShown = true;
            return false;
        }
        if (writerPlugin != "SMB1_Writer") {
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED, "This config file is not intended for this writer plugin!");
            messageShown = true;
            return false;
        }

        //Get the actual checksums of the internal archives
        QString romsArchiveLocation = this->applicationLocation+"/"+Common_Strings::STRING_DATA+"/"+Common_Strings::STRING_GAME_NAME+"/ROMs.sa";
        QString graphicsPacksArchiveLocation = this->applicationLocation+"/"+Common_Strings::STRING_DATA+"/"+Common_Strings::STRING_GAME_NAME+"/Graphics.sa";
        QString musicPacksArchiveLocation = this->applicationLocation+"/"+Common_Strings::STRING_DATA+"/"+Common_Strings::STRING_GAME_NAME+"/Music.sa";
        QString textArchiveLocation = this->applicationLocation+"/"+Common_Strings::STRING_DATA+"/"+Common_Strings::STRING_GAME_NAME+"/Text.sa";
        QString levelScriptsLocation = this->applicationLocation+"/"+Common_Strings::STRING_LEVELS+"/"+Common_Strings::STRING_GAME_NAME+"/"+levelScripts+Common_Strings::STRING_LEVELS_EXTENSION;
        QString romsArchiveChecksum = this->Get_Checksum(romsArchiveLocation);
        QString graphicsPacksArchiveChecksum = this->Get_Checksum(graphicsPacksArchiveLocation);
        QString musicPacksArchiveChecksum = this->Get_Checksum(musicPacksArchiveLocation);
        QString textArchiveChecksum = this->Get_Checksum(textArchiveLocation);
        QString levelScriptsChecksum = this->Get_Checksum(levelScriptsLocation);
        if (romsArchiveChecksum.isEmpty() || graphicsPacksArchiveChecksum.isEmpty() || musicPacksArchiveChecksum.isEmpty() || textArchiveChecksum.isEmpty()) {
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED, "Unable to access internal archives!");
            messageShown = true;
            return false;
        } else if (!generateNewLevels && levelScriptsChecksum.isEmpty()) {
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED, "Level pack \"" + levelScripts + "\" is not installed!");
            messageShown = true;
            return false;
        }

        //Read the intended checksums of the internal archives
        QString intendedROMsArchiveChecksum = "", intendedGraphicsPacksArchiveChecksum = "", intendedMusicPacksArchiveChecksum = "", intendedTextArchiveChecksum = "", intendedLevelScriptsChecksum = "";
        configFile.Get_Value("Intended_ROMs_Archive_Checksum", intendedROMsArchiveChecksum);
        configFile.Get_Value("Intended_Graphics_Pack_Archive_Checksum", intendedGraphicsPacksArchiveChecksum);
        configFile.Get_Value("Intended_Music_Pack_Archive_Checksum", intendedMusicPacksArchiveChecksum);
        configFile.Get_Value("Intended_Text_Archive_Checksum", intendedTextArchiveChecksum);
        configFile.Get_Value("Intended_Level_Scripts_Checksum", intendedLevelScriptsChecksum);

        //Show a warning if the config file was exported with a different version of Level-Headed
        if (version != Version::VERSION) {
            QMessageBox::StandardButton answer = QMessageBox::question(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                "This config file is intended for a different version of " + Common_Strings::STRING_LEVEL_HEADED + ". The settings may not work as intended! Do you wish to import it anyway?",
                QMessageBox::Yes | QMessageBox::No);
            if (answer == QMessageBox::No) {
                messageShown = true;
                return false;
            }
        } else if (!QFileInfo::exists(this->applicationLocation + "/" + Common_Strings::STRING_DATA + "/" + Common_Strings::STRING_GAME_NAME + "/" + baseROM)) {
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED, "This config file was exported using \"" + baseROM + "\" as a base ROM! This ROM is not installed, so the settings cannot be imported!");
            messageShown = true;
            return false;
        } else if ((!intendedROMsArchiveChecksum.isEmpty() && romsArchiveChecksum != intendedROMsArchiveChecksum)
                   || (!intendedGraphicsPacksArchiveChecksum.isEmpty() && graphicsPacksArchiveChecksum != intendedGraphicsPacksArchiveChecksum)
                   || (!intendedMusicPacksArchiveChecksum.isEmpty() && musicPacksArchiveChecksum != intendedMusicPacksArchiveChecksum)
                   || (!intendedTextArchiveChecksum.isEmpty() && textArchiveChecksum != intendedTextArchiveChecksum)) {
            QMessageBox::StandardButton answer = QMessageBox::question(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                "This config file was exported using different internal data archives! The settings may not work as intended! Do you wish to import it anyway?",
                QMessageBox::Yes | QMessageBox::No);
            if (answer == QMessageBox::No) {
                messageShown = true;
                return false;
            }
        } else if (!generateNewLevels && levelScriptsChecksum != intendedLevelScriptsChecksum) {
            QMessageBox::StandardButton answer = QMessageBox::question(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                "This config file was exported using a different version of the level pack \"" + levelScripts + "\"! The settings may not work as intended! Do you wish to import it anyway?",
                QMessageBox::Yes | QMessageBox::No);
            if (answer == QMessageBox::No) {
                messageShown = true;
                return false;
            }
        }
    }
    ps->levelScripts = levelScripts;
    ps->baseROM = baseROM;
    ps->generateNewLevels = generateNewLevels;
    configFile.Get_Value("Random_Number_Of_Worlds", ps->randomNumWorlds);
    configFile.Get_Value("Number_Of_Worlds", ps->numWorlds);
    configFile.Get_Value("Numer_Of_Levels_Per_World", ps->numLevelsPerWorld);
    configFile.Get_Value("Redistribute_Powerups", ps->redistributePowerups);
    configFile.Get_Value("Randomize_Warp_Zones", ps->randomizeWarpZones);
    configFile.Get_Value("Level_Script_Enemies", ps->levelScriptEnemies);
    configFile.Get_Value("No_Auto_Scrolling_Levels", ps->noAutoScrollingLevels);
    configFile.Get_Value("Standard_Level_Distribution", ps->standardLevelDistribution);
    configFile.Get_Value("Include_Standard_Overworld_Levels_In_Random_Distribution", ps->includeStandardOverworldLevelsInRandomDistribution);
    configFile.Get_Value("Include_Underground_Levels_In_Random_Distribution", ps->includeUndergroundLevelsInRandomDistribution);
    configFile.Get_Value("Include_Underwater_Levels_In_Random_Distribution", ps->includeUnderwaterLevelsInRandomDistribution);
    configFile.Get_Value("Include_Bridge_Levels_In_Random_Distribution", ps->includeBridgeLevelsInRandomDistribution);
    configFile.Get_Value("Include_Island_Levels_In_Random_Distribution", ps->includeIslandLevelsInRandomDistribution);
    configFile.Get_Value("SMB_Utility_Compatibility", ps->smbUtilityCompatibility);
    configFile.Get_Value("Difficulty", ps->difficultyComboIndex);
    configFile.Get_Value("Difficulty_Auto_Scroll", ps->difficultyAutoScroll);
    configFile.Get_Value("Difficulty_Auto_Scroll_With_Flying_Cheep_Cheeps", ps->difficultyAutoScrollWithFlyingCheepCheeps);
    configFile.Get_Value("Difficulty_Bullet_Time", ps->difficultyBulletTime);
    configFile.Get_Value("Difficulty_Hammer_Time", ps->difficultyHammerTime);
    configFile.Get_Value("Difficulty_Walking_Hammer_Bros", ps->difficultyWalkingHammerBros);
    configFile.Get_Value("Difficulty_Buzzy_Beetles_Replace_Lone_Goombas", ps->difficultyBuzzyBeetlesReplaceLoneGoombas);
    configFile.Get_Value("Difficulty_Bridge_Flying_Cheep_Cheeps", ps->difficultyBridgeFlyingCheepCheeps);
    configFile.Get_Value("Difficulty_Bridge_Lakitus", ps->difficultyBridgeLakitus);
    configFile.Get_Value("Difficulty_Bridge_Offscreen_Bullet_Bills", ps->difficultyBridgeOffscreenBulletBills);
    configFile.Get_Value("Difficulty_Castle_Fire_Bars", ps->difficultyCastleFireBars);
    configFile.Get_Value("Difficulty_Castle_Flying_Cheep_Cheeps", ps->difficultyCastleFlyingCheepCheeps);
    configFile.Get_Value("Difficulty_Castle_Lakitus", ps->difficultyCastleLakitus);
    configFile.Get_Value("Difficulty_Castle_Offscreen_Bullet_Bills", ps->difficultyCastleOffscreenBulletBills);
    configFile.Get_Value("Difficulty_Island_Flying_Cheep_Cheeps", ps->difficultyIslandFlyingCheepCheeps);
    configFile.Get_Value("Difficulty_Island_Lakitus", ps->difficultyIslandLakitus);
    configFile.Get_Value("Difficulty_Island_Offscreen_Bullet_Bills", ps->difficultyIslandOffscreenBulletBills);
    configFile.Get_Value("Difficulty_Underground_Flying_Cheep_Cheeps", ps->difficultyUndergroundFlyingCheepCheeps);
    configFile.Get_Value("Difficulty_Underground_Lakitus", ps->difficultyUndergroundLakitus);
    configFile.Get_Value("Difficulty_Underground_Offscreen_Bullet_Bills", ps->difficultyUndergroundOffscreenBulletBills);
    configFile.Get_Value("Difficulty_Underwater_Bloopers", ps->difficultyUnderwaterBloopers);
    configFile.Get_Value("Difficulty_Underwater_Flying_Cheep_Cheeps", ps->difficultyUnderwaterFlyingCheepCheeps);
    configFile.Get_Value("Difficulty_Underwater_Hammer_Bros", ps->difficultyUnderwaterHammerBros);
    configFile.Get_Value("Difficulty_Underwater_Lakitus", ps->difficultyUnderwaterLakitus);
    configFile.Get_Value("Difficulty_Underwater_Swimming_Cheep_Cheeps", ps->difficultyUnderwaterSwimmingCheepCheeps);
    configFile.Get_Value("Difficulty_Standard_Overworld_Flying_Cheep_Cheeps", ps->difficultyStandardOverworldFlyingCheepCheeps);
    configFile.Get_Value("Difficulty_Standard_Overworld_Lakitus", ps->difficultyStandardOverworldLakitus);
    configFile.Get_Value("Difficulty_Standard_Overworld_Offscreen_Bullet_Bills", ps->difficultyStandardOverworldOffscreenBulletBills);
    configFile.Get_Value("Difficulty_Minimum_Enemy_Distance", ps->difficultyMinimumEnemyDistance);
    configFile.Get_Value("Difficulty_Minimum_Underwater_Enemy_Distance", ps->difficultyMinimumUnderwaterEnemyDistance);
    configFile.Get_Value("Difficulty_No_Enemies", ps->difficultyNoEnemies);
    configFile.Get_Value("Difficulty_Unlimited_Time", ps->difficultyUnlimitedTime);
    configFile.Get_Value("Difficulty_Hammer_Time_Intensity", ps->difficultyHammerTimeIntensity);
    configFile.Get_Value("Difficulty_Replace_Castle_Loops", ps->difficultyReplaceCastleLoops);
    configFile.Get_Value("Difficulty_Max_Level_Length", ps->difficultyMaxLevelLength);
    configFile.Get_Value("Difficulty_Auto_Scroll_Chance_Per_Level", ps->difficultyAutoScrollChancePerLevel);
    configFile.Get_Value("Difficulty_Lakitu_Spawn_Chance_Per_Level", ps->difficultyLakituSpawnChancePerLevel);
    configFile.Get_Value("Difficulty_Lakitu_Respawn_Speed", ps->difficultyLakituRespawnSpeed);
    configFile.Get_Value("Difficulty_Spiny_Egg_Behavior", ps->difficultySpinyEggBehavior);
    configFile.Get_Value("Difficulty_Disable_All_Other_Enemies_When_A_Lakitu_Spawns", ps->difficultyDisableAllOtherEnemiesWhenALakituSpawns);
    configFile.Get_Value("Difficulty_Disable_All_Other_Enemies_When_Flying_Cheep_Cheeps_Spawn", ps->difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn);
    configFile.Get_Value("Difficulty_Spawner_Priority", ps->difficultySpawnerPriority);
    configFile.Get_Value("Difficulty_Secondary_Mushroom", ps->difficultySecondaryMushroom);
    configFile.Get_Value("Difficulty_Surfing_Lift_Speed", ps->difficultySurfingLiftSpeed);
    configFile.Get_Value("Difficulty_Piranha_Plant_Type", ps->difficultyPiranhaPlantType);
    configFile.Get_Value("Difficulty_Max_Number_Of_Piranha_Plants", ps->difficultyMaxNumberOfPiranhaPlants);
    configFile.Get_Value("Standard_Overworld_Chance", ps->standardOverworldChance);
    configFile.Get_Value("Underground_Chance", ps->undergroundChance);
    configFile.Get_Value("Underwater_Chance", ps->underwaterChance);
    configFile.Get_Value("Bridge_Chance", ps->bridgeChance);
    configFile.Get_Value("Island_Chance", ps->islandChance);
    configFile.Get_Value("Music", ps->music);
    configFile.Get_Value("Combine_Music_With_Other_Packs", ps->combineMusicWithOtherPacks);
    configFile.Get_Value("Tone_Color", ps->toneColor);
    configFile.Get_Value("Random_Sound_Effects", ps->randomSounds);
    configFile.Get_Value("Random_Text", ps->randomText);
    configFile.Get_Value("Randomize_Some_Animations", ps->randomizeSomeAnimations);
    configFile.Get_Value("Graphics", ps->graphics);
    configFile.Get_Value("Combine_Graphics_With_Other_Packs", ps->combineGraphicsWithOtherPacks);
    configFile.Get_Value("Palette", ps->palette);
    configFile.Get_Value("Mario_Sprite", ps->marioSprite);
    configFile.Get_Value("Mario_Sprite_Palette", ps->marioSpritePalette);
    configFile.Get_Value("Infinite_Lives", ps->infiniteLives);
    configFile.Get_Value("Permadeath", ps->permadeath);
    configFile.Get_Value("Always_Restart_From_Current_World_After_Game_Over", ps->alwaysRestartFromCurrentWorldAfterGameOver);
    configFile.Get_Value("Number_Of_Lives", ps->numLives);
    configFile.Get_Value("God_Mode", ps->godMode);
    configFile.Get_Value("Add_Luigi_Game", ps->addLuigiGame);
    configFile.Get_Value("Super_Mario_On_Damage", ps->superMarioOnDamage);
    configFile.Get_Value("Difficulty_European_Blooper_Swim_Height", ps->difficultyEuropeanBlooperSwimHeight);
    configFile.Get_Value("Lakitu_Throw_Arc", ps->lakituThrowArc);
    configFile.Get_Value("Difficulty_Basic_Enemy_Speed", ps->difficultyBasicEnemySpeed);
    configFile.Get_Value("Difficulty_Bowser_Flame_Frequency", ps->difficultyBowserFlameFrequency);
    configFile.Get_Value("Difficulty_Bullet_Bill_Firing_Rate", ps->difficultyBulletBillFiringRate);
    configFile.Get_Value("Difficulty_Bullet_Bill_Speed", ps->difficultyBulletBillSpeed);
    configFile.Get_Value("Difficulty_Leaping_Paratroopa_Speed", ps->difficultyLeapingParatroopaSpeed);
    configFile.Get_Value("Difficulty_Enemy_Revival_Speed", ps->difficultyEnemyRevivalSpeed);
    configFile.Get_Value("Difficulty_Fire_Bar_Length", ps->difficultyFireBarLength);
    configFile.Get_Value("Difficulty_Long_Fire_Bar_Length", ps->difficultyLongFireBarLength);
    configFile.Get_Value("Difficulty_Flying_Cheep_Cheep_Jump_Height", ps->difficultyFlyingCheepCheepJumpHeight);
    configFile.Get_Value("Difficulty_Hammer_Bros_Throw_Rate", ps->difficultyHammerBrosThrowRate);
    configFile.Get_Value("Difficulty_Hammer_Bros_Never_Jump", ps->difficultyHammerBrosNeverJump);
    configFile.Get_Value("Difficulty_Fire_Bowser_Throws_A_Ludicrous_Amount_Of_Fireballs", ps->difficultyFireBowserThrowsALudicrousAmountOfFireballs);
    configFile.Get_Value("Difficulty_Speedy_Objects_And_Enemies", ps->difficultySpeedyObjectsAndEnemies);
    configFile.Get_Value("Difficulty_Piranha_Plants_On_First_Level", ps->difficultyPiranhaPlantsOnFirstLevel);
    configFile.Get_Value("Difficulty_Prevent_The_First_Level_From_Being_Underwater", ps->difficultyPreventTheFirstLevelFromBeingUnderwater);
    configFile.Get_Value("Difficulty_Prevent_Hammer_Time_From_Spawning_Two_Hammer_Bros_In_A_Row", ps->difficultyPreventHammerTimeFromSpawningTwoHammerBrosInARow);
    configFile.Get_Value("Difficulty_Allow_Hammer_Bros_When_Randomizing_Enemies_In_Level_Scripts", ps->difficultyAllowHammerBrosWhenRandomizingEnemiesInLevelScripts);
    configFile.Get_Value("Difficulty_Allow_Hammer_Bros_Groups_When_Randomizing_Enemies_In_Level_Scripts", ps->difficultyAllowHammerBrosGroupsWhenRandomizingEnemiesInLevelScripts);
    configFile.Get_Value("Difficulty_Allow_Lakitus_When_Randomizing_Enemies_In_Level_Scripts", ps->difficultyAllowLakitusWhenRandomizingEnemiesInLevelScripts);
    configFile.Get_Value("Difficulty_Allow_Bullet_Bill_And_Cheep_Cheep_Spawners_When_Randomizing_Enemies_In_Level_Scripts", ps->difficultyAllowBulletBillAndCheepCheepSpawnersWhenRandomizingEnemiesInLevelScripts);
    configFile.Get_Value("Difficulty_Max_Powerups", ps->difficultyMaxPowerups);
    configFile.Get_Value("Difficulty_Max_Hidden_Powerups", ps->difficultyMaxHiddenPowerups);
    configFile.Get_Value("Difficulty_Max_One_Ups", ps->difficultyMaxOneUps);
    configFile.Get_Value("Difficulty_Max_Ten_Coin_Blocks", ps->difficultyMaxTenCoinBlocks);
    configFile.Get_Value("Difficulty_Max_Stars", ps->difficultyMaxStars);
    configFile.Get_Value("Difficulty_Hidden_Powerup_Chance", ps->difficultyHiddenPowerupChance);
    configFile.Get_Value("Difficulty_One_Up_Chance", ps->difficultyOneUpChance);
    configFile.Get_Value("Difficulty_Ten_Coin_Block_Chance", ps->difficultyTenCoinBlockChance);
    configFile.Get_Value("Difficulty_Star_Chance", ps->difficultyStarChance);
    configFile.Get_Value("Powerup", ps->powerup);
    configFile.Get_Value("Mario_Name", ps->marioName);
    configFile.Get_Value("Luigi_Name", ps->luigiName);
    configFile.Discard_And_Close();

    //Verify Settings
    //TODO: Verify all other settings
    if (ps->difficultySecondaryMushroom > 2) ps->difficultySecondaryMushroom = 0;
    return true;
}

QString Config_File_Handler::Get_Checksum(const QString &fileLocation) {
    QFile file(fileLocation);
    if (!file.exists()) return QString();
    if (!file.open(QFile::ReadOnly)) return QString();
    QByteArray buffer = file.readAll();
    return QString(QCryptographicHash::hash(buffer, QCryptographicHash::Sha512).toHex().toUpper());
}
