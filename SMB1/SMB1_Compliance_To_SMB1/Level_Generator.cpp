#include "Level_Generator.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "../../../C_Common_Code/Qt/Text_Insertion_Buffer/Text_Insertion_Buffer.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../Common_SMB1_Files/Level_String.h"
#include "../Common_SMB1_Files/Header_String.h"
#include "../SMB1_Compliance_Generator/Difficulty.h"
#include "../SMB1_Compliance_Generator/Enemy_Buffer.h"
#include "../SMB1_Compliance_Generator/Object_Buffer.h"
#include "../SMB1_Writer/ROM_Filename.h"
#include "../SMB1_Writer/SMB1_Writer_Strings.h"
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QTime>
#include <math.h>
#include <assert.h>

const static int VERY_COMMON_POINTS = 14;
const static int COMMON_POINTS = 8;
const static int UNCOMMON_POINTS = 5;
const static int RARE_POINTS = 2;

Level_Generator::Level_Generator(const QString &applicationLocation, QWidget *parent, Plugin_Settings *pluginSettings,
                                 SMB1_Compliance_Generator_Interface *generatorPlugin, SMB1_Writer_Interface *writerPlugin) {
    assert(pluginSettings);
    assert(generatorPlugin);
    assert(writerPlugin);
    this->applicationLocation = applicationLocation;
    this->parent = parent;
    this->pluginSettings = pluginSettings;
    this->generatorPlugin = generatorPlugin;
    this->writerPlugin = writerPlugin;
    this->sequentialArchiveLoaded = false;
    this->sequentialArchiveLoader = nullptr;
    this->sequentialArchivePlugin = nullptr;
    this->levelLocation = applicationLocation + "/" + Common_Strings::STRING_LEVELS + "/" + Common_Strings::STRING_GAME_NAME;
    QDir dir(applicationLocation);
    if (!dir.exists(Common_Strings::STRING_LEVELS)) dir.mkdir(Common_Strings::STRING_LEVELS); //don't bother checking for success here
    if (dir.cd(Common_Strings::STRING_LEVELS)) {
        if (!dir.exists(Common_Strings::STRING_GAME_NAME)) {
            dir.mkdir(Common_Strings::STRING_GAME_NAME);
        }
    }
    this->veryCommonLevels = new QVector<Level_Type::Level_Type>();
    this->commonLevels = new QVector<Level_Type::Level_Type>();
    this->uncommonLevels = new QVector<Level_Type::Level_Type>();
    this->rareLevels = new QVector<Level_Type::Level_Type>();
    this->allocatedLevels = new QVector<Level_Type::Level_Type>();
    this->chances = new QMap<QString, Chance>();
    this->chances->insert(STRING_VERY_COMMON, VERY_COMMON);
    this->chances->insert(STRING_COMMON, COMMON);
    this->chances->insert(STRING_UNCOMMON, UNCOMMON);
    this->chances->insert(STRING_RARE, RARE);
    this->chances->insert(STRING_NONE, NONE);
    this->numObjectsInLevel = new QMap<Level::Level, int>();
    this->numEnemiesInLevel = new QMap<Level::Level, int>();
}

Level_Generator::~Level_Generator() {
    if (this->sequentialArchiveLoader) this->sequentialArchiveLoader->unload();
    this->sequentialArchiveLoader = nullptr;
    this->sequentialArchivePlugin = nullptr;
    delete this->chances;
    delete this->veryCommonLevels;
    delete this->commonLevels;
    delete this->uncommonLevels;
    delete this->rareLevels;
    delete this->allocatedLevels;
    delete this->numObjectsInLevel;
    delete this->numEnemiesInLevel;
}

bool Level_Generator::Run_Level_Generator() {
    //Seed the random number generator
    Random::Get_Instance().Seed(this->pluginSettings->randomSeed, 5);

    this->Populate_Num_Objects_And_Enemies_In_Levels();
    QString fileName = this->pluginSettings->levelScripts + Common_Strings::STRING_LEVELS_EXTENSION;
    if (this->pluginSettings->generateNewLevels) {
        if (!this->Generate_New_Levels(fileName)) return false;
        this->Populate_Num_Objects_And_Enemies_In_Levels(); //repopulate the maps to restore back to the ROM's default values
    }
    this->Delete_Old_Level_Generations();
    return this->Parse_Level_Map(fileName);
}

SMB1_Compliance_Generator_Arguments Level_Generator::Prepare_Arguments(const QString &generationName, int levelNum, int numLevels) {
    int level = (levelNum%this->pluginSettings->numLevelsPerWorld)+1;
    int world = (levelNum/this->pluginSettings->numLevelsPerWorld)+1;
    SMB1_Compliance_Generator_Arguments args;
    args.fileName = this->levelLocation + "/" + generationName + "/Level_" + QString::number(world) + "_" + QString::number(level) + ".lvl";

    //Determine difficulty
    args.difficulty = static_cast<int>(std::ceil((static_cast<double>(levelNum+1)*10.0)/static_cast<double>(numLevels)));
    assert(args.difficulty >= 1 && args.difficulty <= 10);
    args.useAutoScroll = false;
    if (this->pluginSettings->difficultyReplaceCastleLoopsCurrent == 3) {
        if (this->pluginSettings->difficultyAutoScroll <= args.difficulty) {
            if (Random::Get_Instance().Get_Num(99)+1 <= this->pluginSettings->difficultyAutoScrollChancePerLevel) {
                args.useAutoScroll = true;
            }
        }
    }
    if (this->pluginSettings->noAutoScrollingLevels) args.useAutoScroll = false;
    args.useMidpoints = true;
    args.useVerticalObjectLimit = this->pluginSettings->baseROM.startsWith(ROM_Filename::STRING_FULL_SUPPORT);
    if (this->pluginSettings->smbUtilityCompatibility) args.useVerticalObjectLimit = false;
    args.doesLevelExceedVerticalObjectLimit = false;
    args.difficultyBulletTime = this->pluginSettings->difficultyBulletTime;
    args.difficultyHammerTime = this->pluginSettings->difficultyHammerTime;
    args.difficultyBuzzyBeetlesReplaceLoneGoombas = this->pluginSettings->difficultyBuzzyBeetlesReplaceLoneGoombas;
    args.difficultyBridgeFlyingCheepCheeps = this->pluginSettings->difficultyBridgeFlyingCheepCheeps;
    args.difficultyBridgeLakitus = this->pluginSettings->difficultyBridgeLakitus;
    args.difficultyBridgeOffscreenBulletBills = this->pluginSettings->difficultyBridgeOffscreenBulletBills;
    args.difficultyCastleFireBars = this->pluginSettings->difficultyCastleFireBars;
    args.difficultyCastleFlyingCheepCheeps = this->pluginSettings->difficultyCastleFlyingCheepCheeps;
    args.difficultyCastleLakitus = this->pluginSettings->difficultyCastleLakitus;
    args.difficultyCastleOffscreenBulletBills = this->pluginSettings->difficultyCastleOffscreenBulletBills;
    args.difficultyIslandFlyingCheepCheeps = this->pluginSettings->difficultyIslandFlyingCheepCheeps;
    args.difficultyIslandLakitus = this->pluginSettings->difficultyIslandLakitus;
    args.difficultyIslandOffscreenBulletBills = this->pluginSettings->difficultyIslandOffscreenBulletBills;
    args.difficultyUndergroundFlyingCheepCheeps = this->pluginSettings->difficultyUndergroundFlyingCheepCheeps;
    args.difficultyUndergroundLakitus = this->pluginSettings->difficultyUndergroundLakitus;
    args.difficultyUndergroundOffscreenBulletBills = this->pluginSettings->difficultyUndergroundOffscreenBulletBills;
    args.difficultyUnderwaterBloopers = this->pluginSettings->difficultyUnderwaterBloopers;
    args.difficultyUnderwaterFlyingCheepCheeps = this->pluginSettings->difficultyUnderwaterFlyingCheepCheeps;
    args.difficultyUnderwaterHammerBros = this->pluginSettings->difficultyUnderwaterHammerBros;
    args.difficultyUnderwaterLakitus = this->pluginSettings->difficultyUnderwaterLakitus;
    args.difficultyUnderwaterSwimmingCheepCheeps = this->pluginSettings->difficultyUnderwaterSwimmingCheepCheeps;
    args.difficultyStandardOverworldFlyingCheepCheeps = this->pluginSettings->difficultyStandardOverworldFlyingCheepCheeps;
    args.difficultyStandardOverworldLakitus = this->pluginSettings->difficultyStandardOverworldLakitus;
    args.difficultyStandardOverworldOffscreenBulletBills = this->pluginSettings->difficultyStandardOverworldOffscreenBulletBills;
    args.difficultyMinimumEnemyDistance = this->pluginSettings->difficultyMinimumEnemyDistance;
    args.difficultyMinimumUnderwaterEnemyDistance = this->pluginSettings->difficultyMinimumUnderwaterEnemyDistance;
    args.difficultyNoEnemies = this->pluginSettings->difficultyNoEnemies;
    args.difficultyHammerTimeIntensity = this->pluginSettings->difficultyHammerTimeIntensity;
    args.difficultyLakituSpawnChancePerLevel = this->pluginSettings->difficultyLakituSpawnChancePerLevel;
    args.difficultyDisableAllOtherEnemiesWhenALakituSpawns = this->pluginSettings->difficultyDisableAllOtherEnemiesWhenALakituSpawns;
    args.difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn = this->pluginSettings->difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn;
    args.difficultyPreventHammerTimeFromSpawningTwoHammerBrosInARow = this->pluginSettings->difficultyPreventHammerTimeFromSpawningTwoHammerBrosInARow;
    args.difficultySpawnerPriority = this->pluginSettings->difficultySpawnerPriority;
    args.difficultyMaxPowerups = this->pluginSettings->difficultyMaxPowerups;
    args.difficultyMaxHiddenPowerups = this->pluginSettings->difficultyMaxHiddenPowerups;
    args.difficultyMaxOneUps = this->pluginSettings->difficultyMaxOneUps;
    args.difficultyMaxTenCoinBlocks = this->pluginSettings->difficultyMaxTenCoinBlocks;
    args.difficultyMaxStars = this->pluginSettings->difficultyMaxStars;
    args.difficultyHiddenPowerupChance = this->pluginSettings->difficultyHiddenPowerupChance;
    args.difficultyOneUpChance = this->pluginSettings->difficultyOneUpChance;
    args.difficultyTenCoinBlockChance = this->pluginSettings->difficultyTenCoinBlockChance;
    args.difficultyStarChance = this->pluginSettings->difficultyStarChance;
    args.fireFlowerBouncesLikeStar = this->pluginSettings->fireFlowerBouncesLikeAStar;

    //Determine the level type. The last level of each world should be a castle
    if (level == this->pluginSettings->numLevelsPerWorld) args.levelType = Level_Type::CASTLE;
    else if (!this->pluginSettings->generateNewLevels) args.levelType = Level_Type::STANDARD_OVERWORLD;
    else args.levelType = this->Determine_Level_Type(levelNum, numLevels, this->pluginSettings->numWorlds, this->pluginSettings->numLevelsPerWorld);
    args.levelCompliment = Level_Compliment::BULLET_BILL_TURRETS;
    args.maxLevelLength = this->Get_Level_Length(this->pluginSettings->difficultyMaxLevelLengthBlocks, args.difficulty, args.useAutoScroll, args.levelType);
    switch (args.levelType) {
    case Level_Type::STANDARD_OVERWORLD:
        if (args.difficulty < args.difficultyBulletTime && Random::Get_Instance().Get_Num(4)==0) args.levelCompliment = Level_Compliment::MUSHROOMS;
        if (Random::Get_Instance().Get_Num(1)==0) args.headerScenery = Scenery::MOUNTAINS;
        else args.headerScenery = Scenery::FENCES;
        if (Random::Get_Instance().Get_Num(2)==0) {
            int random = Random::Get_Instance().Get_Num(119);
            if (random < 40) args.headerBackground = Background::NIGHT;
            else if (random < 80) args.headerBackground = Background::SNOW;
            else if (random < 105) args.headerBackground = Background::NIGHT_AND_SNOW;
            else if (random < 120) args.headerBackground = Background::NIGHT_AND_FREEZE;
            else assert(false);
        } else args.headerBackground = Background::BLANK_BACKGROUND;
        break;
    case Level_Type::BRIDGE:
    case Level_Type::ISLAND:
        if (Random::Get_Instance().Get_Num(4)==0) args.levelCompliment = Level_Compliment::MUSHROOMS;
        else args.levelCompliment = Level_Compliment::TREES;
        args.headerScenery = Scenery::ONLY_CLOUDS;
        if (Random::Get_Instance().Get_Num(1)==0) {
            int random = Random::Get_Instance().Get_Num(189);
            if (random < 70) args.headerBackground = Background::OVER_WATER;
            else if (random < 110) args.headerBackground = Background::NIGHT;
            else if (random < 150) args.headerBackground = Background::SNOW;
            else if (random < 175) args.headerBackground = Background::NIGHT_AND_SNOW;
            else if (random < 190) args.headerBackground = Background::NIGHT_AND_FREEZE;
            else assert(false);
        } else args.headerBackground = Background::BLANK_BACKGROUND;
        break;
    case Level_Type::UNDERGROUND:
        if (args.difficulty < args.difficultyBulletTime && Random::Get_Instance().Get_Num(4)==0) args.levelCompliment = Level_Compliment::MUSHROOMS;
        args.headerScenery = Scenery::NO_SCENERY;
        if (args.levelCompliment == Level_Compliment::BULLET_BILL_TURRETS && Random::Get_Instance().Get_Num(3)==0) {
            if (Random::Get_Instance().Get_Num(2)==0) args.headerBackground = Background::NIGHT_AND_FREEZE;
            else args.headerBackground = Background::NIGHT_AND_SNOW;
        } else {
            args.headerBackground = Background::BLANK_BACKGROUND;
        }
        break;
    case Level_Type::UNDERWATER:
        args.levelCompliment = Level_Compliment::TREES;
        args.headerScenery = Scenery::NO_SCENERY;
        args.headerBackground = Background::IN_WATER;
        break;
    case Level_Type::CASTLE:
        args.headerScenery = Scenery::NO_SCENERY;
        args.headerBackground = Background::OVER_WATER;
        break;
    default:
        assert(false); return args;
    }

    //Determine the start castle
    if (args.levelType == Level_Type::CASTLE || args.levelType == Level_Type::UNDERWATER || args.levelType == Level_Type::UNDERGROUND) {
        args.startCastle = Castle::NONE;
    } else {
        if (levelNum == 0 || args.levelType == Level_Type::CASTLE) args.startCastle = Castle::NONE;
        else if (level == 1) args.startCastle = Castle::BIG;
        else args.startCastle = Castle::SMALL;
    }

    //Determine the end castle
    if (args.levelType == Level_Type::CASTLE) {
        args.endCastle = Castle::NONE;
    } else {
        if (level == this->pluginSettings->numLevelsPerWorld-1) args.endCastle = Castle::BIG;
        else if (args.levelType == Level_Type::CASTLE) args.endCastle = Castle::NONE;
        else args.endCastle = Castle::SMALL;
    }

    args.numObjectBytes = 1000;
    args.numEnemyBytes = 1000;
    return args;
}

int Level_Generator::Get_Level_Length(int length, int difficulty, bool autoScroll, Level_Type::Level_Type levelType) {
    if (length <= 0) return 0;

    //Make certain level types shorter
    if (levelType == Level_Type::UNDERWATER) length -= static_cast<int>(static_cast<double>(length)*0.40); //40% shorter
    else if (levelType == Level_Type::ISLAND) length -= static_cast<int>(static_cast<double>(length)*0.10); //10% shorter
    if (autoScroll) length -= static_cast<int>(static_cast<double>(length)*0.30); //30% shorter
    assert(length > 0);

    //Make later levels longer
    length += static_cast<int>(static_cast<double>(length)*0.04)*difficulty; //4% size increase for each difficulty
    return length;
}

QString Level_Generator::Convert_Level_Enum_To_String(Level::Level level) {
    switch (level) {
    case Level::WORLD_1_LEVEL_1:   return Level::STRING_WORLD_1_LEVEL_1;
    case Level::WORLD_1_LEVEL_2:   return Level::STRING_WORLD_1_LEVEL_2;
    case Level::WORLD_1_LEVEL_3:   return Level::STRING_WORLD_1_LEVEL_3;
    case Level::WORLD_1_LEVEL_4:   return Level::STRING_WORLD_1_LEVEL_4;
    case Level::WORLD_2_LEVEL_1:   return Level::STRING_WORLD_2_LEVEL_1;
    case Level::WORLD_2_LEVEL_2:   return Level::STRING_WORLD_2_LEVEL_2;
    case Level::WORLD_2_LEVEL_3:   return Level::STRING_WORLD_2_LEVEL_3;
    case Level::WORLD_2_LEVEL_4:   return Level::STRING_WORLD_2_LEVEL_4;
    case Level::WORLD_3_LEVEL_1:   return Level::STRING_WORLD_3_LEVEL_1;
    case Level::WORLD_3_LEVEL_2:   return Level::STRING_WORLD_3_LEVEL_2;
    case Level::WORLD_3_LEVEL_3:   return Level::STRING_WORLD_3_LEVEL_3;
    case Level::WORLD_3_LEVEL_4:   return Level::STRING_WORLD_3_LEVEL_4;
    case Level::WORLD_4_LEVEL_1:   return Level::STRING_WORLD_4_LEVEL_1;
    case Level::WORLD_4_LEVEL_2:   return Level::STRING_WORLD_4_LEVEL_2;
    case Level::WORLD_4_LEVEL_3:   return Level::STRING_WORLD_4_LEVEL_3;
    case Level::WORLD_4_LEVEL_4:   return Level::STRING_WORLD_4_LEVEL_4;
    case Level::WORLD_5_LEVEL_1:   return Level::STRING_WORLD_5_LEVEL_1;
    case Level::WORLD_5_LEVEL_2:   return Level::STRING_WORLD_5_LEVEL_2;
    case Level::WORLD_6_LEVEL_1:   return Level::STRING_WORLD_6_LEVEL_1;
    case Level::WORLD_6_LEVEL_2:   return Level::STRING_WORLD_6_LEVEL_2;
    case Level::WORLD_6_LEVEL_3:   return Level::STRING_WORLD_6_LEVEL_3;
    case Level::WORLD_7_LEVEL_1:   return Level::STRING_WORLD_7_LEVEL_1;
    case Level::WORLD_7_LEVEL_4:   return Level::STRING_WORLD_7_LEVEL_4;
    case Level::WORLD_8_LEVEL_1:   return Level::STRING_WORLD_8_LEVEL_1;
    case Level::WORLD_8_LEVEL_2:   return Level::STRING_WORLD_8_LEVEL_2;
    case Level::WORLD_8_LEVEL_3:   return Level::STRING_WORLD_8_LEVEL_3;
    case Level::WORLD_8_LEVEL_4:   return Level::STRING_WORLD_8_LEVEL_4;
    case Level::PIPE_INTRO:        return Level::STRING_PIPE_INTRO;
    case Level::UNDERGROUND_BONUS: return Level::STRING_UNDERGROUND_BONUS;
    case Level::CLOUD_BONUS_1:     return Level::STRING_CLOUD_BONUS_1;
    case Level::CLOUD_BONUS_2:     return Level::STRING_CLOUD_BONUS_2;
    case Level::UNDERWATER_BONUS:  return Level::STRING_UNDERWATER_BONUS;
    case Level::WARP_ZONE:         return Level::STRING_WARP_ZONE;
    case Level::UNDERWATER_CASTLE: return Level::STRING_UNDERWATER_CASTLE;
    }
    assert(false);
    return QString();
}

bool Level_Generator::Delete_Old_Level_Generations() {
    //Delete old folder from what are likely failed generations
    QString levelFolderLocation = this->applicationLocation + "/" + Common_Strings::STRING_LEVELS + "/" + Common_Strings::STRING_GAME_NAME;
    QDir dir(levelFolderLocation);
    for (QString folder : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (dir.cd(folder)) {
            dir.removeRecursively();
            if (!dir.cdUp() && !dir.cd(levelFolderLocation)) return false; //something weird happened, so just quit
        }
    }

    //Delete old generations
    const int MAX_OLD_GENERATIONS = 15;
    int generations = 0;
    QStringList nameFilters;
    nameFilters << "Random*"+Common_Strings::STRING_LEVELS_EXTENSION;
    dir.setNameFilters(nameFilters);
    dir.setSorting(QDir::Name | QDir::Reversed);
    for (QString file : dir.entryList(QDir::Files)) {
        ++generations;
        if (generations > MAX_OLD_GENERATIONS) dir.remove(file); //if we can't delete it, just ignore the error and move on
    }
    return true;
}

Level_Type::Level_Type Level_Generator::Determine_Level_Type(int levelNum, int numLevels, int numWorlds, int numLevelsPerWorld) {
    //Allocate the levels on the first call
    if (levelNum == 0) {
        //Clear previous allocations
        this->veryCommonLevels->clear();
        this->commonLevels->clear();
        this->uncommonLevels->clear();
        this->rareLevels->clear();
        this->allocatedLevels->clear();
        this->allocatedLevels->resize(numLevels);
        int numLevelsExcludingCastles = numLevels-numWorlds;
        for (int i = 0; i < this->allocatedLevels->size(); ++i) this->allocatedLevels->data()[i] = Level_Type::STANDARD_OVERWORLD;

        if (this->pluginSettings->standardLevelDistribution) {
            //Parse the weights for each level type
            this->Read_Level_Chance(this->pluginSettings->standardOverworldChance, Level_Type::STANDARD_OVERWORLD);
            this->Read_Level_Chance(this->pluginSettings->undergroundChance, Level_Type::UNDERGROUND);
            this->Read_Level_Chance(this->pluginSettings->underwaterChance, Level_Type::UNDERWATER);
            this->Read_Level_Chance(this->pluginSettings->bridgeChance, Level_Type::BRIDGE);
            this->Read_Level_Chance(this->pluginSettings->islandChance, Level_Type::ISLAND);
            QMap<Level_Type::Level_Type, double> levelWeights;
            QVector<QPair<Level_Type::Level_Type, int>> unallocatedLevels;
            for (int i = 0; i < this->veryCommonLevels->size(); ++i) levelWeights.insert(this->veryCommonLevels->at(i), static_cast<double>(VERY_COMMON_POINTS));
            for (int i = 0; i < this->commonLevels->size(); ++i) levelWeights.insert(this->commonLevels->at(i), static_cast<double>(COMMON_POINTS));
            for (int i = 0; i < this->uncommonLevels->size(); ++i) levelWeights.insert(this->uncommonLevels->at(i), static_cast<double>(UNCOMMON_POINTS));
            for (int i = 0; i < this->rareLevels->size(); ++i) unallocatedLevels.append(QPair<Level_Type::Level_Type, int>(this->rareLevels->at(i), 1));
            double totalWeight = 0.0;
            for (QMap<Level_Type::Level_Type, double>::iterator iter = levelWeights.begin(); iter != levelWeights.end(); ++iter) totalWeight += iter.value();
            int numLevelTypes = this->veryCommonLevels->size()+this->commonLevels->size()+this->uncommonLevels->size()+this->rareLevels->size();
            QVector<int> unallocatedLevelSlots;
            for (int i = 0; i < numLevels; ++i) {
                if ((i+1)%numLevelsPerWorld == 0) this->allocatedLevels->data()[i] = Level_Type::CASTLE; //allocate Castle levels now
                else unallocatedLevelSlots.push_back(i);
            }
            assert(unallocatedLevelSlots.size()+numWorlds == numLevels);

            //Distribute levels based upon the algorithm type
            if (static_cast<double>(numLevelsExcludingCastles) < static_cast<double>(numLevelTypes)*1.5) { //use the old distribution algorithm
                qInfo() << "Rolling for individual levels...";

                //Determine chance
                int veryCommonChance = VERY_COMMON_POINTS*this->veryCommonLevels->size();
                int commonChance = (COMMON_POINTS*this->commonLevels->size())+veryCommonChance;
                int uncommonChance = (UNCOMMON_POINTS*this->uncommonLevels->size())+commonChance;
                int rareChance = (RARE_POINTS*this->rareLevels->size())+uncommonChance;

                //Allocate the level types by probability
                QSet<Level_Type::Level_Type> usedUncommonLevels;
                while (!unallocatedLevelSlots.isEmpty()) {
                    Level_Type::Level_Type levelType = Level_Type::CASTLE;
                    int random = Random::Get_Instance().Get_Num(rareChance-1);
                    int index = 0;
                    if (random < veryCommonChance) { //very common
                        index = (random)/VERY_COMMON_POINTS;
                        assert(index < this->veryCommonLevels->size());
                        levelType = this->veryCommonLevels->at(index);
                    } else if (random < commonChance) { //common
                        index = (random-veryCommonChance)/COMMON_POINTS;
                        assert(index < this->commonLevels->size());
                        levelType = this->commonLevels->at(index);
                    } else if (random < uncommonChance) { //uncommon
                        index = (random-commonChance)/UNCOMMON_POINTS;
                        assert(index < this->uncommonLevels->size());
                        levelType = this->uncommonLevels->at(index);
                        if (usedUncommonLevels.contains(levelType)) { //uncommon levels can only spawn twice
                            this->uncommonLevels->remove(index);
                            uncommonChance = (UNCOMMON_POINTS*this->uncommonLevels->size())+commonChance; //recalculate uncommon chance
                            rareChance = (RARE_POINTS*this->rareLevels->size())+uncommonChance; //recalculate rare chance
                        } else {
                            usedUncommonLevels.insert(levelType);
                        }
                    } else if (random < rareChance) { //rare
                        index = (random-uncommonChance)/RARE_POINTS;
                        assert(index < this->rareLevels->size());
                        levelType = this->rareLevels->at(index);
                        this->rareLevels->remove(index); //rare levels can only spawn once
                        rareChance = (RARE_POINTS*this->rareLevels->size())+uncommonChance; //recalculate rare chance
                    } else assert(false);
                    assert(levelType != Level_Type::CASTLE);
                    index = Random::Get_Instance().Get_Num(unallocatedLevelSlots.size()-1);
                    int levelSlot = unallocatedLevelSlots.at(index);
                    unallocatedLevelSlots.remove(index);
                    this->allocatedLevels->data()[levelSlot] = levelType;
                }
            } else { //use the new distribution algorithm
                qInfo() << "Distributing levels across the entire game...";

                //Determine how many levels to generate for each type
                int totalUnallocatedLevels = 0;
                for (QMap<Level_Type::Level_Type, double>::iterator iter = levelWeights.begin(); iter != levelWeights.end(); ++iter) {
                    int num = static_cast<int>(std::round(static_cast<double>(numLevelsExcludingCastles)*(iter.value()/totalWeight)));
                    if (num == 0) num = 1;
                    totalUnallocatedLevels += num;
                    unallocatedLevels.append(QPair<Level_Type::Level_Type, int>(iter.key(), num));
                }

                //Distribute the remaining levels to the very common types
                while (totalUnallocatedLevels < numLevelsExcludingCastles) {
                    ++totalUnallocatedLevels;
                    Level_Type::Level_Type levelType = this->veryCommonLevels->at(Random::Get_Instance().Get_Num(this->veryCommonLevels->size()-1));
                    for (int i = 0; i < unallocatedLevels.size(); ++i) {
                        if (unallocatedLevels.at(i).first == levelType) ++unallocatedLevels.data()[i].second;
                    }
                }

                //Output the requested number of levels per type
                qInfo().noquote() << "Number of levels excluding Castles:" << numLevelsExcludingCastles;
                for (QVector<QPair<Level_Type::Level_Type, int>>::iterator iter = unallocatedLevels.begin(); iter != unallocatedLevels.end(); ++iter) {
                    switch (iter->first) {
                    case Level_Type::CASTLE:                assert(false); return Level_Type::CASTLE;
                    case Level_Type::PIPE_EXIT:             assert(false); return Level_Type::PIPE_EXIT;
                    case Level_Type::STANDARD_OVERWORLD:    qInfo().noquote() << "Requested Standard Overworld Levels:" << iter->second; break;
                    case Level_Type::UNDERGROUND:           qInfo().noquote() << "Requested Underground Levels:" << iter->second; break;
                    case Level_Type::UNDERWATER:            qInfo().noquote() << "Requested Underwater Levels:" << iter->second; break;
                    case Level_Type::BRIDGE:                qInfo().noquote() << "Requested Bridge Levels:" << iter->second; break;
                    case Level_Type::ISLAND:                qInfo().noquote() << "Requested Island Levels:" << iter->second; break;
                    }
                }

                //Allocate the levels
                QSet<Level_Type::Level_Type> remainingLevelTypes;
                for (int i = 0; i < unallocatedLevels.size(); ++i) {
                    if (unallocatedLevels.at(i).second > 0) remainingLevelTypes.insert(unallocatedLevels.at(i).first);
                }
                for (int i = 0; !unallocatedLevelSlots.isEmpty();) { //iterate through each level type in order
                    //Distribute the remaining levels randomly if there are not enough slots remaining
                    bool randomForRemaining = remainingLevelTypes.size() > unallocatedLevelSlots.size();
                    Level_Type::Level_Type levelType = Level_Type::CASTLE;
                    if (randomForRemaining) {
                        //Get a random level type from the set
                        int remainingLevelTypesIndex = Random::Get_Instance().Get_Num(remainingLevelTypes.size()-1);
                        for (QSet<Level_Type::Level_Type>::iterator iter = remainingLevelTypes.begin(); iter != remainingLevelTypes.end(); ++iter) {
                            if (remainingLevelTypesIndex == 0) {
                                levelType = *iter;
                                break;
                            } else {
                                --remainingLevelTypesIndex;
                            }
                        }
                        assert(levelType != Level_Type::CASTLE);

                        //The index value (i) needs to be found for the specified level type
                        bool found = false;
                        for (int j = 0; j < unallocatedLevels.size() && !found; ++j) {
                            if (unallocatedLevels.at(j).first == levelType) {
                                i = j;
                                found = true;
                            }
                        }
                        assert(found);
                    } else { //distribute in order for uniformity
                        levelType = unallocatedLevels.at(i).first;
                    }
                    int numLevelsLeft = unallocatedLevels.at(i).second;

                    assert(numLevelsLeft >= 0);
                    if (numLevelsLeft > 0) { //allocate the level if there are any left to allocate
                        --unallocatedLevels.data()[i].second;
                        if (randomForRemaining) unallocatedLevels.data()[i].second = 0; //only use one of each remaining type
                        if (unallocatedLevels.data()[i].second == 0) remainingLevelTypes.remove(unallocatedLevels.data()[i].first);
                        int index = Random::Get_Instance().Get_Num(unallocatedLevelSlots.size()-1);
                        int levelSlot = unallocatedLevelSlots.at(index);
                        unallocatedLevelSlots.remove(index);
                        this->allocatedLevels->data()[levelSlot] = levelType;
                    }
                    ++i;
                    if (i == unallocatedLevels.size()) i = 0;
                }
            }

            //Prevent getting 3 of the same type of level in a row
            this->Prevent_First_Level_From_Being_Underwater(numLevelsPerWorld, numLevelTypes);
            if (numLevelTypes > 2) {
                Level_Type::Level_Type slot1 = Level_Type::CASTLE, slot2 = Level_Type::CASTLE, slot3 = Level_Type::CASTLE;
                int lastI = 0;
                for (int i = 0, numFixes = 0; i < this->allocatedLevels->size() && numFixes < numLevels; ++i) {
                    if ((i+1)%numLevelsPerWorld != 0) {
                        slot3 = slot2;
                        slot2 = slot1;
                        slot1 = this->allocatedLevels->at(i);
                        if (slot3 != Level_Type::CASTLE && slot1 == slot2 && slot2 == slot3 && !this->veryCommonLevels->contains(slot1)) {
                            qInfo().noquote().nospace() << "Three levels in a row found! Attempting to fix slot " << lastI << "...";
                            assert(i > 0);
                            assert(i > lastI);
                            Level_Type::Level_Type tmpSlot1 = Level_Type::CASTLE, tmpSlot2 = Level_Type::CASTLE, tmpSlot3 = Level_Type::CASTLE;
                            int lastJ = 0;
                            for (int j = 0; j < this->allocatedLevels->size(); ++j) {
                                if ((j+1)%numLevelsPerWorld != 0) {
                                    tmpSlot3 = tmpSlot2;
                                    tmpSlot2 = tmpSlot1;
                                    tmpSlot1 = this->allocatedLevels->at(j);
                                    if (tmpSlot3 != Level_Type::CASTLE && slot1 != tmpSlot1 && slot1 != tmpSlot2 && slot1 != tmpSlot3) {
                                        //Swap the two middle levels
                                        assert(j > 0);
                                        assert(j > lastJ);
                                        Level_Type::Level_Type tmpLevel = this->allocatedLevels->data()[lastI];
                                        this->allocatedLevels->data()[lastI] = this->allocatedLevels->data()[lastJ];
                                        this->allocatedLevels->data()[lastJ] = tmpLevel;
                                        slot1 = Level_Type::CASTLE, slot2 = Level_Type::CASTLE, slot3 = Level_Type::CASTLE;
                                        ++numFixes;
                                        i = 0; //restart
                                        qInfo() << "Fixed by swapping levels!";
                                        break;
                                    }
                                    lastJ = j;
                                }
                            }

                            //If the swap method fails, simply replace the level with a Very Common level
                            if (i != 0) {
                                this->allocatedLevels->data()[lastI] = this->veryCommonLevels->at(Random::Get_Instance().Get_Num(this->veryCommonLevels->size()-1));
                                slot1 = Level_Type::CASTLE, slot2 = Level_Type::CASTLE, slot3 = Level_Type::CASTLE;
                                ++numFixes;
                                i = 0; //restart
                                qInfo() << "Fixed by replacing with a very common level!";
                            }
                        }
                        lastI = i;
                    }
                }
            }
        } else { //distribute levels without any attempt at uniformity
            QVector<Level_Type::Level_Type> possibleLevels;
            if (this->pluginSettings->includeStandardOverworldLevelsInRandomDistribution) { possibleLevels.append(Level_Type::STANDARD_OVERWORLD); this->veryCommonLevels->append(Level_Type::STANDARD_OVERWORLD); }
            if (this->pluginSettings->includeUndergroundLevelsInRandomDistribution) { possibleLevels.append(Level_Type::UNDERGROUND); this->veryCommonLevels->append(Level_Type::UNDERGROUND); }
            if (this->pluginSettings->includeUnderwaterLevelsInRandomDistribution) { possibleLevels.append(Level_Type::UNDERWATER); this->veryCommonLevels->append(Level_Type::UNDERWATER); }
            if (this->pluginSettings->includeBridgeLevelsInRandomDistribution) { possibleLevels.append(Level_Type::BRIDGE); this->veryCommonLevels->append(Level_Type::BRIDGE); }
            if (this->pluginSettings->includeIslandLevelsInRandomDistribution) { possibleLevels.append(Level_Type::ISLAND); this->veryCommonLevels->append(Level_Type::ISLAND); }
            if (possibleLevels.isEmpty()) { possibleLevels.append(Level_Type::STANDARD_OVERWORLD); this->veryCommonLevels->append(Level_Type::STANDARD_OVERWORLD); } //fall back to standard overworld levels if nothing is specified
            for (int i = 0; i < this->allocatedLevels->size(); ++i) {
                if ((i+1)%numLevelsPerWorld == 0) this->allocatedLevels->data()[i] = Level_Type::CASTLE; //allocate Castle levels now
                else this->allocatedLevels->data()[i] = possibleLevels.at(Random::Get_Instance().Get_Num(possibleLevels.size()-1));
            }
            this->Prevent_First_Level_From_Being_Underwater(numLevelsPerWorld, this->veryCommonLevels->size());
        }
    }

    return this->allocatedLevels->at(levelNum);
}

bool Level_Generator::Generate_New_Levels(QString &generationFileName) {
    //Make the folder to store the random generation in
    QString generationName = "Random " + QDate::currentDate().toString("yy-MM-dd-") + QTime::currentTime().toString("HH-mm-ss-zzz");
    QString folderLocation = this->levelLocation + "/" + generationName;
    generationFileName = generationName + Common_Strings::STRING_LEVELS_EXTENSION;
    QDir dir(this->levelLocation);
    if (!dir.exists(generationName)) {
        if (!dir.mkdir(generationName)) {
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                  "Level-Headed does not have read/write permissions to the Levels directory!", Common_Strings::STRING_OK);
            return false;
        }
    } else {
        if (!dir.cd(generationName) || !dir.removeRecursively()) {
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                  "Level-Headed does not have read/write permissions to the Levels directory!", Common_Strings::STRING_OK);
            return false;
        }
    }

    //Create a new map file buffer
    Text_Insertion_Buffer mapBuffer;
    if (!this->Write_To_Map(mapBuffer, Header::STRING_MAP_NAME)) return false;

    //Randomly determine the number of max levels and levels per world if specified
    if (this->pluginSettings->randomNumWorlds) {
        this->pluginSettings->numWorlds = Random::Get_Instance().Get_Num(4)+2; //max out at 6 worlds
        this->pluginSettings->numLevelsPerWorld = 8;
        while (this->pluginSettings->numLevelsPerWorld*this->pluginSettings->numWorlds > 20) {
            --this->pluginSettings->numLevelsPerWorld;
        }
    }

    //Write the Comment section of the map file
    QString graphicsPack = this->writerPlugin->Graphics_Get_Last_Applied_Graphics_Pack();
    if (graphicsPack.isEmpty()) graphicsPack = "Original";
    if (!this->Write_To_Map(mapBuffer, Level_Type::STRING_BREAK)) return false;
    if (!this->Write_To_Map(mapBuffer, Header::STRING_COOLCORD)) return false;
    if (!this->Write_To_Map(mapBuffer, Header::STRING_CREATED + " " + QDate::currentDate().toString("dddd, MMMM dd, yyyy") + ", at " + QTime::currentTime().toString("hh:mm:ss A."))) return false;
    if (!this->Write_To_Map(mapBuffer, Header::STRING_GRAPHICS_PACK + ": " + graphicsPack)) return false;
    if (!this->Write_To_Map(mapBuffer, Header::STRING_DIFFICULTY + ": " + QString::number(this->pluginSettings->difficultyComboIndex))) return false;
    if (!this->Write_To_Map(mapBuffer, Header::STRING_RANDOM_SEED + ": " + this->pluginSettings->randomSeed)) return false;

    //Write the Header of the map file
    if (!this->Write_To_Map(mapBuffer, Level_Type::STRING_BREAK)) return false;
    if (!this->Write_To_Map(mapBuffer, Header::STRING_NUMBER_OF_WORLDS + ": " + QString::number(this->pluginSettings->numWorlds))) return false;
    if (!this->Write_To_Map(mapBuffer, Header::STRING_NUMBER_OF_LEVELS_PER_WORLD + ": " + QString::number(this->pluginSettings->numLevelsPerWorld))) return false;
    if (!this->Write_To_Map(mapBuffer, Header::STRING_EXCEEDS_VERTICAL_OBJECT_LIMIT + ": " + Header::STRING_FALSE)) return false;
    if (!this->Write_To_Map(mapBuffer, Level_Type::STRING_BREAK)) return false;

    //Build the Move Objects Map
    if (!this->Write_Move_Objects_Map(mapBuffer)) return false;

    //Build the Move Enemies Map
    if (!this->Write_Move_Enemies_Map(mapBuffer)) return false;

    //Build the Room Order Map
    QVector<Level::Level> levelOrder;
    int numLevels = this->pluginSettings->numWorlds*this->pluginSettings->numLevelsPerWorld;
    if (!this->Rearrange_Levels_From_Short_To_Long(levelOrder, numLevels)) {
        qDebug() << "Failed to rearrange the room order";
        return false;
    }
    assert(numLevels == levelOrder.size());

    //Generate the Levels
    bool isVerticalObjectLimitPatchNecessary = false;
    if (!this->Write_To_Map(mapBuffer, Header::STRING_LEVEL_MAP_COMMENT)) return false;
    for (int i = 0; i < numLevels; ++i) {
        //Prepare Arguments
        SMB1_Compliance_Generator_Arguments args = this->Prepare_Arguments(generationName, i, numLevels);

        //Get the number of objects and enemies that are available for the level
        QMap<Level::Level, int>::iterator iter = this->numObjectsInLevel->find(levelOrder.at(i));
        assert(iter != this->numObjectsInLevel->end());
        args.numObjectBytes = iter.value(); //use the value from memory instead of the ROM, as the ROM has not been rearranged yet!
        iter = this->numEnemiesInLevel->find(levelOrder.at(i));
        assert(iter != this->numEnemiesInLevel->end());
        args.numEnemyBytes = iter.value();

        if (!this->generatorPlugin->Generate_Level(args)) {
            qDebug() << "Looks like the generator blew up";
            return false;
        }

        //Write the level to the map
        if (!this->Write_To_Map(mapBuffer, levelOrder.at(i), args.fileName.split("/").last())) return false;
    }
    if (!this->Write_To_Map(mapBuffer, Level_Type::STRING_BREAK)) return false;
    if (!this->Write_To_Map(mapBuffer, Level_Type::STRING_BREAK)) return false;

    //Update the Vertical Object Limit Flag
    if (isVerticalObjectLimitPatchNecessary) {
        mapBuffer.Seek_To_Before_Beginning();
        bool valueUpdated = false;
        while (!mapBuffer.At_End() || !valueUpdated) {
            QString line = mapBuffer.Get_Next_Line();
            if (line.startsWith(Header::STRING_EXCEEDS_VERTICAL_OBJECT_LIMIT)) {
                mapBuffer.Remove_Current_Line();
                mapBuffer.Insert_Before_Current_Line(Header::STRING_EXCEEDS_VERTICAL_OBJECT_LIMIT + ": " + Header::STRING_TRUE);
                valueUpdated = true;
            }
        }
        assert(valueUpdated);
    }

    //Write the Map Buffer to a file
    if (!mapBuffer.Write_To_File(this->levelLocation + "/" + generationName + "/" + Common_Strings::STRING_GAME_NAME + ".map")) {
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                              "The writer plugin failed to write the Map file!", Common_Strings::STRING_OK);
        return false;
    }

    //Pack the Levels into a Sequential Archive
    if (!this->Load_Sequential_Archive_Plugin()) {
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                              "Unable to load the sequential archive plugin!", Common_Strings::STRING_OK);
        return false;
    }
    if (this->sequentialArchivePlugin->Pack(folderLocation, this->levelLocation+"/"+generationFileName) != 0) {
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                              "Unable to pack levels into a sequential archive!", Common_Strings::STRING_OK);
        return false;
    }
    return true;
}

bool Level_Generator::Handle_Map_File() {
    assert(this->sequentialArchivePlugin->Is_Open());
    QTextStream mapFile(this->sequentialArchivePlugin->Read_File("/" + Common_Strings::STRING_GAME_NAME + ".map"), QIODevice::ReadOnly);
    if (mapFile.atEnd()) return false;

    //Parse through the map file starting with the header
    int numWorlds = 0, numLevelsPerWorld = 0, lineNum = 0, errorCode = 0;
    if (!this->Parse_Map_Header(mapFile, numWorlds, numLevelsPerWorld, lineNum, errorCode)) return false;
    if (mapFile.atEnd()) return false;

    //Parse the Move Tables
    QMap<QString, Level::Level> levels;
    this->Populate_Level_Map(levels);
    if (!this->Parse_Move_Table(mapFile, levels, lineNum, errorCode, true)) return false;
    if (mapFile.atEnd()) return false;
    if (!this->Parse_Move_Table(mapFile, levels, lineNum, errorCode, false)) return false;
    if (mapFile.atEnd()) return false;

    //Parse the Levels
    if (!this->Parse_Levels(mapFile, levels, numWorlds, numLevelsPerWorld, lineNum, errorCode)) return false;
    if (!mapFile.atEnd()) return false;
    return true;
}

bool Level_Generator::Load_Sequential_Archive_Plugin() {
    if (this->sequentialArchiveLoaded) return true;
    QString sequentialArchivePluginLocation = this->applicationLocation+"/"+Common_Strings::STRING_PLUGINS+"/Sequential_Archive"+Common_Strings::STRING_PLUGIN_EXTENSION;
    if (!QFile(sequentialArchivePluginLocation).exists()) return false;

    //Load the Sequential Archive Plugin
    this->sequentialArchiveLoader = new QPluginLoader(sequentialArchivePluginLocation);
    QObject *validPlugin = this->sequentialArchiveLoader->instance();
    if (!validPlugin) return false;
    this->sequentialArchivePlugin = qobject_cast<Sequential_Archive_Interface*>(validPlugin);
    bool success = static_cast<bool>(this->sequentialArchivePlugin);
    this->sequentialArchiveLoaded = success;
    return success;
}

void Level_Generator::Read_Level_Chance(const QString &chance, Level_Type::Level_Type levelType) {
    QMap<QString, Chance>::Iterator iter = this->chances->find(chance);
    if (iter == this->chances->end()) assert(false);
    switch (iter.value()) {
    case VERY_COMMON:   veryCommonLevels->append(levelType); break;
    case COMMON:        commonLevels->append(levelType); break;
    case UNCOMMON:      uncommonLevels->append(levelType); break;
    case RARE:          rareLevels->append(levelType); break;
    case NONE:          break;
    }
}

bool Level_Generator::Rearrange_Levels_From_Short_To_Long(QVector<Level::Level> &levelOrder, int numLevels) {
    assert(numLevels > 0 && numLevels <= 20);
    switch (numLevels) { //Add easy levels to the beginning
    case 20:
        levelOrder.append(Level::WORLD_7_LEVEL_1);
        [[clang::fallthrough]]; //fall through
    case 19:
        levelOrder.append(Level::WORLD_1_LEVEL_1);
        [[clang::fallthrough]]; //fall through
    case 18:
        levelOrder.append(Level::WORLD_3_LEVEL_3);
        [[clang::fallthrough]]; //fall through
    case 17:
        levelOrder.append(Level::WORLD_1_LEVEL_4);
        [[clang::fallthrough]]; //fall through
    default:
        break;
    }
    levelOrder.append(Level::WORLD_6_LEVEL_3);
    if (numLevels == 1) return true;
    levelOrder.append(Level::WORLD_2_LEVEL_1);
    if (numLevels == 2) return true;
    levelOrder.append(Level::WORLD_4_LEVEL_3);
    if (numLevels == 3) return true;
    levelOrder.append(Level::WORLD_8_LEVEL_3);
    if (numLevels == 4) return true;
    levelOrder.append(Level::WORLD_3_LEVEL_4);
    if (numLevels == 5) return true;
    levelOrder.append(Level::WORLD_6_LEVEL_1);
    if (numLevels == 6) return true;
    levelOrder.append(Level::WORLD_8_LEVEL_4);
    if (numLevels == 7) return true;
    levelOrder.append(Level::WORLD_5_LEVEL_2);
    if (numLevels == 8) return true;
    levelOrder.append(Level::WORLD_3_LEVEL_1);
    if (numLevels == 9) return true;
    levelOrder.append(Level::WORLD_2_LEVEL_4);
    if (numLevels == 10) return true;
    levelOrder.append(Level::WORLD_8_LEVEL_2);
    if (numLevels == 11) return true;
    levelOrder.append(Level::WORLD_2_LEVEL_2);
    if (numLevels == 12) return true;
    levelOrder.append(Level::WORLD_4_LEVEL_4);
    if (numLevels == 13) return true;
    levelOrder.append(Level::WORLD_2_LEVEL_3);
    if (numLevels == 14) return true;
    levelOrder.append(Level::WORLD_7_LEVEL_4);
    if (numLevels == 15) return true;
    levelOrder.append(Level::WORLD_6_LEVEL_2);
    assert(numLevels >= 16);
    return true;
}

bool Level_Generator::Move_Enemies_In_Map(int numBytes, Level::Level sourceLevel, Level::Level destinationLevel) {
    QMap<Level::Level, int>::iterator sourceLevelIter = this->numEnemiesInLevel->find(sourceLevel);
    assert(sourceLevelIter != this->numEnemiesInLevel->end());
    QMap<Level::Level, int>::iterator destinationLevelIter = this->numEnemiesInLevel->find(destinationLevel);
    assert(destinationLevelIter != this->numEnemiesInLevel->end());
    int newFromLevelValue = sourceLevelIter.value()-numBytes;
    int newToLevelValue = destinationLevelIter.value()+numBytes;
    if (newFromLevelValue < 0) return false;
    this->numEnemiesInLevel->insert(sourceLevel, newFromLevelValue);
    this->numEnemiesInLevel->insert(destinationLevel, newToLevelValue);
    return true;
}

bool Level_Generator::Move_Objects_In_Map(int numBytes, Level::Level sourceLevel, Level::Level destinationLevel) {
    QMap<Level::Level, int>::iterator sourceLevelIter = this->numObjectsInLevel->find(sourceLevel);
    assert(sourceLevelIter != this->numObjectsInLevel->end());
    QMap<Level::Level, int>::iterator destinationLevelIter = this->numObjectsInLevel->find(destinationLevel);
    assert(destinationLevelIter != this->numObjectsInLevel->end());
    int newFromLevelValue = sourceLevelIter.value()-numBytes;
    int newToLevelValue = destinationLevelIter.value()+numBytes;
    if (newFromLevelValue < 0) return false;
    this->numObjectsInLevel->insert(sourceLevel, newFromLevelValue);
    this->numObjectsInLevel->insert(destinationLevel, newToLevelValue);
    return true;
}

bool Level_Generator::Parse_Boolean(const QString &string, bool &value) {
    //The text could be either "True" or "False"
    if (string.toLower() == Header::STRING_TRUE.toLower()) {
        value = true;
        return true;
    } else if (string.toLower() == Header::STRING_FALSE.toLower()) {
        value = false;
        return true;
    }

    //If not, check to see if is a number and convert it
    bool isNumber = false;
    int num = string.toInt(&isNumber, 10);
    if (isNumber) {
        value = static_cast<bool>(num);
        return true;
    }
    return false;
}

bool Level_Generator::Parse_Levels(QTextStream &file, const QMap<QString, Level::Level> &levels, int numWorlds, int numLevelsPerWorld, int &lineNum, int &errorCode) {
    //Read the Level Lines
    int currentLevelNum = 0;
    int currentWorldNum = 1;
    int levelNum = -1;
    QSet<Level::Level> bonusLevels;
    bool success = false, bonusSection = false, lastWasBonusLevel = false;
    do {
        ++lineNum;
        QString line = file.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.at(0) == '#') continue;
        if (line.startsWith("===")) {
            if (bonusSection) {
                success = true;
                break;
            } else {
                bonusSection = true;
            }
        } else {
            QStringList elements = line.split(' ');
            const QMap<QString, Level::Level>::const_iterator iter = levels.find(elements.at(0));
            if (elements.size() > 2 || iter == levels.end()) {
                errorCode = -1;
                QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                      "Syntax error on line " + QString::number(lineNum) + " in " + Common_Strings::STRING_GAME_NAME + ".map!", Common_Strings::STRING_OK);
                return false;
            }

            //Handle the Room order
            Level::Level currentLevel = iter.value();
            bool bonusLevel = bonusSection;
            if (bonusSection) {
                if (elements.size() != 2) {
                    errorCode = -1;
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "Syntax error on line " + QString::number(lineNum) + " in " + Common_Strings::STRING_GAME_NAME + ".map!", Common_Strings::STRING_OK);
                    return false;
                }
            } else {
                //Add the level to the room order table
                if (lastWasBonusLevel) {
                    lastWasBonusLevel = false;
                } else {
                    ++levelNum;
                    ++currentLevelNum;
                    if (currentLevelNum > numLevelsPerWorld) {
                        ++currentWorldNum;
                        currentLevelNum = 1;
                    }
                }
                if (!this->writerPlugin->Room_Table_Set_Next_Level(currentLevel)) {
                    errorCode = -1;
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "Failed to allocate space in the ROM for " + this->Convert_Level_Enum_To_String(currentLevel) + "!", Common_Strings::STRING_OK);
                    return false;
                }

                //Write the midpoint if the level has no script and is not a bonus level
                if (elements.size() == 1) {
                    if (bonusLevels.find(currentLevel) == bonusLevels.end()) {
                        if (!this->writerPlugin->Room_Table_Set_Midpoint_For_Duplicate_Level(currentLevel, currentWorldNum, currentLevelNum)) {
                            errorCode = -1;
                            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                                  "Failed to write thie midpoint for " + this->Convert_Level_Enum_To_String(currentLevel) + "!", Common_Strings::STRING_OK);
                        }
                    } else {
                        bonusLevel = true;
                        lastWasBonusLevel = true;
                    }
                }
            }

            //Parse the level if it has a script
            if (elements.size() == 2) {
                QString scriptName = elements.at(1);
                if (scriptName.size() < 3) {
                    errorCode = -1;
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "Syntax error on line " + QString::number(lineNum) + " in " + Common_Strings::STRING_GAME_NAME + ".map!", Common_Strings::STRING_OK);
                    return false;
                }
                scriptName.chop(1); scriptName = scriptName.remove(0, 1);

                //Check if the level is a bonus level
                QString lowerScriptName = scriptName.toLower();
                if (lowerScriptName.contains("bonus")) {
                    bonusLevel = true;
                    lastWasBonusLevel = true;
                    bonusLevels.insert(currentLevel);
                }

                //Allocate a new level
                if (!this->writerPlugin->New_Level(currentLevel, currentWorldNum, currentLevelNum)) {
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "The writer plugin failed to allocate buffers for a new level!", Common_Strings::STRING_OK);
                    return false;
                }

                //Parse the level script
                SMB1_Compliance_Generator_Arguments args = this->Prepare_Arguments(scriptName, levelNum, numWorlds*numLevelsPerWorld);
                args.difficultyBulletTime = 11; //never use bullet time when parsing!
                SMB1_Compliance_Parser_Arguments parserArgs = this->generatorPlugin->Get_Empty_SMB1_Compliance_Parser_Arguments();
                QTextStream levelStream(this->sequentialArchivePlugin->Read_File("/" + scriptName), QIODevice::ReadOnly);
                parserArgs.levelScriptBytes = levelStream.readAll().toLatin1();
                if (parserArgs.levelScriptBytes.isEmpty()) {
                    errorCode = -1;
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "Unable to open " + scriptName + "!", Common_Strings::STRING_OK);
                    return false;
                }
                parserArgs.enemyBuffer = new Enemy_Buffer(10000);
                parserArgs.objectBuffer = new Object_Buffer(10000, &args);
                if (!this->generatorPlugin->Parse_Level_Script(parserArgs)) {
                    errorCode = -1; delete parserArgs.enemyBuffer; delete parserArgs.objectBuffer;
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "Syntax error on line " + QString::number(parserArgs.lineNum) + " in " + scriptName + "!", Common_Strings::STRING_OK);
                    return false;
                }

                //Update the SMB1_Compliance_Generator_Args
                switch (parserArgs.levelAttribute) {
                case Level_Attribute::OVERWORLD:    args.levelType = Level_Type::STANDARD_OVERWORLD; break;
                case Level_Attribute::UNDERGROUND:  args.levelType = Level_Type::UNDERGROUND; break;
                case Level_Attribute::UNDERWATER:   args.levelType = Level_Type::UNDERWATER; break;
                case Level_Attribute::CASTLE:       args.levelType = Level_Type::CASTLE; break;
                }
                args.levelCompliment = parserArgs.levelCompliment;
                args.headerScenery = parserArgs.headerScenery;
                args.headerBackground = parserArgs.headerBackground;

                //Redistribute Powerups and Enemies if specified
                if (!this->pluginSettings->generateNewLevels) {
                    //Redistribute the Powerups
                    if (this->pluginSettings->redistributePowerups && !this->generatorPlugin->Redistribute_Powerups(args, parserArgs)) {
                        errorCode = -1; delete parserArgs.enemyBuffer; delete parserArgs.objectBuffer;
                        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                              "Powerup redistribution failed in " + scriptName + "!", Common_Strings::STRING_OK);
                        return false;
                    }

                    //Redistribute the Enemies
                    if (this->pluginSettings->redistributeEnemies) {
                        //Update args before redistributing
                        if (this->pluginSettings->performChaoticSwapOnEnemies) args.difficultyHammerTime = 11; //don't use Hammer Time if Chaotic Swap will be used
                        args.difficultyDisableAllOtherEnemiesWhenALakituSpawns = false; //ignore this setting when redistributing enemies

                        if (!this->generatorPlugin->Redistribute_Enemies(args, parserArgs)) {
                            errorCode = -1; delete parserArgs.enemyBuffer; delete parserArgs.objectBuffer;
                            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                                  "Enemy redistribution failed in " + scriptName + "!", Common_Strings::STRING_OK);
                            return false;
                        }
                    }

                    //Perform Chaotic Swap on Enemies
                    if (this->pluginSettings->performChaoticSwapOnEnemies) {
                        if (!this->generatorPlugin->Perform_Enemy_Chaotic_Swap(parserArgs.enemyBuffer, parserArgs.levelAttribute, this->pluginSettings->difficultyAllowHammerBrosWhenRandomizingEnemiesInLevelScripts, this->pluginSettings->difficultyAllowLakitusWhenRandomizingEnemiesInLevelScripts, this->pluginSettings->difficultyAllowBulletBillAndCheepCheepSpawnersWhenRandomizingEnemiesInLevelScripts)) {
                            errorCode = -1; delete parserArgs.enemyBuffer; delete parserArgs.objectBuffer;
                            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                                  "Enemy chaotic swap failed in " + scriptName + "!", Common_Strings::STRING_OK);
                            return false;
                        }
                    }
                }

                //Write the buffers to the ROM
                if (!this->Write_Header_To_Level(parserArgs, bonusLevel)) {
                    errorCode = -1; delete parserArgs.enemyBuffer; delete parserArgs.objectBuffer;
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "The writer plugin failed to write the header for " + scriptName + "!", Common_Strings::STRING_OK);
                    return false;
                }
                if (!this->Write_Objects_To_Level(parserArgs)) {
                    errorCode = -1; delete parserArgs.enemyBuffer; delete parserArgs.objectBuffer;
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "The writer plugin failed to write the object on line " + QString::number(parserArgs.lineNum) + " in " + scriptName + "!", Common_Strings::STRING_OK);
                    return false;
                }
                if (!this->Write_Enemies_To_Level(parserArgs)) {
                    errorCode = -1; delete parserArgs.enemyBuffer; delete parserArgs.objectBuffer;
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "The writer plugin failed to write the enemy on line " + QString::number(parserArgs.lineNum) + " in " + scriptName + "!", Common_Strings::STRING_OK);
                    return false;
                }
                if (!this->writerPlugin->Write_Level()) {
                    errorCode = -1; delete parserArgs.enemyBuffer; delete parserArgs.objectBuffer;
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "The writer plugin failed to write the ROM!", Common_Strings::STRING_OK);
                    return false;
                }

                //Disable the Intro Demo if the first level is an auto scroller
                if (currentWorldNum == 1 && currentLevelNum == 1 && parserArgs.objectBuffer->Was_Auto_Scroll_Used() && !this->writerPlugin->Hacks_Disable_Intro_Demo()) {
                    errorCode = -1; delete parserArgs.enemyBuffer; delete parserArgs.objectBuffer;
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "The writer plugin failed to write the ROM!", Common_Strings::STRING_OK);
                    return false;
                }

                //Deallocate the object and enemy buffers
                delete parserArgs.enemyBuffer; delete parserArgs.objectBuffer;
                parserArgs.enemyBuffer = nullptr; parserArgs.objectBuffer = nullptr;
            }
        }
    } while (!file.atEnd());
    return success;
}

bool Level_Generator::Parse_Level_Map(const QString &fileName) {
    if (!this->Load_Sequential_Archive_Plugin()) {
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                              "Unable to load the sequential archive plugin!", Common_Strings::STRING_OK);
        return false;
    }
    if (!this->sequentialArchivePlugin->Open(this->levelLocation + "/" + fileName)) return false;
    bool success = this->Handle_Map_File();
    this->sequentialArchivePlugin->Close();
    return success;
}

bool Level_Generator::Parse_Map_Header(QTextStream &file, int &numWorlds, int &numLevelsPerWorld, int &lineNum, int &errorCode) {
    //Level Name
    QString line;
    line = file.readLine().trimmed();
    if (line != Header::STRING_MAP_NAME) return false;

    //Notes Section -- Look for 2 seperators
    if (!this->Parse_To_Next_Seperator(file, lineNum)) return false;
    if (!this->Parse_To_Next_Seperator(file, lineNum)) return false;

    //Parse the Number of Worlds
    line = this->Parse_Through_Comments_Until_First_Word(file, Header::STRING_NUMBER_OF_WORLDS + ":", lineNum);
    QStringList elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_NUMBER_OF_WORLDS + ":") return false;
    bool valid = false;
    numWorlds = elements.at(1).toInt(&valid);
    if (!valid) return false; //unable to parse int
    if (!this->writerPlugin->Hacks_Set_Number_Of_Worlds(numWorlds)) {
        errorCode = 3;
        return false;
    }

    //Parse the Number of Levels Per World
    line = this->Parse_Through_Comments_Until_First_Word(file, Header::STRING_NUMBER_OF_LEVELS_PER_WORLD + ":", lineNum);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_NUMBER_OF_LEVELS_PER_WORLD + ":") return false;
    valid = false;
    numLevelsPerWorld = elements.at(1).toInt(&valid);
    if (!valid) return false; //unable to parse int
    if (!this->writerPlugin->Hacks_Set_Number_Of_Levels_Per_World(numLevelsPerWorld)) {
        errorCode = 3;
        return false;
    }

    //Parse the Exceeds Vertical Object Limit Option
    line = this->Parse_Through_Comments_Until_First_Word(file, Header::STRING_EXCEEDS_VERTICAL_OBJECT_LIMIT + ":", lineNum);
    elements = line.split(' ');
    if (elements.size() != 2) return false;
    if (elements.at(0) != Header::STRING_EXCEEDS_VERTICAL_OBJECT_LIMIT + ":") return false;
    bool useVerticalObjectLimitPatch = false;
    if (!this->Parse_Boolean(elements.at(1), useVerticalObjectLimitPatch)) return false;
    if (useVerticalObjectLimitPatch) {
        if (this->pluginSettings->smbUtilityCompatibility) {
            qDebug() << "WARNING: Vertical Object Limit was requested to be removed, but it is disabled!";
        } else {
            if (!this->writerPlugin->Hacks_Remove_Vertical_Object_Limit()) {
                QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                      "This ROM is not compatible with the selected level scripts! Please consider using a fully supported ROM!", Common_Strings::STRING_OK);
                errorCode = 3;
                return false;
            }
        }
    }

    //Parse Seperator at the end
    if (!this->Parse_To_Next_Seperator(file, lineNum)) return false;

    //Randomize the Warp Zones if Specified
    if (!this->pluginSettings->generateNewLevels && this->pluginSettings->randomizeWarpZones && !this->writerPlugin->Hacks_Randomize_Warp_Zone()) return false;

    return true;
}

bool Level_Generator::Parse_Move_Table(QTextStream &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode, bool objects) {
    bool success = false;
    do {
        ++lineNum;
        QString line = file.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.at(0) == '#') continue;
        if (line.startsWith("===")) {
            success = true;
            break;
        } else {
            //Read the source level
            QStringList elements = line.split(' ');
            QMap<QString, Level::Level>::const_iterator iter = levels.find(elements.at(0));
            if (elements.size() > 3 || iter == levels.end()) {
                errorCode = 2; //syntax error
                return false;
            }
            Level::Level sourceLevel = iter.value();

            //Read the destination level
            iter = levels.find(elements.at(1));
            if (iter == levels.end()) {
                errorCode = 2; //syntax error
                return false;
            }
            Level::Level destinationLevel = iter.value();

            //Read the number of bytes
            bool valid = false;
            int numBytes = elements.at(2).toInt(&valid);
            if (!valid || numBytes <= 0) {
                errorCode = 2; //syntax error
                return false;
            }

            //Perform the move
            if (objects) {
                if (!this->Move_Objects_In_Map(numBytes, sourceLevel, destinationLevel) || !this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(sourceLevel, destinationLevel, numBytes)) {
                    errorCode = 3;
                    return false;
                }
            } else {
                if (!this->Move_Enemies_In_Map(numBytes, sourceLevel, destinationLevel) || !this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(sourceLevel, destinationLevel, numBytes)) {
                    errorCode = 3;
                    return false;
                }
            }
        }
    } while (!file.atEnd());
    return success;
}

QString Level_Generator::Parse_Through_Comments_Until_First_Word(QTextStream &file, const QString &word, int &lineNum) {
    do {
        ++lineNum;
        QString line = file.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.at(0) == '#') continue;
        QStringList elements = line.split(' ');
        if (elements.at(0) == word) return line;
    } while (!file.atEnd());
    return "Invalid Line";
}

bool Level_Generator::Parse_To_Next_Seperator(QTextStream &file, int &lineNum) {
    QString line = QString();
    do {
        ++lineNum;
        line = file.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (file.atEnd()) return false; //TODO: Handle this error
    } while (!line.startsWith("==="));
    return true;
}

void Level_Generator::Populate_Level_Map(QMap<QString, Level::Level> &levels) {
    levels.clear();
    levels.insert(Level::STRING_WORLD_1_LEVEL_1, Level::WORLD_1_LEVEL_1);
    levels.insert(Level::STRING_WORLD_1_LEVEL_2, Level::WORLD_1_LEVEL_2);
    levels.insert(Level::STRING_WORLD_1_LEVEL_3, Level::WORLD_1_LEVEL_3);
    levels.insert(Level::STRING_WORLD_1_LEVEL_4, Level::WORLD_1_LEVEL_4);
    levels.insert(Level::STRING_WORLD_2_LEVEL_1, Level::WORLD_2_LEVEL_1);
    levels.insert(Level::STRING_WORLD_2_LEVEL_2, Level::WORLD_2_LEVEL_2);
    levels.insert(Level::STRING_WORLD_2_LEVEL_3, Level::WORLD_2_LEVEL_3);
    levels.insert(Level::STRING_WORLD_2_LEVEL_4, Level::WORLD_2_LEVEL_4);
    levels.insert(Level::STRING_WORLD_3_LEVEL_1, Level::WORLD_3_LEVEL_1);
    levels.insert(Level::STRING_WORLD_3_LEVEL_2, Level::WORLD_3_LEVEL_2);
    levels.insert(Level::STRING_WORLD_3_LEVEL_3, Level::WORLD_3_LEVEL_3);
    levels.insert(Level::STRING_WORLD_3_LEVEL_4, Level::WORLD_3_LEVEL_4);
    levels.insert(Level::STRING_WORLD_4_LEVEL_1, Level::WORLD_4_LEVEL_1);
    levels.insert(Level::STRING_WORLD_4_LEVEL_2, Level::WORLD_4_LEVEL_2);
    levels.insert(Level::STRING_WORLD_4_LEVEL_3, Level::WORLD_4_LEVEL_3);
    levels.insert(Level::STRING_WORLD_4_LEVEL_4, Level::WORLD_4_LEVEL_4);
    levels.insert(Level::STRING_WORLD_5_LEVEL_1, Level::WORLD_5_LEVEL_1);
    levels.insert(Level::STRING_WORLD_5_LEVEL_2, Level::WORLD_5_LEVEL_2);
    levels.insert(Level::STRING_WORLD_5_LEVEL_3, Level::WORLD_5_LEVEL_3);
    levels.insert(Level::STRING_WORLD_5_LEVEL_4, Level::WORLD_5_LEVEL_4);
    levels.insert(Level::STRING_WORLD_6_LEVEL_1, Level::WORLD_6_LEVEL_1);
    levels.insert(Level::STRING_WORLD_6_LEVEL_2, Level::WORLD_6_LEVEL_2);
    levels.insert(Level::STRING_WORLD_6_LEVEL_3, Level::WORLD_6_LEVEL_3);
    levels.insert(Level::STRING_WORLD_6_LEVEL_4, Level::WORLD_6_LEVEL_4);
    levels.insert(Level::STRING_WORLD_7_LEVEL_1, Level::WORLD_7_LEVEL_1);
    levels.insert(Level::STRING_WORLD_7_LEVEL_2, Level::WORLD_7_LEVEL_2);
    levels.insert(Level::STRING_WORLD_7_LEVEL_3, Level::WORLD_7_LEVEL_3);
    levels.insert(Level::STRING_WORLD_7_LEVEL_4, Level::WORLD_7_LEVEL_4);
    levels.insert(Level::STRING_WORLD_8_LEVEL_1, Level::WORLD_8_LEVEL_1);
    levels.insert(Level::STRING_WORLD_8_LEVEL_2, Level::WORLD_8_LEVEL_2);
    levels.insert(Level::STRING_WORLD_8_LEVEL_3, Level::WORLD_8_LEVEL_3);
    levels.insert(Level::STRING_WORLD_8_LEVEL_4, Level::WORLD_8_LEVEL_4);
    levels.insert(Level::STRING_PIPE_INTRO, Level::PIPE_INTRO);
    levels.insert(Level::STRING_UNDERGROUND_BONUS, Level::UNDERGROUND_BONUS);
    levels.insert(Level::STRING_CLOUD_BONUS_1, Level::CLOUD_BONUS_1);
    levels.insert(Level::STRING_CLOUD_BONUS_2, Level::CLOUD_BONUS_2);
    levels.insert(Level::STRING_UNDERWATER_BONUS, Level::UNDERWATER_BONUS);
    levels.insert(Level::STRING_WARP_ZONE, Level::WARP_ZONE);
    levels.insert(Level::STRING_UNDERWATER_CASTLE, Level::UNDERWATER_CASTLE);
}

void Level_Generator::Populate_Num_Objects_And_Enemies_In_Levels() {
    this->numObjectsInLevel->clear();
    this->numObjectsInLevel->insert(Level::WORLD_1_LEVEL_1, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_1_LEVEL_1));
    this->numObjectsInLevel->insert(Level::WORLD_1_LEVEL_2, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_1_LEVEL_2));
    this->numObjectsInLevel->insert(Level::WORLD_1_LEVEL_3, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_1_LEVEL_3));
    this->numObjectsInLevel->insert(Level::WORLD_1_LEVEL_4, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_1_LEVEL_4));
    this->numObjectsInLevel->insert(Level::WORLD_2_LEVEL_1, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_2_LEVEL_1));
    this->numObjectsInLevel->insert(Level::WORLD_2_LEVEL_2, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_2_LEVEL_2));
    this->numObjectsInLevel->insert(Level::WORLD_2_LEVEL_3, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_2_LEVEL_3));
    this->numObjectsInLevel->insert(Level::WORLD_2_LEVEL_4, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_2_LEVEL_4));
    this->numObjectsInLevel->insert(Level::WORLD_3_LEVEL_1, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_3_LEVEL_1));
    this->numObjectsInLevel->insert(Level::WORLD_3_LEVEL_2, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_3_LEVEL_2));
    this->numObjectsInLevel->insert(Level::WORLD_3_LEVEL_3, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_3_LEVEL_3));
    this->numObjectsInLevel->insert(Level::WORLD_3_LEVEL_4, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_3_LEVEL_4));
    this->numObjectsInLevel->insert(Level::WORLD_4_LEVEL_1, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_4_LEVEL_1));
    this->numObjectsInLevel->insert(Level::WORLD_4_LEVEL_2, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_4_LEVEL_2));
    this->numObjectsInLevel->insert(Level::WORLD_4_LEVEL_3, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_4_LEVEL_3));
    this->numObjectsInLevel->insert(Level::WORLD_4_LEVEL_4, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_4_LEVEL_4));
    this->numObjectsInLevel->insert(Level::WORLD_5_LEVEL_1, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_5_LEVEL_1));
    this->numObjectsInLevel->insert(Level::WORLD_5_LEVEL_2, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_5_LEVEL_2));
    this->numObjectsInLevel->insert(Level::WORLD_5_LEVEL_3, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_5_LEVEL_3));
    this->numObjectsInLevel->insert(Level::WORLD_5_LEVEL_4, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_5_LEVEL_4));
    this->numObjectsInLevel->insert(Level::WORLD_6_LEVEL_1, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_6_LEVEL_1));
    this->numObjectsInLevel->insert(Level::WORLD_6_LEVEL_2, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_6_LEVEL_2));
    this->numObjectsInLevel->insert(Level::WORLD_6_LEVEL_3, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_6_LEVEL_3));
    this->numObjectsInLevel->insert(Level::WORLD_6_LEVEL_4, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_6_LEVEL_4));
    this->numObjectsInLevel->insert(Level::WORLD_7_LEVEL_1, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_7_LEVEL_1));
    this->numObjectsInLevel->insert(Level::WORLD_7_LEVEL_2, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_7_LEVEL_2));
    this->numObjectsInLevel->insert(Level::WORLD_7_LEVEL_3, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_7_LEVEL_3));
    this->numObjectsInLevel->insert(Level::WORLD_7_LEVEL_4, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_7_LEVEL_4));
    this->numObjectsInLevel->insert(Level::WORLD_8_LEVEL_1, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_8_LEVEL_1));
    this->numObjectsInLevel->insert(Level::WORLD_8_LEVEL_2, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_8_LEVEL_2));
    this->numObjectsInLevel->insert(Level::WORLD_8_LEVEL_3, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_8_LEVEL_3));
    this->numObjectsInLevel->insert(Level::WORLD_8_LEVEL_4, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WORLD_8_LEVEL_4));
    this->numObjectsInLevel->insert(Level::PIPE_INTRO, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::PIPE_INTRO));
    this->numObjectsInLevel->insert(Level::UNDERGROUND_BONUS, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::UNDERGROUND_BONUS));
    this->numObjectsInLevel->insert(Level::CLOUD_BONUS_1, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::CLOUD_BONUS_1));
    this->numObjectsInLevel->insert(Level::CLOUD_BONUS_2, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::CLOUD_BONUS_2));
    this->numObjectsInLevel->insert(Level::UNDERWATER_BONUS, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::UNDERWATER_BONUS));
    this->numObjectsInLevel->insert(Level::WARP_ZONE, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::WARP_ZONE));
    this->numObjectsInLevel->insert(Level::UNDERWATER_CASTLE, this->writerPlugin->Get_Num_Object_Bytes_In_Level(Level::UNDERWATER_CASTLE));

    this->numEnemiesInLevel->clear();
    this->numEnemiesInLevel->insert(Level::WORLD_1_LEVEL_1, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_1_LEVEL_1));
    this->numEnemiesInLevel->insert(Level::WORLD_1_LEVEL_2, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_1_LEVEL_2));
    this->numEnemiesInLevel->insert(Level::WORLD_1_LEVEL_3, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_1_LEVEL_3));
    this->numEnemiesInLevel->insert(Level::WORLD_1_LEVEL_4, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_1_LEVEL_4));
    this->numEnemiesInLevel->insert(Level::WORLD_2_LEVEL_1, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_2_LEVEL_1));
    this->numEnemiesInLevel->insert(Level::WORLD_2_LEVEL_2, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_2_LEVEL_2));
    this->numEnemiesInLevel->insert(Level::WORLD_2_LEVEL_3, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_2_LEVEL_3));
    this->numEnemiesInLevel->insert(Level::WORLD_2_LEVEL_4, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_2_LEVEL_4));
    this->numEnemiesInLevel->insert(Level::WORLD_3_LEVEL_1, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_3_LEVEL_1));
    this->numEnemiesInLevel->insert(Level::WORLD_3_LEVEL_2, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_3_LEVEL_2));
    this->numEnemiesInLevel->insert(Level::WORLD_3_LEVEL_3, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_3_LEVEL_3));
    this->numEnemiesInLevel->insert(Level::WORLD_3_LEVEL_4, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_3_LEVEL_4));
    this->numEnemiesInLevel->insert(Level::WORLD_4_LEVEL_1, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_4_LEVEL_1));
    this->numEnemiesInLevel->insert(Level::WORLD_4_LEVEL_2, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_4_LEVEL_2));
    this->numEnemiesInLevel->insert(Level::WORLD_4_LEVEL_3, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_4_LEVEL_3));
    this->numEnemiesInLevel->insert(Level::WORLD_4_LEVEL_4, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_4_LEVEL_4));
    this->numEnemiesInLevel->insert(Level::WORLD_5_LEVEL_1, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_5_LEVEL_1));
    this->numEnemiesInLevel->insert(Level::WORLD_5_LEVEL_2, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_5_LEVEL_2));
    this->numEnemiesInLevel->insert(Level::WORLD_5_LEVEL_3, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_5_LEVEL_3));
    this->numEnemiesInLevel->insert(Level::WORLD_5_LEVEL_4, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_5_LEVEL_4));
    this->numEnemiesInLevel->insert(Level::WORLD_6_LEVEL_1, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_6_LEVEL_1));
    this->numEnemiesInLevel->insert(Level::WORLD_6_LEVEL_2, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_6_LEVEL_2));
    this->numEnemiesInLevel->insert(Level::WORLD_6_LEVEL_3, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_6_LEVEL_3));
    this->numEnemiesInLevel->insert(Level::WORLD_6_LEVEL_4, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_6_LEVEL_4));
    this->numEnemiesInLevel->insert(Level::WORLD_7_LEVEL_1, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_7_LEVEL_1));
    this->numEnemiesInLevel->insert(Level::WORLD_7_LEVEL_2, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_7_LEVEL_2));
    this->numEnemiesInLevel->insert(Level::WORLD_7_LEVEL_3, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_7_LEVEL_3));
    this->numEnemiesInLevel->insert(Level::WORLD_7_LEVEL_4, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_7_LEVEL_4));
    this->numEnemiesInLevel->insert(Level::WORLD_8_LEVEL_1, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_8_LEVEL_1));
    this->numEnemiesInLevel->insert(Level::WORLD_8_LEVEL_2, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_8_LEVEL_2));
    this->numEnemiesInLevel->insert(Level::WORLD_8_LEVEL_3, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_8_LEVEL_3));
    this->numEnemiesInLevel->insert(Level::WORLD_8_LEVEL_4, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WORLD_8_LEVEL_4));
    this->numEnemiesInLevel->insert(Level::PIPE_INTRO, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::PIPE_INTRO));
    this->numEnemiesInLevel->insert(Level::UNDERGROUND_BONUS, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::UNDERGROUND_BONUS));
    this->numEnemiesInLevel->insert(Level::CLOUD_BONUS_1, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::CLOUD_BONUS_1));
    this->numEnemiesInLevel->insert(Level::CLOUD_BONUS_2, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::CLOUD_BONUS_2));
    this->numEnemiesInLevel->insert(Level::UNDERWATER_BONUS, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::UNDERWATER_BONUS));
    this->numEnemiesInLevel->insert(Level::WARP_ZONE, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::WARP_ZONE));
    this->numEnemiesInLevel->insert(Level::UNDERWATER_CASTLE, this->writerPlugin->Get_Num_Enemy_Bytes_In_Level(Level::UNDERWATER_CASTLE));
}

void Level_Generator::Prevent_First_Level_From_Being_Underwater(int numLevelsPerWorld, int numLevelTypes) {
    //If the first level is an underwater level, swap it with the first non-underwater level
    if (this->pluginSettings->difficultyPreventTheFirstLevelFromBeingUnderwater && this->allocatedLevels->at(0) == Level_Type::UNDERWATER && numLevelTypes > 1) {
        bool swapSucceeded = false;
        for (int i = 1; i < this->allocatedLevels->size(); ++i) {
            if ((i+1)%numLevelsPerWorld != 0 && this->allocatedLevels->at(i) != Level_Type::UNDERWATER) {
                this->allocatedLevels->data()[0] = this->allocatedLevels->at(i);
                this->allocatedLevels->data()[i] = Level_Type::UNDERWATER;
                swapSucceeded = true;
                break;
            }
        }

        //If the swap method fails, resort to replacing the first level with another level type
        if (!swapSucceeded) {
            bool success = false;
            for (int i = 0; i < this->veryCommonLevels->size(); ++i) {
                if (this->veryCommonLevels->at(i) != Level_Type::UNDERWATER) { this->allocatedLevels->data()[0] = this->veryCommonLevels->at(i); success = true; }
            }
            for (int i = 0; i < this->commonLevels->size() && !success; ++i) {
                if (this->commonLevels->at(i) != Level_Type::UNDERWATER) { this->allocatedLevels->data()[0] = this->commonLevels->at(i); success = true; }
            }
            for (int i = 0; i < this->uncommonLevels->size() && !success; ++i) {
                if (this->uncommonLevels->at(i) != Level_Type::UNDERWATER) { this->allocatedLevels->data()[0] = this->uncommonLevels->at(i); success = true; }
            }
            for (int i = 0; i < this->rareLevels->size() && !success; ++i) {
                if (this->rareLevels->at(i) != Level_Type::UNDERWATER) { this->allocatedLevels->data()[0] = this->rareLevels->at(i); success = true; }
            }
            assert(success);
        }
    }
}

bool Level_Generator::Write_Move_Items_To_Map(Text_Insertion_Buffer &mapBuffer, int numBytes, Level::Level sourceLevel, Level::Level destinationLevel, bool objects) {
    if (objects) {
        if (!this->Move_Objects_In_Map(numBytes, sourceLevel, destinationLevel)) return false;
    } else {
        if (!this->Move_Enemies_In_Map(numBytes, sourceLevel, destinationLevel)) return false;
    }
    return this->Write_To_Map(mapBuffer, this->Convert_Level_Enum_To_String(sourceLevel) + " " + this->Convert_Level_Enum_To_String(destinationLevel) + " " + QString::number(numBytes));
}

bool Level_Generator::Write_Move_Objects_Map(Text_Insertion_Buffer &mapBuffer) {
    if (!this->Write_To_Map(mapBuffer, Header::STRING_OBJECT_MOVE_TABLE_COMMENT)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 8, Level::UNDERWATER_CASTLE, Level::WORLD_1_LEVEL_1, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 14, Level::UNDERWATER_CASTLE, Level::UNDERWATER_BONUS, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 2, Level::UNDERWATER_CASTLE, Level::WORLD_7_LEVEL_1, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 12, Level::WORLD_3_LEVEL_2, Level::WORLD_7_LEVEL_1, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 14, Level::WORLD_3_LEVEL_2, Level::WORLD_3_LEVEL_3, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 2, Level::WARP_ZONE, Level::WORLD_3_LEVEL_3, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 20, Level::WARP_ZONE, Level::CLOUD_BONUS_2, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 18, Level::WARP_ZONE, Level::WORLD_1_LEVEL_4, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 4, Level::WORLD_5_LEVEL_1, Level::WORLD_1_LEVEL_4, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 30, Level::WORLD_5_LEVEL_1, Level::WORLD_6_LEVEL_3, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 26, Level::WORLD_5_LEVEL_1, Level::WORLD_6_LEVEL_1, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 4, Level::WORLD_1_LEVEL_3, Level::WORLD_6_LEVEL_1, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 32, Level::WORLD_1_LEVEL_3, Level::WORLD_2_LEVEL_1, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 26, Level::WORLD_1_LEVEL_3, Level::WORLD_4_LEVEL_3, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 6, Level::WORLD_4_LEVEL_1, Level::WORLD_4_LEVEL_3, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 32, Level::WORLD_4_LEVEL_1, Level::WORLD_8_LEVEL_3, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 32, Level::WORLD_4_LEVEL_1, Level::WORLD_3_LEVEL_4, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 10, Level::WORLD_4_LEVEL_1, Level::WORLD_8_LEVEL_4, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 26, Level::WORLD_8_LEVEL_1, Level::WORLD_8_LEVEL_4, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 36, Level::WORLD_8_LEVEL_1, Level::WORLD_7_LEVEL_4, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 36, Level::WORLD_8_LEVEL_1, Level::WORLD_6_LEVEL_2, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 38, Level::WORLD_8_LEVEL_1, Level::WORLD_5_LEVEL_2, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 8, Level::WORLD_8_LEVEL_1, Level::WORLD_3_LEVEL_1, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 30, Level::WORLD_4_LEVEL_2, Level::WORLD_3_LEVEL_1, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 38, Level::WORLD_4_LEVEL_2, Level::WORLD_2_LEVEL_3, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 40, Level::WORLD_4_LEVEL_2, Level::WORLD_8_LEVEL_2, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 40, Level::WORLD_4_LEVEL_2, Level::WORLD_4_LEVEL_4, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 10, Level::WORLD_4_LEVEL_2, Level::WORLD_2_LEVEL_4, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 32, Level::WORLD_1_LEVEL_2, Level::WORLD_2_LEVEL_4, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 42, Level::WORLD_1_LEVEL_2, Level::WORLD_2_LEVEL_2, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 42, Level::WORLD_1_LEVEL_2, Level::CLOUD_BONUS_1, true)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 44, Level::WORLD_1_LEVEL_2, Level::UNDERGROUND_BONUS, true)) return false;
    if (!this->Write_To_Map(mapBuffer, Level_Type::STRING_BREAK)) return false;
    return true;
}

bool Level_Generator::Write_Move_Enemies_Map(Text_Insertion_Buffer &mapBuffer) {
    if (!this->Write_To_Map(mapBuffer, Header::STRING_ENEMY_MOVE_TABLE_COMMENT)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 2, Level::WORLD_2_LEVEL_1, Level::WORLD_6_LEVEL_3, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 3, Level::CLOUD_BONUS_1, Level::WORLD_3_LEVEL_4, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 1, Level::CLOUD_BONUS_2, Level::WORLD_3_LEVEL_4, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 2, Level::CLOUD_BONUS_2, Level::WORLD_4_LEVEL_3, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 4, Level::WORLD_7_LEVEL_1, Level::WORLD_4_LEVEL_3, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 1, Level::WORLD_7_LEVEL_1, Level::WORLD_3_LEVEL_1, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 5, Level::WORLD_1_LEVEL_1, Level::WORLD_3_LEVEL_1, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 6, Level::WORLD_3_LEVEL_3, Level::WORLD_5_LEVEL_2, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 4, Level::WORLD_1_LEVEL_4, Level::WORLD_5_LEVEL_2, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 2, Level::WORLD_1_LEVEL_4, Level::WORLD_2_LEVEL_4, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 7, Level::WORLD_8_LEVEL_4, Level::WORLD_2_LEVEL_4, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 1, Level::UNDERGROUND_BONUS, Level::WORLD_2_LEVEL_4, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 11, Level::UNDERGROUND_BONUS, Level::WORLD_8_LEVEL_2, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 4, Level::WORLD_4_LEVEL_1, Level::WORLD_8_LEVEL_2, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 9, Level::WORLD_4_LEVEL_1, Level::WORLD_8_LEVEL_3, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 7, Level::UNDERWATER_CASTLE, Level::WORLD_8_LEVEL_3, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 12, Level::UNDERWATER_CASTLE, Level::WORLD_2_LEVEL_2, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 11, Level::WORLD_3_LEVEL_2, Level::WORLD_2_LEVEL_2, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 25, Level::WORLD_3_LEVEL_2, Level::WORLD_6_LEVEL_1, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 15, Level::WORLD_1_LEVEL_3, Level::WORLD_6_LEVEL_1, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 13, Level::WORLD_1_LEVEL_3, Level::WORLD_4_LEVEL_4, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 29, Level::WORLD_5_LEVEL_1, Level::WORLD_4_LEVEL_4, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 6, Level::WORLD_5_LEVEL_1, Level::WORLD_6_LEVEL_2, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 38, Level::WORLD_1_LEVEL_2, Level::WORLD_6_LEVEL_2, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 6, Level::WORLD_1_LEVEL_2, Level::WORLD_2_LEVEL_3, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 44, Level::WORLD_4_LEVEL_2, Level::WORLD_2_LEVEL_3, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 1, Level::WORLD_4_LEVEL_2, Level::WORLD_7_LEVEL_4, false)) return false;
    if (!this->Write_Move_Items_To_Map(mapBuffer, 57, Level::WORLD_8_LEVEL_1, Level::WORLD_7_LEVEL_4, false)) return false;
    if (!this->Write_To_Map(mapBuffer, Level_Type::STRING_BREAK)) return false;
    return true;
}

bool Level_Generator::Write_Header_To_Level(const SMB1_Compliance_Parser_Arguments &args, bool bonusLevel) {
    if (!this->writerPlugin->Header_Attribute(args.levelAttribute)) return false;
    if (!this->writerPlugin->Header_Autowalk(args.autowalk)) return false;
    if (!this->writerPlugin->Header_Starting_Position(args.staringPosition)) return false;
    if (!this->writerPlugin->Header_Brick(args.headerBrick)) return false;
    if (!this->writerPlugin->Header_Background(args.headerBackground))  return false;
    if (!this->writerPlugin->Header_Scenery(args.headerScenery)) return false;
    if (!this->writerPlugin->Header_Level_Compliment(args.levelCompliment)) return false;
    if (!this->writerPlugin->Header_Time(args.time)) return false;
    if (!bonusLevel && !this->writerPlugin->Header_Midpoint(args.midpoint)) return false;
    return true;
}

bool Level_Generator::Write_Objects_To_Level(SMB1_Compliance_Parser_Arguments &args) {
    Object_Buffer *objectBuffer = args.objectBuffer;
    assert(objectBuffer);
    objectBuffer->Seek_To_First_Item();
    while (!objectBuffer->At_End()) {
        Buffer_Data data = objectBuffer->Get_Current();
        args.lineNum = data.lineNum;
        bool success = false;
        switch (data.objectItem) {
        case Object_Item::QUESTION_BLOCK_WITH_MUSHROOM:             success = this->writerPlugin->Object_Question_Block_With_Mushroom(data.x, data.y); break;
        case Object_Item::QUESTION_BLOCK_WITH_COIN:                 success = this->writerPlugin->Object_Question_Block_With_Coin(data.x, data.y); break;
        case Object_Item::HIDDEN_BLOCK_WITH_COIN:                   success = this->writerPlugin->Object_Hidden_Block_With_Coin(data.x, data.y); break;
        case Object_Item::HIDDEN_BLOCK_WITH_1UP:                    success = this->writerPlugin->Object_Hidden_Block_With_1up(data.x, data.y); break;
        case Object_Item::BRICK_WITH_MUSHROOM:                      success = this->writerPlugin->Object_Brick_With_Mushroom(data.x, data.y); break;
        case Object_Item::BRICK_WITH_VINE:                          success = this->writerPlugin->Object_Brick_With_Vine(data.x, data.y); break;
        case Object_Item::BRICK_WITH_STAR:                          success = this->writerPlugin->Object_Brick_With_Star(data.x, data.y); break;
        case Object_Item::BRICK_WITH_10_COINS:                      success = this->writerPlugin->Object_Brick_With_10_Coins(data.x, data.y); break;
        case Object_Item::BRICK_WITH_1UP:                           success = this->writerPlugin->Object_Brick_With_1up(data.x, data.y); break;
        case Object_Item::UNDERWATER_SIDEWAYS_PIPE:                 success = this->writerPlugin->Object_Underwater_Sideways_Pipe(data.x, data.y); break;
        case Object_Item::USED_BLOCK:                               success = this->writerPlugin->Object_Used_Block(data.x, data.y); break;
        case Object_Item::TRAMPOLINE:                               success = this->writerPlugin->Object_Trampoline(data.x, data.y); break;
        case Object_Item::BULLET_BILL_TURRET:                       success = this->writerPlugin->Object_Cannon(data.x, data.y, data.height); break;
        case Object_Item::ISLAND:                                   success = this->writerPlugin->Object_Island(data.x, data.y, data.length); break;
        case Object_Item::HORIZONTAL_BRICKS:                        success = this->writerPlugin->Object_Horizontal_Bricks(data.x, data.y, data.length); break;
        case Object_Item::HORIZONTAL_BLOCKS:                        success = this->writerPlugin->Object_Horizontal_Blocks(data.x, data.y, data.length); break;
        case Object_Item::HORIZONTAL_COINS:                         success = this->writerPlugin->Object_Horizontal_Coins(data.x, data.y, data.length); break;
        case Object_Item::VERTICAL_BRICKS:                          success = this->writerPlugin->Object_Vertical_Bricks(data.x, data.y, data.height); break;
        case Object_Item::VERTICAL_BLOCKS:                          success = this->writerPlugin->Object_Vertical_Blocks(data.x, data.y, data.height); break;
        case Object_Item::CORAL:                                    success = this->writerPlugin->Object_Coral(data.x, data.y, data.height); break;
        case Object_Item::PIPE:                                     success = this->writerPlugin->Object_Pipe(data.x, data.y, data.height, false); break;
        case Object_Item::ENTERABLE_PIPE:                           success = this->writerPlugin->Object_Pipe(data.x, data.y, data.height, true); break;
        case Object_Item::HOLE:                                     success = this->writerPlugin->Object_Hole(data.x, data.length, false); break;
        case Object_Item::HOLE_WITH_WATER:                          success = this->writerPlugin->Object_Hole(data.x, data.length, true); break;
        case Object_Item::BRIDGE:                                   success = this->writerPlugin->Object_Bridge(data.x, data.y, data.length); break;
        case Object_Item::HORIZONTAL_QUESTION_BLOCKS_WITH_COINS:    success = this->writerPlugin->Object_Horizontal_Question_Blocks_With_Coins(data.x, data.y, data.length); break;
        case Object_Item::PAGE_CHANGE:                              success = this->writerPlugin->Object_Page_Change(data.page); break;
        case Object_Item::REVERSE_L_PIPE:                           success = this->writerPlugin->Object_Reverse_L_Pipe(data.x); break;
        case Object_Item::FLAGPOLE:                                 success = this->writerPlugin->Object_Flagpole(data.x); break;
        case Object_Item::CASTLE:                                   success = this->writerPlugin->Object_Castle(data.x); break;
        case Object_Item::BIG_CASTLE:                               success = this->writerPlugin->Object_Big_Castle(data.x); break;
        case Object_Item::AXE:                                      success = this->writerPlugin->Object_Axe(data.x); break;
        case Object_Item::AXE_ROPE:                                 success = this->writerPlugin->Object_Axe_Rope(data.x); break;
        case Object_Item::BOWSER_BRIDGE:                            success = this->writerPlugin->Object_Bowser_Bridge(data.x); break;
        case Object_Item::SCROLL_STOP:                              success = this->writerPlugin->Object_Scroll_Stop(data.x, false); break;
        case Object_Item::SCROLL_STOP_WARP_ZONE:                    success = this->writerPlugin->Object_Scroll_Stop(data.x, true); break;
        case Object_Item::TOGGLE_AUTO_SCROLL:                       success = this->writerPlugin->Object_Toggle_Auto_Scroll(data.x); break;
        case Object_Item::FLYING_CHEEP_CHEEP_SPAWNER:               success = this->writerPlugin->Object_Flying_Cheep_Cheep_Spawner(data.x); break;
        case Object_Item::SWIMMING_CHEEP_CHEEP_SPAWNER:             success = this->writerPlugin->Object_Swimming_Cheep_Cheep_Spawner(data.x); break;
        case Object_Item::BULLET_BILL_SPAWNER:                      success = this->writerPlugin->Object_Bullet_Bill_Spawner(data.x); break;
        case Object_Item::CANCEL_SPAWNER:                           success = this->writerPlugin->Object_Cancel_Spawner(data.x); break;
        case Object_Item::LOOP_COMMAND:                             success = this->writerPlugin->Object_Loop_Command(data.x); break;
        case Object_Item::CHANGE_BRICK_AND_SCENERY:                 success = this->writerPlugin->Object_Change_Brick_And_Scenery(data.x, data.brick, data.scenery); break;
        case Object_Item::CHANGE_BACKGROUND:                        success = this->writerPlugin->Object_Change_Background(data.x, data.background); break;
        case Object_Item::LIFT_ROPE:                                success = this->writerPlugin->Object_Lift_Rope(data.x); break;
        case Object_Item::BALANCE_LIFT_VERTICAL_ROPE:               success = this->writerPlugin->Object_Balance_Lift_Vertical_Rope(data.x, data.height); break;
        case Object_Item::BALANCE_LIFT_HORIZONTAL_ROPE:             success = this->writerPlugin->Object_Balance_Lift_Horizontal_Rope(data.x, data.length); break;
        case Object_Item::STEPS:                                    success = this->writerPlugin->Object_Steps(data.x, data.length); break;
        case Object_Item::END_STEPS:                                success = this->writerPlugin->Object_End_Steps(data.x); break;
        case Object_Item::TALL_REVERSE_L_PIPE:                      success = this->writerPlugin->Object_Tall_Reverse_L_Pipe(data.x, data.y); break;
        case Object_Item::PIPE_WALL:                                success = this->writerPlugin->Object_Pipe_Wall(data.x); break;
        case Object_Item::NOTHING:                                  success = this->writerPlugin->Object_Nothing(data.x); break;
        }
        if (!success) return false;
        objectBuffer->Seek_To_Next();
    }
    return true;
}

bool Level_Generator::Write_Enemies_To_Level(SMB1_Compliance_Parser_Arguments &args) {
    Enemy_Buffer *enemyBuffer = args.enemyBuffer;
    assert(enemyBuffer);
    enemyBuffer->Seek_To_First_Item();
    while (!enemyBuffer->At_End()) {
        Buffer_Data data = enemyBuffer->Get_Current();
        args.lineNum = data.lineNum;
        bool success = false;
        switch (data.enemyItem) {
        case Enemy_Item::GREEN_KOOPA:           success = this->writerPlugin->Enemy_Green_Koopa(data.x, data.y, data.moving, data.onlyHardMode); break;
        case Enemy_Item::RED_KOOPA:             success = this->writerPlugin->Enemy_Red_Koopa(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::BUZZY_BEETLE:          success = this->writerPlugin->Enemy_Buzzy_Beetle(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::HAMMER_BRO:            success = this->writerPlugin->Enemy_Hammer_Bro(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::GOOMBA:                success = this->writerPlugin->Enemy_Goomba(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::BLOOPER:               success = this->writerPlugin->Enemy_Blooper(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::BULLET_BILL:           success = this->writerPlugin->Enemy_Bullet_Bill(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::GREEN_PARATROOPA:      success = this->writerPlugin->Enemy_Green_Paratroopa(data.x, data.y, data.moving, data.leaping, data.onlyHardMode); break;
        case Enemy_Item::RED_PARATROOPA:        success = this->writerPlugin->Enemy_Red_Paratroopa(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::GREEN_CHEEP_CHEEP:     success = this->writerPlugin->Enemy_Green_Cheep_Cheep(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::RED_CHEEP_CHEEP:       success = this->writerPlugin->Enemy_Red_Cheep_Cheep(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::PODOBOO:               success = this->writerPlugin->Enemy_Podoboo(data.x, data.onlyHardMode); break;
        case Enemy_Item::PIRANHA_PLANT:         success = this->writerPlugin->Enemy_Piranha_Plant(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::LAKITU:                success = this->writerPlugin->Enemy_Lakitu(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::SPINY:                 success = this->writerPlugin->Enemy_Spiny(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::BOWSER_FIRE_SPAWNER:   success = this->writerPlugin->Enemy_Bowser_Fire_Spawner(data.x, data.onlyHardMode); break;
        case Enemy_Item::CHEEP_CHEEP_SPAWNER:   success = this->writerPlugin->Enemy_Swimming_Cheep_Cheep_Spawner(data.x, data.leaping, data.onlyHardMode); break;
        case Enemy_Item::BULLET_BILL_SPAWNER:   success = this->writerPlugin->Enemy_Bullet_Bill_Spawner(data.x, data.onlyHardMode); break;
        case Enemy_Item::FIRE_BAR:              success = this->writerPlugin->Enemy_Fire_Bar(data.x, data.y, data.clockwise, data.fast, data.onlyHardMode); break;
        case Enemy_Item::LARGE_FIRE_BAR:        success = this->writerPlugin->Enemy_Large_Fire_Bar(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::LIFT:                  success = this->writerPlugin->Enemy_Lift(data.x, data.y, data.vertical, data.onlyHardMode); break;
        case Enemy_Item::FALLING_LIFT:          success = this->writerPlugin->Enemy_Falling_Lift(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::BALANCE_LIFT:          success = this->writerPlugin->Enemy_Balance_Lift(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::SURFING_LIFT:          success = this->writerPlugin->Enemy_Surfing_Lift(data.x, data.y, data.onlyHardMode); break;
        case Enemy_Item::LIFT_SPAWNER:          success = this->writerPlugin->Enemy_Lift_Spawner(data.x, data.y, data.up, data.small, data.onlyHardMode); break;
        case Enemy_Item::BOWSER:                success = this->writerPlugin->Enemy_Bowser(data.x, data.onlyHardMode); break;
        case Enemy_Item::WARP_ZONE:             success = this->writerPlugin->Enemy_Warp_Zone(data.x); break;
        case Enemy_Item::PIPE_POINTER:          success = this->writerPlugin->Enemy_Pipe_Pointer(data.x, this->Convert_Level_Enum_To_String(data.level), data.world, data.page); break;
        case Enemy_Item::TOAD:                  success = this->writerPlugin->Enemy_Toad(data.x, data.onlyHardMode); break;
        case Enemy_Item::GOOMBA_GROUP:          success = this->writerPlugin->Enemy_Goomba_Group(data.x, data.y, data.num, data.onlyHardMode); break;
        case Enemy_Item::KOOPA_GROUP:           success = this->writerPlugin->Enemy_Koopa_Group(data.x, data.y, data.num, data.onlyHardMode); break;
        case Enemy_Item::PAGE_CHANGE:           success = this->writerPlugin->Enemy_Page_Change(data.page); break;
        case Enemy_Item::NOTHING:               return false;
        }
        if (!success) return false;
        enemyBuffer->Seek_To_Next();
    }
    return true;
}

bool Level_Generator::Write_To_Map(Text_Insertion_Buffer &mapBuffer, const QString &string) {
    mapBuffer.Insert_At_End(string);
    return true;
}

bool Level_Generator::Write_To_Map(Text_Insertion_Buffer &mapBuffer, Level::Level level, const QString &fileName) {
    QString line = this->Convert_Level_Enum_To_String(level);
    if (fileName.size() > 0) line += " \"" + fileName + "\"";
    return this->Write_To_Map(mapBuffer, line);
}

