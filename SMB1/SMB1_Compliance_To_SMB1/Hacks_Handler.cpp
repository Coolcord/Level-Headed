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
        if (!this->writerPlugin->Hacks_Add_Luigi_Game()) return false;
        if (!this->writerPlugin->Graphics_Write_Title_Screen_For_1_Player_Game()) return false;
    } else {
        if (!this->writerPlugin->Graphics_Write_Title_Screen_For_2_Player_Game()) return false;
    }
    if (!this->Handle_Damage()) return false;
    if (!this->Handle_Lakitu_Throw_Arc()) return false;
    if (!this->Handle_Enemy_Speed()) return false;
    if (this->Get_Bool_From_CheckState(this->pluginSettings->autoscroll) && !this->writerPlugin->Hacks_Always_Autoscroll()) return false;
    if (this->Get_Bool_From_CheckState(this->pluginSettings->replaceFireFlowerWithHammerSuit) && !this->writerPlugin->Hacks_Replace_Fire_Flower_With_Hammer_Suit()) return false;

    //The patches below are always applied
    if (!this->writerPlugin->Hacks_Enable_Hitting_Underwater_Blocks()) return false;
    if (!this->writerPlugin->Hacks_Hard_Mode_Does_Not_Affect_Lift_Size()) return false;
    if (!this->writerPlugin->Hacks_Replace_Castle_Loop_With_Autoscroll_Object(1, 1, 1, 1)) return false;
    return this->writerPlugin->Hacks_Write_Watermark(); //write the watermark last
}

bool Hacks_Handler::Handle_Music() {
    assert(this->writerPlugin->Music_Set_Combine_Music_Packs(this->pluginSettings->combineMusicWithOtherPacks));

    int music = this->pluginSettings->music;
    if (music == 0) music = Random::Get_Num(this->writerPlugin->Music_Get_Number_Of_Music_Packs())+2;
    bool success = false;
    switch (music) {
    case 1:     success = this->writerPlugin->Music_Disable(); break; //no music (music is disabled)
    case 2:     success = true; break; //original music
    default:    success = this->writerPlugin->Music_Apply_Music_Pack(music-3); break;
    }
    if (!success) return false;

    //Handle the Tone Color
    int toneColor = this->pluginSettings->toneColor;
    if (toneColor == 0) toneColor = Random::Get_Num(10)+1;
    qDebug().noquote() << "Using tone color " << toneColor-1;
    switch (toneColor) {
    default: assert(false);
    case 1:     success = true; break;
    case 2:     success = this->writerPlugin->Music_Tone_Color_1(); break;
    case 3:     success = this->writerPlugin->Music_Tone_Color_2(); break;
    case 4:     success = this->writerPlugin->Music_Tone_Color_3(); break;
    case 5:     success = this->writerPlugin->Music_Tone_Color_4(); break;
    case 6:     success = this->writerPlugin->Music_Tone_Color_5(); break;
    case 7:     success = this->writerPlugin->Music_Tone_Color_6(); break;
    case 8:     success = this->writerPlugin->Music_Tone_Color_7(); break;
    case 9:     success = this->writerPlugin->Music_Tone_Color_8(); break;
    case 10:    success = this->writerPlugin->Music_Tone_Color_9(); break;
    case 11:    success = this->writerPlugin->Music_Tone_Color_10(); break;
    }
    return success;
}

bool Hacks_Handler::Handle_Graphics() {
    int graphics = this->pluginSettings->graphics;
    if (graphics == 0) graphics = Random::Get_Num(this->writerPlugin->Graphics_Get_Number_Of_Graphic_Packs())+1;
    bool success = false;
    switch (graphics) {
    case 1:     success = true; break; //original graphics
    default:    success = this->writerPlugin->Graphics_Apply_Graphic_Pack(graphics-2); break;
    }
    return success;


    /* Deprecated Code
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
    }*/
}

bool Hacks_Handler::Handle_Lives() {
    if (!this->writerPlugin->Hacks_Set_Starting_Lives(this->pluginSettings->numLives)) return false;
    if (this->pluginSettings->infiniteLives) {
        return this->writerPlugin->Hacks_Infinite_Lives();
    }
    return true;
}

bool Hacks_Handler::Handle_God_Mode() {
    if (this->pluginSettings->godMode) {
        return this->writerPlugin->Hacks_Enable_God_Mode();
    }
    return true;
}

bool Hacks_Handler::Handle_Damage() {
    if (this->Get_Bool_From_CheckState(this->pluginSettings->superMarioOnDamage)) {
        return this->writerPlugin->Hacks_Taking_Damage_As_Fire_Reverts_To_Super();
    }
    return true;
}

bool Hacks_Handler::Handle_Lakitu_Throw_Arc() {
    if (this->Get_Bool_From_CheckState(this->pluginSettings->lakituThrowArc)) {
        return this->writerPlugin->Hacks_Fix_Lakitu_Throw_Arc();
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
    return this->writerPlugin->Hacks_Fast_Enemies(speed);
}

bool Hacks_Handler::Get_Bool_From_CheckState(Qt::CheckState checkState) {
    switch (checkState) {
    default:                        assert(false);
    case Qt::Checked:               return true;
    case Qt::Unchecked:             return false;
    case Qt::PartiallyChecked:      return static_cast<bool>(Random::Get_Num(1));
    }
}
