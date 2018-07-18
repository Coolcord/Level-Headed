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
    if (this->pluginSettings->superMarioOnDamage && !this->writerPlugin->Hacks_Taking_Damage_As_Fire_Reverts_To_Super()) return false;
    if (this->pluginSettings->difficultyStartWithFireFlowerOnRoomChange && !this->writerPlugin->Hacks_Start_With_Fire_Flower_On_Room_Change()) return false;
    if (this->pluginSettings->lakituThrowArc && !this->writerPlugin->Hacks_Fix_Lakitu_Throw_Arc()) return false;
    if (!this->Handle_Piranha_Plants()) return false;
    if (!this->Handle_Lakitu_Respawn_Speed()) return false;
    if (!this->Handle_Enemy_Speed()) return false;
    if (this->Get_Bool_From_CheckState(this->pluginSettings->autoscroll) && !this->writerPlugin->Hacks_Always_Autoscroll()) return false;
    if (!this->Handle_Powerup()) return false;
    if (!this->Handle_Secondary_Mushroom()) return false;

    //The patches below are always applied
    if (!this->writerPlugin->Hacks_Real_Time()) return false;
    if (!this->writerPlugin->Hacks_Enable_Walking_Hammer_Bros(this->pluginSettings->difficultyWalkingHammerBros)) return false;
    if (!this->writerPlugin->Hacks_Enable_Hitting_Underwater_Blocks()) return false;
    if (!this->writerPlugin->Hacks_Hard_Mode_Does_Not_Affect_Lift_Size()) return false;
    if (!this->writerPlugin->Hacks_Replace_Castle_Loop_With_Autoscroll_Object(1, 1, 1, 1)) return false;
    if (!this->writerPlugin->Hacks_Fix_Life_Counter_Bugs()) return false;
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
}

bool Hacks_Handler::Handle_Lakitu_Respawn_Speed() {
    int lakituRespawnSpeed = this->pluginSettings->difficultyLakituRespawnSpeed;
    switch (lakituRespawnSpeed) {
    default:    assert(false);
    case 0:     return this->writerPlugin->Hacks_Set_Lakitu_Respawn_Speed(Random::Get_Num(0xA));
    case 1:     return this->writerPlugin->Hacks_Set_Lakitu_Respawn_Speed(0x10); //Disabled
    case 2:     return this->writerPlugin->Hacks_Set_Lakitu_Respawn_Speed(0xF); //Delayed
    case 3:     return true; //Normal (0x7)
    case 4:     return this->writerPlugin->Hacks_Set_Lakitu_Respawn_Speed(0x4); //Quick
    case 5:     return this->writerPlugin->Hacks_Set_Lakitu_Respawn_Speed(0x2); //Very Quick
    case 6:     return this->writerPlugin->Hacks_Set_Lakitu_Respawn_Speed(0x0); //ASAP
    }
}

bool Hacks_Handler::Handle_Lives() {
    if (this->pluginSettings->infiniteLives) {
        if (!this->writerPlugin->Hacks_Set_Starting_Lives(1)) return false;
        return this->writerPlugin->Hacks_Infinite_Lives();
    } else {
        if (!this->writerPlugin->Hacks_Set_Starting_Lives(this->pluginSettings->numLives)) return false;
    }
    return true;
}

bool Hacks_Handler::Handle_Piranha_Plants() {
    int piranhaPlantType = this->pluginSettings->difficultyPiranhaPlantType;
    if (piranhaPlantType == 0) piranhaPlantType = Random::Get_Num(2)+1;
    switch (piranhaPlantType) {
    default:    assert(false);
    case 1:     return true;
    case 2:     return this->writerPlugin->Hacks_Red_Piranha_Plants();
    case 3:     return this->writerPlugin->Hacks_Black_Piranha_Plants();
    }
}

bool Hacks_Handler::Handle_God_Mode() {
    if (this->pluginSettings->godMode) {
        return this->writerPlugin->Hacks_Enable_God_Mode();
    }
    return true;
}

bool Hacks_Handler::Handle_Enemy_Speed() {
    int speed = this->pluginSettings->difficultyBasicEnemySpeed;
    if (speed == 0) speed = Random::Get_Num(3)+1; //random all
    if (speed == 4) speed = 5; //increase speed for ludicrous setting
    if (!this->writerPlugin->Hacks_Fast_Basic_Enemies(speed)) return false;
    if (this->pluginSettings->difficultySpeedyObjectsAndEnemies && !this->writerPlugin->Hacks_Speedy_Objects_And_Enemies()) return false;
    return true;
}

bool Hacks_Handler::Handle_Powerup() {
    int powerup = this->pluginSettings->powerup;
    if (powerup == 0) powerup = Random::Get_Num(8)+1;
    switch (powerup) {
    default:    assert(false);
    case 1:     return true; //Fire Flower
    case 2:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Cutter_Flower();
    case 3:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Double_Jump_Star();
    case 4:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Fire_Star();
    case 5:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Hammer_Suit();
    case 6:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Poison_Bubbles();
    case 7:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Power_Wand();
    case 8:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Slime_Flower();
    case 9:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Spinball_Flower();
    }
}

bool Hacks_Handler::Handle_Secondary_Mushroom() {
    //Handle random values first
    int secondaryMushroom = this->pluginSettings->secondaryMushroom;
    if (secondaryMushroom == 0) secondaryMushroom = Random::Get_Num(2)+2; //1-Up, Poison, Swimming
    else if (secondaryMushroom == 1) secondaryMushroom = Random::Get_Num(3)+2; //1-Up, Poison, Swimming, Mystery

    //Handle the Mystery Mushroom
    bool randomPalette = false;
    if (secondaryMushroom == 5) {
        randomPalette = true;
        secondaryMushroom = Random::Get_Num(3)+2; //1-Up, Poison, Swimming, Poison or 1-Up
    }

    //Apply the necessary patch
    bool success = false;
    switch (secondaryMushroom) {
    default:    assert(false);
    case 2:     success = true; break; //1-Up
    case 3:     success = this->writerPlugin->Powerups_Replace_1UP_With_Poison_Mushroom(); break;
    case 4:     success = this->writerPlugin->Powerups_Replace_1UP_With_Swimming_Mushroom(); break;
    case 5:     success = this->writerPlugin->Powerups_Replace_1UP_With_Poison_Mushroom_If_Not_Fire_Mario(); break;
    }
    if (!success) return false;
    if (!randomPalette) return true;

    //Change the palette if it is a Mystery Mushroom
    return this->writerPlugin->Graphics_Change_1UP_Palette(Random::Get_Num(3));
}

bool Hacks_Handler::Get_Bool_From_CheckState(Qt::CheckState checkState) {
    switch (checkState) {
    default:                        assert(false);
    case Qt::Checked:               return true;
    case Qt::Unchecked:             return false;
    case Qt::PartiallyChecked:      return static_cast<bool>(Random::Get_Num(1));
    }
}
