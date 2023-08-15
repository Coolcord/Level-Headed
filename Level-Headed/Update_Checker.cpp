#include "Update_Checker.h"
#include "../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.h"
#include "Update_Dialog.h"
#include <QNetworkReply>
#include <QNetworkRequest>

//UPDATE CHECKER SETTINGS
const static QString GITHUB_DOWNLOAD_PAGE = "https://github.com/Coolcord/Level-Headed/releases";
const static QString GITHUB_UPDATE_URL =    "https://github.com/Coolcord/Level-Headed/releases/latest";
const bool FORCE_UPDATE_CHECK = false;

Update_Checker::Update_Checker(QWidget *parent, QApplication *application, Readable_Config_File *readableConfigFile, const QString &version) : QObject() {
    assert(parent); assert(application); assert(readableConfigFile);
    this->parent = parent;
    this->application = application;
    this->readableConfigFile = readableConfigFile;
    this->manager = new QNetworkAccessManager(nullptr);
    this->version = version;
}

Update_Checker::~Update_Checker() {
    delete this->manager;
    this->manager = nullptr;
}

void Update_Checker::Check_For_Updates() {
    //Run the update check
    if (this->version.contains("dev") && !FORCE_UPDATE_CHECK) return; //don't check for updates in development builds
    QString lastIgnoredUpdate;
    if (this->readableConfigFile->Get_Value("Last_Ignored_Update", lastIgnoredUpdate) &&
            this->Is_Version_Newer_Than_Current(lastIgnoredUpdate, this->version)) this->version = lastIgnoredUpdate;

    QNetworkRequest request = QNetworkRequest(QUrl(GITHUB_UPDATE_URL));
    this->manager->get(QNetworkRequest(QUrl(GITHUB_UPDATE_URL)));
    connect(this->manager, &QNetworkAccessManager::finished, this, &Update_Checker::Read_Latest_Version_Response);
}

bool Update_Checker::Get_Version_Numbers_From_String(const QString &version, int &significantVersion, int &majorVersion, int &minorVersion, int &patchVersion) {
    //Read the patch version first
    QStringList numbers = version.split('-');
    if (numbers.size() < 1 || numbers.size() > 2) return false;
    bool valid = false;
    patchVersion = 0;
    if (numbers.size() == 2) patchVersion = numbers.at(1).toInt(&valid);
    if (!valid) patchVersion = 0; //ignore anything that isn't a number

    //Read the other version numbers
    QStringList splitNumbers = numbers.at(0).split('.');
    if (splitNumbers.size() != 3) return false;
    significantVersion = splitNumbers.at(0).toInt(&valid);
    if (!valid) return false;
    majorVersion = splitNumbers.at(1).toInt(&valid);
    if (!valid) return false;
    minorVersion = splitNumbers.at(2).toInt(&valid);
    if (!valid) return false;

    return true;
}

bool Update_Checker::Is_Version_Newer_Than_Current(const QString &version, const QString &currentVersion) {
    int significantVersion = 0, majorVersion = 0, minorVersion = 0, patchVersion = 0;
    int currentSignificantVersion = 0, currentMajorVersion = 0, currentMinorVersion = 0, currentPatchVersion = 0;
    if (!this->Get_Version_Numbers_From_String(version, significantVersion, majorVersion, minorVersion, patchVersion)) return false;
    if (!this->Get_Version_Numbers_From_String(currentVersion, currentSignificantVersion, currentMajorVersion, currentMinorVersion, currentPatchVersion)) return false;
    if (significantVersion > currentSignificantVersion) return true;
    else if (significantVersion < currentSignificantVersion) return false;
    else if (majorVersion > currentMajorVersion) return true;
    else if (majorVersion < currentMajorVersion) return false;
    else if (minorVersion > currentMinorVersion) return true;
    else if (minorVersion < currentMinorVersion) return false;
    else if (patchVersion > currentPatchVersion) return true;
    else if (patchVersion < currentPatchVersion) return false;
    else return false;
}

void Update_Checker::Read_Latest_Version_Response(QNetworkReply *reply) {
    if (!reply || reply->error() != QNetworkReply::NoError) return;
    QString locationRedirect = reply->url().toString();
    if (!locationRedirect.isEmpty()) {
        QString newVersion = locationRedirect.split("/releases/tag/v").last();
        if (!newVersion.isEmpty() && (FORCE_UPDATE_CHECK || this->Is_Version_Newer_Than_Current(newVersion, this->version))) {
            emit Update_Available(newVersion, GITHUB_DOWNLOAD_PAGE);
        }
    }
    reply->deleteLater();
}
