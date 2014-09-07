#ifndef HEADER_WRITER_H
#define HEADER_WRITER_H

#include "../Common SMB1 Files/Level_Compliment.h"
#include "../Common SMB1 Files/Level_Attribute.h"
#include "../Common SMB1 Files/Background.h"
#include "../Common SMB1 Files/Brick.h"
#include "../Common SMB1 Files/Scenery.h"
#include "Binary_Manipulator.h"

class Header_Writer
{
public:
    Header_Writer(QByteArray *buffer);

    //Getters
    int Get_Time();
    bool Get_Autowalk();
    Level_Attribute Get_Starting_Position();
    Background Get_Background();
    Level_Compliment Get_Level_Compliment();
    Scenery Get_Scenery();
    Brick Get_Brick();

    //Setters
    bool Set_Time(int value);
    void Set_Autowalk(bool value);
    bool Set_Starting_Position(Level_Attribute value);
    bool Set_Background(Background value);
    bool Set_Level_Compliment(Level_Compliment value);
    bool Set_Scenery(Scenery value);
    bool Set_Brick(Brick value);

private:
    QByteArray *buffer;
};

#endif // HEADER_WRITER_H
