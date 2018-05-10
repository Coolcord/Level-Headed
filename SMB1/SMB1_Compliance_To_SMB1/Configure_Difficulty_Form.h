#ifndef CONFIGURE_DIFFICULTY_FORM_H
#define CONFIGURE_DIFFICULTY_FORM_H

#include "Difficulty_Settings.h"
#include "Plugin_Settings.h"
#include <QAbstractButton>
#include <QDialog>
#include <QString>

namespace Ui {
class Configure_Difficulty_Form;
}

class Configure_Difficulty_Form : public QDialog
{
    Q_OBJECT

public:
    Configure_Difficulty_Form(QWidget *parent, Difficulty_Settings *difficultySettings, const QString &location);
    ~Configure_Difficulty_Form();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    void Load_Settings();
    void Save_Settings();

    Ui::Configure_Difficulty_Form *ui;
    Difficulty_Settings *difficultySettings;
    QString applicationLocation;
    QWidget *parent;
};

#endif // CONFIGURE_DIFFICULTY_FORM_H
