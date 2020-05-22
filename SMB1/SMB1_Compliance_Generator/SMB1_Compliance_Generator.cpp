#include "SMB1_Compliance_Generator.h"
#include "Standard_Overworld_Generator.h"
#include "Underground_Generator.h"
#include "Underwater_Generator.h"
#include "Castle_Generator.h"
#include "Bridge_Generator.h"
#include "Island_Generator.h"
#include "Level_Script_Parser.h"
#include "Level_Script_Modifier.h"
#include "Pipe_Exit_Generator.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
#include <assert.h>
#include <QDebug>
#include <QTime>

SMB1_Compliance_Generator::SMB1_Compliance_Generator() {
    this->applicationLocation = QString();
}

void SMB1_Compliance_Generator::Startup(QWidget *parent, const QString &location) {
    this->parent = parent;
    this->applicationLocation = location;
}

void SMB1_Compliance_Generator::Shutdown() {

}

void SMB1_Compliance_Generator::Seed_Random_Number_Generator_Instance(const QString &seed) {
    Random::Get_Instance().Seed(seed, 3);
}

bool SMB1_Compliance_Generator::Generate_Level(SMB1_Compliance_Generator_Arguments &args) {
    if (this->applicationLocation.isEmpty()) return false;

    //Create a new file for writing
    QFile file(args.fileName);
    if (!file.open(QFile::ReadWrite | QFile::Truncate)) {
        return false;
    }

    //Determine which level generator to use
    bool success = false;
    switch (args.levelType) {
    default:                                assert(false); return false;
    case Level_Type::STANDARD_OVERWORLD:    success = this->Generate_Standard_Overworld_Level(&file, &args); break;
    case Level_Type::UNDERGROUND:           success = this->Generate_Underground_Level(&file, &args); break;
    case Level_Type::UNDERWATER:            success = this->Generate_Underwater_Level(&file, &args); break;
    case Level_Type::CASTLE:                success = this->Generate_Castle_Level(&file, &args); break;
    case Level_Type::BRIDGE:                success = this->Generate_Bridge_Level(&file, &args); break;
    case Level_Type::ISLAND:                success = this->Generate_Island_Level(&file, &args); break;
    case Level_Type::PIPE_EXIT:             success = this->Generate_Pipe_Exit_Level(&file, &args); break;
    }

    file.close();
    return success;
}

bool SMB1_Compliance_Generator::Parse_Level_Script(SMB1_Compliance_Parser_Arguments &args) {
    return Level_Script_Parser(&args).Parse_Level_Script();
}

bool SMB1_Compliance_Generator::Perform_Enemy_Chaotic_Swap(Enemy_Buffer *enemyBuffer, Level_Attribute::Level_Attribute levelAttribute, bool allowHammerBros, bool allowLakitus, bool allowContinousEnemySpawners) {
    return Level_Script_Modifier().Perform_Enemy_Chaotic_Swap(enemyBuffer, levelAttribute, allowHammerBros, allowLakitus, allowContinousEnemySpawners);
}

bool SMB1_Compliance_Generator::Redistribute_Enemies(SMB1_Compliance_Generator_Arguments &args, SMB1_Compliance_Parser_Arguments &parserArgs) {
    return Level_Script_Modifier().Redistribute_Enemies(args, parserArgs);
}

bool SMB1_Compliance_Generator::Redistribute_Powerups(SMB1_Compliance_Generator_Arguments &args, SMB1_Compliance_Parser_Arguments &parserArgs) {
    return Level_Script_Modifier().Redistribute_Powerups(args, parserArgs);
}

SMB1_Compliance_Generator_Arguments SMB1_Compliance_Generator::Get_Empty_SMB1_Compliance_Generator_Arguments() {
    SMB1_Compliance_Generator_Arguments args;
    args.fileName = QString();
    args.numObjectBytes = 0;
    args.numEnemyBytes = 0;
    args.headerBackground = Background::BLANK_BACKGROUND;
    args.headerScenery = Scenery::NO_SCENERY;
    args.levelCompliment = Level_Compliment::TREES;
    args.startCastle = Castle::NONE;
    args.endCastle = Castle::NONE;
    args.levelType = Level_Type::STANDARD_OVERWORLD;
    args.useAutoScroll = false;
    args.useMidpoints = false;
    args.doesLevelExceedVerticalObjectLimit = false;
    args.useVerticalObjectLimit = false;
    args.fireFlowerBouncesLikeStar = false;
    args.difficulty = 0;
    args.difficultyBulletTime = 0;
    args.difficultyHammerTime = 0;
    args.difficultyBuzzyBeetlesReplaceLoneGoombas = 0;
    args.difficultyBridgeFlyingCheepCheeps = 0;
    args.difficultyBridgeLakitus = 0;
    args.difficultyBridgeOffscreenBulletBills = 0;
    args.difficultyCastleFireBars = 0;
    args.difficultyCastleFlyingCheepCheeps = 0;
    args.difficultyCastleLakitus = 0;
    args.difficultyCastleOffscreenBulletBills = 0;
    args.difficultyIslandFlyingCheepCheeps = 0;
    args.difficultyIslandLakitus = 0;
    args.difficultyIslandOffscreenBulletBills = 0;
    args.difficultyUndergroundFlyingCheepCheeps = 0;
    args.difficultyUndergroundLakitus = 0;
    args.difficultyUndergroundOffscreenBulletBills = 0;
    args.difficultyUnderwaterFlyingCheepCheeps = 0;
    args.difficultyUnderwaterHammerBros = 0;
    args.difficultyUnderwaterLakitus = 0;
    args.difficultyUnderwaterSwimmingCheepCheeps = 0;
    args.difficultyUnderwaterBloopers = 0;
    args.difficultyStandardOverworldFlyingCheepCheeps = 0;
    args.difficultyStandardOverworldLakitus = 0;
    args.difficultyStandardOverworldOffscreenBulletBills = 0;
    args.difficultyMinimumEnemyDistance = 0;
    args.difficultyMinimumUnderwaterEnemyDistance = 0;
    args.difficultyNoEnemies = false;
    args.difficultyHammerTimeIntensity = 0;
    args.difficultyLakituSpawnChancePerLevel = 0;
    args.difficultyDisableAllOtherEnemiesWhenALakituSpawns = false;
    args.difficultyDisableAllOtherEnemiesWhenFlyingCheepCheepsSpawn = false;
    args.difficultyPreventHammerTimeFromSpawningTwoHammerBrosInARow = false;
    args.difficultySpawnerPriority = 0;
    args.maxLevelLength = 0;
    args.difficultyMaxPowerups = 0;
    args.difficultyMaxHiddenPowerups = 0;
    args.difficultyMaxOneUps = 0;
    args.difficultyMaxTenCoinBlocks = 0;
    args.difficultyMaxStars = 0;
    args.difficultyHiddenPowerupChance = 0;
    args.difficultyOneUpChance = 0;
    args.difficultyTenCoinBlockChance = 0;
    args.difficultyStarChance = 0;
    return args;
}

SMB1_Compliance_Parser_Arguments SMB1_Compliance_Generator::Get_Empty_SMB1_Compliance_Parser_Arguments() {
    SMB1_Compliance_Parser_Arguments args;
    args.levelScriptBytes = QByteArray();
    args.headerBackground = Background::BLANK_BACKGROUND;
    args.headerBrick = Brick::NO_BRICKS;
    args.headerScenery = Scenery::NO_SCENERY;
    args.levelAttribute = Level_Attribute::OVERWORLD;
    args.levelCompliment = Level_Compliment::TREES;
    args.staringPosition = Level_Attribute::OVERWORLD;
    args.autowalk = false;
    args.time = 0;
    args.midpoint = 0;
    args.enemyBuffer = nullptr;
    args.objectBuffer = nullptr;
    args.lineNum = 0;
    return args;
}

bool SMB1_Compliance_Generator::Generate_Standard_Overworld_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    Standard_Overworld_Generator levelGenerator(file, args);
    return levelGenerator.Generate_Level();
}

bool SMB1_Compliance_Generator::Generate_Underground_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    Underground_Generator levelGenerator(file, args);
    return levelGenerator.Generate_Level();
}

bool SMB1_Compliance_Generator::Generate_Underwater_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    Underwater_Generator levelGenerator(file, args);
    return levelGenerator.Generate_Level();
}

bool SMB1_Compliance_Generator::Generate_Castle_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    Castle_Generator levelGenerator(file, args);
    return levelGenerator.Generate_Level();
}

bool SMB1_Compliance_Generator::Generate_Bridge_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    Bridge_Generator levelGenerator(file, args);
    return levelGenerator.Generate_Level();
}

bool SMB1_Compliance_Generator::Generate_Island_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    Island_Generator levelGenerator(file, args);
    return levelGenerator.Generate_Level();
}

bool SMB1_Compliance_Generator::Generate_Pipe_Exit_Level(QFile *file, SMB1_Compliance_Generator_Arguments *args) {
    Pipe_Exit_Generator pipeExitGenerator(file, args);
    return pipeExitGenerator.Generate_Level();
}
