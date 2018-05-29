#ifndef DIFFICULTY_LEVEL_FORM_H
#define DIFFICULTY_LEVEL_FORM_H

#include "Difficulty_Level_Settings.h"
#include "Plugin_Settings.h"
#include <QAbstractButton>
#include <QDialog>
#include <QString>

namespace Ui {
class Difficulty_Level_Form;
}

class Difficulty_Level_Form : public QDialog
{
    Q_OBJECT

public:
    Difficulty_Level_Form(QWidget *parent, Difficulty_Level_Settings *difficultySettings, const QString &location);
    ~Difficulty_Level_Form();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    void Load_Settings();
    void Save_Settings();

    Ui::Difficulty_Level_Form *ui;
    Difficulty_Level_Settings *difficultySettings;
    QString applicationLocation;
    QWidget *parent;
};

#endif // DIFFICULTY_LEVEL_FORM_H
