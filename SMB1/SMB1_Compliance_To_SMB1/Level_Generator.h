#ifndef LEVEL_GENERATOR_H
#define LEVEL_GENERATOR_H

#include "../SMB1_Compliance_Generator/SMB1_Compliance_Generator_Interface.h"
#include "../SMB1_Writer/SMB1_Writer_Interface.h"
#include "../Common SMB1 Files/Level.h"
#include "../Common SMB1 Files/Level_Type.h"
#include "../SMB1_Compliance_Generator/SMB1_Compliance_Generator_Arguments.h"
#include "Plugin_Settings.h"
#include <QWidget>
#include <QMap>
#include <QVector>
#include <QString>

class Level_Generator
{
public:
    Level_Generator(const QString &applicationLocation, QWidget *parent, Plugin_Settings *pluginSettings,
                    SMB1_Compliance_Generator_Interface *generatorPlugin, SMB1_Writer_Interface *writerPlugin);
    bool Generate_Levels();

private:
    SMB1_Compliance_Generator_Arguments Prepare_Arguments(int levelNum);
    Level_Type::Level_Type Determine_Level_Type();

    //These functions will be depreciated soon
    bool Append_Level(QVector<Level::Level> &levelOrder, Level::Level level);
    bool Rearrange_Levels_From_Short_To_Long(QVector<Level::Level> &levelOrder);
    Level::Level Get_Random_Castle_Level(QMap<Level::Level, int> &castleLevelUses, QVector<Level::Level> &castleLevels, int &numWorldsLeft);

    QWidget *parent;
    Plugin_Settings *pluginSettings;
    SMB1_Compliance_Generator_Interface *generatorPlugin;
    SMB1_Writer_Interface *writerPlugin;
    QString levelLocation;
};

#endif // LEVEL_GENERATOR_H
