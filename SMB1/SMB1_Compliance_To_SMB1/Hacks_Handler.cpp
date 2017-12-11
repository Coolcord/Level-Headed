#include "Hacks_Handler.h"
#include "../SMB1_Writer/SMB1_Writer_Interface.h"
#include "../../Common_Files/Random.h"
#include <assert.h>

Hacks_Handler::Hacks_Handler(SMB1_Writer_Interface *writerPlugin, Plugin_Settings *pluginSettings) {
    assert(writerPlugin);
    assert(pluginSettings);
    this->writerPlugin = writerPlugin;
    this->pluginSettings = pluginSettings;
}

Hacks_Handler::~Hacks_Handler() {
    this->writerPlugin = NULL;
    this->pluginSettings = NULL;
}

bool Hacks_Handler::Write_Hacks() {
    if (!this->writerPlugin->Write_Watermark()) return false;
    if (!this->Handle_Lives()) return false;
    if (!this->Handle_God_Mode()) return false;
    if (!this->Handle_Play_As_Luigi()) return false;
    if (!this->Handle_Lakitu_Throw_Arc()) return false;
    return this->writerPlugin->Fast_Enemies(this->pluginSettings->enemySpeed);
}

bool Hacks_Handler::Handle_Lives() {
    if (this->pluginSettings->infiniteLives) {
        return this->writerPlugin->Set_Starting_Lives(9);
        return this->writerPlugin->Infinite_Lives();
    } else {
        return this->writerPlugin->Set_Starting_Lives(this->pluginSettings->numLives);
    }
}

bool Hacks_Handler::Handle_God_Mode() {
    if (this->pluginSettings->godMode) {
        return this->writerPlugin->Enable_God_Mode();
    }
    return true;
}

bool Hacks_Handler::Handle_Play_As_Luigi() {
    if (this->Get_Bool_From_CheckState(this->pluginSettings->playAsLuigi)) {
        return this->writerPlugin->Play_As_Luigi();
    }
    return true;
}

bool Hacks_Handler::Handle_Lakitu_Throw_Arc() {
    if (this->Get_Bool_From_CheckState(this->pluginSettings->lakituThrowArc)) {
        return this->writerPlugin->Fix_Lakitu_Throw_Arc();
    }
    return true;
}

bool Hacks_Handler::Get_Bool_From_CheckState(Qt::CheckState checkState) {
    switch (checkState) {
    default:                        assert(false);
    case Qt::Checked:               return true;
    case Qt::Unchecked:             return false;
    case Qt::PartiallyChecked:      return static_cast<bool>(Random::Get_Num(1));
    }
}
