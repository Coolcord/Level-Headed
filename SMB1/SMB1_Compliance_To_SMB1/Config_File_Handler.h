#ifndef CONFIG_FILE_HANDLER_H
#define CONFIG_FILE_HANDLER_H

#include <QString>
#include <QWidget>
#include "Plugin_Settings.h"

class Config_File_Handler {
public:
    Config_File_Handler(QWidget *parent, const QString &applicationLocation);
    ~Config_File_Handler() {}
    bool Save_Plugin_Settings(Plugin_Settings *ps, const QString &configFileLocation, bool internalConfig);
    bool Load_Plugin_Settings(Plugin_Settings *ps, const QString &configFileLocation, bool internalConfig);
    bool Load_Plugin_Settings(Plugin_Settings *ps, const QString &configFileLocation, bool internalConfig, bool &messageShown);

private:
    QString Get_Checksum(const QString &fileLocation);

    QWidget *parent;
    QString applicationLocation;
};

#endif // CONFIG_FILE_HANDLER_H
