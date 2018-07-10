#ifndef TAB_LEVEL_GENERATOR_H
#define TAB_LEVEL_GENERATOR_H

#include "Tab_Interface.h"
#include <QComboBox>

class Tab_Level_Generator : public Tab_Interface {
public:
    Tab_Level_Generator(QWidget *parent, const QString &applicationLocation, SMB1_Writer_Interface *writerPlugin, Ui::Configure_Settings_Form *ui, Plugin_Settings *pluginSettings);
    void Load_Settings();
    void Save_Settings();

    void Clear_All_Random_Level_Scripts();
    void Clear_Chance_ComboBoxes();
    void Populate_Chance_ComboBoxes();
    void Populate_Chance_ComboBox(QComboBox *comboBox);
    void Populate_Level_Scripts_ComboBox();
    bool At_Least_One_Very_Common_Selected();
    void Enable_New_Level_Options(bool enable);
    void Enable_Random_Number_Of_Worlds(bool enable);
    void Update_Worlds();

private:
    int numWorlds;
    int numLevelsPerWorld;
    bool randomNumWorlds;
    QString levelLocation;
};

#endif // TAB_LEVEL_GENERATOR_H
