#ifndef SMB1_WRITER_INTERFACE_H
#define SMB1_WRITER_INTERFACE_H

#include "../Common_SMB1_Files/Level.h"
#include "../Common_SMB1_Files/Level_Compliment.h"
#include "../Common_SMB1_Files/Level_Attribute.h"
#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Scenery.h"
#include <QObject>
#include <QtPlugin>
#include <QString>
#include <QWidget>

class SMB1_Writer_Interface : public QObject {
public:
    virtual void Startup(QWidget *parent, const QString &location)=0;
    virtual void Shutdown()=0;
    virtual void Seed_Random_Number_Generator_Instance(const QString &seed)=0;
    virtual QStringList Get_Installed_ROMs()=0;
    virtual QString Install_ROM()=0;
    virtual bool Load_ROM_First_Time(QString &baseRomName)=0;
    virtual bool Load_ROM(const QString &fileName)=0;
    virtual bool New_Level(Level::Level level, int worldNum, int levelNum)=0;
    virtual bool Write_Level()=0;
    virtual int Get_Num_Object_Bytes()=0;
    virtual int Get_Num_Enemy_Bytes()=0;
    virtual QString Get_Output_ROM_Location()=0;
    virtual bool Set_Output_ROM_Location(const QString &location)=0;

    //Header
    virtual bool Header_Get_Current_Attribute(Level_Attribute::Level_Attribute &levelAttribute)=0;
    virtual bool Header_Time(int value)=0;
    virtual bool Header_Autowalk(bool value)=0;
    virtual bool Header_Starting_Position(Level_Attribute::Level_Attribute value)=0;
    virtual bool Header_Background(Background::Background value)=0;
    virtual bool Header_Level_Compliment(Level_Compliment::Level_Compliment value)=0;
    virtual bool Header_Scenery(Scenery::Scenery value)=0;
    virtual bool Header_Brick(Brick::Brick value)=0;
    virtual bool Header_Attribute(Level_Attribute::Level_Attribute attribute)=0;
    virtual bool Header_Midpoint(int value)=0;

    //Room Table
    virtual bool Room_Table_Set_Next_Level(Level::Level level)=0;
    virtual bool Room_Table_Set_Midpoint_For_Duplicate_Level(Level::Level level, int worldNum, int levelNum)=0;

    //Objects
    virtual bool Object_Question_Block_With_Mushroom(int x, int y)=0;
    virtual bool Object_Question_Block_With_Coin(int x, int y)=0;
    virtual bool Object_Hidden_Block_With_Coin(int x, int y)=0;
    virtual bool Object_Hidden_Block_With_1up(int x, int y)=0;
    virtual bool Object_Brick_With_Mushroom(int x, int y)=0;
    virtual bool Object_Brick_With_Vine(int x, int y)=0;
    virtual bool Object_Brick_With_Star(int x, int y)=0;
    virtual bool Object_Brick_With_10_Coins(int x, int y)=0;
    virtual bool Object_Brick_With_1up(int x, int y)=0;
    virtual bool Object_Underwater_Sideways_Pipe(int x, int y)=0;
    virtual bool Object_Used_Block(int x, int y)=0;
    virtual bool Object_Trampoline(int x, int y)=0;
    virtual bool Object_Cannon(int x, int y, int height)=0;
    virtual bool Object_Island(int x, int y, int length)=0;
    virtual bool Object_Horizontal_Bricks(int x, int y, int length)=0;
    virtual bool Object_Horizontal_Blocks(int x, int y, int length)=0;
    virtual bool Object_Horizontal_Coins(int x, int y, int length)=0;
    virtual bool Object_Vertical_Bricks(int x, int y, int height)=0;
    virtual bool Object_Vertical_Blocks(int x, int y, int height)=0;
    virtual bool Object_Coral(int x, int y, int height)=0;
    virtual bool Object_Pipe(int x, int y, int height, bool enterable = false)=0;
    virtual bool Object_Hole(int x, int length, bool filledWithWater = false)=0;
    virtual bool Object_Bridge(int x, int yPlacement, int length)=0;
    virtual bool Object_Horizontal_Question_Blocks_With_Coins(int x, int yPlacement, int length)=0;
    virtual bool Object_Page_Change(int page)=0;
    virtual bool Object_Reverse_L_Pipe(int x)=0;
    virtual bool Object_Flagpole(int x)=0;
    virtual bool Object_Castle(int x)=0;
    virtual bool Object_Big_Castle(int x)=0;
    virtual bool Object_Axe(int x)=0;
    virtual bool Object_Axe_Rope(int x)=0;
    virtual bool Object_Bowser_Bridge(int x)=0;
    virtual bool Object_Scroll_Stop(int x, bool warpZone = false)=0;
    virtual bool Object_Toggle_Auto_Scroll(int x)=0;
    virtual bool Object_Flying_Cheep_Cheep_Spawner(int x)=0;
    virtual bool Object_Swimming_Cheep_Cheep_Spawner(int x)=0;
    virtual bool Object_Bullet_Bill_Spawner(int x)=0;
    virtual bool Object_Cancel_Spawner(int x)=0;
    virtual bool Object_Loop_Command(int x)=0;
    virtual bool Object_Change_Brick_And_Scenery(int x, Brick::Brick brick, Scenery::Scenery scenery)=0;
    virtual bool Object_Change_Background(int x, Background::Background background)=0;
    virtual bool Object_Lift_Rope(int x)=0;
    virtual bool Object_Balance_Lift_Vertical_Rope(int x, int length)=0;
    virtual bool Object_Balance_Lift_Horizontal_Rope(int x, int length)=0;
    virtual bool Object_Steps(int x, int width)=0;
    virtual bool Object_End_Steps(int x)=0;
    virtual bool Object_Tall_Reverse_L_Pipe(int x, int yPlacement)=0;
    virtual bool Object_Pipe_Wall(int x)=0;
    virtual bool Object_Nothing(int x)=0;

    //Enemies
    virtual bool Enemy_Random_Continous_Enemy_Spawner(int x, bool onlyHardMode)=0;
    virtual bool Enemy_Random_Enemy(int x, int y, bool onlyHardMode, bool allowHammerBros, bool allowLakitus, bool allowContinousEnemySpawners)=0;
    virtual bool Enemy_Random_Enemy_Group(int x, int y, int num, bool onlyHardMode)=0;
    virtual bool Enemy_Random_Flying_Enemy(int x, bool onlyHardMode)=0;
    virtual bool Enemy_Random_Fire_Bar(int x, int y, bool onlyHardMode)=0;
    virtual bool Enemy_Random_Underwater_Enemy(int x, int y, bool onlyHardMode, bool allowHammerBros)=0;
    virtual bool Enemy_Green_Koopa(int x, int y, bool moving = true, bool onlyHardMode = false)=0;
    virtual bool Enemy_Red_Koopa(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Buzzy_Beetle(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Hammer_Bro(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Goomba(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Blooper(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Bullet_Bill(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Green_Paratroopa(int x, int y, bool moving = true, bool leaping = true, bool onlyHardMode = false)=0;
    virtual bool Enemy_Red_Paratroopa(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Green_Cheep_Cheep(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Red_Cheep_Cheep(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Podoboo(int x, bool onlyHardMode = false)=0;
    virtual bool Enemy_Piranha_Plant(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Lakitu(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Spiny(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Bowser_Fire_Spawner(int x, bool onlyHardMode = false)=0;
    virtual bool Enemy_Swimming_Cheep_Cheep_Spawner(int x, bool leaping = false, bool onlyHardMode = false)=0;
    virtual bool Enemy_Bullet_Bill_Spawner(int x, bool onlyHardMode = false)=0;
    virtual bool Enemy_Fire_Bar(int x, int y, bool clockwise = true, bool fast = false, bool onlyHardMode = false)=0;
    virtual bool Enemy_Large_Fire_Bar(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Lift(int x, int y, bool vertical = true, bool onlyHardMode = false)=0;
    virtual bool Enemy_Falling_Lift(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Balance_Lift(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Surfing_Lift(int x, int y, bool onlyHardMode = false)=0;
    virtual bool Enemy_Lift_Spawner(int x, int y, bool up = true, bool small = false, bool onlyHardMode = false)=0;
    virtual bool Enemy_Bowser(int x, bool onlyHardMode = false)=0;
    virtual bool Enemy_Warp_Zone(int x)=0;
    virtual bool Enemy_Toad(int x, bool onlyHardMode = false)=0;
    virtual bool Enemy_Goomba_Group(int x, int y, int num, bool onlyHardMode = false)=0;
    virtual bool Enemy_Koopa_Group(int x, int y, int num, bool onlyHardMode = false)=0;
    virtual bool Enemy_Page_Change(int page)=0;
    virtual bool Enemy_Pipe_Pointer(int x, const QString &levelSlot, int world, int page)=0;

    //Transfer Bytes Between Levels
    virtual bool Send_Object_Bytes_From_One_Level_To_Another(Level::Level fromLevel, Level::Level toLevel, int numBytes)=0;
    virtual bool Send_Enemy_Bytes_From_One_Level_To_Another(Level::Level fromLevel, Level::Level toLevel, int numBytes)=0;

    //Hacks
    virtual bool Hacks_Add_Luigi_Game()=0;
    virtual bool Hacks_Always_Autoscroll()=0;
    virtual bool Hacks_Always_Force_Continue_Code()=0;
    virtual bool Hacks_Black_Piranha_Plants()=0;
    virtual bool Hacks_Destroy_Bowser_Bridge_Randomly()=0;
    virtual bool Hacks_Disable_Intro_Demo()=0;
    virtual bool Hacks_Enable_God_Mode()=0;
    virtual bool Hacks_Enable_Hitting_Underwater_Blocks()=0;
    virtual bool Hacks_Enable_Piranha_Plants_On_First_Level()=0;
    virtual bool Hacks_Enable_Walking_Hammer_Bros(int difficulty)=0;
    virtual bool Hacks_European_Blooper_Swim_Height()=0;
    virtual bool Hacks_Fix_Lakitu_Throw_Arc()=0;
    virtual bool Hacks_Fix_Life_Counter_Bugs()=0;
    virtual bool Hacks_Hard_Mode_Does_Not_Affect_Lift_Size()=0;
    virtual bool Hacks_Infinite_Lives()=0;
    virtual bool Hacks_Permadeath()=0;
    virtual bool Hacks_Random_Goomba_Group_Enemy(bool allowHammerBros)=0;
    virtual bool Hacks_Random_Koopa_Group_Enemy(bool allowHammerBros)=0;
    virtual bool Hacks_Random_Intro_Demo()=0;
    virtual bool Hacks_Random_True_Bowser_Characters()=0;
    virtual bool Hacks_Real_Time()=0;
    virtual bool Hacks_Red_Piranha_Plants()=0;
    virtual bool Hacks_Remove_Vertical_Object_Limit()=0;
    virtual bool Hacks_Replace_Castle_Loop_With_Autoscroll_Object()=0;
    virtual bool Hacks_Replace_Castle_Loop_With_Fire_Bros()=0;
    virtual bool Hacks_Replace_Castle_Loop_With_Start_With_Fire_Flower()=0;
    virtual bool Hacks_Replace_Castle_Loop_With_Top_Of_Flagpole_Gives_1UP()=0;
    virtual bool Hacks_Replace_Mario_With_Luigi()=0;
    virtual bool Hacks_Set_Basic_Enemy_Speed(int speed)=0;
    virtual bool Hacks_Set_Bowser_Bridge_Destruction_Speed(int speed)=0;
    virtual bool Hacks_Set_Bowser_Flame_Frequency(int frequency)=0;
    virtual bool Hacks_Set_Brick_Break_Animation_Bounce_Height(int lowerHeight, int upperHeight)=0;
    virtual bool Hacks_Set_Bullet_Bill_Firing_Rate(int rate)=0;
    virtual bool Hacks_Set_Bullet_Bill_Speed(int speed)=0;
    virtual bool Hacks_Set_Coin_Animation_Bounce_Height(int height)=0;
    virtual bool Hacks_Set_Death_Animation_Jump_Height(int height)=0;
    virtual bool Hacks_Set_Enemy_Group_Size(int value)=0;
    virtual bool Hacks_Set_Enemy_Revival_Time(int speed)=0;
    virtual bool Hacks_Set_Firebar_Length(int length)=0;
    virtual bool Hacks_Set_Flying_Cheep_Cheep_Jump_Height(int height)=0;
    virtual bool Hacks_Set_Long_Firebar_Length(int length)=0;
    virtual bool Hacks_Set_Lakitu_Respawn_Speed(int value)=0;
    virtual bool Hacks_Set_Luigi_Name(const QString &name)=0;
    virtual bool Hacks_Set_Mario_Name(const QString &name)=0;
    virtual bool Hacks_Set_Maximum_Number_Of_Pirahna_Plants(int value)=0;
    virtual bool Hacks_Set_Number_Of_Worlds(int value)=0;
    virtual bool Hacks_Set_Number_Of_Levels_Per_World(int value)=0;
    virtual bool Hacks_Set_Starting_Lives(int lives)=0;
    virtual bool Hacks_Set_Surfing_Lift_Speed(int speed)=0;
    virtual bool Hacks_Set_Hammer_Bros_Throw_Rate(int easyRate, int hardRate)=0;
    virtual bool Hacks_Speedy_Objects_And_Enemies()=0;
    virtual bool Hacks_Spiny_Eggs_Bouncy()=0;
    virtual bool Hacks_Spiny_Eggs_Chase_Mario()=0;
    virtual bool Hacks_Spiny_Eggs_Do_Not_Break()=0;
    virtual bool Hacks_Spiny_Eggs_Explode_Into_Flames()=0;
    virtual bool Hacks_Spiny_Eggs_No_Eggs()=0;
    virtual bool Hacks_Taking_Damage_As_Fire_Reverts_To_Super()=0;
    virtual bool Hacks_Unlimited_Time()=0;
    virtual bool Hacks_Was_Vertical_Object_Limit_Removed()=0;
    virtual bool Hacks_Write_Watermark()=0;

    //Music Functions
    virtual QStringList Music_Get_Bonus_Music_Packs()=0;
    virtual QStringList Music_Get_Music_Packs()=0;
    virtual bool Music_Apply_Music_Pack(int index)=0;
    virtual int Music_Get_Number_Of_Bonus_Music_Packs()=0;
    virtual int Music_Get_Number_Of_Music_Packs()=0;
    virtual bool Music_Disable()=0;
    virtual bool Music_Set_Combine_Music_Packs(bool combineMusicPacks)=0;
    virtual bool Music_Tone_Color_1()=0;
    virtual bool Music_Tone_Color_2()=0;
    virtual bool Music_Tone_Color_3()=0;
    virtual bool Music_Tone_Color_4()=0;
    virtual bool Music_Tone_Color_5()=0;
    virtual bool Music_Tone_Color_6()=0;
    virtual bool Music_Tone_Color_7()=0;
    virtual bool Music_Tone_Color_8()=0;
    virtual bool Music_Tone_Color_9()=0;
    virtual bool Music_Tone_Color_10()=0;
    virtual bool Music_Tone_Color_11()=0;
    virtual bool Music_Tone_Color_12()=0;
    virtual bool Music_Tone_Color_13()=0;
    virtual bool Music_Tone_Color_14()=0;
    virtual bool Music_Tone_Color_15()=0;
    virtual bool Music_Tone_Color_16()=0;
    virtual bool Music_Tone_Color_17()=0;
    virtual bool Music_Tone_Color_18()=0;

    //Sound
    virtual bool Sound_Randomize_Sounds()=0;

    //Powerups
    virtual bool Powerups_Replace_1UP_With_Poison_Mushroom()=0;
    virtual bool Powerups_Replace_1UP_With_Poison_Mushroom_If_Not_Fire_Mario()=0;
    virtual bool Powerups_Replace_1UP_With_Swimming_Mushroom()=0;
    virtual bool Powerups_Replace_Fire_Flower_With_Bone_Caster()=0;
    virtual bool Powerups_Replace_Fire_Flower_With_Coin_Flinger()=0;
    virtual bool Powerups_Replace_Fire_Flower_With_Cutter_Flower()=0;
    virtual bool Powerups_Replace_Fire_Flower_With_Double_Jump_Star()=0;
    virtual bool Powerups_Replace_Fire_Flower_With_Fire_Star()=0;
    virtual bool Powerups_Replace_Fire_Flower_With_Hammer_Suit()=0;
    virtual bool Powerups_Replace_Fire_Flower_With_Luigis_Fire_Flower()=0;
    virtual bool Powerups_Replace_Fire_Flower_With_Pink_Fury()=0;
    virtual bool Powerups_Replace_Fire_Flower_With_Poison_Bubbles()=0;
    virtual bool Powerups_Replace_Fire_Flower_With_Power_Wand()=0;
    virtual bool Powerups_Replace_Fire_Flower_With_Slime_Flower()=0;
    virtual bool Powerups_Replace_Fire_Flower_With_Spinball_Flower()=0;

    //Graphics
    virtual QString Graphics_Get_Last_Applied_Graphics_Pack()=0;
    virtual QString Graphics_Get_Last_Applied_Mario_Sprite()=0;
    virtual QStringList Graphics_Get_Bonus_Graphics_Packs()=0;
    virtual QStringList Graphics_Get_Bonus_Mario_Sprites()=0;
    virtual QStringList Graphics_Get_Graphics_Packs()=0;
    virtual QStringList Graphics_Get_Mario_Sprites()=0;
    virtual bool Graphics_Apply_Graphics_Pack(int index)=0;
    virtual bool Graphics_Apply_Mario_Palette_Present(int present)=0;
    virtual bool Graphics_Apply_Mario_Sprite(int index)=0;
    virtual bool Graphics_Was_Mario_Sprite_A_Bonus_Sprite()=0;
    virtual bool Graphics_Combine_Graphics()=0; //only call this with the original graphics
    virtual int Graphics_Get_Number_Of_Bonus_Graphics_Packs()=0;
    virtual int Graphics_Get_Number_Of_Bonus_Mario_Sprites()=0;
    virtual int Graphics_Get_Number_Of_Graphics_Packs()=0;
    virtual int Graphics_Get_Number_Of_Mario_Sprites()=0;
    virtual bool Graphics_Randomize_Palettes(int paletteMode)=0;
    virtual bool Graphics_Randomize_Mario_Sprite_Palette()=0;
    virtual bool Graphics_Set_Combine_Graphics_Packs(bool combineGraphicsPacks)=0;
    virtual bool Graphics_Change_1UP_Palette(int palette)=0;
    virtual bool Graphics_Write_Title_Screen_For_1_Player_Game()=0;
    virtual bool Graphics_Write_Title_Screen_For_2_Player_Game()=0;
    virtual bool Graphics_Write_Title_Screen_For_Partial_Game()=0;
};

Q_DECLARE_INTERFACE(SMB1_Writer_Interface, "SMB1_Writer_Interface")

#endif // SMB1_WRITER_INTERFACE_H
