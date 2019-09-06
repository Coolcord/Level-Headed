#ifndef COLORS_H
#define COLORS_H

#include "Color.h"

class Colors {
public:
    Colors() {}
    ~Colors() {}

    int Get_Hex_From_Color(Color::Color color);
    bool Get_Color_From_Hex(int hex, Color::Color &color);

    Color::Color Which_Color_Is_Darker(Color::Color color1, Color::Color color2);
    Color::Color Which_Color_Is_Lighter(Color::Color color1, Color::Color color2);

    Color::Color Get_Random_Color();
    Color::Color Get_Random_Base_Color();
    Color::Color Get_Random_Dark_Color();
    Color::Color Get_Random_Light_Color();

    Color::Color Get_Random_Blue_Color();
    Color::Color Get_Random_Brown_Color();
    Color::Color Get_Random_Gray_Color();
    Color::Color Get_Random_Green_Color();
    Color::Color Get_Random_Magenta_Color();
    Color::Color Get_Random_Orange_Color();
    Color::Color Get_Random_Pink_Color();
    Color::Color Get_Random_Purple_Color();
    Color::Color Get_Random_Red_Color();
    Color::Color Get_Random_Turquoise_Color();
    Color::Color Get_Random_Yellow_Color();

    Color::Color Get_Random_Blue_Base_Color();
    Color::Color Get_Random_Blue_Dark_Color();
    Color::Color Get_Random_Blue_Light_Color();
    Color::Color Get_Random_Brown_Base_Color();
    Color::Color Get_Random_Brown_Dark_Color();
    Color::Color Get_Random_Brown_Light_Color();
    Color::Color Get_Random_Gray_Base_Color();
    Color::Color Get_Random_Gray_Dark_Color();
    Color::Color Get_Random_Gray_Light_Color();
    Color::Color Get_Random_Green_Base_Color();
    Color::Color Get_Random_Green_Dark_Color();
    Color::Color Get_Random_Green_Light_Color();
    Color::Color Get_Random_Magenta_Base_Color();
    Color::Color Get_Random_Magenta_Dark_Color();
    Color::Color Get_Random_Magenta_Light_Color();
    Color::Color Get_Random_Orange_Base_Color();
    Color::Color Get_Random_Orange_Dark_Color();
    Color::Color Get_Random_Orange_Light_Color();
    Color::Color Get_Random_Pink_Base_Color();
    Color::Color Get_Random_Pink_Dark_Color();
    Color::Color Get_Random_Pink_Light_Color();
    Color::Color Get_Random_Purple_Base_Color();
    Color::Color Get_Random_Purple_Dark_Color();
    Color::Color Get_Random_Purple_Light_Color();
    Color::Color Get_Random_Red_Base_Color();
    Color::Color Get_Random_Red_Dark_Color();
    Color::Color Get_Random_Red_Light_Color();
    Color::Color Get_Random_Turquoise_Base_Color();
    Color::Color Get_Random_Turquoise_Dark_Color();
    Color::Color Get_Random_Turquoise_Light_Color();
    Color::Color Get_Random_Yellow_Base_Color();
    Color::Color Get_Random_Yellow_Dark_Color();
    Color::Color Get_Random_Yellow_Light_Color();

private:
    int Get_Color_Darkness_Weight(Color::Color color);
};

#endif // COLORS_H
