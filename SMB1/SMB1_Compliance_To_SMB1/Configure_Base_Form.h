#ifndef CONFIGURE_WRITER_FORM_H
#define CONFIGURE_WRITER_FORM_H

#include "../../SMB1/SMB1_Writer/SMB1_Writer_Interface.h"
#include "Base_Game_Settings.h"
#include <QAbstractButton>
#include <QDialog>

namespace Ui {
class Configure_Writer_Form;
}

class Configure_Writer_Form : public QDialog
{
    Q_OBJECT

public:
    Configure_Writer_Form(QWidget *parent, Base_Game_Settings *baseGameSettings, SMB1_Writer_Interface *writerPlugin);
    ~Configure_Writer_Form();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_sbNumWorlds_valueChanged(int arg1);
    void on_cbDuplicateLevels_toggled(bool checked);
    void on_comboBaseROM_currentIndexChanged(const QString &arg1);

    void on_btnInstallNewROM_clicked();

private:
    void Populate_Installed_ROMs();

    Ui::Configure_Writer_Form *ui;
    Base_Game_Settings *baseGameSettings;
    SMB1_Writer_Interface *writerPlugin;
};

#endif // CONFIGURE_WRITER_FORM_H
