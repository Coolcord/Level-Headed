#ifndef COLORS_H
#define COLORS_H

#include <QByteArray>
#include <QVector>
#include "Color.h"

class Colors {
public:
    Colors() {}
    ~Colors() {}

    QByteArray Get_QByteArray_From_Color(Color::Color color);
    int Get_Hex_From_Color(Color::Color color);
    bool Get_Color_From_Hex(int hex, Color::Color &color);
    Color::Color Get_Darker_Shade_From_Color(Color::Color color);
    Color::Color Get_Lighter_Shade_From_Color(Color::Color color);
    Color::Color Get_Slightly_Darker_Color_From_Color(Color::Color color);
    Color::Color Get_Slightly_Lighter_Color_From_Color(Color::Color color);
    Color::Color Get_Darkest_Color_From_Color(Color::Color color);
    Color::Color Get_Lightest_Color_From_Color(Color::Color color);
    Color::Color Get_Darkest_Shade_From_Color(Color::Color color);
    Color::Color Get_Lightest_Shade_From_Color(Color::Color color);
    Color::Color Get_Random_Base_Color_From_Color(Color::Color color);
    Color::Color Get_Random_Dark_Color_From_Color(Color::Color color);
    Color::Color Get_Random_Light_Color_From_Color(Color::Color color);
    Color::Color Which_Color_Is_Darker(Color::Color color1, Color::Color color2);
    Color::Color Which_Color_Is_Lighter(Color::Color color1, Color::Color color2);

    Color::Color Get_Random_Darkest_Shade_Color();
    Color::Color Get_Random_Darkest_Shade_Color(int min, int max);
    Color::Color Get_Random_Dark_Shade_Color();
    Color::Color Get_Random_Dark_Shade_Color(int min, int max);
    Color::Color Get_Random_Light_Shade_Color();
    Color::Color Get_Random_Light_Shade_Color(int min, int max);
    Color::Color Get_Random_Lightest_Shade_Color();
    Color::Color Get_Random_Lightest_Shade_Color(int min, int max);

    Color::Color Get_Random_Darkest_Shade_Color_Excluding_Colors(Color::Color excludedColor);
    Color::Color Get_Random_Darkest_Shade_Color_Excluding_Colors(const QVector<Color::Color> &excludedColors);
    Color::Color Get_Random_Dark_Shade_Color_Excluding_Colors(Color::Color excludedColor);
    Color::Color Get_Random_Dark_Shade_Color_Excluding_Colors(const QVector<Color::Color> &excludedColors);
    Color::Color Get_Random_Light_Shade_Color_Excluding_Colors(Color::Color excludedColor);
    Color::Color Get_Random_Light_Shade_Color_Excluding_Colors(const QVector<Color::Color> &excludedColors);
    Color::Color Get_Random_Lightest_Shade_Color_Excluding_Colors(Color::Color excludedColor);
    Color::Color Get_Random_Lightest_Shade_Color_Excluding_Colors(const QVector<Color::Color> &excludedColors);
    Color::Color Get_Random_Shade_Color_Excluding_Colors(const QVector<Color::Color> &excludedColors, int min, int max);

    Color::Color Get_Random_Color();
    Color::Color Get_Random_Base_Color();
    Color::Color Get_Random_Base_Color(int darkMin, int darkMax, int lightMin, int lightMax);
    Color::Color Get_Random_Dark_Color();
    Color::Color Get_Random_Light_Color();
    Color::Color Get_Random_Base_Or_Dark_Color();
    Color::Color Get_Random_Color_Excluding_Colors(Color::Color excludedColor);
    Color::Color Get_Random_Color_Excluding_Colors(const QVector<Color::Color> &excludedColors);

    Color::Color Get_Random_Blue_Color();
    Color::Color Get_Random_Brown_Color();
    Color::Color Get_Random_Gray_Color();
    Color::Color Get_Random_Green_Color();
    Color::Color Get_Random_Orange_Color();
    Color::Color Get_Random_Pink_Color();
    Color::Color Get_Random_Purple_Color();
    Color::Color Get_Random_Red_Color();
    Color::Color Get_Random_Yellow_Color();

    Color::Color Get_Random_Blue_Base_Color();
    Color::Color Get_Random_Blue_Dark_Color();
    Color::Color Get_Random_Blue_Light_Color();
    Color::Color Get_Random_Brown_Base_Color();
    Color::Color Get_Random_Brown_Light_Color();
    Color::Color Get_Random_Gray_Base_Color();
    Color::Color Get_Random_Gray_Dark_Color();
    Color::Color Get_Random_Gray_Light_Color();
    Color::Color Get_Random_Green_Base_Color();
    Color::Color Get_Random_Green_Dark_Color();
    Color::Color Get_Random_Green_Light_Color();
    Color::Color Get_Random_Orange_Base_Color();
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
    Color::Color Get_Random_Yellow_Base_Color();
    Color::Color Get_Random_Yellow_Dark_Color();
    Color::Color Get_Random_Yellow_Light_Color();

    Color::Color Get_Random_Coral_Color();
    Color::Color Get_Random_Mario_Primary_Color_Excluding_Colors(Color::Color originalColor, Color::Color excludedColor);
    Color::Color Get_Random_Mario_Primary_Color_Excluding_Colors(Color::Color originalColor, const QVector<Color::Color> &excludedColors);
    Color::Color Get_Random_Mario_Secondary_Color_Excluding_Colors(Color::Color originalColor, Color::Color excludedColor);
    Color::Color Get_Random_Mario_Secondary_Color_Excluding_Colors(Color::Color originalColor, const QVector<Color::Color> &excludedColors);
    Color::Color Get_Random_Pipe_Dark_Color();
    Color::Color Get_Random_Pipe_Light_Color();
    Color::Color Get_Random_Pipe_Dark_Color_From_Light_Color(Color::Color lightColor);
    Color::Color Get_Random_Pipe_Light_Color_From_Dark_Color(Color::Color darkColor);
    Color::Color Get_Random_Skin_Color();
    Color::Color Get_Random_Sky_Color();
    Color::Color Get_Random_Sky_Day_Color();
    Color::Color Get_Random_Sky_Night_Color();
    Color::Color Get_Random_Sky_Blue_Color();
    Color::Color Get_Random_Sky_Orange_Color();
    Color::Color Get_Random_Sky_Pink_Color();
    Color::Color Get_Random_Tree_Green_Dark_Color();
    Color::Color Get_Random_Tree_Orange_Dark_Color();
    Color::Color Get_Random_Tree_Snow_Dark_Color();
    Color::Color Get_Random_Tree_Snow_Outline_Color();
    Color::Color Get_Random_Tree_Yellow_Dark_Color();
    Color::Color Get_Random_Tree_Light_Color_From_Dark_Color(Color::Color darkColor);
    Color::Color Get_Random_Water_Color();

    void Swap_Colors(Color::Color &color1, Color::Color &color2);

private:
    bool Is_Shade_Min_Max_Valid(int min, int max);
    int Get_Color_Darkness_Weight(Color::Color color);
    void Get_Color_RGB(Color::Color color, char &r, char &g, char &b);

    bool Is_Blue_Color(Color::Color color);
    bool Is_Brown_Color(Color::Color color);
    bool Is_Gray_Color(Color::Color color);
    bool Is_Green_Color(Color::Color color);
    bool Is_Orange_Color(Color::Color color);
    bool Is_Pink_Color(Color::Color color);
    bool Is_Purple_Color(Color::Color color);
    bool Is_Red_Color(Color::Color color);
    bool Is_Yellow_Color(Color::Color color);

    bool Is_Color_Black_Or_White(Color::Color color);
    bool Is_Color_Darkest_Shade(Color::Color color);
};

#endif // COLORS_H
