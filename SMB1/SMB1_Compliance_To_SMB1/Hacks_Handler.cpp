#include "Hacks_Handler.h"
#include "../SMB1_Writer/SMB1_Writer_Interface.h"
#include "../SMB1_Writer/ROM_Filename.h"
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
    this->writerPlugin = nullptr;
    this->pluginSettings = nullptr;
}

bool Hacks_Handler::Write_Hacks() {
    bool fullSupport = this->pluginSettings->baseROM.startsWith(ROM_Filename::STRING_FULL_SUPPORT);
    if (fullSupport) {
        if (!this->Handle_Graphics()) return false; //graphics patches are typically the largest, so apply them first
        if (!this->Handle_Music()) return false;
        if (this->pluginSettings->randomSounds && !this->writerPlugin->Sound_Randomize_Sounds()) return false;
        if (!this->Handle_Names()) return false;
        if (!this->Handle_God_Mode()) return false;
        if (this->pluginSettings->difficultyUnlimitedTime && !this->writerPlugin->Hacks_Unlimited_Time()) return false;
        if (this->pluginSettings->addLuigiGame) {
            if (!this->writerPlugin->Hacks_Add_Luigi_Game()) return false;
            if (!this->writerPlugin->Graphics_Write_Title_Screen_For_1_Player_Game()) return false;
        } else {
            if (!this->writerPlugin->Graphics_Write_Title_Screen_For_2_Player_Game()) return false;
        }
        if (this->pluginSettings->superMarioOnDamage && !this->writerPlugin->Hacks_Taking_Damage_As_Fire_Reverts_To_Super()) return false;
        if (!this->Handle_Powerup()) return false;
        if (!this->Handle_Secondary_Mushroom()) return false;
        if (!this->Handle_Piranha_Plants()) return false;
        if (!this->Handle_Lakitus()) return false;
        if (!this->Handle_Enemy_Speed()) return false;
        if (!this->Handle_Replace_Castle_Loop()) return false;
        if (!this->writerPlugin->Hacks_Enable_Walking_Hammer_Bros(this->pluginSettings->difficultyWalkingHammerBros)) return false;
        if (!this->writerPlugin->Hacks_Fix_Life_Counter_Bugs()) return false;
    } else {
        if (!this->writerPlugin->Graphics_Write_Title_Screen_For_Partial_Game()) return false;
    }
    if (!this->Handle_Level_Length()) return false;
    if (!this->Handle_Lives()) return false;
    return this->writerPlugin->Hacks_Write_Watermark(); //write the watermark last
}

bool Hacks_Handler::Handle_Music() {
    assert(this->writerPlugin->Music_Set_Combine_Music_Packs(this->pluginSettings->combineMusicWithOtherPacks));

    int music = this->pluginSettings->music;
    if (music == 0) music = Random::Get_Instance().Get_Num(this->writerPlugin->Music_Get_Number_Of_Music_Packs())+2;
    bool success = false;
    switch (music) {
    case 1:     success = this->writerPlugin->Music_Disable(); break; //no music (music is disabled)
    case 2:     success = true; break; //original music
    default:    success = this->writerPlugin->Music_Apply_Music_Pack(music-3); break;
    }
    if (!success) return false;

    //Handle the Tone Color
    int toneColor = this->pluginSettings->toneColor;
    if (toneColor == 0) toneColor = Random::Get_Instance().Get_Num(18)+1;
    qDebug().noquote() << "Using tone color " << toneColor-1;
    switch (toneColor) {
    default:    assert(false); break;
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
    case 12:    success = this->writerPlugin->Music_Tone_Color_11(); break;
    case 13:    success = this->writerPlugin->Music_Tone_Color_12(); break;
    case 14:    success = this->writerPlugin->Music_Tone_Color_13(); break;
    case 15:    success = this->writerPlugin->Music_Tone_Color_14(); break;
    case 16:    success = this->writerPlugin->Music_Tone_Color_15(); break;
    case 17:    success = this->writerPlugin->Music_Tone_Color_16(); break;
    case 18:    success = this->writerPlugin->Music_Tone_Color_17(); break;
    case 19:    success = this->writerPlugin->Music_Tone_Color_18(); break;
    }
    return success;
}

bool Hacks_Handler::Handle_Graphics() {
    int graphics = this->pluginSettings->graphics;
    if (graphics == 0) graphics = Random::Get_Instance().Get_Num(this->writerPlugin->Graphics_Get_Number_Of_Graphics_Packs())+1;
    bool success = false;
    switch (graphics) {
    case 1:     success = true; break; //original graphics
    default:    success = this->writerPlugin->Graphics_Apply_Graphics_Pack(graphics-2); break;
    }
    return success;
}

bool Hacks_Handler::Handle_Lakitus() {
    //Fix the Throw Arc
    if (this->pluginSettings->lakituThrowArc && !this->writerPlugin->Hacks_Fix_Lakitu_Throw_Arc()) return false;

    //Handle the Random Values for the Spiny Egg Behavior
    int spinyEggBehavior = this->pluginSettings->difficultySpinyEggBehavior;
    switch (spinyEggBehavior) {
    default:    break;
    case 0:     spinyEggBehavior = Random::Get_Instance().Get_Num(3, 8); break;
    case 1:     spinyEggBehavior = Random::Get_Instance().Get_Num(4, 8); break;
    case 2:     spinyEggBehavior = Random::Get_Instance().Get_Num(5, 8); break;
    }

    //Handle the Value for the Spiny Egg Behavior
    bool success = false;
    switch (spinyEggBehavior) {
    default:    assert(false); return false;
    case 3:     success = this->writerPlugin->Hacks_Spiny_Eggs_Bouncy(); break; //Bounce
    case 4:     success = this->writerPlugin->Hacks_Spiny_Eggs_Chase_Mario(); break; //Chase Mario
    case 5:     success = this->writerPlugin->Hacks_Spiny_Eggs_Do_Not_Break(); break; //Do Not Break
    case 6:     success = this->writerPlugin->Hacks_Spiny_Eggs_Explode_Into_Flames(); break; //Explode Into Flames
    case 7:     success = this->writerPlugin->Hacks_Spiny_Eggs_No_Eggs(); break; //No Eggs
    case 8:     success = true; break; //Normal
    }
    if (!success) return false;

    //Handle the Respawn Speed
    int lakituRespawnSpeed = this->pluginSettings->difficultyLakituRespawnSpeed;
    switch (lakituRespawnSpeed) {
    default:    assert(false); return false;
    case 0:     return this->writerPlugin->Hacks_Set_Lakitu_Respawn_Speed(Random::Get_Instance().Get_Num(0xA));
    case 1:     return this->writerPlugin->Hacks_Set_Lakitu_Respawn_Speed(0x10); //Disabled
    case 2:     return this->writerPlugin->Hacks_Set_Lakitu_Respawn_Speed(0xF); //Delayed
    case 3:     return true; //Normal (0x7)
    case 4:     return this->writerPlugin->Hacks_Set_Lakitu_Respawn_Speed(0x4); //Quick
    case 5:     return this->writerPlugin->Hacks_Set_Lakitu_Respawn_Speed(0x2); //Very Quick
    case 6:     return this->writerPlugin->Hacks_Set_Lakitu_Respawn_Speed(0x0); //ASAP
    }
}

bool Hacks_Handler::Handle_Level_Length() {
    switch (this->pluginSettings->difficultyMaxLevelLength) {
    default: return false;
    case 0: //Random
        this->pluginSettings->difficultyMaxLevelLengthBlocks = Random::Get_Instance().Get_Num(64, 321);
        if (this->pluginSettings->difficultyMaxLevelLengthBlocks == 321) this->pluginSettings->difficultyMaxLevelLengthBlocks = 0;
        return true;
    case 1: //As Short as Possible
        this->pluginSettings->difficultyMaxLevelLengthBlocks = 1; return true;
    case 2: //Very Short
        this->pluginSettings->difficultyMaxLevelLengthBlocks = 96; return true;
    case 3: //Short
        this->pluginSettings->difficultyMaxLevelLengthBlocks = 144; return true;
    case 4: //Normal (about 12 pages)
        this->pluginSettings->difficultyMaxLevelLengthBlocks = 192; return true;
    case 5: //Long
        this->pluginSettings->difficultyMaxLevelLengthBlocks = 240; return true;
    case 6: //Very Long
        this->pluginSettings->difficultyMaxLevelLengthBlocks = 288; return true;
    case 7: //As Long as Possible
        this->pluginSettings->difficultyMaxLevelLengthBlocks = 0; return true;
    }
}

bool Hacks_Handler::Handle_Lives() {
    if (this->pluginSettings->infiniteLives) return this->writerPlugin->Hacks_Infinite_Lives();
    else if (this->pluginSettings->permadeath) return this->writerPlugin->Hacks_Permadeath();
    else return this->writerPlugin->Hacks_Set_Starting_Lives(this->pluginSettings->numLives);
}

bool Hacks_Handler::Handle_Piranha_Plants() {
    int piranhaPlantType = this->pluginSettings->difficultyPiranhaPlantType;
    if (piranhaPlantType == 0) piranhaPlantType = Random::Get_Instance().Get_Num(2)+1;
    switch (piranhaPlantType) {
    default:    assert(false); return false;
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
    //Handle basic enemy speed
    int speed = this->pluginSettings->difficultyBasicEnemySpeed;
    if (speed == 0) speed = Random::Get_Instance().Get_Num(3)+1; //random all
    if (speed == 4) speed = 5; //increase speed for ludicrous setting
    if (!this->writerPlugin->Hacks_Set_Basic_Enemy_Speed(speed)) return false;

    //Handle bullet bill speed
    speed = this->pluginSettings->difficultyBulletBillSpeed;
    if (speed == 0) speed = Random::Get_Instance().Get_Num(4)+1;
    if (!this->writerPlugin->Hacks_Set_Bullet_Bill_Speed(speed)) return false;

    //Handle bullet bill firing rate
    speed = this->pluginSettings->difficultyBulletBillFiringRate;
    if (speed == 0) speed = Random::Get_Instance().Get_Num(1, 7);
    if (!this->writerPlugin->Hacks_Set_Bullet_Bill_Firing_Rate(speed)) return false;

    //Handle the flying Cheep-Cheep jump height
    speed = this->pluginSettings->difficultyFlyingCheepCheepJumpHeight;
    if (speed == 0) speed = Random::Get_Instance().Get_Num(1, 2);
    if (speed == 1) speed = 4;
    else speed = 5;
    if (!this->writerPlugin->Hacks_Set_Flying_Cheep_Cheep_Jump_Height(speed)) return false;

    //Handle the Hammer Bros throw rate
    speed = this->pluginSettings->difficultyHammerBrosThrowRate;
    int easySpeed = 0, hardSpeed = 0;
    switch (speed) {
    default: assert(false); return false;
    case 1:     easySpeed = 108; hardSpeed = 88; break;
    case 2:     easySpeed = 88; hardSpeed = 68; break;
    case 3:     easySpeed = 68; hardSpeed = 48; break;
    case 4:     easySpeed = 48; hardSpeed = 28; break;
    case 5:     easySpeed = 28; hardSpeed = 16; break;
    case 6:     easySpeed = 16; hardSpeed = 9; break;
    case 7:     easySpeed = 9; hardSpeed = 5; break;
    case 8:     easySpeed = 1; hardSpeed = 1; break;
    case 0:
        easySpeed = Random::Get_Instance().Get_Num(1, 108);
        hardSpeed = Random::Get_Instance().Get_Num(easySpeed, 108);
        break;
    }
    if (!this->writerPlugin->Hacks_Set_Hammer_Bros_Throw_Rate(easySpeed, hardSpeed)) return false;

    //Handle the speedy enemies patch
    if (this->pluginSettings->difficultySpeedyObjectsAndEnemies && !this->writerPlugin->Hacks_Speedy_Objects_And_Enemies()) return false;
    return true;
}

bool Hacks_Handler::Handle_Powerup() {
    int powerup = this->pluginSettings->powerup;
    if (powerup == 0) powerup = Random::Get_Instance().Get_Num(1, 13);
    switch (powerup) {
    default:    assert(false); return false;
    case 1:     return true; //Fire Flower
    case 2:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Bone_Caster();
    case 3:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Coin_Flinger();
    case 4:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Cutter_Flower();
    case 5:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Double_Jump_Star();
    case 6:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Fire_Star();
    case 7:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Hammer_Suit();
    case 8:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Luigis_Fire_Flower();
    case 9:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Pink_Fury();
    case 10:    return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Poison_Bubbles();
    case 11:    return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Power_Wand();
    case 12:    return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Slime_Flower();
    case 13:    return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Spinball_Flower();
    }
}

bool Hacks_Handler::Handle_Secondary_Mushroom() {
    //Handle random values first
    int secondaryMushroom = this->pluginSettings->secondaryMushroom;
    if (secondaryMushroom == 0) secondaryMushroom = Random::Get_Instance().Get_Num(2)+2; //1-Up, Poison, Swimming
    else if (secondaryMushroom == 1) secondaryMushroom = Random::Get_Instance().Get_Num(3)+2; //1-Up, Poison, Swimming, Mystery

    //Handle the Mystery Mushroom
    bool randomPalette = false;
    if (secondaryMushroom == 5) {
        randomPalette = true;
        secondaryMushroom = Random::Get_Instance().Get_Num(3)+2; //1-Up, Poison, Swimming, Poison or 1-Up
    }

    //Apply the necessary patch
    bool success = false;
    switch (secondaryMushroom) {
    default:    assert(false); return false;
    case 2:     success = true; break; //1-Up
    case 3:     success = this->writerPlugin->Powerups_Replace_1UP_With_Poison_Mushroom(); break;
    case 4:     success = this->writerPlugin->Powerups_Replace_1UP_With_Swimming_Mushroom(); break;
    case 5:     success = this->writerPlugin->Powerups_Replace_1UP_With_Poison_Mushroom_If_Not_Fire_Mario(); break;
    }
    if (!success) return false;
    if (!randomPalette) return true;

    //Change the palette if it is a Mystery Mushroom
    return this->writerPlugin->Graphics_Change_1UP_Palette(Random::Get_Instance().Get_Num(3));
}

bool Hacks_Handler::Handle_Names() {
    if (!this->writerPlugin->Hacks_Set_Mario_Name(this->pluginSettings->marioName)) return false;
    if (!this->writerPlugin->Hacks_Set_Luigi_Name(this->pluginSettings->luigiName)) return false;
    return true;
}

bool Hacks_Handler::Handle_Replace_Castle_Loop() {
    this->pluginSettings->difficultyReplaceCastleLoopsCurrent = this->pluginSettings->difficultyReplaceCastleLoops;
    if (this->pluginSettings->difficultyReplaceCastleLoopsCurrent == 0) {
        this->pluginSettings->difficultyReplaceCastleLoopsCurrent = Random::Get_Instance().Get_Num(3, 5); //(no fireflower buffs)
    } else if (this->pluginSettings->difficultyReplaceCastleLoopsCurrent == 1) {
        this->pluginSettings->difficultyReplaceCastleLoopsCurrent = Random::Get_Instance().Get_Num(3, 4); //(only Auto Scroll or Fire Bros)
    }
    switch (this->pluginSettings->difficultyReplaceCastleLoopsCurrent) {
    default:    assert(false); return false;
    case 2:     return true; //no complimentary hack
    case 3:     return this->writerPlugin->Hacks_Replace_Castle_Loop_With_Autoscroll_Object();
    case 4:     return this->writerPlugin->Hacks_Replace_Castle_Loop_With_Fire_Bros();
    case 5:     return this->writerPlugin->Hacks_Replace_Castle_Loop_With_Top_Of_Flagpole_Gives_1UP();
    case 6:     return this->writerPlugin->Hacks_Replace_Castle_Loop_With_Start_With_Fire_Flower();
    case 7:
        if (!this->writerPlugin->Hacks_Replace_Castle_Loop_With_Top_Of_Flagpole_Gives_1UP()) return false;
        return this->writerPlugin->Hacks_Replace_Castle_Loop_With_Start_With_Fire_Flower();
    }
}

bool Hacks_Handler::Get_Bool_From_CheckState(Qt::CheckState checkState) {
    switch (checkState) {
    case Qt::Checked:               return true;
    case Qt::Unchecked:             return false;
    case Qt::PartiallyChecked:      return static_cast<bool>(Random::Get_Instance().Get_Num(1));
    }
    assert(false); return false;
}
