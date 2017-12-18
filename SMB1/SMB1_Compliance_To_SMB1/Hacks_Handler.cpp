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
    if (!this->Handle_Graphics()) return false; //graphics patches are typically the largest, so apply them first
    if (!this->Handle_Music()) return false;
    if (!this->Handle_Lives()) return false;
    if (!this->Handle_God_Mode()) return false;
    if (this->pluginSettings->addLuigiGame && !this->writerPlugin->Add_Luigi_Game()) return false;
    if (!this->Handle_Damage()) return false;
    if (!this->Handle_Lakitu_Throw_Arc()) return false;
    if (!this->Handle_Enemy_Speed()) return false;
    return this->writerPlugin->Write_Watermark(); //write the watermark last
}

bool Hacks_Handler::Handle_Music() {
    int music = this->pluginSettings->music;
    if (music == 0) music = Random::Get_Num(16)+1;
    switch (music) {
    default: assert(false);
    case 1: return true; //original music
    case 2: return this->writerPlugin->Music_Alternative_Tone_Color_For_SQ1_And_SQ2_1(); //by w7n
    case 3: return this->writerPlugin->Music_Alternative_Tone_Color_For_SQ1_And_SQ2_2(); //by w7n
    case 4: return this->writerPlugin->Music_Enigmario(); //by Dr. Floppy and Curly M.C.
    case 5: return this->writerPlugin->Music_U1_1();
    case 6: return this->writerPlugin->Music_U1_2();
    case 7: return this->writerPlugin->Music_U1_3();
    case 8: return this->writerPlugin->Music_U1_4();
    case 9: if (!this->writerPlugin->Music_U1_4()) return false;
            return this->writerPlugin->Music_U1_3();
    case 10: return this->writerPlugin->Music_U1_5();
    case 11: return this->writerPlugin->Music_U1_6();
    case 12: if (!this->writerPlugin->Music_U1_6()) return false;
            return this->writerPlugin->Music_U1_3();
    case 13: return this->writerPlugin->Music_U1_7();
    case 14: return this->writerPlugin->Music_U1_8();
    case 15: return this->writerPlugin->Music_U1_9();
    case 16: if (!this->writerPlugin->Music_U1_9()) return false;
            return this->writerPlugin->Music_U1_3();
    case 17: return this->writerPlugin->Music_U1_10();
    }
}

bool Hacks_Handler::Handle_Graphics() {
    int graphics = this->pluginSettings->graphics;
    if (graphics == 0) graphics = Random::Get_Num(6)+1;
    switch (graphics) {
    default: assert(false);
    case 1: return true; //original graphics
    case 2: return this->writerPlugin->Graphics_Dream_Mario_Bros(); //by TheSuperShow
    case 3: return this->writerPlugin->Graphics_Lost_Levels(); //by Captain Antonio Falcon
    case 4: return this->writerPlugin->Graphics_Pocket_Edition(); //by Fantendo
    case 5: return this->writerPlugin->Graphics_Super_Mario_Bros_DX(); //by flamepanther
    case 6: return this->writerPlugin->Graphics_Super_Mario_MCB(); //by MacBee
    case 7: return this->writerPlugin->Graphics_Super_Mario_S(); //by Sogun
    }
}

bool Hacks_Handler::Handle_Lives() {
    if (!this->writerPlugin->Set_Starting_Lives(this->pluginSettings->numLives)) return false;
    if (this->pluginSettings->infiniteLives) {
        return this->writerPlugin->Infinite_Lives();
    }
    return true;
}

bool Hacks_Handler::Handle_God_Mode() {
    if (this->pluginSettings->godMode) {
        return this->writerPlugin->Enable_God_Mode();
    }
    return true;
}

bool Hacks_Handler::Handle_Damage() {
    if (this->Get_Bool_From_CheckState(this->pluginSettings->superMarioOnDamage)) {
        return this->writerPlugin->Taking_Damage_As_Fire_Reverts_To_Super();
    }
    return true;
}

bool Hacks_Handler::Handle_Lakitu_Throw_Arc() {
    if (this->Get_Bool_From_CheckState(this->pluginSettings->lakituThrowArc)) {
        return this->writerPlugin->Fix_Lakitu_Throw_Arc();
    }
    return true;
}

bool Hacks_Handler::Handle_Enemy_Speed() {
    int speed = this->pluginSettings->enemySpeed;
    if (speed == 6) {
        speed = Random::Get_Num(2)+1; //don't get ludicrous speed or turbo
    } else if (speed == 7) {
        speed = Random::Get_Num(4)+1; //random all
    }
    return this->writerPlugin->Fast_Enemies(speed);
}

bool Hacks_Handler::Get_Bool_From_CheckState(Qt::CheckState checkState) {
    switch (checkState) {
    default:                        assert(false);
    case Qt::Checked:               return true;
    case Qt::Unchecked:             return false;
    case Qt::PartiallyChecked:      return static_cast<bool>(Random::Get_Num(1));
    }
}
