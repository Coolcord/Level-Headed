#ifndef CONFIGURE_SETTINGS_FORM_H
#define CONFIGURE_SETTINGS_FORM_H

#include "../../SMB1/SMB1_Writer/SMB1_Writer_Interface.h"
#include "Difficulty_Level_Settings.h"
#include "Plugin_Settings.h"
#include "Difficulty_Level_Form.h"
#include <QAbstractButton>
#include <QComboBox>
#include <QDialog>

namespace Ui {
class Configure_Settings_Form;
}

class Configure_Settings_Form : public QDialog {
    Q_OBJECT

public:
    Configure_Settings_Form(QWidget *parent, Plugin_Settings *pluginSettings, SMB1_Writer_Interface *writerPlugin, const QString &location);
    ~Configure_Settings_Form();

private slots:
    //Base Game Tab
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_btnInstallNewROM_clicked();
    void on_cbInfiniteLives_toggled(bool checked);
    void on_btnOutputROMLocation_clicked();

    //Level Generator Tab
    void on_cbGenerateNewLevels_toggled(bool checked);
    void on_btnClearAllRandomLevelScripts_clicked();
    void on_cbRandomNumWorlds_clicked(bool checked);
    void on_sbNumLevelsPerWorld_valueChanged(int numLevelsPerWorld);
    void on_sbNumWorlds_valueChanged(int numWorlds);
    void on_btnNewRandomSeed_clicked();
    void on_comboDifficulty_activated(int index);

private:
    //Base Game Tab
    void Populate_Installed_ROMs();
    void Populate_Graphics_Packs();
    void Populate_Music_Packs();
    void Load_Settings();
    void Save_Settings();

    //Level Generator Tab
    void Clear_Chance_ComboBoxes();
    void Populate_Chance_ComboBoxes();
    void Populate_Chance_ComboBox(QComboBox *comboBox);
    void Populate_Level_Scripts_ComboBox();
    bool At_Least_One_Very_Common_Selected();
    void Enable_New_Level_Options(bool enable);

    Ui::Configure_Settings_Form *ui;
    Plugin_Settings *pluginSettings;
    SMB1_Writer_Interface *writerPlugin;
    QString applicationLocation;
    QWidget *parent;
    QString levelLocation;
    Difficulty_Level_Settings difficultySettings;
    int numWorlds;
    int numLevelsPerWorld;
    bool randomNumWorlds;
    int difficultyComboIndex;
    bool loading;
};

#endif // CONFIGURE_SETTINGS_FORM_H
