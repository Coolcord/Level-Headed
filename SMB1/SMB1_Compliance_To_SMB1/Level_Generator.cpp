#include "Level_Generator.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../../Common_Files/Random.h"
#include "../SMB1_Compliance_Generator/Difficulty.h"
#include "../Common_SMB1_Files/Level_String.h"
#include "../Common_SMB1_Files/Header_String.h"
#include "../SMB1_Writer/SMB1_Writer_Strings.h"
#include "SMB1_Compliance_Parser.h"
#include <QMessageBox>
#include <QDir>
#include <QDate>
#include <QTextStream>
#include <QTime>
#include <QDebug>
#include <math.h>
#include <assert.h>

const static int VERY_COMMON_POINTS = 16;
const static int COMMON_POINTS = 8;
const static int UNCOMMON_POINTS = 4;
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
}

bool Level_Generator::Generate_Levels() {
    QString folderLocation = QString();
    bool success = this->Generate_Levels_And_Pack(folderLocation);
    QDir dir(folderLocation);
    dir.removeRecursively();
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

bool Level_Generator::Parse_Map_Header(QTextStream &file, int &lineNum, int &errorCode) {
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

bool Level_Generator::Parse_Levels(QTextStream &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode) {
    //Read the Level Lines
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
            QStringList elements = line.split(' ');
            const QMap<QString, Level::Level>::const_iterator iter = levels.find(elements.at(0));
            if (elements.size() > 2 || iter == levels.end()) {
                errorCode = 2; //syntax error
                return false;
            }

            //Add the level to the room order table
            Level::Level currentLevel = iter.value();
            if (!this->writerPlugin->Room_Table_Set_Next_Level(currentLevel)) {
                errorCode = 3;
                return false;
            }

            //Parse the level if it has a script
            if (elements.size() == 2) {
                QString scriptName = elements.at(1);
                if (scriptName.size() < 3) {
                    errorCode = 2; //syntax error
                    return false;
                }
                scriptName.chop(1); scriptName = scriptName.remove(0, 1);
                if (!this->writerPlugin->New_Level(currentLevel)) {
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "The writer plugin failed to allocate buffers for a new level!", Common_Strings::STRING_OK);
                    return false;
                }

                //TODO: Make these error messages file specific
                SMB1_Compliance_Parser parser(this->writerPlugin);
                int levelLineNum = 0;
                QTextStream levelStream(this->sequentialArchivePlugin->Read_File("/" + scriptName), QIODevice::ReadOnly);
                int levelErrorCode = parser.Parse_Level(&levelStream, levelLineNum);
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
            if (Random::Get_Num(99)+1 <= this->pluginSettings->difficultyAutoScrollChancePerLevel) {
                args.useAutoScroll = true;
            }
        }
    }
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
    args.difficultyUnderwaterLakitus = this->pluginSettings->difficultyUnderwaterLakitus;
    args.difficultyUnderwaterSwimmingCheepCheeps = this->pluginSettings->difficultyUnderwaterSwimmingCheepCheeps;
    args.difficultyStandardOverworldFlyingCheepCheeps = this->pluginSettings->difficultyStandardOverworldFlyingCheepCheeps;
    args.difficultyStandardOverworldLakitus = this->pluginSettings->difficultyStandardOverworldLakitus;
    args.difficultyStandardOverworldOffscreenBulletBills = this->pluginSettings->difficultyStandardOverworldOffscreenBulletBills;
    args.difficultyNoEnemies = this->pluginSettings->difficultyNoEnemies;
    args.difficultyHammerTimeIntensity = this->pluginSettings->difficultyHammerTimeIntensity;
    args.difficultyLakituSpawnChancePerLevel = this->pluginSettings->difficultyLakituSpawnChancePerLevel;
    args.difficultyDisableAllOtherEnemiesWhenALakituSpawns = this->pluginSettings->difficultyDisableAllOtherEnemiesWhenALakituSpawns;
    args.difficultySpawnerPriority = this->pluginSettings->difficultySpawnerPriority;

    //Determine the level type. The last level of each world should be a castle
    if (level == this->pluginSettings->numLevelsPerWorld) args.levelType = Level_Type::CASTLE;
    else args.levelType = this->Determine_Level_Type();
    args.levelCompliment = Level_Compliment::BULLET_BILL_TURRETS;
    switch (args.levelType) {
    case Level_Type::STANDARD_OVERWORLD:
        if (args.difficulty < args.difficultyBulletTime && Random::Get_Num(4)==0) args.levelCompliment = Level_Compliment::MUSHROOMS;
        if (Random::Get_Num(1)==0) args.headerScenery = Scenery::MOUNTAINS;
        else args.headerScenery = Scenery::FENCES;
        if (Random::Get_Num(2)==0) {
            int random = Random::Get_Num(119);
            if (random < 40) args.headerBackground = Background::NIGHT;
            else if (random < 80) args.headerBackground = Background::SNOW;
            else if (random < 105) args.headerBackground = Background::NIGHT_AND_SNOW;
            else if (random < 120) args.headerBackground = Background::NIGHT_AND_FREEZE;
            else assert(false);
        } else args.headerBackground = Background::BLANK_BACKGROUND;
        break;
    case Level_Type::BRIDGE:
    case Level_Type::ISLAND:
        if (Random::Get_Num(4)==0) args.levelCompliment = Level_Compliment::MUSHROOMS;
        else args.levelCompliment = Level_Compliment::TREES;
        args.headerScenery = Scenery::ONLY_CLOUDS;
        if (Random::Get_Num(1)==0) {
            int random = Random::Get_Num(169);
            if (random < 50) args.headerBackground = Background::OVER_WATER;
            else if (random < 90) args.headerBackground = Background::NIGHT;
            else if (random < 130) args.headerBackground = Background::SNOW;
            else if (random < 155) args.headerBackground = Background::NIGHT_AND_SNOW;
            else if (random < 170) args.headerBackground = Background::NIGHT_AND_FREEZE;
            else assert(false);
        } else args.headerBackground = Background::BLANK_BACKGROUND;
        break;
    case Level_Type::UNDERGROUND:
        if (args.difficulty < args.difficultyBulletTime && Random::Get_Num(4)==0) args.levelCompliment = Level_Compliment::MUSHROOMS;
        args.headerScenery = Scenery::NO_SCENERY;
        if (args.levelCompliment == Level_Compliment::BULLET_BILL_TURRETS && Random::Get_Num(3)==0) {
            if (Random::Get_Num(2)==0) args.headerBackground = Background::NIGHT_AND_FREEZE;
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

Level_Type::Level_Type Level_Generator::Determine_Level_Type() {
    //Get the amount of each chance type
    int numVeryCommon = this->veryCommonLevels->size();
    int numCommon = this->commonLevels->size();
    int numUncommon = this->uncommonLevels->size();
    int numRare = this->rareLevels->size();

    //Determine chance
    int veryCommonChance = VERY_COMMON_POINTS*numVeryCommon; int commonChance = (COMMON_POINTS*numCommon)+veryCommonChance;
    int uncommonChance = (UNCOMMON_POINTS*numUncommon)+commonChance; int rareChance = (RARE_POINTS*numRare)+uncommonChance;
    int random = Random::Get_Num(rareChance-1);
    int index = 0;

    //Determine the level type by probability
    Level_Type::Level_Type levelType = Level_Type::STANDARD_OVERWORLD;
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
    return levelType;
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
            //TODO: Show a read/write error here
            return false;
        }
    } else {
        if (!dir.cd(generationName) || !dir.removeRecursively()) {
            //TODO: Show a read/write error here
            return false;
        }
    }

    //Create a new map file
    QFile map(this->levelLocation + "/" + generationName + "/" + Common_Strings::STRING_GAME_NAME + ".map");
    if (!map.open(QFile::ReadWrite | QFile::Truncate)) {
        //TODO: Show a read/write error here
        return false;
    }
    QTextStream mapStream(&map);
    if (!this->Write_To_Map(mapStream, Header::STRING_MAP_NAME)) return false;

    //Set up the parser
    SMB1_Compliance_Parser parser(this->writerPlugin);

    //Seed the random number generator... the location here is important
    qsrand(Random::Get_Seed_From_QString(this->pluginSettings->randomSeed));
    qDebug().noquote() << "Seed: " << this->pluginSettings->randomSeed;

    //Randomly determine the number of max levels and levels per world if specified
    if (this->pluginSettings->randomNumWorlds) {
        this->pluginSettings->numWorlds = Random::Get_Num(4)+2; //max out at 6 worlds
        this->pluginSettings->numLevelsPerWorld = 9;
        while (this->pluginSettings->numLevelsPerWorld*this->pluginSettings->numWorlds > 20) {
            --this->pluginSettings->numLevelsPerWorld;
        }
    }

    //Write the Number of Worlds
    if (!this->writerPlugin->Hacks_Set_Number_Of_Worlds(this->pluginSettings->numWorlds)) {
        qDebug() << "Failed to write the number of worlds to the ROM!";
        return false;
    }

    //Write the Comment section of the map file
    if (!this->Write_To_Map(mapStream, Level_Type::STRING_BREAK)) return false;
    if (!this->Write_To_Map(mapStream, Header::STRING_COOLCORD)) return false;
    if (!this->Write_To_Map(mapStream, Header::STRING_CREATED + " " + QDate::currentDate().toString("dddd, MMMM dd, yyyy") + ", at " + QTime::currentTime().toString("hh:mm:ss A."))) return false;
    if (!this->Write_To_Map(mapStream, Header::STRING_STANDARD_OVERWORLD_LEVELS_COMMONALITY + ": " + this->pluginSettings->standardOverworldChance)) return false;
    if (!this->Write_To_Map(mapStream, Header::STRING_UNDERGROUND_LEVELS_COMMONALITY + ": " + this->pluginSettings->undergroundChance)) return false;
    if (!this->Write_To_Map(mapStream, Header::STRING_UNDERWATER_LEVELS_COMMONALITY + ": " + this->pluginSettings->underwaterChance)) return false;
    if (!this->Write_To_Map(mapStream, Header::STRING_BRIDGE_LEVELS_COMMONALITY + ": " + this->pluginSettings->bridgeChance)) return false;
    if (!this->Write_To_Map(mapStream, Header::STRING_ISLAND_LEVELS_COMMONALITY + ": " + this->pluginSettings->islandChance)) return false;
    if (!this->Write_To_Map(mapStream, Header::STRING_DIFFICULTY + ": " + QString::number(this->pluginSettings->difficultyComboIndex))) return false;
    if (!this->Write_To_Map(mapStream, Header::STRING_RANDOM_SEED + ": " + this->pluginSettings->randomSeed)) return false;

    //Write the Header of the map file
    if (!this->Write_To_Map(mapStream, Level_Type::STRING_BREAK)) return false;
    if (!this->Write_To_Map(mapStream, Header::STRING_NUMBER_OF_WORLDS + ": " + QString::number(this->pluginSettings->numWorlds))) return false;
    if (!this->Write_To_Map(mapStream, Level_Type::STRING_BREAK)) return false;

    //Build the Move Objects Map
    if (!this->Write_Move_Objects_Map(mapStream)) return false;

    //Build the Move Enemies Map
    if (!this->Write_Move_Enemies_Map(mapStream)) return false;

    //Build the Room Order Map
    QVector<Level::Level> levelOrder;
    int numLevels = this->pluginSettings->numWorlds*this->pluginSettings->numLevelsPerWorld;
    if (!this->Rearrange_Levels_From_Short_To_Long(levelOrder, numLevels)) {
        qDebug() << "Failed to rearrange the room order";
        return false;
    }
    assert(numLevels == levelOrder.size());

    //Determine the number of level types for each chance type
    this->Read_Level_Chance(this->pluginSettings->standardOverworldChance, Level_Type::STANDARD_OVERWORLD);
    this->Read_Level_Chance(this->pluginSettings->undergroundChance, Level_Type::UNDERGROUND);
    this->Read_Level_Chance(this->pluginSettings->underwaterChance, Level_Type::UNDERWATER);
    this->Read_Level_Chance(this->pluginSettings->bridgeChance, Level_Type::BRIDGE);
    this->Read_Level_Chance(this->pluginSettings->islandChance, Level_Type::ISLAND);

    //Generate the Levels
    if (!this->Write_To_Map(mapStream, Header::STRING_LEVEL_MAP_COMMENT)) return false;
    for (int i = 0; i < numLevels; ++i) {
        //Prepare Arguments
        SMB1_Compliance_Generator_Arguments args = this->Prepare_Arguments(generationName, i, numLevels);

        if (!this->writerPlugin->New_Level(levelOrder.at(i))) {
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

        int lineNum = 0;
        QFile levelFile(args.fileName);
        if (!levelFile.open(QIODevice::ReadOnly)) return false;
        QTextStream levelStream(&levelFile);
        int errorCode = parser.Parse_Level(&levelStream, lineNum);
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

        //Write the level to the map
        if (!this->Write_To_Map(mapStream, levelOrder.at(i), args.fileName.split("/").last())) return false;
    }
    if (!this->Write_To_Map(mapStream, Level_Type::STRING_BREAK)) return false;

    mapStream.flush();
    map.close();

    //Get a new seed for the next generation, since this one was successful
    this->pluginSettings->randomSeed = QString::number(QTime::currentTime().msecsSinceStartOfDay());

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
    int lineNum = 0;
    int errorCode = 0;
    if (!this->Parse_Map_Header(mapFile, lineNum, errorCode)) return false;
    if (mapFile.atEnd()) return false;

    //Parse the Move Tables
    QMap<QString, Level::Level> levels;
    this->Populate_Level_Map(levels);
    if (!this->Parse_Move_Object_Table(mapFile, levels, lineNum, errorCode)) return false;
    if (mapFile.atEnd()) return false;
    if (!this->Parse_Move_Enemy_Table(mapFile, levels, lineNum, errorCode)) return false;
    if (mapFile.atEnd()) return false;

    //Parse the Levels
    if (!this->Parse_Levels(mapFile, levels, lineNum, errorCode)) return false;
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

bool Level_Generator::Write_Move_Objects_Map(QTextStream &mapStream) {
    if (!this->Write_To_Map(mapStream, Header::STRING_OBJECT_MOVE_TABLE_COMMENT)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_UNDERWATER_CASTLE + " " + Level::STRING_WORLD_1_LEVEL_1 + " " + QString::number(8))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::UNDERWATER_CASTLE, Level::WORLD_1_LEVEL_1, 8)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_UNDERWATER_CASTLE + " " + Level::STRING_UNDERWATER_BONUS + " " + QString::number(14))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::UNDERWATER_CASTLE, Level::UNDERWATER_BONUS, 14)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_UNDERWATER_CASTLE + " " + Level::STRING_WORLD_7_LEVEL_1 + " " + QString::number(2))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::UNDERWATER_CASTLE, Level::WORLD_7_LEVEL_1, 2)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_3_LEVEL_2 + " " + Level::STRING_WORLD_7_LEVEL_1 + " " + QString::number(12))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_3_LEVEL_2, Level::WORLD_7_LEVEL_1, 12)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_3_LEVEL_2 + " " + Level::STRING_WORLD_3_LEVEL_3 + " " + QString::number(14))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_3_LEVEL_2, Level::WORLD_3_LEVEL_3, 14)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WARP_ZONE + " " + Level::STRING_WORLD_3_LEVEL_3 + " " + QString::number(2))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WARP_ZONE, Level::WORLD_3_LEVEL_3, 2)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WARP_ZONE + " " + Level::STRING_CLOUD_BONUS_2 + " " + QString::number(20))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WARP_ZONE, Level::CLOUD_BONUS_2, 20)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WARP_ZONE + " " + Level::STRING_WORLD_1_LEVEL_4 + " " + QString::number(18))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WARP_ZONE, Level::WORLD_1_LEVEL_4, 18)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_5_LEVEL_1 + " " + Level::STRING_WORLD_1_LEVEL_4 + " " + QString::number(4))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_5_LEVEL_1, Level::WORLD_1_LEVEL_4, 4)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_5_LEVEL_1 + " " + Level::STRING_WORLD_6_LEVEL_3 + " " + QString::number(30))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_5_LEVEL_1, Level::WORLD_6_LEVEL_3, 30)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_5_LEVEL_1 + " " + Level::STRING_WORLD_6_LEVEL_1 + " " + QString::number(26))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_5_LEVEL_1, Level::WORLD_6_LEVEL_1, 26)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_3 + " " + Level::STRING_WORLD_6_LEVEL_1 + " " + QString::number(4))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_3, Level::WORLD_6_LEVEL_1, 4)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_3 + " " + Level::STRING_WORLD_2_LEVEL_1 + " " + QString::number(32))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_3, Level::WORLD_2_LEVEL_1, 32)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_3 + " " + Level::STRING_WORLD_4_LEVEL_3 + " " + QString::number(26))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_3, Level::WORLD_4_LEVEL_3, 26)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_4_LEVEL_3 + " " + QString::number(6))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_4_LEVEL_3, 6)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_8_LEVEL_3 + " " + QString::number(32))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_8_LEVEL_3, 32)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_3_LEVEL_4 + " " + QString::number(32))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_3_LEVEL_4, 32)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_8_LEVEL_4 + " " + QString::number(10))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_8_LEVEL_4, 10)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_8_LEVEL_4 + " " + QString::number(26))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_8_LEVEL_4, 26)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_7_LEVEL_4 + " " + QString::number(36))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_7_LEVEL_4, 36)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_6_LEVEL_2 + " " + QString::number(36))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_6_LEVEL_2, 36)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_5_LEVEL_2 + " " + QString::number(38))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_5_LEVEL_2, 38)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_3_LEVEL_1 + " " + QString::number(8))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_3_LEVEL_1, 8)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_3_LEVEL_1 + " " + QString::number(30))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_3_LEVEL_1, 30)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_3 + " " + QString::number(38))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_2_LEVEL_3, 38)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_8_LEVEL_2 + " " + QString::number(40))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_8_LEVEL_2, 40)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_4_LEVEL_4 + " " + QString::number(40))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_4_LEVEL_4, 40)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_4 + " " + QString::number(10))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_2_LEVEL_4, 10)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_4 + " " + QString::number(32))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::WORLD_2_LEVEL_4, 32)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_2 + " " + QString::number(42))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::WORLD_2_LEVEL_2, 42)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_CLOUD_BONUS_1 + " " + QString::number(42))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::CLOUD_BONUS_1, 42)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_UNDERGROUND_BONUS + " " + QString::number(44))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::UNDERGROUND_BONUS, 44)) return false;
    if (!this->Write_To_Map(mapStream, Level_Type::STRING_BREAK)) return false;
    return true;
}

bool Level_Generator::Write_Move_Enemies_Map(QTextStream &mapStream) {
    if (!this->Write_To_Map(mapStream, Header::STRING_ENEMY_MOVE_TABLE_COMMENT)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_2_LEVEL_1 + " " + Level::STRING_WORLD_6_LEVEL_3 + " " + QString::number(2))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_2_LEVEL_1, Level::WORLD_6_LEVEL_3, 2)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_CLOUD_BONUS_1 + " " + Level::STRING_WORLD_3_LEVEL_4 + " " + QString::number(3))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::CLOUD_BONUS_1, Level::WORLD_3_LEVEL_4, 3)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_CLOUD_BONUS_2 + " " + Level::STRING_WORLD_3_LEVEL_4 + " " + QString::number(1))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::CLOUD_BONUS_2, Level::WORLD_3_LEVEL_4, 1)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_CLOUD_BONUS_2 + " " + Level::STRING_WORLD_4_LEVEL_3 + " " + QString::number(2))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::CLOUD_BONUS_2, Level::WORLD_4_LEVEL_3, 2)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_7_LEVEL_1 + " " + Level::STRING_WORLD_4_LEVEL_3 + " " + QString::number(4))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_7_LEVEL_1, Level::WORLD_4_LEVEL_3, 4)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_7_LEVEL_1 + " " + Level::STRING_WORLD_3_LEVEL_1 + " " + QString::number(1))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_7_LEVEL_1, Level::WORLD_3_LEVEL_1, 1)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_1 + " " + Level::STRING_WORLD_3_LEVEL_1 + " " + QString::number(5))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_1, Level::WORLD_3_LEVEL_1, 5)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_3_LEVEL_3 + " " + Level::STRING_WORLD_5_LEVEL_2 + " " + QString::number(6))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_3_LEVEL_3, Level::WORLD_5_LEVEL_2, 6)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_4 + " " + Level::STRING_WORLD_5_LEVEL_2 + " " + QString::number(4))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_4, Level::WORLD_5_LEVEL_2, 4)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_4 + " " + Level::STRING_WORLD_2_LEVEL_4 + " " + QString::number(2))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_4, Level::WORLD_2_LEVEL_4, 2)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_8_LEVEL_4 + " " + Level::STRING_WORLD_2_LEVEL_4 + " " + QString::number(7))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_4, Level::WORLD_2_LEVEL_4, 7)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_UNDERGROUND_BONUS + " " + Level::STRING_WORLD_2_LEVEL_4 + " " + QString::number(1))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::UNDERGROUND_BONUS, Level::WORLD_2_LEVEL_4, 1)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_UNDERGROUND_BONUS + " " + Level::STRING_WORLD_8_LEVEL_2 + " " + QString::number(11))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::UNDERGROUND_BONUS, Level::WORLD_8_LEVEL_2, 11)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_8_LEVEL_2 + " " + QString::number(4))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_8_LEVEL_2, 4)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_8_LEVEL_3 + " " + QString::number(9))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_8_LEVEL_3, 9)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_UNDERWATER_CASTLE + " " + Level::STRING_WORLD_8_LEVEL_3 + " " + QString::number(7))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::UNDERWATER_CASTLE, Level::WORLD_8_LEVEL_3, 7)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_UNDERWATER_CASTLE + " " + Level::STRING_WORLD_2_LEVEL_2 + " " + QString::number(12))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::UNDERWATER_CASTLE, Level::WORLD_2_LEVEL_2, 12)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_3_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_2 + " " + QString::number(11))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_3_LEVEL_2, Level::WORLD_2_LEVEL_2, 11)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_3_LEVEL_2 + " " + Level::STRING_WORLD_6_LEVEL_1 + " " + QString::number(25))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_3_LEVEL_2, Level::WORLD_6_LEVEL_1, 25)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_3 + " " + Level::STRING_WORLD_6_LEVEL_1 + " " + QString::number(15))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_3, Level::WORLD_6_LEVEL_1, 15)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_3 + " " + Level::STRING_WORLD_4_LEVEL_4 + " " + QString::number(13))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_3, Level::WORLD_4_LEVEL_4, 13)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_5_LEVEL_1 + " " + Level::STRING_WORLD_4_LEVEL_4 + " " + QString::number(29))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_5_LEVEL_1, Level::WORLD_4_LEVEL_4, 29)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_5_LEVEL_1 + " " + Level::STRING_WORLD_6_LEVEL_2 + " " + QString::number(6))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_5_LEVEL_1, Level::WORLD_6_LEVEL_2, 6)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_WORLD_6_LEVEL_2 + " " + QString::number(38))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::WORLD_6_LEVEL_2, 38)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_3 + " " + QString::number(6))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::WORLD_2_LEVEL_3, 6)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_3 + " " + QString::number(44))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_2_LEVEL_3, 44)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_7_LEVEL_4 + " " + QString::number(1))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_7_LEVEL_4, 1)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_7_LEVEL_4 + " " + QString::number(57))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_7_LEVEL_4, 57)) return false;
    if (!this->Write_To_Map(mapStream, Level_Type::STRING_BREAK)) return false;
    return true;
}

bool Level_Generator::Write_To_Map(QTextStream &mapStream, const QString &string) {
    mapStream << string << Common_Strings::STRING_NEW_LINE;
    if (mapStream.status() != QTextStream::Ok) {
        //TODO: Show a read/write error here
        return false;
    }
    return true;
}

bool Level_Generator::Write_To_Map(QTextStream &mapStream, Level::Level level, const QString &fileName) {
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
    return this->Write_To_Map(mapStream, line);
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
