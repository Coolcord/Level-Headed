#ifndef LEVEL_GENERATOR_H
#define LEVEL_GENERATOR_H

#include "../SMB1_Compliance_Generator/SMB1_Compliance_Generator_Interface.h"
#include "../SMB1_Writer/SMB1_Writer_Interface.h"
#include "../Common_SMB1_Files/Level.h"
#include "../Common_SMB1_Files/Level_Type.h"
#include "../SMB1_Compliance_Generator/SMB1_Compliance_Generator_Arguments.h"
#include "Plugin_Settings.h"
#include <QFile>
#include <QTextStream>
#include <QWidget>
#include <QMap>
#include <QVector>
#include <QString>

class Level_Generator
{
public:
    Level_Generator(const QString &applicationLocation, QWidget *parent, Plugin_Settings *pluginSettings,
                    SMB1_Compliance_Generator_Interface *generatorPlugin, SMB1_Writer_Interface *writerPlugin);
    ~Level_Generator();
    bool Generate_Levels();
    bool Parse_Level_Map();
    bool Parse_Map_Header(QFile &file, int &lineNum, int &errorCode);
    bool Parse_Move_Object_Table(QFile &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode);
    bool Parse_Move_Enemy_Table(QFile &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode);
    bool Parse_Levels(QFile &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode);

private:
    SMB1_Compliance_Generator_Arguments Prepare_Arguments(const QString &generationName, int levelNum);
    Level_Type::Level_Type Determine_Level_Type();
    void Read_Level_Chance(const QString &chance, Level_Type::Level_Type levelType);
    QString Parse_Through_Comments_Until_First_Word(QFile &file, const QString &word, int &lineNum);
    bool Parse_Through_Comments_Until_String(QFile &file, const QString &value, int &lineNum);
    void Populate_Level_Map(QMap<QString, Level::Level> &levels);
    bool Parse_To_Next_Seperator(QFile &file, int &lineNum);
    bool Parse_Move_Table(QFile &file, const QMap<QString, Level::Level> &levels, int &lineNum, int &errorCode, bool objects);

    //These functions will be depreciated soon
    bool Append_Level(QVector<Level::Level> &levelOrder, Level::Level level);
    bool Rearrange_Levels_From_Short_To_Long(QVector<Level::Level> &levelOrder, int numLevels);
    bool Write_Move_Objects_Map(QTextStream &mapStream);
    bool Write_Move_Enemies_Map(QTextStream &mapStream);
    bool Write_To_Map(QTextStream &mapStream, const QString &string);
    bool Write_To_Map(QTextStream &mapStream, Level::Level level, const QString &fileName = "");

    QWidget *parent;
    Plugin_Settings *pluginSettings;
    SMB1_Compliance_Generator_Interface *generatorPlugin;
    SMB1_Writer_Interface *writerPlugin;
    QString levelLocation;
    QMap<QString, Chance> *chances;
    QVector<Level_Type::Level_Type> *veryCommonLevels;
    QVector<Level_Type::Level_Type> *commonLevels;
    QVector<Level_Type::Level_Type> *uncommonLevels;
    QVector<Level_Type::Level_Type> *rareLevels;
};

#endif // LEVEL_GENERATOR_H
