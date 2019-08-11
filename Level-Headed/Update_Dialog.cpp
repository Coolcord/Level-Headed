#include "Update_Dialog.h"
#include "ui_Update_Dialog.h"
#include "../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.h"
#include <QDebug>
#include <QDesktopServices>

Update_Dialog::Update_Dialog(QWidget *parent, Readable_Config_File *readableConfigFile, const QString &newVersion, const QString &updatePage) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::Update_Dialog)
{
    assert(readableConfigFile);
    this->readableConfigFile = readableConfigFile;
    this->newVersion = newVersion;
    ui->setupUi(this);
    this->ui->tvUpdateMessage->setText("<p align=\"center\">New Version: v"+newVersion+
                                       "</p>\n\n<p align=\"center\">Would you like to download it now?</p>");
    int numberOfLines = 3; //necessary for determining the QTextBrowser height
    int height = QFontMetrics(this->ui->tvUpdateMessage->font()).height()*numberOfLines;
    this->ui->tvUpdateMessage->setMinimumHeight(height);
    this->ui->tvUpdateMessage->setMaximumHeight(height);
    this->updatePage = updatePage;
}

Update_Dialog::~Update_Dialog() {
    delete ui;
}

void Update_Dialog::on_Update_Dialog_accepted() {
    QDesktopServices::openUrl(QUrl(this->updatePage));
}

void Update_Dialog::on_Update_Dialog_rejected() {
    if (this->ui->cbDontAskAgain->isChecked()) this->readableConfigFile->Set_Value("Last_Ignored_Update", this->newVersion);
}
