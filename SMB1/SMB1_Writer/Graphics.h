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
    bool Change_1UP_Palette(int palette);
    bool Write_Title_Screen_For_1_Player_Game();
    bool Write_Title_Screen_For_2_Player_Game();

private:
    bool Write_Title_Screen_Core();
    QByteArray Get_Version_Bytes();

    Text *text;
    Sequential_Archive_Handler *sequentialArchiveHandler;
    qint64 versionOffset;
};

#endif // GRAPHICS_H
