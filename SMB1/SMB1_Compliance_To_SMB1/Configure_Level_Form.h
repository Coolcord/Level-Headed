#ifndef CONFIGURE_LEVEL_FORM_H
#define CONFIGURE_LEVEL_FORM_H

#include "Difficulty_Level_Settings.h"
#include "Plugin_Settings.h"
#include "Difficulty_Level_Form.h"
#include <QAbstractButton>
#include <QComboBox>
#include <QDialog>
#include <QString>

struct Difficulty_Level_Settings;

namespace Ui {
class Configure_Level_Form;
}

class Configure_Level_Form : public QDialog
{
    Q_OBJECT

public:
    Configure_Level_Form(QWidget *parent, Plugin_Settings *pluginSettings, const QString &location);
    ~Configure_Level_Form();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_cbGenerateNewLevels_toggled(bool checked);
    void on_btnClearAllRandomLevelScripts_clicked();
    void on_cbRandomNumWorlds_clicked(bool checked);
    void on_sbNumLevelsPerWorld_valueChanged(int numLevelsPerWorld);
    void on_sbNumWorlds_valueChanged(int numWorlds);
    void on_btnNewRandomSeed_clicked();
    void on_comboDifficulty_activated(int index);

private:
    void Clear_Chance_ComboBoxes();
    void Populate_Chance_ComboBoxes();
    void Populate_Chance_ComboBox(QComboBox *comboBox);
    void Populate_Level_Scripts_ComboBox();
    bool At_Least_One_Very_Common_Selected();
    void Enable_New_Level_Options(bool enable);
    void Save_Settings();

    Ui::Configure_Level_Form *ui;
    Plugin_Settings *pluginSettings;
    QString levelLocation;
    QString applicationLocation;
    Difficulty_Level_Settings difficultySettings;
    int numWorlds;
    int numLevelsPerWorld;
    bool randomNumWorlds;
    int difficultyComboIndex;
    bool loading;
    QWidget *parent;
};

#endif // CONFIGURE_LEVEL_FORM_H
