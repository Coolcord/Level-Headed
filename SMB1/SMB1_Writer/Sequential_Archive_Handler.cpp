#include "Sequential_Archive_Handler.h"
#include "../../Level-Headed/Common_Strings.h"
#include "SMB1_Writer_Strings.h"
#include <assert.h>

Sequential_Archive_Handler::Sequential_Archive_Handler(const QString &applicationLocation) {
    this->file = NULL;
    this->hexagonPlugin = NULL;
    this->sequentialArchivePlugin = NULL;
    this->hexagonLoader = NULL;
    this->sequentialArchiveLoader = NULL;
    this->graphicsPackStrings = QStringList();
    this->musicPackStrings = QStringList();
    this->pluginLocation = applicationLocation + "/" + Common_Strings::STRING_PLUGINS + "/";
    this->graphicsPacksArchiveLocation = applicationLocation+"/"+Common_Strings::STRING_DATA+"/"+Common_Strings::STRING_GAME_NAME+"/Graphics.sa";
    this->musicPacksArchiveLocation = applicationLocation+"/"+Common_Strings::STRING_DATA+"/"+Common_Strings::STRING_GAME_NAME+"/Music.sa";
}

Sequential_Archive_Handler::~Sequential_Archive_Handler() {
    if (this->hexagonLoader) this->hexagonLoader->unload();
    if (this->sequentialArchiveLoader) this->sequentialArchiveLoader->unload();
    this->hexagonLoader = NULL;
    this->sequentialArchiveLoader = NULL;
    this->hexagonPlugin = NULL;
    this->sequentialArchivePlugin = NULL;
}

void Sequential_Archive_Handler::Set_File(QFile *file) {
    this->file = file;
}

bool Sequential_Archive_Handler::Apply_Graphics_Pack_At_Index(int index) {
    if (this->graphicsPackStrings.isEmpty()) this->Get_Graphics_Packs();
    assert(index < this->graphicsPackStrings.size());
    if (!this->file) return false;
    if (!this->Load_Plugins_If_Necessary()) return false;
    if (!this->sequentialArchivePlugin->Open(this->graphicsPacksArchiveLocation)) return false;
    QByteArray patchBytes = this->sequentialArchivePlugin->Read_File("/"+this->graphicsPackStrings.at(index));
    this->sequentialArchivePlugin->Close();
    if (patchBytes.isEmpty()) return false;
    int lineNum = 0;
    return this->hexagonPlugin->Apply_Hexagon_Patch(patchBytes, this->file, false, lineNum) == Hexagon_Error_Codes::OK;
}
bool Sequential_Archive_Handler::Apply_Music_Pack_At_Index(int index) {
    if (this->musicPackStrings.isEmpty()) this->Get_Music_Packs();
    assert(index < this->musicPackStrings.size());
    if (!this->file) return false;
    if (!this->Load_Plugins_If_Necessary()) return false;
    if (!this->sequentialArchivePlugin->Open(this->musicPacksArchiveLocation)) return false;
    QByteArray patchBytes = this->sequentialArchivePlugin->Read_File("/"+this->musicPackStrings.at(index));
    this->sequentialArchivePlugin->Close();
    if (patchBytes.isEmpty()) return false;
    int lineNum = 0;
    return this->hexagonPlugin->Apply_Hexagon_Patch(patchBytes, this->file, false, lineNum) == Hexagon_Error_Codes::OK;
}

QStringList Sequential_Archive_Handler::Get_Graphics_Packs() {
    if (!this->graphicsPackStrings.isEmpty()) return this->graphicsPackStrings;
    if (!this->Load_Plugins_If_Necessary()) return this->graphicsPackStrings;
    if (!this->sequentialArchivePlugin->Open(this->graphicsPacksArchiveLocation)) return this->graphicsPackStrings;
    this->graphicsPackStrings = this->sequentialArchivePlugin->Get_Files();
    this->sequentialArchivePlugin->Close();
    return this->graphicsPackStrings;
}

QString Sequential_Archive_Handler::Get_Graphics_Pack_At_Index(int index) {
    if (this->graphicsPackStrings.isEmpty()) this->Get_Graphics_Packs();
    assert(index < this->graphicsPackStrings.size());
    return this->graphicsPackStrings.at(index);
}

QStringList Sequential_Archive_Handler::Get_Music_Packs() {
    if (!this->musicPackStrings.isEmpty()) return this->musicPackStrings;
    if (!this->Load_Plugins_If_Necessary()) return this->musicPackStrings;
    if (!this->sequentialArchivePlugin->Open(this->musicPacksArchiveLocation)) return this->musicPackStrings;
    this->musicPackStrings = this->sequentialArchivePlugin->Get_Files();
    this->sequentialArchivePlugin->Close();
    return this->musicPackStrings;
}

QString Sequential_Archive_Handler::Get_Music_Pack_At_Index(int index) {
    if (this->musicPackStrings.isEmpty()) this->Get_Music_Packs();
    assert(index < this->musicPackStrings.size());
    return this->musicPackStrings.at(index);
}

int Sequential_Archive_Handler::Get_Number_Of_Graphics_Packs() {
    if (this->graphicsPackStrings.isEmpty()) this->Get_Graphics_Packs();
    return this->graphicsPackStrings.size();
}

int Sequential_Archive_Handler::Get_Number_Of_Music_Packs() {
    if (this->musicPackStrings.isEmpty()) this->Get_Music_Packs();
    return this->musicPackStrings.size();
}

bool Sequential_Archive_Handler::Load_Plugins_If_Necessary() {
    bool success = true;
    if (!this->hexagonPlugin && !this->Load_Hexagon_Plugin()) success = false;
    if (!this->sequentialArchivePlugin && !this->Load_Sequential_Archive_Plugin()) success = false;
    return success;
}

bool Sequential_Archive_Handler::Load_Hexagon_Plugin() {
    QString hexagonPluginLocation = this->pluginLocation + "Hexagon" + Common_Strings::STRING_PLUGIN_EXTENSION;
    if (!QFile(hexagonPluginLocation).exists()) return false;

    //Load the Hexagon Plugin
    this->hexagonLoader = new QPluginLoader(hexagonPluginLocation);
    QObject *validPlugin = this->hexagonLoader->instance();
    if (!validPlugin) return false;
    this->hexagonPlugin = qobject_cast<Hexagon_Interface*>(validPlugin);
    return this->hexagonPlugin;
}

bool Sequential_Archive_Handler::Load_Sequential_Archive_Plugin() {
    QString sequentialArchivePluginLocation = this->pluginLocation + "Sequential_Archive" + Common_Strings::STRING_PLUGIN_EXTENSION;
    if (!QFile(sequentialArchivePluginLocation).exists()) return false;

    //Load the Sequential Archive Plugin
    this->sequentialArchiveLoader = new QPluginLoader(sequentialArchivePluginLocation);
    QObject *validPlugin = this->sequentialArchiveLoader->instance();
    if (!validPlugin) return false;
    this->sequentialArchivePlugin = qobject_cast<Sequential_Archive_Interface*>(validPlugin);
    return this->sequentialArchivePlugin;
}
