#ifndef CONFIGURE_SETTINGS_FORM_H
#define CONFIGURE_SETTINGS_FORM_H

#include "../../SMB1/SMB1_Writer/SMB1_Writer_Interface.h"
#include "Difficulty_Level_Settings.h"
#include "Plugin_Settings.h"
#include <QAbstractButton>
#include <QComboBox>
#include <QDialog>

class Tab_Base_Game;
class Tab_Difficulty;
class Tab_Level_Generator;

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
    void on_btnOutputROMLocation_clicked();

    //Level Generator Tab
    void on_cbGenerateNewLevels_toggled(bool checked);
    void on_btnClearAllRandomLevelScripts_clicked();
    void on_cbRandomNumWorlds_clicked(bool checked);
    void on_sbNumLevelsPerWorld_valueChanged();
    void on_sbNumWorlds_valueChanged();
    void on_btnNewRandomSeed_clicked();

    //Difficulty Tab
    void on_cbInfiniteLives_toggled(bool checked);
    void on_comboDifficulty_currentIndexChanged(int index);

private:
    void Load_Settings();
    void Save_Settings();

    Ui::Configure_Settings_Form *ui;
    Tab_Base_Game *tabBaseGame;
    Tab_Difficulty *tabDifficulty;
    Tab_Level_Generator *tabLevelGenerator;
    Plugin_Settings *pluginSettings;
    SMB1_Writer_Interface *writerPlugin;
    QString applicationLocation;
    QWidget *parent;
};

#endif // CONFIGURE_SETTINGS_FORM_H
