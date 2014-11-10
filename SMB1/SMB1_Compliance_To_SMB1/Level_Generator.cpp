#include "Level_Generator.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../Common SMB1 Files/Level_String.h"
#include "../Common SMB1 Files/Header_String.h"
#include "../SMB1_Writer/SMB1_Writer_Strings.h"
#include "SMB1_Compliance_Parser.h"
#include <QMessageBox>
#include <QFile>
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
    this->levelLocation = applicationLocation + "/" + Common_Strings::LEVELS + "/" + Common_Strings::GAME_NAME;
    QDir dir(applicationLocation);
    if (!dir.exists(Common_Strings::LEVELS)) dir.mkdir(Common_Strings::LEVELS); //don't bother checking for success here
    if (dir.cd(Common_Strings::LEVELS)) {
        if (!dir.exists(Common_Strings::GAME_NAME)) {
            dir.mkdir(Common_Strings::GAME_NAME);
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

    //Write the Number of Worlds
    if (!this->writerPlugin->Set_Number_Of_Worlds(this->pluginSettings->numWorlds)) {
        qDebug() << "Failed to write the number of worlds to the ROM!";
        return false;
    }

    //Write the Comment section of the map file
    if (!this->Write_To_Map(mapStream, Level_Type::STRING_BREAK)) return false;
    if (!this->Write_To_Map(mapStream, Header::STRING_COOLCORD)) return false;
    if (!this->Write_To_Map(mapStream, Header::STRING_CREATED + " " + QDate::currentDate().toString("dddd, MMMM dd, yyyy") + ", at " + QTime::currentTime().toString("hh:mm:ss A."))) return false;

    //Write the Header of the map file
    if (!this->Write_To_Map(mapStream, Level_Type::STRING_BREAK)) return false;
    if (!this->Write_To_Map(mapStream, Header::STRING_NUMBER_OF_WORLDS + ": " + QString::number(this->pluginSettings->numWorlds))) return false;
    if (!this->Write_To_Map(mapStream, Header::STRING_NUMBER_OF_LEVELS_PER_WORLD + ": " + QString::number(this->pluginSettings->numLevelsPerWorld))) return false;
    if (!this->Write_To_Map(mapStream, Level_Type::STRING_BREAK)) return false;

    //Build the Room Order Map
    QVector<Level::Level> levelOrder;
    if (!this->Rearrange_Levels_From_Short_To_Long(levelOrder)) {
        qDebug() << "Failed to rearrange the room order";
        return false;
    }

    //Determine the number of levels to use
    int numLevels = this->pluginSettings->numWorlds*this->pluginSettings->numLevelsPerWorld;
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

        //TODO: This if will be depricated once Castle levels can be generated
        if (args.levelType == Level_Type::CASTLE) {
            this->writerPlugin->Header_Midpoint(levelOrder.at(i), 0);
            if (!this->Write_To_Map(mapStream, levelOrder.at(i))) return false;
        } else {
            if (!this->writerPlugin->New_Level(levelOrder.at(i))) {
                QMessageBox::critical(this->parent, Common_Strings::LEVEL_HEADED,
                                      "The writer plugin failed to allocate buffers for a new level!", Common_Strings::OK);
                return false;
            }
            args.numObjectBytes = this->writerPlugin->Get_Num_Object_Bytes();
            args.numEnemyBytes = this->writerPlugin->Get_Num_Enemy_Bytes();
            qDebug() << args.numObjectBytes;

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
                QMessageBox::critical(this->parent, Common_Strings::LEVEL_HEADED,
                                      "Unable to open " + args.fileName + "!", Common_Strings::OK);
                return false;
            case 2: //Syntax error
                QMessageBox::critical(this->parent, Common_Strings::LEVEL_HEADED,
                                      "Syntax error on line " + QString::number(lineNum) + "!", Common_Strings::OK);
                return false;
            case 3: //Writer was unable to write an item
                QMessageBox::critical(this->parent, Common_Strings::LEVEL_HEADED,
                                      "The writer plugin failed to write item on line " + QString::number(lineNum) + "!", Common_Strings::OK);
                return false;
            default:
                assert(false);
            }

            if (!this->writerPlugin->Write_Level()) {
                QMessageBox::critical(this->parent, Common_Strings::LEVEL_HEADED,
                                      "The writer plugin failed to write the ROM!", Common_Strings::OK);
                return false;
            }

            //Write the level to the map
            if (!this->Write_To_Map(mapStream, levelOrder.at(i), args.fileName.split("/").last())) return false;
        }
    }
    if (!this->Write_To_Map(mapStream, Level_Type::STRING_BREAK)) return false;

    mapStream.flush();
    map.close();
    return true;
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
        if (qrand()%5==0) args.levelCompliment = Level_Compliment::MUSHROOMS;
        else args.levelCompliment = Level_Compliment::TREES;
        if (qrand()%2==0) args.headerScenery = Scenery::MOUNTAINS;
        else args.headerScenery = Scenery::FENCES;
        if (qrand()%4==0) {
            int random = qrand()%115;
            if (random < 40) args.headerBackground = Background::NIGHT;
            if (random < 80) args.headerBackground = Background::SNOW;
            else if (random < 105) args.headerBackground = Background::NIGHT_AND_SNOW;
            else if (random < 115) args.headerBackground = Background::NIGHT_AND_FREEZE;
            else assert(false);
        } else args.headerBackground = Background::BLANK_BACKGROUND;
        break;
    case Level_Type::BRIDGE:
    case Level_Type::ISLAND:
        if (qrand()%5==0) args.levelCompliment = Level_Compliment::MUSHROOMS;
        else args.levelCompliment = Level_Compliment::TREES;
        args.headerScenery = Scenery::ONLY_CLOUDS;
        if (qrand()%3==0) args.headerBackground = Background::OVER_WATER;
        else if (qrand()%4==0) {
            int random = qrand()%115;
            if (random < 40) args.headerBackground = Background::NIGHT;
            if (random < 80) args.headerBackground = Background::SNOW;
            else if (random < 105) args.headerBackground = Background::NIGHT_AND_SNOW;
            else if (random < 115) args.headerBackground = Background::NIGHT_AND_FREEZE;
            else assert(false);
        } else args.headerBackground = Background::BLANK_BACKGROUND;
        break;
    case Level_Type::UNDERGROUND:
        if (qrand()%5==0) args.levelCompliment = Level_Compliment::MUSHROOMS;
        else args.levelCompliment = Level_Compliment::TREES;
        args.headerScenery = Scenery::NO_SCENERY;
        if (args.levelCompliment == Level_Compliment::TREES && qrand()%5==0) {
            if (qrand()%5==0) args.headerBackground = Background::NIGHT_AND_FREEZE;
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
    int random = (qrand()%rareChance);
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

bool Level_Generator::Append_Level(QVector<Level::Level> &levelOrder, Level::Level level) {
    if (!this->writerPlugin->Room_Table_Set_Next_Level(level)) return false;
    levelOrder.append(level);
    return true;
}

//TODO: This will be deprecated once item sending is implemented
bool Level_Generator::Rearrange_Levels_From_Short_To_Long(QVector<Level::Level> &levelOrder) {
    QMap<Level::Level, int> castleLevelUses;
    QVector<Level::Level> castleLevels;
    castleLevelUses.insert(Level::WORLD_1_LEVEL_4, 0);
    castleLevels.append(Level::WORLD_1_LEVEL_4);
    castleLevelUses.insert(Level::WORLD_2_LEVEL_4, 0);
    castleLevels.append(Level::WORLD_2_LEVEL_4);
    castleLevelUses.insert(Level::WORLD_3_LEVEL_4, 0);
    castleLevels.append(Level::WORLD_3_LEVEL_4);
    castleLevelUses.insert(Level::WORLD_4_LEVEL_4, 0);
    castleLevels.append(Level::WORLD_4_LEVEL_4);
    castleLevelUses.insert(Level::WORLD_7_LEVEL_4, 0);
    castleLevels.append(Level::WORLD_7_LEVEL_4);
    castleLevelUses.insert(Level::WORLD_8_LEVEL_4, 0);
    castleLevels.append(Level::WORLD_8_LEVEL_4);
    int numWorldsLeft = this->pluginSettings->numWorlds;
    switch (this->pluginSettings->numWorlds) {
    case 7:
    case 8:
        if (!this->Append_Level(levelOrder, Level::WORLD_3_LEVEL_2)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_5_LEVEL_1)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_4_LEVEL_1)) return false;
        if (!this->Append_Level(levelOrder, this->Get_Random_Castle_Level(castleLevelUses, castleLevels, numWorldsLeft))) return false;
    case 5:
    case 6:
        if (!this->Append_Level(levelOrder, Level::WORLD_1_LEVEL_3)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_7_LEVEL_1)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_1_LEVEL_1)) return false;
        if (!this->Append_Level(levelOrder, this->Get_Random_Castle_Level(castleLevelUses, castleLevels, numWorldsLeft))) return false;
    case 1:
    case 2:
    case 3:
    case 4:
        if (!this->Append_Level(levelOrder, Level::WORLD_3_LEVEL_3)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_6_LEVEL_3)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_2_LEVEL_1)) return false;
        if (!this->Append_Level(levelOrder, this->Get_Random_Castle_Level(castleLevelUses, castleLevels, numWorldsLeft))) return false;
        if (this->pluginSettings->numWorlds == 1) break;
        if (!this->Append_Level(levelOrder, Level::WORLD_4_LEVEL_3)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_8_LEVEL_3)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_6_LEVEL_1)) return false;
        if (!this->Append_Level(levelOrder, this->Get_Random_Castle_Level(castleLevelUses, castleLevels, numWorldsLeft))) return false;
        if (this->pluginSettings->numWorlds == 2) break;
        if (!this->Append_Level(levelOrder, Level::WORLD_5_LEVEL_2)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_3_LEVEL_1)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_8_LEVEL_2)) return false;
        if (!this->Append_Level(levelOrder, this->Get_Random_Castle_Level(castleLevelUses, castleLevels, numWorldsLeft))) return false;
        if (this->pluginSettings->numWorlds == 3) break;
        if (!this->Append_Level(levelOrder, Level::WORLD_2_LEVEL_2)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_2_LEVEL_3)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_6_LEVEL_2)) return false;
        if (!this->Append_Level(levelOrder, this->Get_Random_Castle_Level(castleLevelUses, castleLevels, numWorldsLeft))) return false;
        if (this->pluginSettings->numWorlds == 4 || this->pluginSettings->numWorlds == 5) break;
        if (!this->Append_Level(levelOrder, Level::WORLD_8_LEVEL_1)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_4_LEVEL_2)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_1_LEVEL_2)) return false;
        if (!this->Append_Level(levelOrder, this->Get_Random_Castle_Level(castleLevelUses, castleLevels, numWorldsLeft))) return false;
        if (this->pluginSettings->numWorlds != 8) break;
        //Duplicate Levels
        if (!this->Append_Level(levelOrder, Level::WORLD_5_LEVEL_2)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_2_LEVEL_3)) return false;
        if (!this->Append_Level(levelOrder, Level::WORLD_6_LEVEL_2)) return false;
        if (!this->Append_Level(levelOrder, this->Get_Random_Castle_Level(castleLevelUses, castleLevels, numWorldsLeft))) return false;
        break;
    default:
        assert(false); return false;
    }
    return true;
}

bool Level_Generator::Write_To_Map(QTextStream &mapStream, const QString &string) {
    mapStream << string << "\n";
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

//TODO: This area will be deprecated once Castle Levels are implemented
Level::Level Level_Generator::Get_Random_Castle_Level(QMap<Level::Level, int> &castleLevelUses, QVector<Level::Level> &castleLevels, int &numWorldsLeft) {
    assert(castleLevels.size() > 0);
    int index = (qrand()%castleLevels.size());
    Level::Level level = castleLevels.at(index);
    int numUses = castleLevelUses[level];
    assert(numUses < 2);
    if (numUses == 0) {
        castleLevelUses.insert(level, 1);
    } else {
        castleLevels.removeAt(index);
        castleLevelUses.remove(level);
    }
    --numWorldsLeft;
    return level;
}
