#ifndef UPDATE_THREAD_H
#define UPDATE_THREAD_H

#include <QApplication>
#include <QThread>

class Readable_Config_File;

class Update_Thread : public QThread {
    Q_OBJECT

public:
    Update_Thread(QWidget *parent, QApplication *application, Readable_Config_File *readableConfigFile, const QString &version, const QString &gitLocation);
    ~Update_Thread() {}
    void run();

signals:
    void Update_Available(const QString &newVersion, const QString &updatePage);

private:
    QWidget *parent;
    QApplication *application;
    Readable_Config_File *readableConfigFile;
    QString version;
    QString newVersion;
    QString gitLocation;
};

#endif // UPDATE_THREAD_H
