#ifndef UPDATE_CHECKER_H
#define UPDATE_CHECKER_H

#include <QApplication>
#include <QNetworkAccessManager>

class Readable_Config_File;

class Update_Checker : public QObject {
    Q_OBJECT

public:
    Update_Checker(QWidget *parent, QApplication *application, Readable_Config_File *readableConfigFile, const QString &version);
    ~Update_Checker();
    void Check_For_Updates();

signals:
    void Update_Available(const QString &newVersion, const QString &updatePage);

private:
    bool Get_Version_Numbers_From_String(const QString &version, int &significantVersion, int &majorVersion, int &minorVersion, int &patchVersion);
    bool Is_Version_Newer_Than_Current(const QString &version, const QString &currentVersion);
    void Read_Latest_Version_Response();

    QWidget *parent;
    QApplication *application;
    QNetworkAccessManager *manager;
    Readable_Config_File *readableConfigFile;
    QString version;
};

#endif // UPDATE_CHECKER_H
