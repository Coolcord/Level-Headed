#ifndef TAB_BASE_GAME_H
#define TAB_BASE_GAME_H

#include <QComboBox>
#include <QSet>
#include "Tab_Interface.h"

class Tab_Base_Game : public Tab_Interface {
public:
    Tab_Base_Game(QWidget *parent, const QString &applicationLocation, SMB1_Writer_Interface *writerPlugin, Ui::Configure_Settings_Form *ui, Plugin_Settings *pluginSettings);
    void Load_Settings();
    void Load_Settings(Plugin_Settings *ps);
    void Save_Settings();
    void Save_Settings(Plugin_Settings *ps);

    void Install_New_ROM();
    bool Is_Partial_Support_Mode_Enabled();
    void Enable_Partial_Support_Mode(bool enabled);
    void Use_Original_Settings();
    void Use_Basic_Settings();
    void Use_Random_Settings();

private:
    void Populate_Installed_ROMs();
    void Populate_Graphics_Packs();
    void Populate_Mario_Sprites();
    void Populate_Music_Packs();
    void Load_Combo_Box_Value(QComboBox *comboBox, int &value);

    bool partialSupportMode;
};

#endif // TAB_BASE_GAME_H
