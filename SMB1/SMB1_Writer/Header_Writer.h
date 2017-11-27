#ifndef HEADER_WRITER_H
#define HEADER_WRITER_H

#include "../Common_SMB1_Files/Level_Compliment.h"
#include "../Common_SMB1_Files/Level_Attribute.h"
#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Scenery.h"
#include "Binary_Manipulator.h"

class Header_Writer
{
public:
    Header_Writer(QByteArray *buffer);

    //Getters
    int Get_Time();
    bool Get_Autowalk();
    Level_Attribute::Level_Attribute Get_Starting_Position();
    Background::Background Get_Background();
    Level_Compliment::Level_Compliment Get_Level_Compliment();
    Scenery::Scenery Get_Scenery();
    Brick::Brick Get_Brick();

    //Setters
    bool Set_Time(int value);
    void Set_Autowalk(bool value);
    bool Set_Starting_Position(Level_Attribute::Level_Attribute value);
    bool Set_Background(Background::Background value);
    bool Set_Level_Compliment(Level_Compliment::Level_Compliment value);
    bool Set_Scenery(Scenery::Scenery value);
    bool Set_Brick(Brick::Brick value);

private:
    QByteArray *buffer;
};

#endif // HEADER_WRITER_H
