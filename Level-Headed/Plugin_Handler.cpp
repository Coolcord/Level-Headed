#include "Plugin_Handler.h"
#include "Common_Strings.h"
#include <QString>
#include <QDir>
#include <QApplication>
#include <QMessageBox>
#include <assert.h>

Plugin_Handler::Plugin_Handler()
{
    this->parent = new QWidget();
}

bool Plugin_Handler::Create_Directories() {
    //Make sure the root folder exists
    QDir dir(QApplication::applicationDirPath());
    if (!dir.exists()) {
        this->Show_Read_Write_Error();
        return false;
    }

    //Make the plugins folder
    QString pluginPath = QApplication::applicationDirPath() + "/" + Common_Strings::PLUGINS;
    if (!dir.exists(pluginPath)) {
        if (!dir.mkdir(pluginPath)) {
            this->Show_Read_Write_Error();
            return false;
        }
    }
    if (!dir.cd(pluginPath)) {
        this->Show_Read_Write_Error();
        return false;
    }

    //Make the subdirectory folders
    if (!dir.exists(pluginPath + "/" + Common_Strings::GENERATORS)) {
        if (!dir.mkdir(pluginPath + "/" + Common_Strings::GENERATORS)) {
            this->Show_Read_Write_Error();
            return false;
        }
    }
    if (!dir.exists(pluginPath + "/" + Common_Strings::INTERPRETERS)) {
        if (!dir.mkdir(pluginPath + "/" + Common_Strings::INTERPRETERS)) {
            this->Show_Read_Write_Error();
            return false;
        }
    }
    if (!dir.exists(pluginPath + "/" + Common_Strings::WRITERS)) {
        if (!dir.mkdir(pluginPath + "/" + Common_Strings::WRITERS)) {
            this->Show_Read_Write_Error();
            return false;
        }
    }

    return true;
}

void Plugin_Handler::Show_Read_Write_Error() {

    QMessageBox::critical(this->parent, Common_Strings::LEVEL_HEADED, Common_Strings::LEVEL_HEADED +
                         " does not have proper read/write permissions. Cannot continue!",
                          Common_Strings::OK);
}

bool Plugin_Handler::Get_Plugins() {
    //QDir dir(QApplication::applicationDirPath());
    //if (!dir.cd(Common_Strings::GENERATORS)) return false;

    return false;
}
