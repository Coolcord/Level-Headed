#include "Level_Generator.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../SMB1_Writer/SMB1_Writer_Strings.h"
#include "SMB1_Compliance_Parser.h"
#include <QMessageBox>
#include <QDir>
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
    QDir dir(applicationLocation + "/" + Common_Strings::LEVELS);
    if (!dir.exists(this->levelLocation)) dir.mkdir(this->levelLocation); //don't bother checking for success here
}

bool Level_Generator::Generate_Levels() {
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

    //Set up the parser
    SMB1_Compliance_Parser parser(this->writerPlugin);

    //Write the Number of Worlds
    if (!this->writerPlugin->Set_Number_Of_Worlds(this->pluginSettings->numWorlds)) {
        qDebug() << "Failed to write the number of worlds to the ROM!";
        return false;
    }

    //Build the Room Order Map
    QVector<Level::Level> levelOrder;
    if (!this->Rearrange_Levels_From_Short_To_Long(levelOrder)) {
        qDebug() << "Failed to rearrange the room order";
        return false;
    }

    //Determine the number of levels to use
    int numLevels = (this->pluginSettings->numWorlds*4);
    assert(numLevels == levelOrder.size());

    //Generate the Levels
    for (int i = 0; i < numLevels; ++i) {
        //Prepare Arguments
        SMB1_Compliance_Generator_Arguments args = this->Prepare_Arguments(i);
        if (!this->generatorPlugin->Generate_Level(args)) {
            qDebug() << "Looks like the generator blew up";
            return false;
        }

        int lineNum = 0;
        int errorCode = parser.Parse_Level(args.fileName, levelOrder.at(i), lineNum);
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
        case 4: //Writer was unable to allocate buffers for a new level
            QMessageBox::critical(this->parent, Common_Strings::LEVEL_HEADED,
                                  "The writer plugin failed to allocate buffers for a new level!", Common_Strings::OK);
            return false;
        case 5: //Writer was unable to write to the ROM
            QMessageBox::critical(this->parent, Common_Strings::LEVEL_HEADED,
                                  "The writer plugin failed to write the ROM!", Common_Strings::OK);
            return false;
        default:
            assert(false);
        }
    }
    return true;
}

SMB1_Compliance_Generator_Arguments Level_Generator::Prepare_Arguments(int levelNum) {
    ++levelNum;
    int level = (levelNum%4)+1;
    int world = (levelNum/4)+1;
    SMB1_Compliance_Generator_Arguments args;
    args.fileName = this->levelLocation + "/Level_" + QString::number(world) + "_" + QString::number(level) + ".lvl";
    args.levelType = this->Determine_Level_Type();
    switch (args.levelType) {
    case Level_Type::STANDARD_OVERWORLD:
        if (qrand()%5==0) args.levelCompliment = Level_Compliment::MUSHROOMS;
        else args.levelCompliment = Level_Compliment::TREES;
        if (qrand()%2==0) args.headerScenery = Scenery::MOUNTAINS;
        else args.headerScenery = Scenery::FENCES;
        if (levelNum == (this->pluginSettings->numWorlds*4)-1) args.headerBackground = Background::CASTLE_WALL;
        else if (level == 3 && (qrand()%4)==0) args.headerBackground = Background::CASTLE_WALL;
        else if (qrand()%4==0) {
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
    if (levelNum == 1 || args.levelType == Level_Type::CASTLE) args.startCastle = Castle::NONE;
    else if (level == 1) args.startCastle = Castle::BIG;
    else args.startCastle = Castle::SMALL;

    //Determine the end castle
    if (level == 3) args.endCastle = Castle::BIG;
    else if (args.levelType == Level_Type::CASTLE) args.endCastle = Castle::NONE;
    else args.endCastle = Castle::SMALL;

    args.numObjectBytes = this->writerPlugin->Get_Num_Object_Bytes();
    args.numEnemyBytes = this->writerPlugin->Get_Num_Enemy_Bytes();
    return args;
}

Level_Type::Level_Type Level_Generator::Determine_Level_Type() {




}

bool Level_Generator::Append_Level(QVector<Level::Level> &levelOrder, Level::Level level) {
    if (!this->writerPlugin->Room_Table_Set_Next_Level(level)) return false;
    levelOrder.append(level);
    return true;
}

//TODO: This will be depreciated once item sending is implemented
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

//TODO: This area will be depreciated once Castle Levels are implemented
Level::Level Level_Generator::Get_Random_Castle_Level(QMap<Level::Level, int> &castleLevelUses, QVector<Level::Level> &castleLevels, int &numWorldsLeft) {
    int index = (qrand()%castleLevels.size())+1;
    Level::Level level = castleLevels.at(index);
    int numUses = castleLevelUses[level];
    assert(numUses < 2);
    if (numWorldsLeft > 6 && numUses == 0) {
        castleLevelUses.insert(level, 1);
    } else {
        castleLevels.removeAt(index);
        castleLevelUses.remove(level);
    }
    --numWorldsLeft;
    return level;
}
