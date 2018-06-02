#include "Hacks_Handler.h"
#include "../SMB1_Writer/SMB1_Writer_Interface.h"
#include "../../Common_Files/Random.h"
#include <assert.h>

#include <QDebug>

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
    if (this->pluginSettings->addLuigiGame) {
        if (!this->writerPlugin->Add_Luigi_Game()) return false;
        if (!this->writerPlugin->Graphics_Write_Title_Screen_For_1_Player_Game()) return false;
    } else {
        if (!this->writerPlugin->Graphics_Write_Title_Screen_For_2_Player_Game()) return false;
    }
    if (!this->Handle_Damage()) return false;
    if (!this->Handle_Lakitu_Throw_Arc()) return false;
    if (!this->Handle_Enemy_Speed()) return false;
    if (this->Get_Bool_From_CheckState(this->pluginSettings->autoscroll) && !this->writerPlugin->Enable_Autoscroll()) return false;
    if (!this->writerPlugin->Enable_Hitting_Underwater_Blocks()) return false;
    return this->writerPlugin->Write_Watermark(); //write the watermark last
}

bool Hacks_Handler::Handle_Music() {
    int music = this->pluginSettings->music;
    if (music == 0) music = Random::Get_Num(17)+2;
    switch (music) {
    default: assert(false);
    case 1: return this->writerPlugin->Music_Disable(); //no music (music is disabled)
    case 2: return true; //original music
    case 3: return this->writerPlugin->Music_Alternative_Tone_Color_For_SQ1_And_SQ2_1(); //by w7n
    case 4: return this->writerPlugin->Music_Alternative_Tone_Color_For_SQ1_And_SQ2_2(); //by w7n
    case 5: return this->writerPlugin->Music_Enigmario(); //by Dr. Floppy and Curly M.C.
    case 6: return this->writerPlugin->Music_U1_1();
    case 7: return this->writerPlugin->Music_U1_2();
    case 8: return this->writerPlugin->Music_U1_3();
    case 9: return this->writerPlugin->Music_U1_4();
    case 10: if (!this->writerPlugin->Music_U1_4()) return false;
             return this->writerPlugin->Music_U1_3();
    case 11: return this->writerPlugin->Music_U1_5();
    case 12: return this->writerPlugin->Music_U1_6();
    case 13: if (!this->writerPlugin->Music_U1_6()) return false;
             return this->writerPlugin->Music_U1_3();
    case 14: return this->writerPlugin->Music_U1_7();
    case 15: return this->writerPlugin->Music_U1_8();
    case 16: return this->writerPlugin->Music_U1_9();
    case 17: if (!this->writerPlugin->Music_U1_9()) return false;
             return this->writerPlugin->Music_U1_3();
    case 18: return this->writerPlugin->Music_U1_10();
    case 19: return this->writerPlugin->Music_Water_Mario(); //by Eden GT
    }
}

bool Hacks_Handler::Handle_Graphics() {
    int graphics = this->pluginSettings->graphics;
    if (graphics == 0) graphics = Random::Get_Num(16)+1;
    switch (graphics) {
    default: assert(false);
    case 1: return true; //original graphics
    case 2: return this->writerPlugin->Graphics_Dream_Mario_Bros(); //by TheSuperShow
    case 3: return this->writerPlugin->Graphics_Harmony7(); //by Harmony7
    case 4: return this->writerPlugin->Graphics_Harmony7_Googie(); //by Googie
    case 5: return this->writerPlugin->Graphics_Hyper_Mario_Rev_A(); //by Unknown
    case 6: return this->writerPlugin->Graphics_Hyper_Mario_Rev_B(); //by Unknown
    case 7: return this->writerPlugin->Graphics_Lost_Levels(); //by Captain Antonio Falcon
    case 8: return this->writerPlugin->Graphics_Paradyce_SMB1_Palette_1(); //by Insectduel
    case 9: return this->writerPlugin->Graphics_Paradyce_SMB1_Palette_2(); //by Insectduel
    case 10: return this->writerPlugin->Graphics_Paradyce_SMB2(); //by Insectduel
    case 11: return this->writerPlugin->Graphics_Pocket_Edition(); //by Fantendo
    case 12: return this->writerPlugin->Graphics_Super_Bubba_Bros(); //by Unknown
    case 13: return this->writerPlugin->Graphics_Super_Duper_Mario_Bros(); //by Tabmok99
    case 14: return this->writerPlugin->Graphics_Super_Mario_Bros_DX_v2(); //by flamepanther
    case 15: return this->writerPlugin->Graphics_Super_Mario_Bros_DX_v3_4(); //by flamepanther
    case 16: return this->writerPlugin->Graphics_Super_Mario_MCB(); //by MacBee
    case 17: return this->writerPlugin->Graphics_Super_Mario_S(); //by Sogun
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
