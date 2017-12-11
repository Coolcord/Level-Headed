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
    bool Invincibility();
    bool Moon_Jump();
    bool Star_Color_Mario();
    bool Fireballs_Kill_Everything_Onscreen();
    bool Fix_Lakitu_Throw_Arc();
    bool Fast_Enemies(int speed);
private:
    bool Write_Bytes_To_Offset(int offset, const QByteArray &bytes);

    QFile *file;
    Level_Offset *levelOffset;
};

#endif // HACKS_H
