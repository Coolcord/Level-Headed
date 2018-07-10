#ifndef TAB_INTERFACE_H
#define TAB_INTERFACE_H

#include "../../SMB1/SMB1_Writer/SMB1_Writer_Interface.h"
#include "Plugin_Settings.h"
#include <assert.h>
#include <QString>
#include <QWidget>

namespace Ui {
class Configure_Settings_Form;
}

class Tab_Interface {
public:
    Tab_Interface(QWidget *parent, const QString &applicationLocation, SMB1_Writer_Interface *writerPlugin, Ui::Configure_Settings_Form *ui, Plugin_Settings *pluginSettings) {
        assert(parent); assert(writerPlugin); assert(ui); assert(pluginSettings);
        this->parent = parent; this->applicationLocation = applicationLocation; this->writerPlugin = writerPlugin; this->ui = ui; this->pluginSettings = pluginSettings;
    }
    virtual ~Tab_Interface() {}
    virtual void Load_Settings()=0;
    virtual void Save_Settings()=0;

protected:
    Ui::Configure_Settings_Form *ui;
    Plugin_Settings *pluginSettings;
    QString applicationLocation;
    QWidget *parent;
    SMB1_Writer_Interface *writerPlugin;
};

#endif // TAB_INTERFACE_H
