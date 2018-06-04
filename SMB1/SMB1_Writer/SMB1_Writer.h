#ifndef SMB1_WRITER_H
#define SMB1_WRITER_H

#include "SMB1_Writer_Interface.h"
#include <QFile>
#include <QByteArray>
#include <QString>

class Level_Offset;
class Midpoint_Writer;
class Object_Writer;
class Enemy_Writer;
class Header_Writer;
class Enemy_Bytes_Tracker;
class ROM_Handler;
class Room_ID_Handler;
class Room_Order_Writer;
class Room_Address_Writer;
class Hacks;
class Music;
class Text;
class Graphics;

class SMB1_Writer : public SMB1_Writer_Interface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "SMB1_Writer_Interface")
    Q_INTERFACES(SMB1_Writer_Interface)

public:
    SMB1_Writer();
    void Startup(QWidget *parent, const QString &location);
    void Shutdown();
    QStringList Get_Installed_ROMs();
    QString Install_ROM();
    bool Load_ROM();
    bool Load_ROM(const QString &fileName);
    bool New_Level(Level::Level level);
    bool Write_Level();
    int Get_Num_Object_Bytes();
    int Get_Num_Enemy_Bytes();
    bool Set_Number_Of_Worlds(int value);
    QString Get_Output_ROM_Location();
    bool Set_Output_ROM_Location(const QString &location);

    //Header
    bool Header_Time(int value);
    bool Header_Autowalk(bool value);
    bool Header_Starting_Position(Level_Attribute::Level_Attribute value);
    bool Header_Background(Background::Background value);
    bool Header_Level_Compliment(Level_Compliment::Level_Compliment value);
    bool Header_Scenery(Scenery::Scenery value);
    bool Header_Brick(Brick::Brick value);
    bool Header_Attribute(Level_Attribute::Level_Attribute attribute);
    bool Header_Midpoint(int value);
    bool Header_Midpoint(Level::Level level, int value);

    //Room Table
    bool Room_Table_Set_Next_Level(Level::Level level);
    bool Room_Table_Set_Number_Of_Worlds(int value);

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
    bool Object_Corral(int x, int y, int height);
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
    bool Enemy_Pirana_Plant(int x, int y, bool onlyHardMode = false);
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
    bool Enemy_Pipe_Pointer(int x, const QString &levelSlot, int page);
    bool Enemy_Nothing(int x);

    //Transfer Bytes Between Levels
    bool Send_Object_Bytes_From_One_Level_To_Another(Level::Level fromLevel, Level::Level toLevel, int numBytes);
    bool Send_Enemy_Bytes_From_One_Level_To_Another(Level::Level fromLevel, Level::Level toLevel, int numBytes);

    //Hacks
    bool Hacks_Add_Luigi_Game();
    bool Hacks_Always_Autoscroll();
    bool Hacks_Enable_God_Mode();
    bool Hacks_Enable_Hitting_Underwater_Blocks();
    bool Hacks_Enable_Piranha_Plants_On_First_Level();
    bool Hacks_Fast_Enemies(int speed);
    bool Hacks_Fix_Lakitu_Throw_Arc();
    bool Hacks_Hard_Mode_Does_Not_Affect_Lift_Size();
    bool Hacks_Infinite_Lives();
    bool Hacks_Replace_Castle_Loop_With_Autoscroll_Object(int overworldSpeed, int undergroundSpeed, int underwaterSpeed, int castleSpeed);
    bool Hacks_Replace_Fire_Flower_With_Hammer_Suit();
    bool Hacks_Replace_Mario_With_Luigi();
    bool Hacks_Set_Starting_Lives(int lives);
    bool Hacks_Taking_Damage_As_Fire_Reverts_To_Super();
    bool Hacks_Write_Watermark();

    //Music
    bool Music_Disable();
    bool Music_Alternative_Tone_Color_For_SQ1_And_SQ2_1();
    bool Music_Alternative_Tone_Color_For_SQ1_And_SQ2_2();
    bool Music_Enigmario();
    bool Music_Flower_Mario();
    bool Music_Star_Remix();
    bool Music_Super_Mario_263_Shiki();
    bool Music_Super_Mario_Evolution_Mikamashi();
    bool Music_Super_Mushroom();
    bool Music_U1_1();
    bool Music_U1_2();
    bool Music_U1_3();
    bool Music_U1_4();
    bool Music_U1_5();
    bool Music_U1_6();
    bool Music_U1_7();
    bool Music_U1_8();
    bool Music_U1_9();
    bool Music_U1_10();
    bool Music_Water_Mario();

    //Graphics
    bool Graphics_Write_Title_Screen_For_1_Player_Game();
    bool Graphics_Write_Title_Screen_For_2_Player_Game();
    bool Graphics_Dream_Mario_Bros();
    bool Graphics_Hyper_Mario_Rev_A();
    bool Graphics_Hyper_Mario_Rev_B();
    bool Graphics_Lost_Levels();
    bool Graphics_Harmony7();
    bool Graphics_Harmony7_Googie();
    bool Graphics_Paradyce_SMB1_Palette_1();
    bool Graphics_Paradyce_SMB1_Palette_2();
    bool Graphics_Paradyce_SMB2();
    bool Graphics_Pocket_Edition();
    bool Graphics_Super_Bubba_Bros();
    bool Graphics_Super_Duper_Mario_Bros();
    bool Graphics_Super_Mario_Bros_DX_v2();
    bool Graphics_Super_Mario_Bros_DX_v3_4();
    bool Graphics_Super_Mario_MCB();
    bool Graphics_Super_Mario_S();

private:
    bool Load_ROM_Offsets(bool cancel);
    bool Create_ROM_Directory();
    bool Are_Buffers_Allocated();
    void Deallocate_Buffers();
    bool Write_Buffer(const int offset, QByteArray *buffer);
    bool Read_Level_Header();
    bool Read_Objects();
    bool Read_Enemies();

    int numObjectBytes;
    int objectOffset;
    int enemyOffset;
    QFile *file;
    QByteArray *headerBuffer;
    QByteArray *objectsBuffer;
    QByteArray *enemiesBuffer;
    Level_Offset *levelOffset;
    Midpoint_Writer *midpointWriter;
    Object_Writer *objectWriter;
    Enemy_Writer *enemyWriter;
    Header_Writer *headerWriter;
    Enemy_Bytes_Tracker *enemyBytesTracker;
    Room_ID_Handler *roomIDHandler;
    Room_Order_Writer *roomOrderWriter;
    Room_Address_Writer *roomAddressWriter;
    Hacks *hacks;
    Music *music;
    Text *text;
    Graphics *graphics;
    ROM_Handler *romHandler;
    QString applicationLocation;
    QString romFileName;
    QWidget *parent;
};

#endif // SMB1_WRITER_H
