#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Byte_Writer.h"
#include <QByteArray>

class Text;
class Sequential_Archive_Handler;

class Graphics : public Byte_Writer
{
public:
    Graphics(QFile *file, Level_Offset *levelOffset, Sequential_Archive_Handler *sequentialArchiveHandler, Text *text);
    ~Graphics() {}
    bool Apply_Hammer_Suit_Fix();
    bool Apply_Title_Screen_1P_Fix(qint64 &versionOffset);
    bool Apply_Title_Screen_2P_Fix(qint64 &versionOffset);
    bool Change_1UP_Palette(int palette);
    bool Write_Title_Screen_For_1_Player_Game();
    bool Write_Title_Screen_For_2_Player_Game();

private:
    bool Write_Title_Screen_Core();
    QByteArray Get_Version_Bytes();
    void Get_Version_Offset_From_Title_Screen_Fix(const QByteArray &patchBytes, qint64 &versionOffset);

    Text *text;
    Sequential_Archive_Handler *sequentialArchiveHandler;
    qint64 versionOffset;
};

#endif // GRAPHICS_H
