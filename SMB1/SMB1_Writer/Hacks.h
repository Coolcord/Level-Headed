#ifndef HACKS_H
#define HACKS_H

#include <QByteArray>
#include <QFile>

class Level_Offset;

class Hacks
{
public:
    Hacks(QFile *file, Level_Offset *levelOffset);
    ~Hacks();
    bool Write_Watermark();
    bool Infinite_Lives();
    bool Set_Starting_Lives(int lives);
    bool Replace_Mario_With_Luigi();
    bool Add_Luigi_Game();
    bool Invincibility();
    bool Moon_Jump();
    bool Star_Color_Mario();
    bool Fireballs_Kill_Everything_Onscreen();
    bool Taking_Damage_As_Fire_Reverts_To_Super();
    bool Fix_Lakitu_Throw_Arc();
    bool Fast_Enemies(int speed);
private:
    bool Write_Bytes_To_Offset(int offset, const QByteArray &bytes);

    QFile *file;
    Level_Offset *levelOffset;
};

#endif // HACKS_H
