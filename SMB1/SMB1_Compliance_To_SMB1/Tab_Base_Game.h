#ifndef TAB_BASE_GAME_H
#define TAB_BASE_GAME_H

#include <QSet>
#include "Tab_Interface.h"

class Tab_Base_Game : public Tab_Interface {
public:
    Tab_Base_Game(QWidget *parent, const QString &applicationLocation, SMB1_Writer_Interface *writerPlugin, Ui::Configure_Settings_Form *ui, Plugin_Settings *pluginSettings);
    void Load_Settings();
    void Save_Settings();

    void Install_New_ROM();
    bool Is_Partial_Support_Mode_Enabled();
    void Enable_Partial_Support_Mode(bool enabled);
    void Use_Original_Settings();
    void Use_Random_Settings();

private:
    void Populate_Installed_ROMs();
    void Populate_Graphics_Packs();
    void Populate_Music_Packs();

    bool partialSupportMode;
};

#endif // TAB_BASE_GAME_H
