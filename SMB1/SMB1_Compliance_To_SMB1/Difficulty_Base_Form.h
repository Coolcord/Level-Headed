#ifndef DIFFICULTY_BASE_FORM_H
#define DIFFICULTY_BASE_FORM_H

#include <QAbstractButton>
#include <QDialog>

namespace Ui {
class Difficulty_Base_Form;
}

class Difficulty_Base_Form : public QDialog
{
    Q_OBJECT

public:
    explicit Difficulty_Base_Form(QWidget *parent = 0);
    ~Difficulty_Base_Form();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Difficulty_Base_Form *ui;
};

#endif // DIFFICULTY_BASE_FORM_H
