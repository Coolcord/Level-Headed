#ifndef TAB_DIFFICULTY_H
#define TAB_DIFFICULTY_H

#include <QComboBox>
#include <QSpinBox>
#include "Tab_Interface.h"

class Tab_Difficulty : public Tab_Interface {
public:
    Tab_Difficulty(QWidget *parent, const QString &applicationLocation, SMB1_Writer_Interface *writerPlugin, Ui::Configure_Settings_Form *ui, Plugin_Settings *pluginSettings)
        : Tab_Interface(parent, applicationLocation, writerPlugin, ui, pluginSettings) { }
    void Load_Settings();
    void Load_Settings(Plugin_Settings *ps);
    void Save_Settings();
    void Save_Settings(Plugin_Settings *ps);

private:
    void Load_Combo_Box_Value(QComboBox *comboBox, int &value);
    void Load_Spin_Box_Value(QSpinBox *spinBox, int &value);
    void Make_Difficulty_Value_Sane(int &value);
};

#endif // TAB_DIFFICULTY_H
