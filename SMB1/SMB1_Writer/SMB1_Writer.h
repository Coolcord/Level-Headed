#ifndef SMB1_WRITER_H
#define SMB1_WRITER_H

#include "SMB1_Writer_Interface.h"
#include <QFile>
#include <QByteArray>
#include <QString>

class Level_Offset;
class Midpoint_Writer;
class Object_Buffer;
class Enemy_Buffer;
class Header_Writer;
class Enemy_Bytes_Tracker;
class ROM_Handler;
class Room_ID_Handler;
class Room_Order_Writer;
class Room_Address_Writer;
class Hacks;
class Music;
class Sound;
class Sequential_Archive_Handler;
class Text;
class Graphics;
class Palettes;
class Powerups;

class SMB1_Writer : public SMB1_Writer_Interface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "SMB1_Writer_Interface")
    Q_INTERFACES(SMB1_Writer_Interface)

public:
    SMB1_Writer();
    void Startup(QWidget *parent, const QString &location);
    void Shutdown();
    void Seed_Random_Number_Generator_Instance(const QString &seed);
    QStringList Get_Installed_ROMs();
    QString Install_ROM();
    bool Load_ROM_First_Time(QString &baseRomName);
    bool Load_ROM(const QString &fileName);
    bool New_Level(Level::Level level, int worldNum, int levelNum);
    bool Write_Level();
    int Get_Num_Object_Bytes();
    int Get_Num_Enemy_Bytes();
    QString Get_Output_ROM_Location();
    bool Set_Output_ROM_Location(const QString &location);

    //Header
    bool Header_Get_Current_Attribute(Level_Attribute::Level_Attribute &levelAttribute);
    bool Header_Time(int value);
    bool Header_Autowalk(bool value);
    bool Header_Starting_Position(Level_Attribute::Level_Attribute value);
    bool Header_Background(Background::Background value);
    bool Header_Level_Compliment(Level_Compliment::Level_Compliment value);
    bool Header_Scenery(Scenery::Scenery value);
    bool Header_Brick(Brick::Brick value);
    bool Header_Attribute(Level_Attribute::Level_Attribute attribute);
    bool Header_Midpoint(int value);

    //Room Table
    bool Room_Table_Set_Next_Level(Level::Level level);
    bool Room_Table_Set_Midpoint_For_Duplicate_Level(Level::Level level, int worldNum, int levelNum);

    //Objects
    bool Object_Question_Block_With_Mushroom(int x, int y);
    bool Object_Question_Block_With_Coin(int x, int y);
    bool Object_Hidden_Block_With_Coin(int x, int y);
    bool Object_Hidden_Block_With_1up(int x, int y);
    bool Object_Brick_With_Mushroom(int x, int y);
    bool Object_Brick_With_Vine(int x, int y);
    bool Object_Brick_With_Star(int x, int y);
    bool Object_Brick_With_10_Coins(int x, int y);
    bool Object_Brick_With_1up(int x, int y);
    bool Object_Underwater_Sideways_Pipe(int x, int y);
    bool Object_Used_Block(int x, int y);
    bool Object_Trampoline(int x, int y);
    bool Object_Cannon(int x, int y, int height);
    bool Object_Island(int x, int y, int length);
    bool Object_Horizontal_Bricks(int x, int y, int length);
    bool Object_Horizontal_Blocks(int x, int y, int length);
    bool Object_Horizontal_Coins(int x, int y, int length);
    bool Object_Vertical_Bricks(int x, int y, int height);
    bool Object_Vertical_Blocks(int x, int y, int height);
    bool Object_Coral(int x, int y, int height);
    bool Object_Pipe(int x, int y, int height, bool enterable = false);
    bool Object_Hole(int x, int length, bool filledWithWater = false);
    bool Object_Bridge(int x, int yPlacement, int length);
    bool Object_Horizontal_Question_Blocks_With_Coins(int x, int yPlacement, int length);
    bool Object_Page_Change(int page);
    bool Object_Reverse_L_Pipe(int x);
    bool Object_Flagpole(int x);
    bool Object_Castle(int x);
    bool Object_Big_Castle(int x);
    bool Object_Axe(int x);
    bool Object_Axe_Rope(int x);
    bool Object_Bowser_Bridge(int x);
    bool Object_Scroll_Stop(int x, bool warpZone = false);
    bool Object_Toggle_Auto_Scroll(int x);
    bool Object_Flying_Cheep_Cheep_Spawner(int x);
    bool Object_Swimming_Cheep_Cheep_Spawner(int x);
    bool Object_Bullet_Bill_Spawner(int x);
    bool Object_Cancel_Spawner(int x);
    bool Object_Loop_Command(int x);
    bool Object_Change_Brick_And_Scenery(int x, Brick::Brick brick, Scenery::Scenery scenery);
    bool Object_Change_Background(int x, Background::Background background);
    bool Object_Lift_Rope(int x);
    bool Object_Balance_Lift_Vertical_Rope(int x, int length);
    bool Object_Balance_Lift_Horizontal_Rope(int x, int length);
    bool Object_Steps(int x, int width);
    bool Object_End_Steps(int x);
    bool Object_Tall_Reverse_L_Pipe(int x, int yPlacement);
    bool Object_Pipe_Wall(int x);
    bool Object_Nothing(int x);

    //Enemies
    bool Enemy_Random_Continous_Enemy_Spawner(int x, bool onlyHardMode);
    bool Enemy_Random_Enemy(int x, int y, bool onlyHardMode, bool allowHammerBros, bool allowLakitus, bool allowContinousEnemySpawners);
    bool Enemy_Random_Enemy_Group(int x, int y, int num, bool onlyHardMode);
    bool Enemy_Random_Fire_Bar(int x, int y, bool onlyHardMode);
    bool Enemy_Random_Flying_Enemy(int x, bool onlyHardMode);
    bool Enemy_Random_Underwater_Enemy(int x, int y, bool onlyHardMode, bool allowHammerBros);
    bool Enemy_Green_Koopa(int x, int y, bool moving = true, bool onlyHardMode = false);
    bool Enemy_Red_Koopa(int x, int y, bool onlyHardMode = false);
    bool Enemy_Buzzy_Beetle(int x, int y, bool onlyHardMode = false);
    bool Enemy_Hammer_Bro(int x, int y, bool onlyHardMode = false);
    bool Enemy_Goomba(int x, int y, bool onlyHardMode = false);
    bool Enemy_Blooper(int x, int y, bool onlyHardMode = false);
    bool Enemy_Bullet_Bill(int x, int y, bool onlyHardMode = false);
    bool Enemy_Green_Paratroopa(int x, int y, bool moving = true, bool leaping = true, bool onlyHardMode = false);
    bool Enemy_Red_Paratroopa(int x, int y, bool onlyHardMode = false);
    bool Enemy_Green_Cheep_Cheep(int x, int y, bool onlyHardMode = false);
    bool Enemy_Red_Cheep_Cheep(int x, int y, bool onlyHardMode = false);
    bool Enemy_Podoboo(int x, bool onlyHardMode = false);
    bool Enemy_Piranha_Plant(int x, int y, bool onlyHardMode = false);
    bool Enemy_Lakitu(int x, int y, bool onlyHardMode = false);
    bool Enemy_Spiny(int x, int y, bool onlyHardMode = false);
    bool Enemy_Bowser_Fire_Spawner(int x, bool onlyHardMode = false);
    bool Enemy_Swimming_Cheep_Cheep_Spawner(int x, bool leaping = false, bool onlyHardMode = false);
    bool Enemy_Bullet_Bill_Spawner(int x, bool onlyHardMode = false);
    bool Enemy_Fire_Bar(int x, int y, bool clockwise = true, bool fast = false, bool onlyHardMode = false);
    bool Enemy_Large_Fire_Bar(int x, int y, bool onlyHardMode = false);
    bool Enemy_Lift(int x, int y, bool vertical = true, bool onlyHardMode = false);
    bool Enemy_Falling_Lift(int x, int y, bool onlyHardMode = false);
    bool Enemy_Balance_Lift(int x, int y, bool onlyHardMode = false);
    bool Enemy_Surfing_Lift(int x, int y, bool onlyHardMode = false);
    bool Enemy_Lift_Spawner(int x, int y, bool up = true, bool small = false, bool onlyHardMode = false);
    bool Enemy_Bowser(int x, bool onlyHardMode = false);
    bool Enemy_Warp_Zone(int x);
    bool Enemy_Toad(int x, bool onlyHardMode = false);
    bool Enemy_Goomba_Group(int x, int y, int num, bool onlyHardMode = false);
    bool Enemy_Koopa_Group(int x, int y, int num, bool onlyHardMode = false);
    bool Enemy_Page_Change(int page);
    bool Enemy_Pipe_Pointer(int x, const QString &levelSlot, int world, int page);

    //Transfer Bytes Between Levels
    bool Send_Object_Bytes_From_One_Level_To_Another(Level::Level fromLevel, Level::Level toLevel, int numBytes);
    bool Send_Enemy_Bytes_From_One_Level_To_Another(Level::Level fromLevel, Level::Level toLevel, int numBytes);

    //Hacks
    bool Hacks_Add_Luigi_Game();
    bool Hacks_Always_Autoscroll();
    bool Hacks_Always_Force_Continue_Code();
    bool Hacks_Black_Piranha_Plants();
    bool Hacks_Destroy_Bowser_Bridge_Randomly();
    bool Hacks_Disable_Intro_Demo();
    bool Hacks_Disable_One_Up_Coin_Limits();
    bool Hacks_Enable_God_Mode();
    bool Hacks_Enable_Hitting_Underwater_Blocks();
    bool Hacks_Enable_Piranha_Plants_On_First_Level();
    bool Hacks_Enable_Walking_Hammer_Bros(int difficulty);
    bool Hacks_European_Blooper_Swim_Height();
    bool Hacks_Fix_Lakitu_Throw_Arc();
    bool Hacks_Fix_Life_Counter_Bugs();
    bool Hacks_Hard_Mode_Does_Not_Affect_Lift_Size();
    bool Hacks_Infinite_Lives();
    bool Hacks_Permadeath();
    bool Hacks_Random_Goomba_Group_Enemy(bool allowHammerBros);
    bool Hacks_Random_Koopa_Group_Enemy(bool allowHammerBros);
    bool Hacks_Random_Intro_Demo();
    bool Hacks_Random_True_Bowser_Characters();
    bool Hacks_Real_Time();
    bool Hacks_Red_Piranha_Plants();
    bool Hacks_Remove_Vertical_Object_Limit();
    bool Hacks_Replace_Castle_Loop_With_Autoscroll_Object();
    bool Hacks_Replace_Castle_Loop_With_Fire_Bros();
    bool Hacks_Replace_Castle_Loop_With_Start_With_Fire_Flower();
    bool Hacks_Replace_Castle_Loop_With_Top_Of_Flagpole_Gives_1UP();
    bool Hacks_Replace_Mario_With_Luigi();
    bool Hacks_Set_Basic_Enemy_Speed(int speed);
    bool Hacks_Set_Bowser_Bridge_Destruction_Speed(int speed);
    bool Hacks_Set_Bowser_Flame_Frequency(int frequency);
    bool Hacks_Set_Brick_Break_Animation_Bounce_Height(int lowerHeight, int upperHeight);
    bool Hacks_Set_Bullet_Bill_Firing_Rate(int rate);
    bool Hacks_Set_Bullet_Bill_Speed(int speed);
    bool Hacks_Set_Coin_Animation_Bounce_Height(int height);
    bool Hacks_Set_Death_Animation_Jump_Height(int height);
    bool Hacks_Set_Enemy_Group_Size(int value);
    bool Hacks_Set_Enemy_Revival_Time(int speed);
    bool Hacks_Set_Firebar_Length(int length);
    bool Hacks_Set_Flying_Cheep_Cheep_Jump_Height(int height);
    bool Hacks_Set_Long_Firebar_Length(int length);
    bool Hacks_Set_Lakitu_Respawn_Speed(int value);
    bool Hacks_Set_Luigi_Name(const QString &name);
    bool Hacks_Set_Mario_Name(const QString &name);
    bool Hacks_Set_Maximum_Number_Of_Pirahna_Plants(int value);
    bool Hacks_Set_Number_Of_Worlds(int value);
    bool Hacks_Set_Number_Of_Levels_Per_World(int value);
    bool Hacks_Set_Starting_Lives(int lives);
    bool Hacks_Set_Surfing_Lift_Speed(int speed);
    bool Hacks_Set_Hammer_Bros_Throw_Rate(int easyRate, int hardRate);
    bool Hacks_Speedy_Objects_And_Enemies();
    bool Hacks_Spiny_Eggs_Bouncy();
    bool Hacks_Spiny_Eggs_Chase_Mario();
    bool Hacks_Spiny_Eggs_Do_Not_Break();
    bool Hacks_Spiny_Eggs_Explode_Into_Flames();
    bool Hacks_Spiny_Eggs_No_Eggs();
    bool Hacks_Taking_Damage_As_Fire_Reverts_To_Super();
    bool Hacks_Unlimited_Time();
    bool Hacks_Was_Vertical_Object_Limit_Removed();
    bool Hacks_Write_Watermark();

    //Music Functions
    QStringList Music_Get_Bonus_Music_Packs();
    QStringList Music_Get_Music_Packs();
    bool Music_Apply_Music_Pack(int index);
    int Music_Get_Number_Of_Bonus_Music_Packs();
    int Music_Get_Number_Of_Music_Packs();
    bool Music_Disable();
    bool Music_Set_Combine_Music_Packs(bool combineMusicPacks);
    bool Music_Tone_Color_1();
    bool Music_Tone_Color_2();
    bool Music_Tone_Color_3();
    bool Music_Tone_Color_4();
    bool Music_Tone_Color_5();
    bool Music_Tone_Color_6();
    bool Music_Tone_Color_7();
    bool Music_Tone_Color_8();
    bool Music_Tone_Color_9();
    bool Music_Tone_Color_10();
    bool Music_Tone_Color_11();
    bool Music_Tone_Color_12();
    bool Music_Tone_Color_13();
    bool Music_Tone_Color_14();
    bool Music_Tone_Color_15();
    bool Music_Tone_Color_16();
    bool Music_Tone_Color_17();
    bool Music_Tone_Color_18();

    //Sound
    bool Sound_Randomize_Sounds();

    //Powerups
    bool Powerups_Replace_1UP_With_Poison_Mushroom();
    bool Powerups_Replace_1UP_With_Swimming_Mushroom();
    bool Powerups_Replace_Fire_Flower_With_Bone_Caster();
    bool Powerups_Replace_Fire_Flower_With_Coin_Flinger();
    bool Powerups_Replace_Fire_Flower_With_Cutter_Flower();
    bool Powerups_Replace_Fire_Flower_With_Double_Jump_Star();
    bool Powerups_Replace_Fire_Flower_With_Fire_Star();
    bool Powerups_Replace_Fire_Flower_With_Hammer_Suit();
    bool Powerups_Replace_Fire_Flower_With_Luigis_Fire_Flower();
    bool Powerups_Replace_Fire_Flower_With_Pink_Fury();
    bool Powerups_Replace_Fire_Flower_With_Poison_Bubbles();
    bool Powerups_Replace_Fire_Flower_With_Power_Wand();
    bool Powerups_Replace_Fire_Flower_With_Slime_Flower();
    bool Powerups_Replace_Fire_Flower_With_Spinball_Flower();

    //Graphics
    QString Graphics_Get_Last_Applied_Graphics_Pack();
    QString Graphics_Get_Last_Applied_Mario_Sprite();
    QStringList Graphics_Get_Bonus_Graphics_Packs();
    QStringList Graphics_Get_Bonus_Mario_Sprites();
    QStringList Graphics_Get_Graphics_Packs();
    QStringList Graphics_Get_Mario_Sprites();
    bool Graphics_Apply_Graphics_Pack(int index);
    bool Graphics_Apply_Mario_Palette_Present(int present);
    bool Graphics_Apply_Mario_Sprite(int index);
    bool Graphics_Was_Mario_Sprite_A_Bonus_Sprite();
    bool Graphics_Combine_Graphics(); //only call this with the original graphics
    bool Graphics_Combine_Mario();
    int Graphics_Get_Number_Of_Bonus_Graphics_Packs();
    int Graphics_Get_Number_Of_Bonus_Mario_Sprites();
    int Graphics_Get_Number_Of_Graphics_Packs();
    int Graphics_Get_Number_Of_Mario_Sprites();
    bool Graphics_Randomize_Palettes(int paletteMode);
    bool Graphics_Randomize_Mario_Sprite_Palette();
    bool Graphics_Set_Combine_Graphics_Packs(bool combineGraphicsPacks);
    bool Graphics_Change_1UP_Palette(int palette);
    bool Graphics_Write_Title_Screen_For_1_Player_Game();
    bool Graphics_Write_Title_Screen_For_2_Player_Game();
    bool Graphics_Write_Title_Screen_For_Partial_Game();

private:
    bool Load_ROM_Offsets(bool cancel);
    bool Create_ROM_Directory();
    bool Are_Buffers_Allocated();
    void Deallocate_Buffers();
    bool Write_Buffer(const qint64 offset, QByteArray *buffer);
    bool Read_Level_Header();
    bool Read_Objects();
    bool Read_Enemies();

    int numObjectBytes;
    qint64 objectOffset;
    qint64 enemyOffset;
    QFile *file;
    QByteArray *headerBuffer;
    QByteArray *objectsBuffer;
    QByteArray *enemiesBuffer;
    Level_Offset *levelOffset;
    Midpoint_Writer *midpointWriter;
    Object_Buffer *objectBuffer;
    Enemy_Buffer *enemyBuffer;
    Header_Writer *headerWriter;
    Enemy_Bytes_Tracker *enemyBytesTracker;
    Room_ID_Handler *roomIDHandler;
    Room_Order_Writer *roomOrderWriter;
    Room_Address_Writer *roomAddressWriter;
    Hacks *hacks;
    Music *music;
    Sound *sound;
    Sequential_Archive_Handler *sequentialArchiveHandler;
    Text *text;
    Graphics *graphics;
    Palettes *palettes;
    Powerups *powerups;
    ROM_Handler *romHandler;
    QString applicationLocation;
    QString romFileName;
    QWidget *parent;
};

#endif // SMB1_WRITER_H
