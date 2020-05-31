#include "Hacks_Handler.h"
#include "../SMB1_Writer/SMB1_Writer_Interface.h"
#include "../SMB1_Writer/ROM_Filename.h"
#include "../../../C_Common_Code/Qt/Random/Random.h"
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
        if (!this->Handle_Animations()) return false;
        if (!this->Handle_God_Mode()) return false;
        if (this->pluginSettings->difficultyUnlimitedTime && !this->writerPlugin->Hacks_Unlimited_Time()) return false;
        if (this->pluginSettings->addLuigiGame) {
            if (!this->writerPlugin->Hacks_Add_Luigi_Game()) return false;
            if (!this->writerPlugin->Graphics_Write_Title_Screen_For_1_Player_Game()) return false;
        } else {
            if (!this->writerPlugin->Graphics_Write_Title_Screen_For_2_Player_Game()) return false;
        }
        if (!this->Handle_Text()) return false;
        if (this->pluginSettings->superMarioOnDamage && !this->writerPlugin->Hacks_Taking_Damage_As_Fire_Reverts_To_Super()) return false;
        if (!this->Handle_Powerup()) return false; //must be applied BEFORE Lakitus
        if (!this->Handle_Secondary_Mushroom()) return false;
        if (!this->Handle_Piranha_Plants()) return false;
        if (!this->Handle_Lakitus()) return false;
        if (!this->Handle_Replace_Castle_Loop()) return false;
        if (!this->Handle_Enemy_Speed()) return false;
        if (!this->writerPlugin->Hacks_Enable_Walking_Hammer_Bros(this->pluginSettings->difficultyWalkingHammerBros)) return false;
        if (!this->writerPlugin->Hacks_Fix_Life_Counter_Bugs()) return false;
    } else {
        if (!this->writerPlugin->Graphics_Write_Title_Screen_For_Partial_Game()) return false;
    }
    if (!this->pluginSettings->generateNewLevels && this->pluginSettings->randomizeWarpZones && !this->writerPlugin->Hacks_Randomize_Warp_Zone()) return false;
    if (!this->Handle_Enemy_Groups()) return false;
    if (!this->Handle_Level_Length()) return false;
    if (!this->Handle_Lives()) return false;
    return this->writerPlugin->Hacks_Write_Watermark(); //write the watermark last
}

bool Hacks_Handler::Handle_Animations() {
    if (!this->pluginSettings->randomizeSomeAnimations) return true; //nothing to do

    //Set the Intro Demo
    if (!this->writerPlugin->Hacks_Random_Intro_Demo()) return false;

    //Set Brick Block Destruction Bounce Height
    if (!this->writerPlugin->Hacks_Set_Brick_Break_Animation_Bounce_Height(Random::Get_Instance().Get_Num(0, 5), Random::Get_Instance().Get_Num(0, 9))) return false;

    //Set the Death Animation Jump Height
    if (!this->writerPlugin->Hacks_Set_Death_Animation_Jump_Height(Random::Get_Instance().Get_Num(0, 7))) return false;

    //Handle "True" Bowser Forms
    if (!this->writerPlugin->Hacks_Random_True_Bowser_Characters()) return false;

    //Handle the Bowser Bridge Destruction Order
    if (!this->writerPlugin->Hacks_Destroy_Bowser_Bridge_Randomly()) return false;
    if (!this->writerPlugin->Hacks_Set_Bowser_Bridge_Destruction_Speed(Random::Get_Instance().Get_Num(1, 7))) return false;

    //Set the Coin Animation Bounce Height
    return this->writerPlugin->Hacks_Set_Coin_Animation_Bounce_Height(Random::Get_Instance().Get_Num(1, 9));
}

bool Hacks_Handler::Handle_Enemy_Groups() {
    //Handle Enemy Distribution Settings
    this->pluginSettings->redistributeEnemies = false;
    this->pluginSettings->performChaoticSwapOnEnemies = false;
    if (!this->pluginSettings->generateNewLevels) {
        int value = this->pluginSettings->levelScriptEnemies;
        if (value == 0) value = Random::Get_Instance().Get_Num(2, 4); //no original
        switch (value) {
        default:
            assert(false);
            break;
        case 1: //Original
            this->pluginSettings->performChaoticSwapOnEnemies = false;
            this->pluginSettings->redistributeEnemies = false;
            break;
        case 2: //Redistributed
            this->pluginSettings->redistributeEnemies = true;
            this->pluginSettings->performChaoticSwapOnEnemies = false;
            break;
        case 3: //Chaotic Swap
            this->pluginSettings->redistributeEnemies = false;
            this->pluginSettings->performChaoticSwapOnEnemies = true;
            break;
        case 4: //Redistributed + Chaotic Swap
            this->pluginSettings->redistributeEnemies = true;
            this->pluginSettings->performChaoticSwapOnEnemies = true;
            break;
        }
    }

    if (!this->pluginSettings->generateNewLevels && this->pluginSettings->performChaoticSwapOnEnemies) {
        if (!this->writerPlugin->Hacks_Random_Goomba_Group_Enemy(this->pluginSettings->difficultyAllowHammerBrosGroupsWhenRandomizingEnemiesInLevelScripts)) return false;
        if (!this->writerPlugin->Hacks_Random_Koopa_Group_Enemy(this->pluginSettings->difficultyAllowHammerBrosGroupsWhenRandomizingEnemiesInLevelScripts)) return false;
    }
   return true;
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
    if (toneColor == 0) toneColor = Random::Get_Instance().Get_Num(21)+1;
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
    case 20:    success = this->writerPlugin->Music_Tone_Color_19(); break;
    case 21:    success = this->writerPlugin->Music_Tone_Color_20(); break;
    case 22:    success = this->writerPlugin->Music_Tone_Color_21(); break;
    }
    return success;
}

bool Hacks_Handler::Handle_Graphics() {
    //Write the Graphics Pack
    if (!this->writerPlugin->Graphics_Set_Combine_Graphics_Packs(this->pluginSettings->combineGraphicsWithOtherPacks)) return false;
    int graphics = this->pluginSettings->graphics;
    if (graphics == 0) graphics = Random::Get_Instance().Get_Num(this->writerPlugin->Graphics_Get_Number_Of_Graphics_Packs())+1;
    bool success = false;
    switch (graphics) {
    case 1:     success = this->writerPlugin->Graphics_Combine_Graphics(); break; //original graphics
    default:    success = this->writerPlugin->Graphics_Apply_Graphics_Pack(graphics-2); break;
    }
    if (!success) return false;

    //Randomize the Palette
    int palette = this->pluginSettings->palette;
    if (palette == 0) palette = Random::Get_Instance().Get_Num(1, 8); //random up to Excessive
    if (!this->writerPlugin->Graphics_Randomize_Palettes(palette)) return false;

    //Write the Mario Sprite
    int marioSprite = this->pluginSettings->marioSprite;
    if (marioSprite == 0) marioSprite = Random::Get_Instance().Get_Num(this->writerPlugin->Graphics_Get_Number_Of_Mario_Sprites()+this->writerPlugin->Graphics_Get_Number_Of_Bonus_Mario_Sprites())+2;
    else if (marioSprite == 1) marioSprite = Random::Get_Instance().Get_Num(this->writerPlugin->Graphics_Get_Number_Of_Mario_Sprites())+2;
    success = false;
    switch (marioSprite) {
    case 2:     success = true; break; //use Mario Sprite included in graphics pack
    default:    success = this->writerPlugin->Graphics_Apply_Mario_Sprite(marioSprite-3); break;
    }
    if (!success) return false;

    palette = this->pluginSettings->marioSpritePalette;
    if (palette == 0) palette = Random::Get_Instance().Get_Num(3, 10); //Random (No Randomly Generated)
    else if (palette == 1) palette = Random::Get_Instance().Get_Num(2, 10); //Random All
    if (palette > 3) {
        success = this->writerPlugin->Graphics_Apply_Mario_Palette_Present(palette-4);
    } else {
        switch (palette) {
        default:    assert(false); break;
        case 2:     success = this->writerPlugin->Graphics_Randomize_Mario_Sprite_Palette(); break; //Randomly Generated
        case 3:     success = true; break; //Mario Sprite Default
        }
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
        this->pluginSettings->difficultyMaxLevelLengthBlocks = Random::Get_Instance().Get_Num(64, 320);
        if (this->pluginSettings->difficultyMaxLevelLengthBlocks == 321) this->pluginSettings->difficultyMaxLevelLengthBlocks = 0;
        return true;
    case 1: //As Short as Possible
        this->pluginSettings->difficultyMaxLevelLengthBlocks = 1; return true;
    case 2: //Very Short
        this->pluginSettings->difficultyMaxLevelLengthBlocks = 96; return true;
    case 3: //Short
        this->pluginSettings->difficultyMaxLevelLengthBlocks = 144; return true;
    case 4: //Normal (about 11 pages)
        this->pluginSettings->difficultyMaxLevelLengthBlocks = 176; return true;
    case 5: //Long
        this->pluginSettings->difficultyMaxLevelLengthBlocks = 224; return true;
    case 6: //Very Long
        this->pluginSettings->difficultyMaxLevelLengthBlocks = 272; return true;
    case 7: //As Long as Possible
        this->pluginSettings->difficultyMaxLevelLengthBlocks = 0; return true;
    }
}

bool Hacks_Handler::Handle_Lives() {
    if (!this->writerPlugin->Hacks_Disable_One_Up_Coin_Limits()) return false;
    if (this->pluginSettings->infiniteLives) return this->writerPlugin->Hacks_Infinite_Lives();
    else if (this->pluginSettings->permadeath) return this->writerPlugin->Hacks_Permadeath();
    else return this->writerPlugin->Hacks_Set_Starting_Lives(this->pluginSettings->numLives);
}

bool Hacks_Handler::Handle_Piranha_Plants() {
    if (this->pluginSettings->difficultyPiranhaPlantsOnFirstLevel && !this->writerPlugin->Hacks_Enable_Piranha_Plants_On_First_Level()) return false;
    int maxNumberOfPlants = this->pluginSettings->difficultyMaxNumberOfPiranhaPlants;
    if (maxNumberOfPlants == 0) maxNumberOfPlants = Random::Get_Instance().Get_Num(1, 6);
    if (!this->writerPlugin->Hacks_Set_Maximum_Number_Of_Pirahna_Plants(maxNumberOfPlants)) return false;

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
    int speed = 0;
    switch (this->pluginSettings->difficultyBasicEnemySpeed) {
    default:    assert(false); return false;
    case 1:     speed = 4; break;  //Very Slow
    case 2:     speed = 6; break;  //Slow
    case 3:     speed = 8; break;  //Normal
    case 4:     speed = 12; break; //Fast
    case 5:     speed = 18; break; //Very Fast
    case 6:     speed = 24; break; //Speedy
    case 7:     speed = 48; break; //Ludicrous Speed!
    case 0:     //Random
        speed = Random::Get_Instance().Get_Num(4, 25);
        if (speed == 25) speed = 48;
        break;
    }
    if (!this->writerPlugin->Hacks_Set_Basic_Enemy_Speed(speed)) return false;

    //Handle surfing lift speed
    speed = 0;
    switch (this->pluginSettings->difficultySurfingLiftSpeed) {
    default:    assert(false); return false;
    case 1:     speed = 10; break; //Very Slow
    case 2:     speed = 13; break; //Slow
    case 3:     speed = 16; break; //Normal
    case 4:     speed = 20; break; //Fast
    case 5:     speed = 24; break; //Very Fast
    case 6:     speed = 28; break; //Ludicrous Speed!
    case 0:     speed = Random::Get_Instance().Get_Num(10, 28); break;
    }
    if (!this->writerPlugin->Hacks_Set_Surfing_Lift_Speed(speed)) return false;

    //Handle bullet bill speed
    speed = this->pluginSettings->difficultyBulletBillSpeed;
    if (speed == 0) speed = Random::Get_Instance().Get_Num(4)+1;
    if (!this->writerPlugin->Hacks_Set_Bullet_Bill_Speed(speed)) return false;

    //Handle bullet bill firing rate
    speed = this->pluginSettings->difficultyBulletBillFiringRate;
    if (speed == 0) speed = Random::Get_Instance().Get_Num(1, 7);
    if (!this->writerPlugin->Hacks_Set_Bullet_Bill_Firing_Rate(speed)) return false;

    //Handle the Bowser Flame Frequency
    speed = this->pluginSettings->difficultyBowserFlameFrequency;
    if (speed == 0) speed = Random::Get_Instance().Get_Num(1, 3);
    if (!this->writerPlugin->Hacks_Set_Bowser_Flame_Frequency(speed)) return false;

    //Handle the flying Cheep-Cheep jump height
    speed = this->pluginSettings->difficultyFlyingCheepCheepJumpHeight;
    if (speed == 0) speed = Random::Get_Instance().Get_Num(1, 2);
    if (speed == 1) speed = 4;
    else speed = 5;
    if (!this->writerPlugin->Hacks_Set_Flying_Cheep_Cheep_Jump_Height(speed)) return false;

    //Handle the European Blooper Swim Height
    if (this->pluginSettings->difficultyEuropeanBlooperSwimHeight && !this->writerPlugin->Hacks_European_Blooper_Swim_Height()) return false;

    //Handle the Leaping Paratroopa Speed
    speed = this->pluginSettings->difficultyLeapingParatroopaSpeed;
    if (speed == 0) speed = Random::Get_Instance().Get_Num(1, 3);
    switch (speed) {
    default:    assert(false); return false;
    case 1:     if (!this->writerPlugin->Hacks_Slow_Leaping_Paratroopas()) return false;
    case 2:     break;
    case 3:     if (!this->writerPlugin->Hacks_Fast_Leaping_Paratroopas()) return false;
    }

    //Handle the Hammer Bros throw rate
    speed = this->pluginSettings->difficultyHammerBrosThrowRate;
    int easySpeed = 0, hardSpeed = 0;
    if (this->pluginSettings->difficultyReplaceCastleLoopsCurrent == 4) { //Fire Bros. Throw Rates
        switch (speed) {
        default:    assert(false); return false;
        case 1:     easySpeed = 248; hardSpeed = 218; break;
        case 2:     easySpeed = 208; hardSpeed = 178; break;
        case 3:     easySpeed = 168; hardSpeed = 138; break;
        case 4:     easySpeed = 128; hardSpeed = 108; break;
        case 5:     easySpeed = 102; hardSpeed = 82; break;
        case 6:     easySpeed = 75; hardSpeed = 55; break;
        case 7:     easySpeed = 48; hardSpeed = 28; break;
        case 8:     easySpeed = 18; hardSpeed = 13; break;
        case 0:
            easySpeed = Random::Get_Instance().Get_Num(13, 248);
            hardSpeed = Random::Get_Instance().Get_Num(easySpeed, 248);
            break;
        }
    } else { //Hammer Bros. Throw Rates
        switch (speed) {
        default: assert(false); return false;
        case 1:     easySpeed = 108; hardSpeed = 88; break;
        case 2:     easySpeed = 88; hardSpeed = 68; break;
        case 3:     easySpeed = 68; hardSpeed = 48; break;
        case 4:     easySpeed = 48; hardSpeed = 28; break;
        case 5:     easySpeed = 28; hardSpeed = 20; break;
        case 6:     easySpeed = 20; hardSpeed = 15; break;
        case 7:     easySpeed = 15; hardSpeed = 10; break;
        case 8:     easySpeed = 10; hardSpeed = 7; break;
        case 0:
            easySpeed = Random::Get_Instance().Get_Num(1, 108);
            hardSpeed = Random::Get_Instance().Get_Num(easySpeed, 108);
            break;
        }
    }
    if (!this->writerPlugin->Hacks_Set_Hammer_Bros_Throw_Rate(easySpeed, hardSpeed)) return false;
    if (this->pluginSettings->difficultyHammerBrosNeverJump && !this->writerPlugin->Hacks_Hammer_Bros_Never_Jump()) return false;

    //Handle the Firebar length
    switch (this->pluginSettings->difficultyFireBarLength) {
    case 1:     speed = 4; break; //Very Short
    case 2:     speed = 5; break; //Short
    case 3:     speed = 6; break; //Normal
    case 0:     speed = Random::Get_Instance().Get_Num(4, 6); break;
    }
    if (!this->writerPlugin->Hacks_Set_Firebar_Length(speed)) return false;

    //Handle the Long Firebar length
    switch (this->pluginSettings->difficultyLongFireBarLength) {
    case 1:     speed = 8; break;  //Very Short
    case 2:     speed = 8; break;  //Short
    case 3:     speed = 10; break; //Slightly Short
    case 4:     speed = 12; break; //Normal
    case 0:     speed = Random::Get_Instance().Get_Num(6, 12); break;
    }
    if (!this->writerPlugin->Hacks_Set_Long_Firebar_Length(speed)) return false;

    //Handle the Enemy Revival Speed
    switch (this->pluginSettings->difficultyEnemyRevivalSpeed) {
    default: assert(false); return false;
    case 0:     speed = Random::Get_Instance().Get_Num(3, 21); break;
    case 1:     speed = 21; break;
    case 2:     speed = 16; break;
    case 3:     speed = 12; break;
    case 4:     speed = 3; break;
    }
    if (!this->writerPlugin->Hacks_Set_Enemy_Revival_Time(speed)) return false;
    if (this->pluginSettings->difficultySpeedyObjectsAndEnemies && !this->writerPlugin->Hacks_Speedy_Objects_And_Enemies()) return false;
    return true;
}

bool Hacks_Handler::Handle_Powerup() {
    this->pluginSettings->fireFlowerBouncesLikeAStar = false;
    int powerup = this->pluginSettings->powerup;
    if (powerup == 0) powerup = Random::Get_Instance().Get_Num(1, 15);
    switch (powerup) {
    default:    assert(false); return false;
    case 1:     return true; //Fire Flower
    case 2:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Bone_Caster();
    case 3:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Coin_Flinger();
    case 4:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Cutter_Flower();
    case 5:     this->pluginSettings->fireFlowerBouncesLikeAStar = true; return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Double_Jump_Star();
    case 6:     this->pluginSettings->fireFlowerBouncesLikeAStar = true; return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Fire_Star();
    case 7:     this->pluginSettings->fireFlowerBouncesLikeAStar = true; return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Hammer_Suit();
    case 8:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Luigis_Fire_Flower();
    case 9:     return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Pink_Fury();
    case 10:    return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Poison_Bubbles();
    case 11:    return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Power_Wand();
    case 12:    return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Pyro_Spark_Wand();
    case 13:    return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Slime_Flower();
    case 14:    return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Snowballs();
    case 15:    return this->writerPlugin->Powerups_Replace_Fire_Flower_With_Spinball_Flower();
    }
}

bool Hacks_Handler::Handle_Secondary_Mushroom() {
    //Handle random values first
    int secondaryMushroom = this->pluginSettings->difficultySecondaryMushroom;
    if (secondaryMushroom == 0) { //Random
        secondaryMushroom = Random::Get_Instance().Get_Num(3, 6);
    } else if (secondaryMushroom == 1) { //Random Easy
        secondaryMushroom = 3; //1-Up
        if (Random::Get_Instance().Get_Num(1) == 0) secondaryMushroom = 5; //swimming
    } else  if (secondaryMushroom == 2) { //Random Purist
        secondaryMushroom = 3; //1-Up
        if (Random::Get_Instance().Get_Num(1) == 0) secondaryMushroom = 4; //poison
    }

    //Handle the Mystery Mushroom
    bool randomPalette = false;
    if (secondaryMushroom == 6) {
        randomPalette = true;
        secondaryMushroom = Random::Get_Instance().Get_Num(3, 5); //1-Up, Poison, or Swimming
    }

    //Apply the necessary patch
    bool success = false;
    switch (secondaryMushroom) {
    default:    assert(false); return false;
    case 3:     success = true; break; //1-Up
    case 4:     success = this->writerPlugin->Powerups_Replace_1UP_With_Poison_Mushroom(); break;
    case 5:     success = this->writerPlugin->Powerups_Replace_1UP_With_Swimming_Mushroom(); break;
    }
    if (!success) return false;
    if (!randomPalette) return true;

    //Change the palette if it is a Mystery Mushroom
    if (Random::Get_Instance().Get_Num(1)) return this->writerPlugin->Graphics_Change_1UP_Palette(1); //green
    else return this->writerPlugin->Graphics_Change_1UP_Palette(3); //black
}

bool Hacks_Handler::Handle_Replace_Castle_Loop() {
    this->pluginSettings->difficultyReplaceCastleLoopsCurrent = this->pluginSettings->difficultyReplaceCastleLoops;
    if (this->pluginSettings->difficultyReplaceCastleLoopsCurrent == 0) {
        this->pluginSettings->difficultyReplaceCastleLoopsCurrent = Random::Get_Instance().Get_Num(3, 5); //(no fireflower buffs)
    } else if (this->pluginSettings->difficultyReplaceCastleLoopsCurrent == 1) {
        this->pluginSettings->difficultyReplaceCastleLoopsCurrent = Random::Get_Instance().Get_Num(3, 4); //(only Auto Scroll or Fire Bros)
    }

    bool success = false;
    switch (this->pluginSettings->difficultyReplaceCastleLoopsCurrent) {
    default:
        assert(false); return false;
    case 2: //no complimentary hack
        success = true;
        break;
    case 3:
        success = this->writerPlugin->Hacks_Replace_Castle_Loop_With_Autoscroll_Object();
        break;
    case 4:
        if (!this->writerPlugin->Hacks_Replace_Castle_Loop_With_Fire_Bros()) return false;
        if (this->pluginSettings->difficultyFireBowserThrowsALudicrousAmountOfFireballs && !this->writerPlugin->Hacks_Fire_Bowser_Throws_A_Ludicrous_Amount_Of_Fireballs()) return false;
        success = true;
        break;
    case 5:
        success = this->writerPlugin->Hacks_Replace_Castle_Loop_With_Top_Of_Flagpole_Gives_1UP();
        break;
    case 6:
        success = this->writerPlugin->Hacks_Replace_Castle_Loop_With_Start_With_Fire_Flower();
        break;
    case 7:
        if (!this->writerPlugin->Hacks_Replace_Castle_Loop_With_Top_Of_Flagpole_Gives_1UP()) return false;
        success = this->writerPlugin->Hacks_Replace_Castle_Loop_With_Start_With_Fire_Flower();
        break;
    }
    return success;
}

bool Hacks_Handler::Handle_Text() {
    if (this->pluginSettings->randomText) {
        if (!this->writerPlugin->Hacks_Random_Thank_You_Text()) return false;
        if (!this->writerPlugin->Hacks_Random_But_Our_Princess_Is_In_Another_Castle_Text()) return false;
    }
    if (!this->writerPlugin->Hacks_Set_Mario_Name(this->pluginSettings->marioName)) return false;
    return this->writerPlugin->Hacks_Set_Luigi_Name(this->pluginSettings->luigiName);
}

bool Hacks_Handler::Get_Bool_From_CheckState(Qt::CheckState checkState) {
    switch (checkState) {
    case Qt::Checked:               return true;
    case Qt::Unchecked:             return false;
    case Qt::PartiallyChecked:      return static_cast<bool>(Random::Get_Instance().Get_Num(1));
    }
    assert(false); return false;
}
