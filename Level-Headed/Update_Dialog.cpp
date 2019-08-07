#include "Update_Dialog.h"
#include "ui_Update_Dialog.h"
#include <QDebug>
#include <QDesktopServices>

Update_Dialog::Update_Dialog(QWidget *parent, const QString &version, const QString &updatePage) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::Update_Dialog)
{
    ui->setupUi(this);
    this->ui->teUpdateMessage->setText("<p align=\"center\">New Version: v"+version+
                                       "</p>\n\n<p align=\"center\">Would you like to download it now?</p>");
    int numberOfLines = 3; //necessary for determining the QTextBrowser height
    int height = QFontMetrics(this->ui->teUpdateMessage->font()).height()*numberOfLines;
    this->ui->teUpdateMessage->setMinimumHeight(height);
    this->ui->teUpdateMessage->setMaximumHeight(height);
    this->updatePage = updatePage;
}

Update_Dialog::~Update_Dialog() {
    delete ui;
}

void Update_Dialog::on_Update_Dialog_accepted() {
    QDesktopServices::openUrl(QUrl(this->updatePage));
}

void Update_Dialog::on_Update_Dialog_rejected() {
    qDebug() << "rejected";
}
