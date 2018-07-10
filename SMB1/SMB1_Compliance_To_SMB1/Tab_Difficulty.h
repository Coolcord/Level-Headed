#ifndef TAB_DIFFICULTY_H
#define TAB_DIFFICULTY_H

#include "Tab_Interface.h"

class Tab_Difficulty : public Tab_Interface {
public:
    Tab_Difficulty(QWidget *parent, const QString &applicationLocation, SMB1_Writer_Interface *writerPlugin, Ui::Configure_Settings_Form *ui, Plugin_Settings *pluginSettings)
        : Tab_Interface(parent, applicationLocation, writerPlugin, ui, pluginSettings) { }
    void Load_Settings();
    void Save_Settings();
};

#endif // TAB_DIFFICULTY_H
