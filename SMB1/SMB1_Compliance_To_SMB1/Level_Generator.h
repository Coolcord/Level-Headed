#ifndef LEVEL_GENERATOR_H
#define LEVEL_GENERATOR_H

#include "../../../Sequential_Archive/Sequential_Archive/Sequential_Archive_Interface.h"
#include "../SMB1_Compliance_Generator/SMB1_Compliance_Generator_Interface.h"
#include "../SMB1_Writer/SMB1_Writer_Interface.h"
#include "Plugin_Settings.h"
#include <QPluginLoader>
#include <QTextStream>
#include <QWidget>

class Text_Insertion_Buffer;

class Level_Generator {
public:
    Level_Generator(const QString &applicationLocation, QWidget *parent, Plugin_Settings *pluginSettings,
                    SMB1_Compliance_Generator_Interface *generatorPlugin, SMB1_Writer_Interface *writerPlugin);
    ~Level_Generator();
    bool Run_Level_Generator();

private:
    SMB1_Compliance_Generator_Arguments Prepare_Arguments(const QString &generationName, int levelNum, int numLevels);

    SMB1_Compliance_Generator_Arguments Prepare_Pipe_Exit_Small_Castle_Arguments(const QString &generationName);
    SMB1_Compliance_Generator_Arguments Prepare_Pipe_Exit_Big_Castle_Arguments(const QString &generationName);
    SMB1_Compliance_Generator_Arguments Prepare_Underground_Bonus_Arguments(const QString &generationName);
    //SMB1_Compliance_Generator_Arguments Prepare_Pipe_Intro_Arguments(const QString &generationName); //TODO: Implement this!
    //SMB1_Compliance_Generator_Arguments Prepare_Cloud_Bonus_Day_Arguments(const QString &generationName); //TODO: Implement this!
    //SMB1_Compliance_Generator_Arguments Prepare_Cloud_Bonus_Night_Arguments(const QString &generationName); //TODO: Implement this!
    //SMB1_Compliance_Generator_Arguments Prepare_Underwater_Bonus_Arguments(const QString &generationName); //TODO: Implement this!

    void Prepare_Difficulty_Arguments(SMB1_Compliance_Generator_Arguments &args);
    int Get_Level_Length(int index, int difficulty, bool autoScroll, Level_Type::Level_Type levelType);
    QString Convert_Level_Enum_To_String(Level::Level level);
    bool Delete_Old_Level_Generations();
    Level_Type::Level_Type Determine_Level_Type(int levelNum, int numLevels, int numWorlds, int numLevelsPerWorld);
    bool Determine_If_Auto_Scroller(int levelNum, int difficulty);
    bool Generate_New_Levels(QString &generationFileName);
    bool Handle_Map_File();
    bool Load_Sequential_Archive_Plugin();
    void Read_Level_Chance(const QString &chance, Level_Type::Level_Type levelType);
    bool Rearrange_Levels_From_Short_To_Long(QVector<Level::Level> &levelOrder, int numLevels);
    bool Move_Enemies_In_Map(int numBytes, Level::Level sourceLevel, Level::Level destinationLevel);
    bool Move_Objects_In_Map(int numBytes, Level::Level sourceLevel, Level::Level destinationLevel);
    bool Parse_Boolean(const QString &string, bool &value);
    bool Parse_Levels(QTextStream &file, const QMap<QString, Level::Level> &levels, int numWorlds, int numLevelsPerWorld, int &lineNum, int &errorCode);
    bool Parse_Level_Map(const QString &fileName);
    bool Parse_Map_Header(QTextStream &file, int &numWorlds, int &numLevelsPerWorld, int &lineNum, int &errorCode);
    bool Parse_Move_Table(QTextStream &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode, bool objects);
    QString Parse_Through_Comments_Until_First_Word(QTextStream &file, const QString &word, int &lineNum);
    bool Parse_To_Next_Seperator(QTextStream &file, int &lineNum);
    void Populate_Level_Map(QMap<QString, Level::Level> &levels);
    void Populate_Num_Objects_And_Enemies_In_Levels();
    void Prevent_First_Level_From_Being_Underwater(int numLevelsPerWorld, int numLevelTypes);
    void Sort_Very_Rare_And_Mythic_Levels();
    bool Write_Move_Items_To_Map(Text_Insertion_Buffer &mapBuffer, int numBytes, Level::Level sourceLevel, Level::Level destinationLevel, bool objects);
    bool Write_Move_Objects_Map(Text_Insertion_Buffer &mapBuffer);
    bool Write_Move_Enemies_Map(Text_Insertion_Buffer &mapBuffer);
    bool Write_Header_To_Level(const SMB1_Compliance_Parser_Arguments &args, bool bonusLevel);
    bool Write_Objects_To_Level(SMB1_Compliance_Parser_Arguments &args);
    bool Write_Enemies_To_Level(SMB1_Compliance_Parser_Arguments &args);
    bool Write_To_Map(Text_Insertion_Buffer &mapBuffer, const QString &string);
    bool Write_To_Map(Text_Insertion_Buffer &mapBuffer, Level::Level level, const QString &fileName = "");

    QWidget *parent;
    QString applicationLocation;
    Plugin_Settings *pluginSettings;
    SMB1_Compliance_Generator_Interface *generatorPlugin;
    SMB1_Writer_Interface *writerPlugin;
    QString levelLocation;
    QMap<QString, Chance> *chances;
    QMap<Level::Level, int> *numObjectsInLevel;
    QMap<Level::Level, int> *numEnemiesInLevel;
    QVector<Level_Type::Level_Type> *veryCommonLevels;
    QVector<Level_Type::Level_Type> *commonLevels;
    QVector<Level_Type::Level_Type> *uncommonLevels;
    QVector<Level_Type::Level_Type> *rareLevels;
    QVector<Level_Type::Level_Type> *veryRareLevels;
    QVector<Level_Type::Level_Type> *mythicLevels;
    QVector<Level_Type::Level_Type> *allocatedLevels;
    Sequential_Archive_Interface *sequentialArchivePlugin;
    QPluginLoader *sequentialArchiveLoader;
    bool sequentialArchiveLoaded;
};

#endif // LEVEL_GENERATOR_H
