#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Byte_Writer.h"
#include <QByteArray>
#include <QString>

class Graphics : public Byte_Writer
{
public:
    Graphics(QFile *file, Level_Offset *levelOffset);
    ~Graphics() {}
    QByteArray Convert_String_To_SMB_Bytes(const QString &string);

    bool Write_Title_Screen_For_1_Player_Game();
    bool Write_Title_Screen_For_2_Player_Game();
    bool Dream_Mario_Bros();
    bool Lost_Levels();
    bool Paradyce_SMB1_Palette_1();
    bool Paradyce_SMB1_Palette_2();
    bool Paradyce_SMB2();
    bool Pocket_Edition();
    bool Super_Duper_Mario_Bros();
    bool Super_Mario_Bros_DX_v2();
    bool Super_Mario_Bros_DX_v3_4();
    bool Super_Mario_MCB();
    bool Super_Mario_S();
    bool Is_Axe_Rope_Broken();

private:
    bool Write_Title_Screen_Core();
    QByteArray Get_Version_Bytes();

    bool brokenAxeRope;
};

#endif // GRAPHICS_H
