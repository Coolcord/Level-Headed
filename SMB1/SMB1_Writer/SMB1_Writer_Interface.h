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
    virtual QStringList Get_Installed_ROMs()=0;
    virtual QString Install_ROM()=0;
    virtual bool Load_ROM()=0;
    virtual bool Load_ROM(const QString &fileName)=0;
    virtual bool New_Level(Level::Level level)=0;
    virtual bool Write_Level()=0;
    virtual int Get_Num_Object_Bytes()=0;
    virtual int Get_Num_Enemy_Bytes()=0;
    virtual bool Set_Number_Of_Worlds(int value)=0;
    virtual QString Get_Output_ROM_Location()=0;
    virtual bool Set_Output_ROM_Location(const QString &location)=0;

    //Header
    virtual bool Header_Time(int value)=0;
    virtual bool Header_Autowalk(bool value)=0;
    virtual bool Header_Starting_Position(Level_Attribute::Level_Attribute value)=0;
    virtual bool Header_Background(Background::Background value)=0;
    virtual bool Header_Level_Compliment(Level_Compliment::Level_Compliment value)=0;
    virtual bool Header_Scenery(Scenery::Scenery value)=0;
    virtual bool Header_Brick(Brick::Brick value)=0;
    virtual bool Header_Attribute(Level_Attribute::Level_Attribute attribute)=0;
    virtual bool Header_Midpoint(int value)=0;
    virtual bool Header_Midpoint(Level::Level level, int value)=0;

    //Room Table
    virtual bool Room_Table_Set_Next_Level(Level::Level level)=0;
    virtual bool Room_Table_Set_Number_Of_Worlds(int value)=0;

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
    virtual bool Object_Corral(int x, int y, int height)=0;
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
    virtual bool Enemy_Pirana_Plant(int x, int y, bool onlyHardMode = false)=0;
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
    virtual bool Enemy_Pipe_Pointer(int x, const QString &levelSlot, int page)=0;
    virtual bool Enemy_Nothing(int x)=0;

    //Transfer Bytes Between Levels
    virtual bool Send_Object_Bytes_From_One_Level_To_Another(Level::Level fromLevel, Level::Level toLevel, int numBytes)=0;
    virtual bool Send_Enemy_Bytes_From_One_Level_To_Another(Level::Level fromLevel, Level::Level toLevel, int numBytes)=0;

    //Hacks
    virtual bool Write_Watermark()=0;
    virtual bool Infinite_Lives()=0;
    virtual bool Set_Starting_Lives(int lives)=0;
    virtual bool Enable_God_Mode()=0;
    virtual bool Replace_Mario_With_Luigi()=0;
    virtual bool Add_Luigi_Game()=0;
    virtual bool Taking_Damage_As_Fire_Reverts_To_Super()=0;
    virtual bool Fix_Lakitu_Throw_Arc()=0;
    virtual bool Fast_Enemies(int speed)=0;

    //Music
    virtual bool Music_Enigmario()=0;
    virtual bool Music_U1_1()=0;
    virtual bool Music_U1_3()=0;
    virtual bool Music_U1_6()=0;
    virtual bool Music_U1_7()=0;
    virtual bool Music_U1_9()=0;

    //Graphics
    virtual bool Graphics_Pocket_Edition()=0;
    virtual bool Graphics_Super_Mario_Bros_DX()=0;
};

Q_DECLARE_INTERFACE(SMB1_Writer_Interface, "SMB1_Writer_Interface")

#endif // SMB1_WRITER_INTERFACE_H
