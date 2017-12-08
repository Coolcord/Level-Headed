#include "Level_Generator.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../../Common_Files/Random.h"
#include "../Common_SMB1_Files/Level_String.h"
#include "../Common_SMB1_Files/Header_String.h"
#include "../SMB1_Writer/SMB1_Writer_Strings.h"
#include "SMB1_Compliance_Parser.h"
#include <QMessageBox>
#include <QDir>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <assert.h>

Level_Generator::Level_Generator(const QString &applicationLocation, QWidget *parent, Plugin_Settings *pluginSettings,
                                 SMB1_Compliance_Generator_Interface *generatorPlugin, SMB1_Writer_Interface *writerPlugin) {
    assert(parent);
    assert(pluginSettings);
    assert(generatorPlugin);
    assert(writerPlugin);
    this->parent = parent;
    this->pluginSettings = pluginSettings;
    this->generatorPlugin = generatorPlugin;
    this->writerPlugin = writerPlugin;
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
    delete this->chances;
    delete this->veryCommonLevels;
    delete this->commonLevels;
    delete this->uncommonLevels;
    delete this->rareLevels;
}

bool Level_Generator::Generate_Levels() {
    this->veryCommonLevels->clear();
    this->commonLevels->clear();
    this->uncommonLevels->clear();
    this->rareLevels->clear();

    //Load a ROM into the Writer Plugin
    bool loaded = false;
    if (this->pluginSettings->baseROM.isEmpty()) {
        loaded = this->writerPlugin->Load_ROM();
    } else {
        loaded = this->writerPlugin->Load_ROM(this->pluginSettings->baseROM);
    }
    if (!loaded) {
        qDebug() << "Failed to load the ROM!";
        return false;
    }

    //Make the folder to store the random generation in
    QString generationName = "Random " + QDate::currentDate().toString("yy-MM-dd-") + QTime::currentTime().toString("HH-mm-ss-zzz");
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
    QFile map(this->levelLocation + "/" + generationName + "/" + generationName + ".map");
    if (!map.open(QFile::ReadWrite | QFile::Truncate)) {
        //TODO: Show a read/write error here
        return false;
    }
    QTextStream mapStream(&map);
    if (!this->Write_To_Map(mapStream, Header::STRING_MAP_NAME)) return false;

    //Set up the parser
    SMB1_Compliance_Parser parser(this->writerPlugin);

    //Seed the random number generator... the location here is important
    qsrand(this->pluginSettings->randomSeed);

    //Randomly determine the number of max levels and levels per world if specified
    if (this->pluginSettings->randomNumWorlds) {
        this->pluginSettings->numWorlds = Random::Get_Num(4)+2; //max out at 6 worlds
        this->pluginSettings->numLevelsPerWorld = 8;
        while (this->pluginSettings->numLevelsPerWorld*this->pluginSettings->numWorlds > 20) {
            --this->pluginSettings->numLevelsPerWorld;
        }
    }

    //Write the Number of Worlds
    if (!this->writerPlugin->Set_Number_Of_Worlds(this->pluginSettings->numWorlds)) {
        qDebug() << "Failed to write the number of worlds to the ROM!";
        return false;
    }

    //Write the watermark
    if (!this->writerPlugin->Write_Watermark()) {
        qDebug() << "Failed to write the watermark to the ROM!";
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
    if (!this->Write_To_Map(mapStream, Header::STRING_RANDOM_SEED + ": " + QString::number(this->pluginSettings->randomSeed))) return false;
    if (this->pluginSettings->hammerTime) {
        if (!this->Write_To_Map(mapStream, Header::STRING_HAMMER_TIME + ": " + Header::STRING_TRUE)) return false;
    } else {
        if (!this->Write_To_Map(mapStream, Header::STRING_HAMMER_TIME + ": " + Header::STRING_FALSE)) return false;
    }

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
    for (int i = 0; i < numLevels; ++i) {
        //Prepare Arguments
        SMB1_Compliance_Generator_Arguments args = this->Prepare_Arguments(generationName, i);
        args.hammerTime = this->pluginSettings->hammerTime;

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
        int errorCode = parser.Parse_Level(args.fileName, lineNum);
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
    this->pluginSettings->randomSeed = QTime::currentTime().msecsSinceStartOfDay();
    return true;
}

bool Level_Generator::Parse_Level_Map() {
    QFile mapFile(this->levelLocation + "/" + this->pluginSettings->levelScripts + "/" + this->pluginSettings->levelScripts + ".map");
    if (!mapFile.exists() || !mapFile.open(QFile::ReadOnly)) {
        //TODO: Show an error here
        return false;
    }

    //Load a ROM into the Writer Plugin
    //TODO: This section is duplicate
    bool loaded = false;
    if (this->pluginSettings->baseROM.isEmpty()) {
        loaded = this->writerPlugin->Load_ROM();
    } else {
        loaded = this->writerPlugin->Load_ROM(this->pluginSettings->baseROM);
    }
    if (!loaded) {
        qDebug() << "Failed to load the ROM!";
        return false;
    }

    //Parse through the map file starting with the header
    int lineNum = 0;
    int errorCode = 0;
    if (!this->Parse_Map_Header(mapFile, lineNum, errorCode)) return false;
    if (mapFile.atEnd()) return 2;

    //Parse the Move Tables
    QMap<QString, Level::Level> levels;
    this->Populate_Level_Map(levels);
    if (!this->Parse_Move_Object_Table(mapFile, levels, lineNum, errorCode)) return false;
    if (mapFile.atEnd()) return 2;
    if (!this->Parse_Move_Enemy_Table(mapFile, levels, lineNum, errorCode)) return false;
    if (mapFile.atEnd()) return 2;

    //Parse the Levels
    if (!this->Parse_Levels(mapFile, levels, lineNum, errorCode)) return false;
    if (!mapFile.atEnd()) return 2;
    mapFile.close();
    return true;
}

bool Level_Generator::Parse_Map_Header(QFile &file, int &lineNum, int &errorCode) {
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
    if (!this->writerPlugin->Set_Number_Of_Worlds(numWorlds)) {
        errorCode = 3;
        return false;
    }

    //Write the watermark
    if (!this->writerPlugin->Write_Watermark()) {
        errorCode = 3;
        return false;
    }

    //Parse Seperator at the end
    if (!this->Parse_To_Next_Seperator(file, lineNum)) return false;

    return true;
}

bool Level_Generator::Parse_Move_Object_Table(QFile &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode) {
    return this->Parse_Move_Table(file, levels, lineNum, errorCode, true);
}

bool Level_Generator::Parse_Move_Enemy_Table(QFile &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode) {
    return this->Parse_Move_Table(file, levels, lineNum, errorCode, false);
}

bool Level_Generator::Parse_Levels(QFile &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode) {
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
                int lastIndex = file.fileName().lastIndexOf("/");
                scriptName = file.fileName().remove(lastIndex, file.fileName().size()-lastIndex) + "/" + scriptName;
                if (!this->writerPlugin->New_Level(currentLevel)) {
                    QMessageBox::critical(this->parent, Common_Strings::STRING_LEVEL_HEADED,
                                          "The writer plugin failed to allocate buffers for a new level!", Common_Strings::STRING_OK);
                    return false;
                }

                //TODO: Make these error messages file specific
                SMB1_Compliance_Parser parser(this->writerPlugin);
                int levelLineNum = 0;
                int levelErrorCode = parser.Parse_Level(scriptName, levelLineNum);
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

SMB1_Compliance_Generator_Arguments Level_Generator::Prepare_Arguments(const QString &generationName, int levelNum) {
    int level = (levelNum%this->pluginSettings->numLevelsPerWorld)+1;
    int world = (levelNum/this->pluginSettings->numLevelsPerWorld)+1;
    SMB1_Compliance_Generator_Arguments args;
    args.fileName = this->levelLocation + "/" + generationName + "/Level_" + QString::number(world) + "_" + QString::number(level) + ".lvl";

    //Determine the level type. The last level of each world should be a castle
    if (level == this->pluginSettings->numLevelsPerWorld) args.levelType = Level_Type::CASTLE;
    else args.levelType = this->Determine_Level_Type();
    switch (args.levelType) {
    case Level_Type::STANDARD_OVERWORLD:
        if (Random::Get_Num(4)==0) args.levelCompliment = Level_Compliment::MUSHROOMS;
        else args.levelCompliment = Level_Compliment::TREES;
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
        if (Random::Get_Num(4)==0) args.levelCompliment = Level_Compliment::MUSHROOMS;
        else args.levelCompliment = Level_Compliment::TREES;
        args.headerScenery = Scenery::NO_SCENERY;
        if (args.levelCompliment == Level_Compliment::TREES && Random::Get_Num(3)==0) {
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
        args.levelCompliment = Level_Compliment::TREES;
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
    int veryCommonChance = 50*numVeryCommon; int commonChance = (30*numCommon)+veryCommonChance;
    int uncommonChance = (15*numUncommon)+commonChance; int rareChance = (5*numRare)+uncommonChance;
    int random = Random::Get_Num(rareChance-1);
    int index = 0;

    //Determine the level type by probability
    Level_Type::Level_Type levelType = Level_Type::STANDARD_OVERWORLD;
    if (random < veryCommonChance) { //very common
        index = (random)/50;
        assert(index < this->veryCommonLevels->size());
        levelType = this->veryCommonLevels->at(index);
    } else if (random < commonChance) { //common
        index = (random-veryCommonChance)/30;
        assert(index < this->commonLevels->size());
        levelType = this->commonLevels->at(index);
    } else if (random < uncommonChance) { //uncommon
        index = (random-commonChance)/15;
        assert(index < this->uncommonLevels->size());
        levelType = this->uncommonLevels->at(index);
    } else if (random < rareChance) { //rare
        index = (random-uncommonChance)/5;
        assert(index < this->rareLevels->size());
        levelType = this->rareLevels->at(index);
    } else assert(false);
    return levelType;
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
    default:
        assert(false); return;
    }
}

//TODO: This is a duplicate of the Header Handler function. Refactor so that there is only one
QString Level_Generator::Parse_Through_Comments_Until_First_Word(QFile &file, const QString &word, int &lineNum) {
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
bool Level_Generator::Parse_Through_Comments_Until_String(QFile &file, const QString &value, int &lineNum) {
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

bool Level_Generator::Parse_To_Next_Seperator(QFile &file, int &lineNum) {
    QString line = QString();
    do {
        ++lineNum;
        line = file.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (file.atEnd()) return false; //TODO: Handle this error
    } while (!line.startsWith("==="));
    return true;
}

bool Level_Generator::Parse_Move_Table(QFile &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode, bool objects) {
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
    case 19:
        if (!this->Append_Level(levelOrder, Level::WORLD_1_LEVEL_1)) return false;
    case 18:
        if (!this->Append_Level(levelOrder, Level::WORLD_3_LEVEL_3)) return false;
    case 17:
        if (!this->Append_Level(levelOrder, Level::WORLD_1_LEVEL_4)) return false;
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
    if (!this->Write_To_Map(mapStream, Level::STRING_WARP_ZONE + " " + Level::STRING_WORLD_7_LEVEL_1 + " " + QString::number(14))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WARP_ZONE, Level::WORLD_7_LEVEL_1, 14)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WARP_ZONE + " " + Level::STRING_WORLD_1_LEVEL_1 + " " + QString::number(10))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WARP_ZONE, Level::WORLD_1_LEVEL_1, 10)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WARP_ZONE + " " + Level::STRING_WORLD_3_LEVEL_3 + " " + QString::number(6))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WARP_ZONE, Level::WORLD_3_LEVEL_3, 6)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WARP_ZONE + " " + Level::STRING_UNDERWATER_BONUS + " " + QString::number(10))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WARP_ZONE, Level::UNDERWATER_BONUS, 10)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_3_LEVEL_2 + " " + Level::STRING_WORLD_3_LEVEL_3 + " " + QString::number(14))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_3_LEVEL_2, Level::WORLD_3_LEVEL_3, 14)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_3_LEVEL_2 + " " + Level::STRING_WORLD_1_LEVEL_4 + " " + QString::number(26))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_3_LEVEL_2, Level::WORLD_1_LEVEL_4, 26)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_3_LEVEL_2 + " " + Level::STRING_WORLD_6_LEVEL_3 + " " + QString::number(8))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_3_LEVEL_2, Level::WORLD_6_LEVEL_3, 8)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_5_LEVEL_1 + " " + Level::STRING_WORLD_6_LEVEL_3 + " " + QString::number(18))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_5_LEVEL_1, Level::WORLD_6_LEVEL_3, 18)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_5_LEVEL_1 + " " + Level::STRING_WORLD_2_LEVEL_1 + " " + QString::number(32))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_5_LEVEL_1, Level::WORLD_2_LEVEL_1, 32)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_5_LEVEL_1 + " " + Level::STRING_WORLD_4_LEVEL_3 + " " + QString::number(10))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_5_LEVEL_1, Level::WORLD_4_LEVEL_3, 10)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_4_LEVEL_3 + " " + QString::number(24))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_4_LEVEL_3, 24)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_8_LEVEL_3 + " " + QString::number(36))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_8_LEVEL_3, 36)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_1 + " " + Level::STRING_WORLD_3_LEVEL_4 + " " + QString::number(20))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_1, Level::WORLD_3_LEVEL_4, 20)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_3 + " " + Level::STRING_WORLD_3_LEVEL_4 + " " + QString::number(18))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_3, Level::WORLD_3_LEVEL_4, 18)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_3 + " " + Level::STRING_WORLD_6_LEVEL_1 + " " + QString::number(36))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_3, Level::WORLD_6_LEVEL_1, 36)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_3 + " " + Level::STRING_WORLD_8_LEVEL_4 + " " + QString::number(28))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_3, Level::WORLD_8_LEVEL_4, 28)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_8_LEVEL_4 + " " + QString::number(12))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_8_LEVEL_4, 12)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_5_LEVEL_2 + " " + QString::number(40))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_5_LEVEL_2, 40)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_3_LEVEL_1 + " " + QString::number(42))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_3_LEVEL_1, 42)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_2_LEVEL_4 + " " + QString::number(48))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_2_LEVEL_4, 48)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_8_LEVEL_1 + " " + Level::STRING_WORLD_8_LEVEL_2 + " " + QString::number(2))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_8_LEVEL_1, Level::WORLD_8_LEVEL_2, 2)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_8_LEVEL_2 + " " + QString::number(44))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_8_LEVEL_2, 44)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_2 + " " + QString::number(48))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_2_LEVEL_2, 48)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_4_LEVEL_4 + " " + QString::number(48))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_4_LEVEL_4, 48)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_4_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_3 + " " + QString::number(18))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_4_LEVEL_2, Level::WORLD_2_LEVEL_3, 18)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_WORLD_2_LEVEL_3 + " " + QString::number(28))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::WORLD_2_LEVEL_3, 28)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_WORLD_7_LEVEL_4 + " " + QString::number(44))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::WORLD_7_LEVEL_4, 44)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_WORLD_6_LEVEL_2 + " " + QString::number(44))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::WORLD_6_LEVEL_2, 44)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_UNDERGROUND_BONUS + " " + QString::number(38))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::UNDERGROUND_BONUS, 38)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_1_LEVEL_2 + " " + Level::STRING_CLOUD_BONUS_1 + " " + QString::number(6))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::WORLD_1_LEVEL_2, Level::CLOUD_BONUS_1, 6)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_UNDERWATER_CASTLE + " " + Level::STRING_CLOUD_BONUS_1 + " " + QString::number(20))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::UNDERWATER_CASTLE, Level::CLOUD_BONUS_1, 20)) return false;
    if (!this->Write_To_Map(mapStream, Level::STRING_UNDERWATER_CASTLE + " " + Level::STRING_CLOUD_BONUS_2 + " " + QString::number(4))) return false;
    if (!this->writerPlugin->Send_Object_Bytes_From_One_Level_To_Another(Level::UNDERWATER_CASTLE, Level::CLOUD_BONUS_2, 4)) return false;
    if (!this->Write_To_Map(mapStream, Level_Type::STRING_BREAK)) return false;
    return true;
}

bool Level_Generator::Write_Move_Enemies_Map(QTextStream &mapStream) {
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
    /* Bugged 1-2 (Seed 0)
    if (!this->Write_To_Map(mapStream, Level::STRING_WORLD_7_LEVEL_1 + " " + Level::STRING_WORLD_3_LEVEL_1 + " " + QString::number(1))) return false;
    if (!this->writerPlugin->Send_Enemy_Bytes_From_One_Level_To_Another(Level::WORLD_7_LEVEL_1, Level::WORLD_3_LEVEL_1, 1)) return false;
    */
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
    default:                        assert(false);
    }

    if (fileName.size() > 0) line += " \"" + fileName + "\"";
    return this->Write_To_Map(mapStream, line);
}
