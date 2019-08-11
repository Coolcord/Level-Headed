#include "Update_Thread.h"
#include "../../C_Common_Code/Qt/Git_Update_Checker/Git_Update_Checker.h"
#include "../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.h"
#include "Update_Dialog.h"

Update_Thread::Update_Thread(QWidget *parent, QApplication *application, Readable_Config_File *readableConfigFile, const QString &version, const QString &gitLocation) : QThread(parent) {
    assert(parent); assert(application); assert(readableConfigFile);
    this->parent = parent;
    this->application = application;
    this->readableConfigFile = readableConfigFile;
    this->version = version;
    this->newVersion = QString();
    this->gitLocation = gitLocation;
}

void Update_Thread::run() {
    //UPDATE CHECKER SETTINGS
    const QString REMOTE_GIT_SERVER = "https://github.com/Coolcord/Level-Headed.git";
    const QString UPDATE_PAGE = "https://github.com/Coolcord/Level-Headed/releases";
    const bool FORCE_UPDATE_CHECK = false;

    //Run the update check
    QString lastIgnoredUpdate = QString();
    Git_Update_Checker gitUpdateChecker;
    if (this->readableConfigFile->Get_Value("Last_Ignored_Update", lastIgnoredUpdate) &&
            gitUpdateChecker.Is_Version_Newer_Than_Current(lastIgnoredUpdate, this->version)) this->version = lastIgnoredUpdate;
    if (FORCE_UPDATE_CHECK) this->version = "0.0.0";
    QString newVersion = gitUpdateChecker.Check_For_Updates(this->version, REMOTE_GIT_SERVER, this->gitLocation);
    if (!newVersion.isEmpty()) emit Update_Available(newVersion, UPDATE_PAGE);
}
