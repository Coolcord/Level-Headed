#include "Plugin_Handler.h"
#include "Common_Strings.h"
#include <QString>
#include <QStringList>
#include <QMap>
#include <QDir>
#include <QApplication>
#include <QMessageBox>
#include <assert.h>

Plugin_Handler::Plugin_Handler(Main_Window *window) {
    assert(window);
    this->window = window;
    this->widget = new QWidget();
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

QStringList Plugin_Handler::Get_Writer_Plugins() {
    //Get all of the available plugins
    QStringList writerList = this->Get_Plugins_From_Folder(Common_Strings::WRITERS);
    if (writerList.empty()) return QStringList();
    QStringList generatorList = this->Get_Plugins_From_Folder(Common_Strings::GENERATORS);
    if (generatorList.empty()) return QStringList();
    QStringList interpreterList = this->Get_Plugins_From_Folder(Common_Strings::INTERPRETERS);
    if (interpreterList.empty()) return QStringList();

    //Get all of the possible Generator and Writer Plugins
    QMap<QString, bool> generators;
    QMap<QString, bool> writers;
    foreach (QString generator, generatorList) {
        generators.insert(generator.split(Common_Strings::GENERATOR_SPLIT).first(), false);
    }
    foreach (QString writer, writerList) {
        writers.insert(writer.split(Common_Strings::WRITER_SPLIT).first(), false);
    }

    //Determine which writer plugins are valid
    QStringList validPlugins;
    foreach (QString interpreter, interpreterList) {
        interpreter.chop(interpreter.size()-interpreter.lastIndexOf('.'));
        QStringList names = interpreter.split(Common_Strings::INTERPRETER_SPLIT);
        QString generator = names.at(0);
        QString writer = names.at(1);
        if (generators.contains(generator) && writers.contains(writer)
            && !writers[writer]) {
            validPlugins.append(writer.replace("_", " "));
            writers[writer] = true; //writer added
        }
    }

    return validPlugins;
}

QStringList Plugin_Handler::Get_Generator_Plugins(const QString &writerPlugin) {
    //Get all of the available plugins
    QStringList generatorList = this->Get_Plugins_From_Folder(Common_Strings::GENERATORS);
    if (generatorList.empty()) return QStringList();
    QStringList interpreterList = this->Get_Plugins_From_Folder(Common_Strings::INTERPRETERS);
    if (interpreterList.empty()) return QStringList();

    //Get all of the possible Generator Plugins
    QMap<QString, bool> generators;
    foreach (QString generator, generatorList) {
        generators.insert(generator.split(Common_Strings::GENERATOR_SPLIT).first(), false);
    }

    //Determine which writer plugins are valid
    QStringList validPlugins;
    foreach (QString interpreter, interpreterList) {
        interpreter.chop(interpreter.size()-interpreter.lastIndexOf('.'));
        QStringList names = interpreter.split(Common_Strings::INTERPRETER_SPLIT);
        QString generator = names.at(0);
        QString writer = names.at(1);
        if (writerPlugin == writer && generators.contains(generator)
            && !generators[generator]) {
            validPlugins.append(generator.replace("_", " "));
            generators[generator] = true; //generator added
        }
    }

    return validPlugins;
}

QString Plugin_Handler::Get_Interpreter_Name(QString writerPlugin, QString generatorPlugin){
    if (writerPlugin == NULL || generatorPlugin == NULL || writerPlugin.isEmpty() || generatorPlugin.isEmpty()) {
        return QString(); //invalid arguments
    }
    return generatorPlugin.replace(" ", "_") + Common_Strings::INTERPRETER_SPLIT
            + writerPlugin.replace(" ", "_") + Common_Strings::PLUGIN_EXTENSION;
}

void Plugin_Handler::Show_Read_Write_Error() {
    QMessageBox::critical(this->widget, Common_Strings::LEVEL_HEADED, Common_Strings::LEVEL_HEADED +
                         " does not have proper read/write permissions. Cannot continue!",
                          Common_Strings::OK);
}

QStringList Plugin_Handler::Get_Plugins_From_Folder(const QString &folder) {
    //Only Windows uses .dll files
    QStringList filters;
    filters.append("*" + Common_Strings::PLUGIN_EXTENSION);

    //Get all of the available plugins in the directory
    QDir dir(QApplication::applicationDirPath());
    if (!dir.cd(Common_Strings::PLUGINS) || !dir.cd(folder)) {
        this->Show_Read_Write_Error();
        return QStringList();
    }
    QStringList pluginList = dir.entryList(filters, QDir::Files, QDir::Name);
    if (pluginList.empty()) {
        QString pluginType = folder.toLower();
        pluginType.chop(1);
        QMessageBox::critical(this->widget, Common_Strings::LEVEL_HEADED, Common_Strings::LEVEL_HEADED
                              + " could not find any " + pluginType + " plugins! Place compatible plugins in the "
                              + folder + " folder.",
                              Common_Strings::OK);
        return QStringList(); //at least one plugin is necessary
    }
    if (!dir.cdUp()) {
        this->Show_Read_Write_Error();
        return QStringList();
    }
    return pluginList;
}


