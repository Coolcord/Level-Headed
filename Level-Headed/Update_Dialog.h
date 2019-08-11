#ifndef UPDATE_DIALOG_H
#define UPDATE_DIALOG_H

#include <QAbstractButton>
#include <QDialog>

class Readable_Config_File;

namespace Ui {
class Update_Dialog;
}

class Update_Dialog : public QDialog {
    Q_OBJECT

public:
    Update_Dialog(QWidget *parent, Readable_Config_File *readableConfigFile, const QString &newVersion, const QString &updatePage);
    ~Update_Dialog();

private slots:
    void on_Update_Dialog_accepted();
    void on_Update_Dialog_rejected();

private:
    Ui::Update_Dialog *ui;
    Readable_Config_File *readableConfigFile;
    QString updatePage;
    QString newVersion;
};

#endif // UPDATE_DIALOG_H
