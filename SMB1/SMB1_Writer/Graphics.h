#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Byte_Writer.h"
#include <QByteArray>

class Sequential_Archive_Handler;
class Text;

class Graphics : public Byte_Writer
{
public:
    Graphics(QFile *file, Level_Offset *levelOffset, Sequential_Archive_Handler *sequentialArchiveHandler, Text *text);
    ~Graphics() {}
    bool Write_Title_Screen_For_1_Player_Game();
    bool Write_Title_Screen_For_2_Player_Game();

private:
    bool Write_Title_Screen_Core();
    QByteArray Get_Version_Bytes();
    Sequential_Archive_Handler *sequentialArchiveHandler;
    Text *text;
};

#endif // GRAPHICS_H
