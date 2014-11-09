#ifndef CONFIGURE_BASE_FORM_H
#define CONFIGURE_BASE_FORM_H

#include "../../SMB1/SMB1_Writer/SMB1_Writer_Interface.h"
#include "Plugin_Settings.h"
#include <QAbstractButton>
#include <QDialog>

namespace Ui {
class Configure_Base_Form;
}

class Configure_Base_Form : public QDialog
{
    Q_OBJECT

public:
    Configure_Base_Form(QWidget *parent, Plugin_Settings *pluginSettings, SMB1_Writer_Interface *writerPlugin);
    ~Configure_Base_Form();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_cbDuplicateLevels_toggled(bool checked);
    void on_btnInstallNewROM_clicked();

private:
    void Populate_Installed_ROMs();

    Ui::Configure_Base_Form *ui;
    Plugin_Settings *pluginSettings;
    SMB1_Writer_Interface *writerPlugin;
};

#endif // CONFIGURE_BASE_FORM_H
