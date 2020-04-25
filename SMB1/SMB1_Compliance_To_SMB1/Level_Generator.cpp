#include "Level_Generator.h"
#include "../../../C_Common_Code/Qt/Text_Insertion_Buffer/Text_Insertion_Buffer.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include "../Common_SMB1_Files/Level_String.h"
#include "../Common_SMB1_Files/Header_String.h"
#include "../SMB1_Compliance_Generator/Difficulty.h"
#include "../SMB1_Writer/ROM_Filename.h"
#include "../SMB1_Writer/SMB1_Writer_Strings.h"
#include "SMB1_Compliance_Parser.h"
#include <QMessageBox>
#include <QDir>
#include <QDate>
#include <QSet>
#include <QTextStream>
#include <QTime>
#include <QDebug>
#include <math.h>
#include <assert.h>

const static int VERY_COMMON_POINTS = 14;
const static int COMMON_POINTS = 8;
const static int UNCOMMON_POINTS = 5;
const static int RARE_POINTS = 1;

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
}

bool Level_Generator::Generate_Levels() {
    QString folderLocation = QString();
    bool success = this->Generate_Levels_And_Pack(folderLocation);
    QDir dir(folderLocation);
    this->Delete_Old_Level_Generations();
    return success;
}

bool Level_Generator::Parse_Level_Map() {
    if (!this->Load_Sequential_Archive_Plugin()) {
        QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                              "Unable to load the sequential archive plugin!", Common_Strings::STRING_OK);
        return false;
    }
    if (!this->sequentialArchivePlugin->Open(this->levelLocation + "/" + this->pluginSettings->levelScripts + Common_Strings::STRING_LEVELS_EXTENSION)) return false;
    bool success = this->Handle_Map_File();
    this->sequentialArchivePlugin->Close();
    return success;
}

bool Level_Generator::Parse_Map_Header(QTextStream &file, int &numLevelsPerWorld, int &lineNum, int &errorCode) {
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
    int numWorlds = elements.at(1).toInt(&valid);
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

    return true;
}

bool Level_Generator::Parse_Move_Object_Table(QTextStream &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode) {
    return this->Parse_Move_Table(file, levels, lineNum, errorCode, true);
}

bool Level_Generator::Parse_Move_Enemy_Table(QTextStream &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode) {
    return this->Parse_Move_Table(file, levels, lineNum, errorCode, false);
}

bool Level_Generator::Parse_Levels(QTextStream &file, const QMap<QString, Level::Level> &levels, int numLevelsPerWorld, int &lineNum, int &errorCode) {
    //Read the Level Lines
    int currentLevelNum = 0;
    int currentWorldNum = 1;
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
                errorCode = 2; //syntax error
                return false;
            }

            //Handle the Room order
            Level::Level currentLevel = iter.value();
            bool bonusLevel = bonusSection;
            if (bonusSection) {
                if (elements.size() != 2) {
                    errorCode = 2; //syntax error
                    return false;
                }
            } else {
                //Add the level to the room order table
                if (lastWasBonusLevel) {
                    lastWasBonusLevel = false;
                } else {
                    ++currentLevelNum;
                    if (currentLevelNum > numLevelsPerWorld) {
                        ++currentWorldNum;
                        currentLevelNum = 1;
                    }
                }
                if (!this->writerPlugin->Room_Table_Set_Next_Level(currentLevel)) {
                    errorCode = 3;
                    return false;
                }

                //Write the midpoint if the level has no script and is not a bonus level
                if (elements.size() == 1) {
                    if (bonusLevels.find(currentLevel) == bonusLevels.end()) {
                        if (!this->writerPlugin->Room_Table_Set_Midpoint_For_Duplicate_Level(currentLevel, currentWorldNum, currentLevelNum)) {
                            errorCode = 3;
                            return false;
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
                    errorCode = 2; //syntax error
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

                //TODO: Make these error messages file specific
                SMB1_Compliance_Parser parser(this->writerPlugin, this->pluginSettings->randomEnemies, this->pluginSettings->difficultyAllowHammerBrosWhenRandomizingEnemiesInLevelScripts, this->pluginSettings->difficultyAllowLakitusWhenRandomizingEnemiesInLevelScripts, this->pluginSettings->difficultyAllowBulletBillAndCheepCheepSpawnersWhenRandomizingEnemiesInLevelScripts);
                int levelLineNum = 0;
                QTextStream levelStream(this->sequentialArchivePlugin->Read_File("/" + scriptName), QIODevice::ReadOnly);
                int levelErrorCode = parser.Parse_Level(&levelStream, bonusLevel, levelLineNum);
                errorCode = -1;
                switch (levelErrorCode) {
                case -1: //An error occurred and was handled within the parser
                    return false;
                case 0: break; //Parser ran fine
                case 1: //Unable to open the file
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "Unable to open " + elements.at(1) + "!", Common_Strings::STRING_OK);
                    return false;
                case 2: //Syntax error
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "Syntax error on line " + QString::number(levelLineNum) + " in " + elements.at(1) + "!", Common_Strings::STRING_OK);
                    return false;
                case 3: //Writer was unable to write an item
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "The writer plugin failed to write item on line " + QString::number(levelLineNum) + " in " + elements.at(1) + "!", Common_Strings::STRING_OK);
                    return false;
                default:
                    assert(false);
                }
                errorCode = 0;

                if (!this->writerPlugin->Write_Level()) {
                    errorCode = -1;
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "The writer plugin failed to write the ROM!", Common_Strings::STRING_OK);
                    return false;
                }

                //Disable the Intro Demo if the first level is an auto scroller
                if (currentWorldNum == 1 && currentLevelNum == 1 && parser.Was_Auto_Scroll_Used() && !this->writerPlugin->Hacks_Disable_Intro_Demo()) {
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "The writer plugin failed to write the ROM!", Common_Strings::STRING_OK);
                    return false;
                }
            }
        }
    } while (!file.atEnd());
    return success;
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
            int random = Random::Get_Instance().Get_Num(169);
            if (random < 50) args.headerBackground = Background::OVER_WATER;
            else if (random < 90) args.headerBackground = Background::NIGHT;
            else if (random < 130) args.headerBackground = Background::SNOW;
            else if (random < 155) args.headerBackground = Background::NIGHT_AND_SNOW;
            else if (random < 170) args.headerBackground = Background::NIGHT_AND_FREEZE;
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

    args.numObjectBytes = this->writerPlugin->Get_Num_Object_Bytes();
    args.numEnemyBytes = this->writerPlugin->Get_Num_Enemy_Bytes();
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
        int expectedNumberOfCommonLevels = static_cast<int>(std::round(static_cast<double>(numLevelsExcludingCastles)*(static_cast<double>(COMMON_POINTS)/totalWeight)));

        //Distribute levels based upon the algorithm type
        if (expectedNumberOfCommonLevels <= 1) { //use the old distribution algorithm
            qInfo() << "Rolling for individual levels...";

            //Get the amount of each chance type
            int numVeryCommon = this->veryCommonLevels->size();
            int numCommon = this->commonLevels->size();
            int numUncommon = this->uncommonLevels->size();
            int numRare = this->rareLevels->size();

            //Determine chance
            int veryCommonChance = VERY_COMMON_POINTS*numVeryCommon; int commonChance = (COMMON_POINTS*numCommon)+veryCommonChance;
            int uncommonChance = (UNCOMMON_POINTS*numUncommon)+commonChance; int rareChance = (RARE_POINTS*numRare)+uncommonChance;

            //Determine the level type by probability
            for (int i = 0; i < numLevels; ++i) {
                Level_Type::Level_Type levelType = Level_Type::STANDARD_OVERWORLD;
                if ((i+1)%numLevelsPerWorld == 0) {
                    levelType = Level_Type::CASTLE; //allocate Castle levels now
                } else {
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
                    } else if (random < rareChance) { //rare
                        index = (random-uncommonChance)/RARE_POINTS;
                        assert(index < this->rareLevels->size());
                        levelType = this->rareLevels->at(index);
                    } else assert(false);
                    this->allocatedLevels->data()[i] = levelType;
                }
            }
        } else { //use the new distribution algorithm
            qInfo() << "Distributing levels across the entire game...";

            //Determine how many levels to generate for each type
            QVector<int> unallocatedLevelSlots;
            for (int i = 0; i < numLevels; ++i) {
                if ((i+1)%numLevelsPerWorld == 0) this->allocatedLevels->data()[i] = Level_Type::CASTLE; //allocate Castle levels now
                else unallocatedLevelSlots.push_back(i);
            }
            assert(unallocatedLevelSlots.size()+numWorlds == numLevels);
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
            for (int i = 0; !unallocatedLevelSlots.isEmpty();) { //iterate through each level type in order
                Level_Type::Level_Type levelType = unallocatedLevels.at(i).first;
                int numLevelsLeft = unallocatedLevels.at(i).second;
                assert(numLevelsLeft >= 0);
                if (numLevelsLeft > 0) { //allocate the level if there are any left to allocate
                    --unallocatedLevels.data()[i].second;
                    int index = Random::Get_Instance().Get_Num(unallocatedLevelSlots.size()-1);
                    int levelSlot = unallocatedLevelSlots.at(index);
                    unallocatedLevelSlots.remove(index);
                    this->allocatedLevels->data()[levelSlot] = levelType;
                }
                ++i;
                if (i == unallocatedLevels.size()) i = 0;
            }
        }

        //If the first level is an underwater level, swap it with the first non-underwater level
        if (this->pluginSettings->difficultyPreventTheFirstLevelFromBeingUnderwater && this->allocatedLevels->at(0) == Level_Type::UNDERWATER && numLevelTypes > 1) {
            for (int i = 1; i < this->allocatedLevels->size(); ++i) {
                if ((i+1)%numLevelsPerWorld != 0 && this->allocatedLevels->at(i) != Level_Type::UNDERWATER) {
                    this->allocatedLevels->data()[0] = this->allocatedLevels->at(i);
                    this->allocatedLevels->data()[i] = Level_Type::UNDERWATER;
                    break;
                }
            }
        }
    }

    return this->allocatedLevels->at(levelNum);
}

bool Level_Generator::Generate_Levels_And_Pack(QString &folderLocation) {
    this->veryCommonLevels->clear();
    this->commonLevels->clear();
    this->uncommonLevels->clear();
    this->rareLevels->clear();

    //Make the folder to store the random generation in
    QString generationName = "Random " + QDate::currentDate().toString("yy-MM-dd-") + QTime::currentTime().toString("HH-mm-ss-zzz");
    folderLocation = this->levelLocation + "/" + generationName;
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

    //Set up the parser
    SMB1_Compliance_Parser parser(this->writerPlugin, false, false, false, false);

    //Seed the random number generator... the location here is important
    Random::Get_Instance().Seed(this->pluginSettings->randomSeed, 5);

    //Randomly determine the number of max levels and levels per world if specified
    if (this->pluginSettings->randomNumWorlds) {
        this->pluginSettings->numWorlds = Random::Get_Instance().Get_Num(4)+2; //max out at 6 worlds
        this->pluginSettings->numLevelsPerWorld = 8;
        while (this->pluginSettings->numLevelsPerWorld*this->pluginSettings->numWorlds > 20) {
            --this->pluginSettings->numLevelsPerWorld;
        }
    }

    //Write the Number of Worlds
    if (!this->writerPlugin->Hacks_Set_Number_Of_Worlds(this->pluginSettings->numWorlds)) {
        qDebug() << "Failed to write the number of worlds to the ROM!";
        return false;
    }
    if (!this->writerPlugin->Hacks_Set_Number_Of_Levels_Per_World(this->pluginSettings->numLevelsPerWorld)) {
        qDebug() << "Failed to write the number of levels per world to the ROM!";
        return false;
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

        if (!this->writerPlugin->New_Level(levelOrder.at(i), (i/this->pluginSettings->numLevelsPerWorld)+1, (i%this->pluginSettings->numLevelsPerWorld)+1)) {
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                  "The writer plugin failed to allocate buffers for a new level!", Common_Strings::STRING_OK);
            return false;
        }
        args.numObjectBytes = this->writerPlugin->Get_Num_Object_Bytes();
        args.numEnemyBytes = this->writerPlugin->Get_Num_Enemy_Bytes();

        if (!this->generatorPlugin->Generate_Level(args)) {
            qDebug() << "Looks like the generator blew up";
            return false;
        }

        if (args.doesLevelExceedVerticalObjectLimit) isVerticalObjectLimitPatchNecessary = true;
        int lineNum = 0;
        QFile levelFile(args.fileName);
        if (!levelFile.open(QIODevice::ReadOnly)) return false;
        QTextStream levelStream(&levelFile);
        int errorCode = parser.Parse_Level(&levelStream, false, lineNum);
        levelFile.close();
        switch (errorCode) {
        case -1: //An error occurred and was handled within the parser
            return false;
        case 0: break; //Parser ran fine
        case 1: //Unable to open the file
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                  "Unable to open " + args.fileName + "!", Common_Strings::STRING_OK);
            return false;
        case 2: //Syntax error
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                  "Syntax error on line " + QString::number(lineNum) + "!", Common_Strings::STRING_OK);
            return false;
        case 3: //Writer was unable to write an item
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                  "The writer plugin failed to write item on line " + QString::number(lineNum) + "!", Common_Strings::STRING_OK);
            return false;
        default:
            assert(false);
        }

        if (!this->writerPlugin->Write_Level()) {
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                  "The writer plugin failed to write the ROM!", Common_Strings::STRING_OK);
            return false;
        }

        //Disable the Intro Demo if the first level is an auto scroller
        if (i == 0 && parser.Was_Auto_Scroll_Used() && !this->writerPlugin->Hacks_Disable_Intro_Demo()) {
            QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                  "The writer plugin failed to write the ROM!", Common_Strings::STRING_OK);
            return false;
        }

        //Write the level to the map
        if (!this->Write_To_Map(mapBuffer, levelOrder.at(i), args.fileName.split("/").last())) return false;
    }
    if (!this->Write_To_Map(mapBuffer, Level_Type::STRING_BREAK)) return false;
    if (!this->Write_To_Map(mapBuffer, Level_Type::STRING_BREAK)) return false;

    //Remove the Vertical Object Limit
    if (isVerticalObjectLimitPatchNecessary) {
        //Update the Header
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

        //Apply the Patch
        if (this->pluginSettings->smbUtilityCompatibility) qDebug() << "WARNING: Vertical Object Limit was requested to be removed, but it is disabled!";
        else assert(this->writerPlugin->Hacks_Remove_Vertical_Object_Limit());
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
    if (this->sequentialArchivePlugin->Pack(folderLocation, folderLocation+Common_Strings::STRING_LEVELS_EXTENSION) != 0) {
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
    int numLevelsPerWorld = 0, lineNum = 0, errorCode = 0;
    if (!this->Parse_Map_Header(mapFile, numLevelsPerWorld, lineNum, errorCode)) return false;
    if (mapFile.atEnd()) return false;

    //Parse the Move Tables
    QMap<QString, Level::Level> levels;
    this->Populate_Level_Map(levels);
    if (!this->Parse_Move_Object_Table(mapFile, levels, lineNum, errorCode)) return false;
    if (mapFile.atEnd()) return false;
    if (!this->Parse_Move_Enemy_Table(mapFile, levels, lineNum, errorCode)) return false;
    if (mapFile.atEnd()) return false;

    //Parse the Levels
    if (!this->Parse_Levels(mapFile, levels, numLevelsPerWorld, lineNum, errorCode)) return false;
    if (!mapFile.atEnd()) return false;
    return true;
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

//TODO: This is a duplicate of the Header Handler function. Refactor so that there is only one
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

//TODO: This is a duplicate of the Header Handler function. Refactor so that there is only one
bool Level_Generator::Parse_Through_Comments_Until_String(QTextStream &file, const QString &value, int &lineNum) {
    do {
        ++lineNum;
        QString line = file.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.at(0) == '#') continue;
        if (line == value) return true;
    } while (!file.atEnd());
    return false;
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
                if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(sourceLevel, destinationLevel, numBytes)) {
                    errorCode = 3;
                    return false;
                }
            } else {
                if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(sourceLevel, destinationLevel, numBytes)) {
                    errorCode = 3;
                    return false;
                }
            }
        }
    } while (!file.atEnd());
    return success;
}

bool Level_Generator::Append_Level(QVector<Level::Level> &levelOrder, Level::Level level) {
    if (!this->writerPlugin->Room_Table_Set_Next_Level(level)) return false;
    levelOrder.append(level);
    return true;
}

bool Level_Generator::Rearrange_Levels_From_Short_To_Long(QVector<Level::Level> &levelOrder, int numLevels) {
    assert(numLevels > 0 && numLevels <= 20);
    switch (numLevels) { //Add easy levels to the beginning
    case 20:
        if (!this->Append_Level(levelOrder, Level::WORLD_7_LEVEL_1)) return false;
        [[clang::fallthrough]]; //fall through
    case 19:
        if (!this->Append_Level(levelOrder, Level::WORLD_1_LEVEL_1)) return false;
        [[clang::fallthrough]]; //fall through
    case 18:
        if (!this->Append_Level(levelOrder, Level::WORLD_3_LEVEL_3)) return false;
        [[clang::fallthrough]]; //fall through
    case 17:
        if (!this->Append_Level(levelOrder, Level::WORLD_1_LEVEL_4)) return false;
        [[clang::fallthrough]]; //fall through
    default:
        break;
    }
    if (!this->Append_Level(levelOrder, Level::WORLD_6_LEVEL_3)) return false;
    if (numLevels == 1) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_2_LEVEL_1)) return false;
    if (numLevels == 2) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_4_LEVEL_3)) return false;
    if (numLevels == 3) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_8_LEVEL_3)) return false;
    if (numLevels == 4) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_3_LEVEL_4)) return false;
    if (numLevels == 5) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_6_LEVEL_1)) return false;
    if (numLevels == 6) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_8_LEVEL_4)) return false;
    if (numLevels == 7) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_5_LEVEL_2)) return false;
    if (numLevels == 8) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_3_LEVEL_1)) return false;
    if (numLevels == 9) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_2_LEVEL_4)) return false;
    if (numLevels == 10) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_8_LEVEL_2)) return false;
    if (numLevels == 11) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_2_LEVEL_2)) return false;
    if (numLevels == 12) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_4_LEVEL_4)) return false;
    if (numLevels == 13) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_2_LEVEL_3)) return false;
    if (numLevels == 14) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_7_LEVEL_4)) return false;
    if (numLevels == 15) return true;
    if (!this->Append_Level(levelOrder, Level::WORLD_6_LEVEL_2)) return false;
    assert(numLevels >= 16);
    return true;
}

bool Level_Generator::Write_Move_Objects_Map(Text_Insertion_Buffer &mapBuffer) {
    if (!this->Write_To_Map(mapBuffer, Header::STRING_OBJECT_MOVE_TABLE_COMMENT)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_UNDERWATER_CASTLE + " " + Level::STRING_WORLD_1_LEVEL_1 + " " + QString::number(8))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::UNDERWATER_CASTLE, Level::WORLD_1_LEVEL_1, 8)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_UNDERWATER_CASTLE + " " + Level::STRING_UNDERWATER_BONUS + " " + QString::number(14))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::UNDERWATER_CASTLE, Level::UNDERWATER_BONUS, 14)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_UNDERWATER_CASTLE + " " + Level::STRING_WORLD_7_LEVEL_1 + " " + QString::number(2))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::UNDERWATER_CASTLE, Level::WORLD_7_LEVEL_1, 2)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_3_LEVEL_2 + " " + Level::STRING_WORLD_7_LEVEL_1 + " " + QString::number(12))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_3_LEVEL_2, Level::WORLD_7_LEVEL_1, 12)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_3_LEVEL_2 + " " + Level::STRING_WORLD_3_LEVEL_3 + " " + QString::number(14))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_3_LEVEL_2, Level::WORLD_3_LEVEL_3, 14)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WARP_ZONE + " " + Level::STRING_WORLD_3_LEVEL_3 + " " + QString::number(2))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WARP_ZONE, Level::WORLD_3_LEVEL_3, 2)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WARP_ZONE + " " + Level::STRING_CLOUD_BONUS_2 + " " + QString::number(20))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WARP_ZONE, Level::CLOUD_BONUS_2, 20)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WARP_ZONE + " " + Level::STRING_WORLD_1_LEVEL_4 + " " + QString::number(18))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WARP_ZONE, Level::WORLD_1_LEVEL_4, 18)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_5_LEVEL_1 + " " + Level::STRING_WORLD_1_LEVEL_4 + " " + QString::number(4))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_5_LEVEL_1, Level::WORLD_1_LEVEL_4, 4)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_5_LEVEL_1 + " " + Level::STRING_WORLD_6_LEVEL_3 + " " + QString::number(30))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_5_LEVEL_1, Level::WORLD_6_LEVEL_3, 30)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_5_LEVEL_1 + " " + Level::STRING_WORLD_6_LEVEL_1 + " " + QString::number(26))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_5_LEVEL_1, Level::WORLD_6_LEVEL_1, 26)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_1_LEVEL_3 + " " + Level::STRING_WORLD_6_LEVEL_1 + " " + QString::number(4))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_3, Level::WORLD_6_LEVEL_1, 4)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_1_LEVEL_3 + " " + Level::STRING_WORLD_2_LEVEL_1 + " " + QString::number(32))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_3, Level::WORLD_2_LEVEL_1, 32)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_1_LEVEL_3 + " " + Level::STRING_WORLD_4_LEVEL_3 + " " + QString::number(26))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_3, Level::WORLD_4_LEVEL_3, 26)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_4_LEVEL_3 + " " + QString::number(6))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_4_LEVEL_3, 6)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_8_LEVEL_3 + " " + QString::number(32))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_8_LEVEL_3, 32)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_3_LEVEL_4 + " " + QString::number(32))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_3_LEVEL_4, 32)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_8_LEVEL_4 + " " + QString::number(10))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_8_LEVEL_4, 10)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_8_LEVEL_4 + " " + QString::number(26))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_8_LEVEL_4, 26)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_7_LEVEL_4 + " " + QString::number(36))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_7_LEVEL_4, 36)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_6_LEVEL_2 + " " + QString::number(36))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_6_LEVEL_2, 36)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_5_LEVEL_2 + " " + QString::number(38))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_5_LEVEL_2, 38)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_3_LEVEL_1 + " " + QString::number(8))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_3_LEVEL_1, 8)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_3_LEVEL_1 + " " + QString::number(30))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_3_LEVEL_1, 30)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_3 + " " + QString::number(38))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_2_LEVEL_3, 38)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_8_LEVEL_2 + " " + QString::number(40))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_8_LEVEL_2, 40)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_4_LEVEL_4 + " " + QString::number(40))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_4_LEVEL_4, 40)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_4 + " " + QString::number(10))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_2_LEVEL_4, 10)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_4 + " " + QString::number(32))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::WORLD_2_LEVEL_4, 32)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_2 + " " + QString::number(42))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::WORLD_2_LEVEL_2, 42)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_CLOUD_BONUS_1 + " " + QString::number(42))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::CLOUD_BONUS_1, 42)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_UNDERGROUND_BONUS + " " + QString::number(44))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::UNDERGROUND_BONUS, 44)) return false;
    if (!this->Write_To_Map(mapBuffer, Level_Type::STRING_BREAK)) return false;
    return true;
}

bool Level_Generator::Write_Move_Enemies_Map(Text_Insertion_Buffer &mapBuffer) {
    if (!this->Write_To_Map(mapBuffer, Header::STRING_ENEMY_MOVE_TABLE_COMMENT)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_2_LEVEL_1 + " " + Level::STRING_WORLD_6_LEVEL_3 + " " + QString::number(2))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_2_LEVEL_1, Level::WORLD_6_LEVEL_3, 2)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_CLOUD_BONUS_1 + " " + Level::STRING_WORLD_3_LEVEL_4 + " " + QString::number(3))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::CLOUD_BONUS_1, Level::WORLD_3_LEVEL_4, 3)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_CLOUD_BONUS_2 + " " + Level::STRING_WORLD_3_LEVEL_4 + " " + QString::number(1))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::CLOUD_BONUS_2, Level::WORLD_3_LEVEL_4, 1)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_CLOUD_BONUS_2 + " " + Level::STRING_WORLD_4_LEVEL_3 + " " + QString::number(2))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::CLOUD_BONUS_2, Level::WORLD_4_LEVEL_3, 2)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_7_LEVEL_1 + " " + Level::STRING_WORLD_4_LEVEL_3 + " " + QString::number(4))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_7_LEVEL_1, Level::WORLD_4_LEVEL_3, 4)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_7_LEVEL_1 + " " + Level::STRING_WORLD_3_LEVEL_1 + " " + QString::number(1))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_7_LEVEL_1, Level::WORLD_3_LEVEL_1, 1)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_1_LEVEL_1 + " " + Level::STRING_WORLD_3_LEVEL_1 + " " + QString::number(5))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_1, Level::WORLD_3_LEVEL_1, 5)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_3_LEVEL_3 + " " + Level::STRING_WORLD_5_LEVEL_2 + " " + QString::number(6))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_3_LEVEL_3, Level::WORLD_5_LEVEL_2, 6)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_1_LEVEL_4 + " " + Level::STRING_WORLD_5_LEVEL_2 + " " + QString::number(4))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_4, Level::WORLD_5_LEVEL_2, 4)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_1_LEVEL_4 + " " + Level::STRING_WORLD_2_LEVEL_4 + " " + QString::number(2))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_4, Level::WORLD_2_LEVEL_4, 2)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_8_LEVEL_4 + " " + Level::STRING_WORLD_2_LEVEL_4 + " " + QString::number(7))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_4, Level::WORLD_2_LEVEL_4, 7)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_UNDERGROUND_BONUS + " " + Level::STRING_WORLD_2_LEVEL_4 + " " + QString::number(1))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::UNDERGROUND_BONUS, Level::WORLD_2_LEVEL_4, 1)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_UNDERGROUND_BONUS + " " + Level::STRING_WORLD_8_LEVEL_2 + " " + QString::number(11))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::UNDERGROUND_BONUS, Level::WORLD_8_LEVEL_2, 11)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_8_LEVEL_2 + " " + QString::number(4))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_8_LEVEL_2, 4)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_8_LEVEL_3 + " " + QString::number(9))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_8_LEVEL_3, 9)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_UNDERWATER_CASTLE + " " + Level::STRING_WORLD_8_LEVEL_3 + " " + QString::number(7))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::UNDERWATER_CASTLE, Level::WORLD_8_LEVEL_3, 7)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_UNDERWATER_CASTLE + " " + Level::STRING_WORLD_2_LEVEL_2 + " " + QString::number(12))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::UNDERWATER_CASTLE, Level::WORLD_2_LEVEL_2, 12)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_3_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_2 + " " + QString::number(11))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_3_LEVEL_2, Level::WORLD_2_LEVEL_2, 11)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_3_LEVEL_2 + " " + Level::STRING_WORLD_6_LEVEL_1 + " " + QString::number(25))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_3_LEVEL_2, Level::WORLD_6_LEVEL_1, 25)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_1_LEVEL_3 + " " + Level::STRING_WORLD_6_LEVEL_1 + " " + QString::number(15))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_3, Level::WORLD_6_LEVEL_1, 15)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_1_LEVEL_3 + " " + Level::STRING_WORLD_4_LEVEL_4 + " " + QString::number(13))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_3, Level::WORLD_4_LEVEL_4, 13)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_5_LEVEL_1 + " " + Level::STRING_WORLD_4_LEVEL_4 + " " + QString::number(29))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_5_LEVEL_1, Level::WORLD_4_LEVEL_4, 29)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_5_LEVEL_1 + " " + Level::STRING_WORLD_6_LEVEL_2 + " " + QString::number(6))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_5_LEVEL_1, Level::WORLD_6_LEVEL_2, 6)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_WORLD_6_LEVEL_2 + " " + QString::number(38))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::WORLD_6_LEVEL_2, 38)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_3 + " " + QString::number(6))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::WORLD_2_LEVEL_3, 6)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_3 + " " + QString::number(44))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_2_LEVEL_3, 44)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_7_LEVEL_4 + " " + QString::number(1))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_7_LEVEL_4, 1)) return false;
    if (!this->Write_To_Map(mapBuffer, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_7_LEVEL_4 + " " + QString::number(57))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_7_LEVEL_4, 57)) return false;
    if (!this->Write_To_Map(mapBuffer, Level_Type::STRING_BREAK)) return false;
    return true;
}

bool Level_Generator::Write_To_Map(Text_Insertion_Buffer &mapBuffer, const QString &string) {
    mapBuffer.Insert_At_End(string);
    return true;
}

bool Level_Generator::Write_To_Map(Text_Insertion_Buffer &mapBuffer, Level::Level level, const QString &fileName) {
    QString line = "";

    switch (level) {
    case Level::WORLD_1_LEVEL_1:    line = Level::STRING_WORLD_1_LEVEL_1; break;
    case Level::WORLD_1_LEVEL_2:    line = Level::STRING_WORLD_1_LEVEL_2; break;
    case Level::WORLD_1_LEVEL_3:    line = Level::STRING_WORLD_1_LEVEL_3; break;
    case Level::WORLD_1_LEVEL_4:    line = Level::STRING_WORLD_1_LEVEL_4; break;
    case Level::WORLD_2_LEVEL_1:    line = Level::STRING_WORLD_2_LEVEL_1; break;
    case Level::WORLD_2_LEVEL_2:    line = Level::STRING_WORLD_2_LEVEL_2; break;
    case Level::WORLD_2_LEVEL_3:    line = Level::STRING_WORLD_2_LEVEL_3; break;
    case Level::WORLD_2_LEVEL_4:    line = Level::STRING_WORLD_2_LEVEL_4; break;
    case Level::WORLD_3_LEVEL_1:    line = Level::STRING_WORLD_3_LEVEL_1; break;
    case Level::WORLD_3_LEVEL_2:    line = Level::STRING_WORLD_3_LEVEL_2; break;
    case Level::WORLD_3_LEVEL_3:    line = Level::STRING_WORLD_3_LEVEL_3; break;
    case Level::WORLD_3_LEVEL_4:    line = Level::STRING_WORLD_3_LEVEL_4; break;
    case Level::WORLD_4_LEVEL_1:    line = Level::STRING_WORLD_4_LEVEL_1; break;
    case Level::WORLD_4_LEVEL_2:    line = Level::STRING_WORLD_4_LEVEL_2; break;
    case Level::WORLD_4_LEVEL_3:    line = Level::STRING_WORLD_4_LEVEL_3; break;
    case Level::WORLD_4_LEVEL_4:    line = Level::STRING_WORLD_4_LEVEL_4; break;
    case Level::WORLD_5_LEVEL_1:    line = Level::STRING_WORLD_5_LEVEL_1; break;
    case Level::WORLD_5_LEVEL_2:    line = Level::STRING_WORLD_5_LEVEL_2; break;
    case Level::WORLD_6_LEVEL_1:    line = Level::STRING_WORLD_6_LEVEL_1; break;
    case Level::WORLD_6_LEVEL_2:    line = Level::STRING_WORLD_6_LEVEL_2; break;
    case Level::WORLD_6_LEVEL_3:    line = Level::STRING_WORLD_6_LEVEL_3; break;
    case Level::WORLD_7_LEVEL_1:    line = Level::STRING_WORLD_7_LEVEL_1; break;
    case Level::WORLD_7_LEVEL_4:    line = Level::STRING_WORLD_7_LEVEL_4; break;
    case Level::WORLD_8_LEVEL_1:    line = Level::STRING_WORLD_8_LEVEL_1; break;
    case Level::WORLD_8_LEVEL_2:    line = Level::STRING_WORLD_8_LEVEL_2; break;
    case Level::WORLD_8_LEVEL_3:    line = Level::STRING_WORLD_8_LEVEL_3; break;
    case Level::WORLD_8_LEVEL_4:    line = Level::STRING_WORLD_8_LEVEL_4; break;
    case Level::PIPE_INTRO:         line = Level::STRING_PIPE_INTRO; break;
    case Level::UNDERGROUND_BONUS:  line = Level::STRING_UNDERGROUND_BONUS; break;
    case Level::CLOUD_BONUS_1:      line = Level::STRING_CLOUD_BONUS_1; break;
    case Level::CLOUD_BONUS_2:      line = Level::STRING_CLOUD_BONUS_2; break;
    case Level::UNDERWATER_BONUS:   line = Level::STRING_UNDERWATER_BONUS; break;
    case Level::WARP_ZONE:          line = Level::STRING_WARP_ZONE; break;
    case Level::UNDERWATER_CASTLE:  line = Level::STRING_UNDERWATER_CASTLE; break;
    }

    if (fileName.size() > 0) line += " \"" + fileName + "\"";
    return this->Write_To_Map(mapBuffer, line);
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
