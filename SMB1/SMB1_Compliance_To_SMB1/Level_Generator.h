#ifndef LEVEL_GENERATOR_H
#define LEVEL_GENERATOR_H

#include "../../../Sequential_Archive/Sequential_Archive/Sequential_Archive_Interface.h"
#include "../SMB1_Compliance_Generator/SMB1_Compliance_Generator_Interface.h"
#include "../SMB1_Writer/SMB1_Writer_Interface.h"
#include "../Common_SMB1_Files/Level.h"
#include "../Common_SMB1_Files/Level_Type.h"
#include "../SMB1_Compliance_Generator/SMB1_Compliance_Generator_Arguments.h"
#include "Plugin_Settings.h"
#include <QFile>
#include <QPluginLoader>
#include <QTextStream>
#include <QWidget>
#include <QMap>
#include <QVector>
#include <QString>

class Text_Insertion_Buffer;

class Level_Generator {
public:
    Level_Generator(const QString &applicationLocation, QWidget *parent, Plugin_Settings *pluginSettings,
                    SMB1_Compliance_Generator_Interface *generatorPlugin, SMB1_Writer_Interface *writerPlugin);
    ~Level_Generator();
    bool Generate_Levels();
    bool Parse_Level_Map();
    bool Parse_Map_Header(QTextStream &file, int &numLevelsPerWorld, int &lineNum, int &errorCode);
    bool Parse_Move_Object_Table(QTextStream &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode);
    bool Parse_Move_Enemy_Table(QTextStream &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode);
    bool Parse_Levels(QTextStream &file, const QMap<QString, Level::Level> &levels, int numLevelsPerWorld, int &lineNum, int &errorCode);

private:
    SMB1_Compliance_Generator_Arguments Prepare_Arguments(const QString &generationName, int levelNum, int numLevels);
    int Get_Level_Length(int index, int difficulty, bool autoScroll, Level_Type::Level_Type levelType);
    bool Delete_Old_Level_Generations();
    Level_Type::Level_Type Determine_Level_Type(int levelNum);
    bool Generate_Levels_And_Pack(QString &folderLocation);
    bool Handle_Map_File();
    void Read_Level_Chance(const QString &chance, Level_Type::Level_Type levelType);
    QString Parse_Through_Comments_Until_First_Word(QTextStream &file, const QString &word, int &lineNum);
    bool Parse_Through_Comments_Until_String(QTextStream &file, const QString &value, int &lineNum);
    void Populate_Level_Map(QMap<QString, Level::Level> &levels);
    bool Parse_To_Next_Seperator(QTextStream &file, int &lineNum);
    bool Parse_Move_Table(QTextStream &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode, bool objects);
    bool Append_Level(QVector<Level::Level> &levelOrder, Level::Level level);
    bool Rearrange_Levels_From_Short_To_Long(QVector<Level::Level> &levelOrder, int numLevels);
    bool Write_Move_Objects_Map(Text_Insertion_Buffer &mapBuffer);
    bool Write_Move_Enemies_Map(Text_Insertion_Buffer &mapBuffer);
    bool Write_To_Map(Text_Insertion_Buffer &mapBuffer, const QString &string);
    bool Write_To_Map(Text_Insertion_Buffer &mapBuffer, Level::Level level, const QString &fileName = "");
    bool Load_Sequential_Archive_Plugin();
    bool Parse_Boolean(const QString &string, bool &value);

    QWidget *parent;
    QString applicationLocation;
    Plugin_Settings *pluginSettings;
    SMB1_Compliance_Generator_Interface *generatorPlugin;
    SMB1_Writer_Interface *writerPlugin;
    QString levelLocation;
    QMap<QString, Chance> *chances;
    QVector<Level_Type::Level_Type> *veryCommonLevels;
    QVector<Level_Type::Level_Type> *commonLevels;
    QVector<Level_Type::Level_Type> *uncommonLevels;
    QVector<Level_Type::Level_Type> *rareLevels;
    Sequential_Archive_Interface *sequentialArchivePlugin;
    QPluginLoader *sequentialArchiveLoader;
    bool sequentialArchiveLoaded;
};

#endif // LEVEL_GENERATOR_H
