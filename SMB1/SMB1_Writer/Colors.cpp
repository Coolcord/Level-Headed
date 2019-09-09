#include "Colors.h"
#include "../../Common_Files/Random.h"
#include <assert.h>

QByteArray Colors::Get_QByteArray_From_Color(Color::Color color) {
    return QByteArray(1, static_cast<char>(this->Get_Hex_From_Color(color)));
}

int Colors::Get_Hex_From_Color(Color::Color color) {
    return static_cast<int>(color);
}

bool Colors::Get_Color_From_Hex(int hex, Color::Color &color) {
    if (hex < 0x00 || hex > 0x3F) return false; //invalid color range
    switch (hex) {
    default:        assert(false); return false;
    case 0x00:      color = Color::GRAY; return true;
    case 0x01:      color = Color::BLUE_DARKEST; return true;
    case 0x02:      color = Color::BLUE_DARKER; return true;
    case 0x03:      color = Color::PURPLE_DARKEST; return true;
    case 0x04:      color = Color::PURPLE_DARK; return true;
    case 0x05:      color = Color::PINK_DARKEST; return true;
    case 0x06:      color = Color::RED_DARK; return true;
    case 0x07:      color = Color::BROWN; return true;
    case 0x08:      color = Color::OLIVE_DARK; return true;
    case 0x09:      color = Color::GREEN_DARKEST; return true;
    case 0x0A:      color = Color::GREEN_DARKER; return true;
    case 0x0B:      color = Color::TURQUOISE_DARK; return true;
    case 0x0C:      color = Color::AQUAMARINE_DARK; return true;
    case 0x0D:      color = Color::BLACK; return true;
    case 0x0E:      color = Color::BLACK; return true;
    case 0x0F:      color = Color::BLACK; return true;
    case 0x10:      color = Color::GRAY_LIGHTEST; return true;
    case 0x11:      color = Color::BLUE; return true;
    case 0x12:      color = Color::BLUE_DARK; return true;
    case 0x13:      color = Color::PURPLE; return true;
    case 0x14:      color = Color::MAGENTA; return true;
    case 0x15:      color = Color::PINK_DARK; return true;
    case 0x16:      color = Color::RED; return true;
    case 0x17:      color = Color::BROWN_LIGHT; return true;
    case 0x18:      color = Color::OLIVE; return true;
    case 0x19:      color = Color::GREEN_DARK; return true;
    case 0x1A:      color = Color::GREEN; return true;
    case 0x1B:      color = Color::TURQUOISE; return true;
    case 0x1C:      color = Color::AQUAMARINE; return true;
    case 0x1D:      color = Color::BLACK; return true;
    case 0x1E:      color = Color::BLACK; return true;
    case 0x1F:      color = Color::BLACK; return true;
    case 0x20:      color = Color::WHITE; return true;
    case 0x21:      color = Color::BLUE_LIGHTER; return true;
    case 0x22:      color = Color::BLUE_LIGHT; return true;
    case 0x23:      color = Color::PURPLE_LIGHT; return true;
    case 0x24:      color = Color::MAGENTA_LIGHT; return true;
    case 0x25:      color = Color::PINK; return true;
    case 0x26:      color = Color::RED_LIGHT; return true;
    case 0x27:      color = Color::ORANGE; return true;
    case 0x28:      color = Color::YELLOW; return true;
    case 0x29:      color = Color::GREEN_LIGHT; return true;
    case 0x2A:      color = Color::GREEN_LIGHTER; return true;
    case 0x2B:      color = Color::TURQUOISE_LIGHT; return true;
    case 0x2C:      color = Color::AQUAMARINE_LIGHT; return true;
    case 0x2D:      color = Color::GRAY_DARK; return true;
    case 0x2E:      color = Color::BLACK; return true;
    case 0x2F:      color = Color::BLACK; return true;
    case 0x30:      color = Color::WHITE; return true;
    case 0x31:      color = Color::BLUE_LIGHTEST; return true;
    case 0x32:      color = Color::BLUE_LIGHTEST_PURPLE; return true;
    case 0x33:      color = Color::PURPLE_LIGHTEST; return true;
    case 0x34:      color = Color::MAGENTA_LIGHTEST; return true;
    case 0x35:      color = Color::PINK_LIGHT; return true;
    case 0x36:      color = Color::RED_LIGHTEST; return true;
    case 0x37:      color = Color::ORANGE_LIGHT; return true;
    case 0x38:      color = Color::YELLOW_LIGHT; return true;
    case 0x39:      color = Color::GREEN_LIGHTEST_YELLOW; return true;
    case 0x3A:      color = Color::GREEN_LIGHTEST; return true;
    case 0x3B:      color = Color::TURQUOISE_LIGHTEST; return true;
    case 0x3C:      color = Color::AQUAMARINE_LIGHTEST; return true;
    case 0x3D:      color = Color::GRAY_LIGHT; return true;
    case 0x3E:      color = Color::BLACK; return true;
    case 0x3F:      color = Color::BLACK; return true;
    }
}

Color::Color Colors::Get_Darker_Shade_From_Color(Color::Color color) {
    int hex = this->Get_Hex_From_Color(color);
    assert(hex >= 0x00 && hex <= 0x3F);
    if (hex < 0x10) return color; //lightest shade available
    if (hex == 0x30) hex = 0x20; //change white's code to one that can be handled by the algorithm
    hex -= 0x10;
    assert(this->Get_Color_From_Hex(hex, color));
    return color;
}

Color::Color Colors::Get_Lighter_Shade_From_Color(Color::Color color) {
    int hex = this->Get_Hex_From_Color(color);
    assert(hex >= 0x00 && hex <= 0x3F);
    if (hex >= 0x30) return color; //darkest shade available
    if (hex == 0x0F) hex = 0x1D; //change black's code to one that can be handled by the algorithm
    hex += 0x10;
    assert(this->Get_Color_From_Hex(hex, color));
    return color;
}

Color::Color Colors::Get_Slightly_Darker_Color_From_Color(Color::Color color) {
    switch (color) {
    case Color::GRAY:                   return Color::GRAY_DARK;
    case Color::BLUE_DARKEST:           return Color::BLACK;
    case Color::BLUE_DARKER:            return Color::BLUE_DARKEST;
    case Color::PURPLE_DARKEST:         return Color::BLACK;
    case Color::PURPLE_DARK:            return Color::PURPLE_DARKEST;
    case Color::PINK_DARKEST:           return Color::PURPLE_DARK;
    case Color::RED_DARK:               return Color::BLACK;
    case Color::BROWN:                  return Color::BLACK;
    case Color::OLIVE_DARK:             return Color::BLACK;
    case Color::GREEN_DARKEST:          return Color::OLIVE_DARK;
    case Color::GREEN_DARKER:           return Color::GREEN_DARKEST;
    case Color::TURQUOISE_DARK:         return Color::GREEN_DARKER;
    case Color::AQUAMARINE_DARK:        return Color::BLUE_DARKER;
    case Color::BLACK:                  return Color::BLACK;
    case Color::GRAY_LIGHTEST:          return Color::GRAY_LIGHT;
    case Color::BLUE:                   return Color::BLUE_DARK;
    case Color::BLUE_DARK:              return Color::BLUE_DARKER;
    case Color::PURPLE:                 return Color::PURPLE_DARKEST;
    case Color::MAGENTA:                return Color::PINK_DARKEST;
    case Color::PINK_DARK:              return Color::MAGENTA;
    case Color::RED:                    return Color::RED_DARK;
    case Color::BROWN_LIGHT:            return Color::BROWN;
    case Color::OLIVE:                  return Color::OLIVE_DARK;
    case Color::GREEN_DARK:             return Color::GREEN_DARKER;
    case Color::GREEN:                  return Color::GREEN_DARK;
    case Color::TURQUOISE:              return Color::TURQUOISE_DARK;
    case Color::AQUAMARINE:             return Color::AQUAMARINE_DARK;
    case Color::WHITE:                  return Color::GRAY_LIGHTEST;
    case Color::BLUE_LIGHTER:           return Color::BLUE_LIGHT;
    case Color::BLUE_LIGHT:             return Color::BLUE;
    case Color::PURPLE_LIGHT:           return Color::PURPLE;
    case Color::MAGENTA_LIGHT:          return Color::PINK_DARK;
    case Color::PINK:                   return Color::MAGENTA_LIGHT;
    case Color::RED_LIGHT:              return Color::RED;
    case Color::ORANGE:                 return Color::RED_LIGHT;
    case Color::YELLOW:                 return Color::OLIVE;
    case Color::GREEN_LIGHT:            return Color::GREEN;
    case Color::GREEN_LIGHTER:          return Color::GREEN_LIGHT;
    case Color::TURQUOISE_LIGHT:        return Color::TURQUOISE;
    case Color::AQUAMARINE_LIGHT:       return Color::AQUAMARINE;
    case Color::GRAY_DARK:              return Color::BLACK;
    case Color::BLUE_LIGHTEST:          return Color::BLUE_LIGHTER;
    case Color::BLUE_LIGHTEST_PURPLE:   return Color::BLUE_LIGHTEST;
    case Color::PURPLE_LIGHTEST:        return Color::PURPLE_LIGHT;
    case Color::MAGENTA_LIGHTEST:       return Color::PINK;
    case Color::PINK_LIGHT:             return Color::MAGENTA_LIGHTEST;
    case Color::RED_LIGHTEST:           return Color::RED_LIGHT;
    case Color::ORANGE_LIGHT:           return Color::ORANGE;
    case Color::YELLOW_LIGHT:           return Color::YELLOW;
    case Color::GREEN_LIGHTEST_YELLOW:  return Color::GREEN_LIGHTEST;
    case Color::GREEN_LIGHTEST:         return Color::GREEN_LIGHTER;
    case Color::TURQUOISE_LIGHTEST:     return Color::TURQUOISE_LIGHT;
    case Color::AQUAMARINE_LIGHTEST:    return Color::AQUAMARINE_LIGHT;
    case Color::GRAY_LIGHT:             return Color::GRAY;
    }
    assert(false); return Color::BLACK;
}

Color::Color Colors::Get_Slightly_Lighter_Color_From_Color(Color::Color color) {
    switch (color) {
    case Color::GRAY:                   return Color::GRAY_LIGHT;
    case Color::BLUE_DARKEST:           return Color::BLUE_DARKER;
    case Color::BLUE_DARKER:            return Color::BLUE_DARK;
    case Color::PURPLE_DARKEST:         return Color::PURPLE_DARK;
    case Color::PURPLE_DARK:            return Color::PINK_DARKEST;
    case Color::PINK_DARKEST:           return Color::MAGENTA;
    case Color::RED_DARK:               return Color::RED;
    case Color::BROWN:                  return Color::BROWN_LIGHT;
    case Color::OLIVE_DARK:             return Color::OLIVE;
    case Color::GREEN_DARKEST:          return Color::GREEN_DARKER;
    case Color::GREEN_DARKER:           return Color::GREEN_DARK;
    case Color::TURQUOISE_DARK:         return Color::TURQUOISE;
    case Color::AQUAMARINE_DARK:        return Color::AQUAMARINE;
    case Color::BLACK:                  return Color::GRAY_DARK;
    case Color::GRAY_LIGHTEST:          return Color::WHITE;
    case Color::BLUE:                   return Color::BLUE_LIGHT;
    case Color::BLUE_DARK:              return Color::BLUE;
    case Color::PURPLE:                 return Color::PURPLE_LIGHT;
    case Color::MAGENTA:                return Color::PINK_DARK;
    case Color::PINK_DARK:              return Color::MAGENTA_LIGHT;
    case Color::RED:                    return Color::RED_LIGHT;
    case Color::BROWN_LIGHT:            return Color::ORANGE_LIGHT;
    case Color::OLIVE:                  return Color::YELLOW;
    case Color::GREEN_DARK:             return Color::GREEN;
    case Color::GREEN:                  return Color::GREEN_LIGHT;
    case Color::TURQUOISE:              return Color::TURQUOISE_LIGHT;
    case Color::AQUAMARINE:             return Color::AQUAMARINE_LIGHT;
    case Color::WHITE:                  return Color::WHITE;
    case Color::BLUE_LIGHTER:           return Color::BLUE_LIGHTEST;
    case Color::BLUE_LIGHT:             return Color::BLUE_LIGHTER;
    case Color::PURPLE_LIGHT:           return Color::PURPLE_LIGHTEST;
    case Color::MAGENTA_LIGHT:          return Color::PINK;
    case Color::PINK:                   return Color::MAGENTA_LIGHTEST;
    case Color::RED_LIGHT:              return Color::ORANGE;
    case Color::ORANGE:                 return Color::ORANGE_LIGHT;
    case Color::YELLOW:                 return Color::YELLOW_LIGHT;
    case Color::GREEN_LIGHT:            return Color::GREEN_LIGHTER;
    case Color::GREEN_LIGHTER:          return Color::GREEN_LIGHTEST;
    case Color::TURQUOISE_LIGHT:        return Color::TURQUOISE_LIGHTEST;
    case Color::AQUAMARINE_LIGHT:       return Color::AQUAMARINE_LIGHTEST;
    case Color::GRAY_DARK:              return Color::GRAY;
    case Color::BLUE_LIGHTEST:          return Color::BLUE_LIGHTEST_PURPLE;
    case Color::BLUE_LIGHTEST_PURPLE:   return Color::WHITE;
    case Color::PURPLE_LIGHTEST:        return Color::WHITE;
    case Color::MAGENTA_LIGHTEST:       return Color::PINK_LIGHT;
    case Color::PINK_LIGHT:             return Color::WHITE;
    case Color::RED_LIGHTEST:           return Color::WHITE;
    case Color::ORANGE_LIGHT:           return Color::WHITE;
    case Color::YELLOW_LIGHT:           return Color::WHITE;
    case Color::GREEN_LIGHTEST_YELLOW:  return Color::WHITE;
    case Color::GREEN_LIGHTEST:         return Color::GREEN_LIGHTEST_YELLOW;
    case Color::TURQUOISE_LIGHTEST:     return Color::WHITE;
    case Color::AQUAMARINE_LIGHTEST:    return Color::WHITE;
    case Color::GRAY_LIGHT:             return Color::WHITE;
    }
    assert(false); return Color::BLACK;
}

Color::Color Colors::Get_Darkest_Color_From_Color(Color::Color color) {
    Color::Color previousColor = color;
    if (this->Is_Gray_Color(color)) return Color::BLACK;
    while (color != Color::BLACK) {
        previousColor = color;
        color = this->Get_Slightly_Darker_Color_From_Color(color);
    }
    return previousColor;
}

Color::Color Colors::Get_Lightest_Color_From_Color(Color::Color color) {
    Color::Color previousColor = color;
    if (this->Is_Gray_Color(color)) return Color::WHITE;
    while (color != Color::WHITE) {
        previousColor = color;
        color = this->Get_Slightly_Lighter_Color_From_Color(color);
    }
    return previousColor;
}

Color::Color Colors::Get_Darkest_Shade_From_Color(Color::Color color) {
    int hex = this->Get_Hex_From_Color(color);
    assert(hex >= 0x00 && hex <= 0x3F);
    hex = hex&0x0F;
    assert(this->Get_Color_From_Hex(hex, color));
    return color;
}

Color::Color Colors::Get_Lightest_Shade_From_Color(Color::Color color) {
    if (color == Color::BLACK) return Color::GRAY_LIGHT;
    int hex = this->Get_Hex_From_Color(color);
    assert(hex >= 0x00 && hex <= 0x3F);
    hex = hex&0x0F;
    hex += 0x30;
    assert(this->Get_Color_From_Hex(hex, color));
    return color;
}

Color::Color Colors::Get_Random_Base_Color_From_Color(Color::Color color) {
    if (this->Is_Blue_Color(color))     return this->Get_Random_Blue_Base_Color();
    if (this->Is_Brown_Color(color))    return this->Get_Random_Brown_Base_Color();
    if (this->Is_Gray_Color(color))     return this->Get_Random_Gray_Base_Color();
    if (this->Is_Green_Color(color))    return this->Get_Random_Green_Base_Color();
    if (this->Is_Orange_Color(color))   return this->Get_Random_Orange_Base_Color();
    if (this->Is_Pink_Color(color))     return this->Get_Random_Pink_Base_Color();
    if (this->Is_Purple_Color(color))   return this->Get_Random_Purple_Base_Color();
    if (this->Is_Red_Color(color))      return this->Get_Random_Red_Base_Color();
    if (this->Is_Yellow_Color(color))   return this->Get_Random_Yellow_Base_Color();
    assert(false); return Color::BLACK;
}

Color::Color Colors::Get_Random_Dark_Color_From_Color(Color::Color color) {
    if (this->Is_Blue_Color(color))     return this->Get_Random_Blue_Dark_Color();
    if (this->Is_Brown_Color(color))    return this->Get_Random_Brown_Base_Color();
    if (this->Is_Gray_Color(color))     return this->Get_Random_Gray_Dark_Color();
    if (this->Is_Green_Color(color))    return this->Get_Random_Green_Dark_Color();
    if (this->Is_Orange_Color(color))   return this->Get_Random_Orange_Base_Color();
    if (this->Is_Pink_Color(color))     return this->Get_Random_Pink_Dark_Color();
    if (this->Is_Purple_Color(color))   return this->Get_Random_Purple_Dark_Color();
    if (this->Is_Red_Color(color))      return this->Get_Random_Red_Dark_Color();
    if (this->Is_Yellow_Color(color))   return this->Get_Random_Yellow_Dark_Color();
    assert(false); return Color::BLACK;
}

Color::Color Colors::Get_Random_Light_Color_From_Color(Color::Color color) {
    if (this->Is_Blue_Color(color))     return this->Get_Random_Blue_Light_Color();
    if (this->Is_Brown_Color(color))    return this->Get_Random_Brown_Light_Color();
    if (this->Is_Gray_Color(color))     return this->Get_Random_Gray_Light_Color();
    if (this->Is_Green_Color(color))    return this->Get_Random_Green_Light_Color();
    if (this->Is_Orange_Color(color))   return this->Get_Random_Orange_Light_Color();
    if (this->Is_Pink_Color(color))     return this->Get_Random_Pink_Light_Color();
    if (this->Is_Purple_Color(color))   return this->Get_Random_Purple_Light_Color();
    if (this->Is_Red_Color(color))      return this->Get_Random_Red_Light_Color();
    if (this->Is_Yellow_Color(color))   return this->Get_Random_Yellow_Light_Color();
    assert(false); return Color::BLACK;
}

Color::Color Colors::Which_Color_Is_Darker(Color::Color color1, Color::Color color2) {
    if (color1 == color2) return color1;
    int color1Weight = this->Get_Color_Darkness_Weight(color1);
    int color2Weight = this->Get_Color_Darkness_Weight(color2);
    if (color1Weight >= color2Weight) return color1;
    else return color2;
}

Color::Color Colors::Which_Color_Is_Lighter(Color::Color color1, Color::Color color2) {
    if (color1 == color2) return color1;
    int color1Weight = this->Get_Color_Darkness_Weight(color1);
    int color2Weight = this->Get_Color_Darkness_Weight(color2);
    assert(color1Weight != color2Weight);
    if (color1Weight < color2Weight) return color1;
    else return color2;
}

Color::Color Colors::Get_Random_Darkest_Shade_Color() {
    return this->Get_Random_Darkest_Shade_Color(0x00, 0x0C);
}

Color::Color Colors::Get_Random_Darkest_Shade_Color(int min, int max) {
    assert(this->Is_Shade_Min_Max_Valid(min, max));
    int hex = Random::Get_Instance().Get_Num(min, max);
    Color::Color color = Color::BLACK;
    assert(this->Get_Color_From_Hex(hex, color));
    return color;
}

Color::Color Colors::Get_Random_Dark_Shade_Color() {
    return this->Get_Random_Dark_Shade_Color(0x00, 0x0D);
}

Color::Color Colors::Get_Random_Dark_Shade_Color(int min, int max) {
    assert(this->Is_Shade_Min_Max_Valid(min, max));
    min += 0x10; max += 0x10;
    int hex = Random::Get_Instance().Get_Num(min, max);
    Color::Color color = Color::BLACK;
    assert(this->Get_Color_From_Hex(hex, color));
    return color;
}

Color::Color Colors::Get_Random_Light_Shade_Color() {
    return this->Get_Random_Light_Shade_Color(0x01, 0x0D);
}

Color::Color Colors::Get_Random_Light_Shade_Color(int min, int max) {
    assert(this->Is_Shade_Min_Max_Valid(min, max));
    min += 0x20; max += 0x20;
    int hex = Random::Get_Instance().Get_Num(min, max);
    Color::Color color = Color::BLACK;
    assert(this->Get_Color_From_Hex(hex, color));
    return color;
}

Color::Color Colors::Get_Random_Lightest_Shade_Color() {
    return this->Get_Random_Lightest_Shade_Color(0x00, 0x0D);
}

Color::Color Colors::Get_Random_Lightest_Shade_Color(int min, int max) {
    assert(this->Is_Shade_Min_Max_Valid(min, max));
    min += 0x30; max += 0x30;
    int hex = Random::Get_Instance().Get_Num(min, max);
    Color::Color color = Color::BLACK;
    assert(this->Get_Color_From_Hex(hex, color));
    return color;
}

Color::Color Colors::Get_Random_Color() {
    switch (Random::Get_Instance().Get_Num(53)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return Color::GRAY;
    case 1:     return Color::BLUE_DARKEST;
    case 2:     return Color::BLUE_DARKER;
    case 3:     return Color::PURPLE_DARKEST;
    case 4:     return Color::PURPLE_DARK;
    case 5:     return Color::PINK_DARKEST;
    case 6:     return Color::RED_DARK;
    case 7:     return Color::BROWN;
    case 8:     return Color::OLIVE_DARK;
    case 9:     return Color::GREEN_DARKEST;
    case 10:    return Color::GREEN_DARKER;
    case 11:    return Color::TURQUOISE_DARK;
    case 12:    return Color::AQUAMARINE_DARK;
    case 13:    return Color::BLACK;
    case 14:    return Color::GRAY_LIGHTEST;
    case 15:    return Color::BLUE;
    case 16:    return Color::BLUE_DARK;
    case 17:    return Color::PURPLE;
    case 18:    return Color::MAGENTA;
    case 19:    return Color::PINK_DARK;
    case 20:    return Color::RED;
    case 21:    return Color::BROWN_LIGHT;
    case 22:    return Color::OLIVE;
    case 23:    return Color::GREEN_DARK;
    case 24:    return Color::GREEN;
    case 25:    return Color::TURQUOISE;
    case 26:    return Color::AQUAMARINE;
    case 27:    return Color::WHITE;
    case 28:    return Color::BLUE_LIGHTER;
    case 29:    return Color::BLUE_LIGHT;
    case 30:    return Color::PURPLE_LIGHT;
    case 31:    return Color::MAGENTA_LIGHT;
    case 32:    return Color::PINK;
    case 33:    return Color::RED_LIGHT;
    case 34:    return Color::ORANGE;
    case 35:    return Color::YELLOW;
    case 36:    return Color::GREEN_LIGHT;
    case 37:    return Color::GREEN_LIGHTER;
    case 38:    return Color::TURQUOISE_LIGHT;
    case 39:    return Color::AQUAMARINE_LIGHT;
    case 40:    return Color::GRAY_DARK;
    case 41:    return Color::BLUE_LIGHTEST;
    case 42:    return Color::BLUE_LIGHTEST_PURPLE;
    case 43:    return Color::PURPLE_LIGHTEST;
    case 44:    return Color::MAGENTA_LIGHTEST;
    case 45:    return Color::PINK_LIGHT;
    case 46:    return Color::RED_LIGHTEST;
    case 47:    return Color::ORANGE_LIGHT;
    case 48:    return Color::YELLOW_LIGHT;
    case 49:    return Color::GREEN_LIGHTEST_YELLOW;
    case 50:    return Color::GREEN_LIGHTEST;
    case 51:    return Color::TURQUOISE_LIGHTEST;
    case 52:    return Color::AQUAMARINE_LIGHTEST;
    case 53:    return Color::GRAY_LIGHT;
    }
}

Color::Color Colors::Get_Random_Base_Color() {
    return this->Get_Random_Base_Color(0x00, 0x0D, 0x01, 0x0D);
}

Color::Color Colors::Get_Random_Base_Color(int darkMin, int darkMax, int lightMin, int lightMax) {
    if (Random::Get_Instance().Get_Num(1)) return this->Get_Random_Dark_Shade_Color(darkMin, darkMax);
    else return this->Get_Random_Light_Shade_Color(lightMin, lightMax);
}

Color::Color Colors::Get_Random_Dark_Color() {
    return this->Get_Random_Darkest_Shade_Color();
}

Color::Color Colors::Get_Random_Light_Color() {
    return this->Get_Random_Lightest_Shade_Color();
}

Color::Color Colors::Get_Random_Blue_Color() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Blue_Base_Color();
    case 1:     return this->Get_Random_Blue_Dark_Color();
    case 2:     return this->Get_Random_Blue_Light_Color();
    }
}

Color::Color Colors::Get_Random_Brown_Color() {
    if (Random::Get_Instance().Get_Num(1)) return this->Get_Random_Brown_Base_Color();
    else return this->Get_Random_Brown_Light_Color();
}

Color::Color Colors::Get_Random_Gray_Color() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Gray_Base_Color();
    case 1:     return this->Get_Random_Gray_Dark_Color();
    case 2:     return this->Get_Random_Gray_Light_Color();
    }
}

Color::Color Colors::Get_Random_Green_Color() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Green_Base_Color();
    case 1:     return this->Get_Random_Green_Dark_Color();
    case 2:     return this->Get_Random_Green_Light_Color();
    }
}

Color::Color Colors::Get_Random_Orange_Color() {
    if (Random::Get_Instance().Get_Num(1)) return this->Get_Random_Orange_Base_Color();
    else return this->Get_Random_Orange_Light_Color();
}

Color::Color Colors::Get_Random_Pink_Color() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Pink_Base_Color();
    case 1:     return this->Get_Random_Pink_Dark_Color();
    case 2:     return this->Get_Random_Pink_Light_Color();
    }
}

Color::Color Colors::Get_Random_Purple_Color() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Purple_Base_Color();
    case 1:     return this->Get_Random_Purple_Dark_Color();
    case 2:     return this->Get_Random_Purple_Light_Color();
    }
}

Color::Color Colors::Get_Random_Red_Color() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Red_Base_Color();
    case 1:     return this->Get_Random_Red_Dark_Color();
    case 2:     return this->Get_Random_Red_Light_Color();
    }
}

Color::Color Colors::Get_Random_Yellow_Color() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Yellow_Base_Color();
    case 1:     return this->Get_Random_Yellow_Dark_Color();
    case 2:     return this->Get_Random_Yellow_Light_Color();
    }
}

Color::Color Colors::Get_Random_Blue_Base_Color() {
    switch (Random::Get_Instance().Get_Num(3)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return Color::BLUE;
    case 1:     return Color::BLUE_DARK;
    case 2:     return Color::BLUE_LIGHT;
    case 3:     return Color::AQUAMARINE;
    }
}

Color::Color Colors::Get_Random_Blue_Dark_Color() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return Color::BLUE_DARKEST;
    case 1:     return Color::BLUE_DARKER;
    case 2:     return Color::AQUAMARINE_DARK;
    }
}

Color::Color Colors::Get_Random_Blue_Light_Color() {
    switch (Random::Get_Instance().Get_Num(4)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return Color::BLUE_LIGHTER;
    case 1:     return Color::BLUE_LIGHTEST;
    case 2:     return Color::AQUAMARINE_LIGHT;
    case 3:     return Color::AQUAMARINE_LIGHTEST;
    case 4:     return Color::BLUE_LIGHTEST_PURPLE;
    }
}

Color::Color Colors::Get_Random_Brown_Base_Color() {
    return Color::BROWN;
}

Color::Color Colors::Get_Random_Brown_Light_Color() {
    return Color::BROWN_LIGHT;
}

Color::Color Colors::Get_Random_Gray_Base_Color() {
    if (Random::Get_Instance().Get_Num(1)) return Color::GRAY_LIGHT;
    else return Color::GRAY;
}

Color::Color Colors::Get_Random_Gray_Dark_Color() {
    return Color::GRAY_DARK;
}

Color::Color Colors::Get_Random_Gray_Light_Color() {
    return Color::GRAY_LIGHTEST;
}

Color::Color Colors::Get_Random_Green_Base_Color() {
    switch (Random::Get_Instance().Get_Num(3)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return Color::GREEN;
    case 1:     return Color::GREEN_DARK;
    case 2:     return Color::TURQUOISE;
    case 3:     return Color::GREEN_LIGHT;
    }
}

Color::Color Colors::Get_Random_Green_Dark_Color() {
    switch (Random::Get_Instance().Get_Num(3)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return Color::GREEN_DARKER;
    case 1:     return Color::GREEN_DARKEST;
    case 2:     return Color::TURQUOISE_DARK;
    case 3:     return Color::OLIVE_DARK;
    }
}

Color::Color Colors::Get_Random_Green_Light_Color() {
    switch (Random::Get_Instance().Get_Num(4)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return Color::GREEN_LIGHTER;
    case 1:     return Color::GREEN_LIGHTEST;
    case 2:     return Color::GREEN_LIGHTEST_YELLOW;
    case 3:     return Color::TURQUOISE_LIGHT;
    case 4:     return Color::TURQUOISE_LIGHTEST;
    }
}

Color::Color Colors::Get_Random_Orange_Base_Color() {
    return Color::ORANGE;
}

Color::Color Colors::Get_Random_Orange_Light_Color() {
    return Color::ORANGE_LIGHT;
}

Color::Color Colors::Get_Random_Pink_Base_Color() {
    switch (Random::Get_Instance().Get_Num(4)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return Color::PINK;
    case 1:     return Color::PINK_DARK;
    case 2:     return Color::MAGENTA;
    case 3:     return Color::MAGENTA_LIGHT;
    }
}

Color::Color Colors::Get_Random_Pink_Dark_Color() {
    return Color::PINK_DARKEST;
}

Color::Color Colors::Get_Random_Pink_Light_Color() {
    if (Random::Get_Instance().Get_Num(1)) return Color::PINK_LIGHT;
    else return Color::MAGENTA_LIGHTEST;
}

Color::Color Colors::Get_Random_Purple_Base_Color() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return Color::PURPLE;
    case 1:     return Color::PURPLE_DARK;
    case 2:     return Color::PURPLE_LIGHT;
    }
}

Color::Color Colors::Get_Random_Purple_Dark_Color() {
    return Color::PURPLE_DARKEST;
}

Color::Color Colors::Get_Random_Purple_Light_Color() {
    return Color::PURPLE_LIGHTEST;
}

Color::Color Colors::Get_Random_Red_Base_Color() {
    if (Random::Get_Instance().Get_Num(1)) return Color::RED;
    else return Color::RED_LIGHT;
}

Color::Color Colors::Get_Random_Red_Dark_Color() {
    return Color::RED_DARK;
}

Color::Color Colors::Get_Random_Red_Light_Color() {
    return Color::RED_LIGHTEST;
}

Color::Color Colors::Get_Random_Yellow_Base_Color() {
    return Color::YELLOW;
}

Color::Color Colors::Get_Random_Yellow_Dark_Color() {
    return Color::OLIVE;
}

Color::Color Colors::Get_Random_Yellow_Light_Color() {
    return Color::YELLOW_LIGHT;
}

Color::Color Colors::Get_Random_Coral_Color() {
    if (Random::Get_Instance().Get_Num(12) == 0) return Color::GRAY_LIGHTEST;
    else return this->Get_Random_Dark_Shade_Color(0x03, 0x0D);
}

Color::Color Colors::Get_Random_Sky_Color() {
    if (Random::Get_Instance().Get_Num(1)) return this->Get_Random_Sky_Day_Color();
    else return this->Get_Random_Sky_Night_Color();
}

Color::Color Colors::Get_Random_Sky_Day_Color() {
    if (Random::Get_Instance().Get_Num(3) == 0) {
        if (Random::Get_Instance().Get_Num(1)) return this->Get_Random_Sky_Pink_Color();
        else return this->Get_Random_Sky_Orange_Color();
    } else {
        return this->Get_Random_Sky_Blue_Color();
    }
}

Color::Color Colors::Get_Random_Sky_Night_Color() {
    if (Random::Get_Instance().Get_Num(3) == 0) {
        if (Random::Get_Instance().Get_Num(1)) return this->Get_Random_Sky_Pink_Color();
        else return this->Get_Random_Sky_Orange_Color();
    } else {
        return Color::BLACK;
    }
}

Color::Color Colors::Get_Random_Sky_Blue_Color() {
    switch (Random::Get_Instance().Get_Num(8)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return Color::BLUE;
    case 1:     return Color::BLUE_DARK;
    case 2:     return Color::BLUE_LIGHT;
    case 3:     return Color::AQUAMARINE;
    case 4:     return Color::BLUE_DARKEST;
    case 5:     return Color::BLUE_DARKER;
    case 6:     return Color::AQUAMARINE_DARK;
    case 7:     return Color::BLUE_LIGHTER;
    case 8:     return Color::AQUAMARINE_LIGHT;
    }
}

Color::Color Colors::Get_Random_Sky_Orange_Color() {
    return Color::ORANGE;
}

Color::Color Colors::Get_Random_Sky_Pink_Color() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return Color::PINK;
    case 1:     return Color::PINK_DARK;
    case 2:     return Color::MAGENTA_LIGHT;
    }
}

Color::Color Colors::Get_Random_Tree_Dark_Color() {
    if (Random::Get_Instance().Get_Num(3) == 0) {
        if (Random::Get_Instance().Get_Num(1)) return this->Get_Random_Tree_Orange_Dark_Color();
        else return this->Get_Random_Tree_Yellow_Dark_Color();
    } else {
        return this->Get_Random_Tree_Green_Dark_Color();
    }
}

Color::Color Colors::Get_Random_Tree_Green_Dark_Color() {
    if (Random::Get_Instance().Get_Num(1)) return Color::GREEN;
    else return Color::GREEN_DARK;
}

Color::Color Colors::Get_Random_Tree_Orange_Dark_Color() {
    if (Random::Get_Instance().Get_Num(1)) return Color::RED;
    else return Color::BROWN_LIGHT;
}

Color::Color Colors::Get_Random_Tree_Yellow_Dark_Color() {
    return Color::OLIVE;
}

Color::Color Colors::Get_Random_Tree_Light_Color_From_Dark_Color(Color::Color darkColor) {
    if (this->Is_Green_Color(darkColor)) {
        if (Random::Get_Instance().Get_Num(1)) return Color::GREEN_LIGHT;
        else return Color::GREEN_LIGHTER;
    } else if (this->Is_Orange_Color(darkColor)) {
        if (Random::Get_Instance().Get_Num(1)) return Color::RED_LIGHT;
        else return Color::ORANGE;
    } else if (this->Is_Yellow_Color(darkColor)) {
        return Color::YELLOW;
    } else {
        assert(false); return Color::BLACK;
    }
}

Color::Color Colors::Get_Random_Underground_Color() {
    switch (Random::Get_Instance().Get_Num(24)) {
    default:     assert(false); return Color::BLACK;
    case 0:      return Color::BLUE;
    case 1:      return Color::BLUE_DARK;
    case 2:      return Color::PURPLE;
    case 3:      return Color::MAGENTA;
    case 4:      return Color::PINK_DARK;
    case 5:      return Color::RED;
    case 6:      return Color::BROWN_LIGHT;
    case 7:      return Color::OLIVE;
    case 8:      return Color::GREEN_DARK;
    case 9:      return Color::GREEN;
    case 10:     return Color::TURQUOISE;
    case 11:     return Color::AQUAMARINE;
    case 12:     return Color::BLUE_LIGHTER;
    case 13:     return Color::BLUE_LIGHT;
    case 14:     return Color::PURPLE_LIGHT;
    case 15:     return Color::MAGENTA_LIGHT;
    case 16:     return Color::PINK;
    case 17:     return Color::RED_LIGHT;
    case 18:     return Color::ORANGE;
    case 19:     return Color::YELLOW;
    case 20:     return Color::GREEN_LIGHT;
    case 21:     return Color::GREEN_LIGHTER;
    case 22:     return Color::TURQUOISE_LIGHT;
    case 23:     return Color::AQUAMARINE_LIGHT;
    case 24:     return Color::GRAY_DARK;
    }
}

Color::Color Colors::Get_Random_Water_Color() {
    switch (Random::Get_Instance().Get_Num(3)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return Color::BLUE;
    case 1:     return Color::BLUE_DARK;
    case 2:     return Color::BLUE_DARKEST;
    case 3:     return Color::BLUE_DARKER;
    }
}

bool Colors::Is_Shade_Min_Max_Valid(int min, int max) {
    if (min > max) return false;
    if (min < 0x00 || min > 0x0F) return false;
    if (max < 0x00 || max > 0x0F) return false;
    return true;
}

int Colors::Get_Color_Darkness_Weight(Color::Color color) {
    char r = 0x00, g = 0x00, b = 0x00;
    this->Get_Color_RGB(color, r, g, b);
    r = 0xFF-r; g = 0xFF-g; b = 0xFF-b;
    return r+g+b;
}

void Colors::Get_Color_RGB(Color::Color color, char &r, char &g, char &b) {
    switch (color) {
    case Color::GRAY:                   r = static_cast<char>(0x7F); g = static_cast<char>(0x7F); b = static_cast<char>(0x7F); return; //7f7f7f
    case Color::BLUE_DARKEST:           r = static_cast<char>(0x20); g = static_cast<char>(0x00); b = static_cast<char>(0xB0); return; //2000b0
    case Color::BLUE_DARKER:            r = static_cast<char>(0x28); g = static_cast<char>(0x00); b = static_cast<char>(0xB8); return; //2800b8
    case Color::PURPLE_DARKEST:         r = static_cast<char>(0x60); g = static_cast<char>(0x10); b = static_cast<char>(0xA0); return; //6010a0
    case Color::PURPLE_DARK:            r = static_cast<char>(0x98); g = static_cast<char>(0x20); b = static_cast<char>(0x78); return; //982078
    case Color::PINK_DARKEST:           r = static_cast<char>(0xB0); g = static_cast<char>(0x10); b = static_cast<char>(0x30); return; //b01030
    case Color::RED_DARK:               r = static_cast<char>(0xA0); g = static_cast<char>(0x30); b = static_cast<char>(0x00); return; //a03000
    case Color::BROWN:                  r = static_cast<char>(0x74); g = static_cast<char>(0x40); b = static_cast<char>(0x00); return; //784000
    case Color::OLIVE_DARK:             r = static_cast<char>(0x48); g = static_cast<char>(0x58); b = static_cast<char>(0x00); return; //485800
    case Color::GREEN_DARKEST:          r = static_cast<char>(0x38); g = static_cast<char>(0x68); b = static_cast<char>(0x00); return; //386800
    case Color::GREEN_DARKER:           r = static_cast<char>(0x38); g = static_cast<char>(0x6C); b = static_cast<char>(0x00); return; //386c00
    case Color::TURQUOISE_DARK:         r = static_cast<char>(0x30); g = static_cast<char>(0x60); b = static_cast<char>(0x40); return; //306040
    case Color::AQUAMARINE_DARK:        r = static_cast<char>(0x30); g = static_cast<char>(0x50); b = static_cast<char>(0x80); return; //305080
    case Color::BLACK:                  r = static_cast<char>(0x00); g = static_cast<char>(0x00); b = static_cast<char>(0x00); return; //000000
    case Color::GRAY_LIGHTEST:          r = static_cast<char>(0xBC); g = static_cast<char>(0xBC); b = static_cast<char>(0xBC); return; //bcbcbc
    case Color::BLUE:                   r = static_cast<char>(0x40); g = static_cast<char>(0x60); b = static_cast<char>(0xF8); return; //4060f8
    case Color::BLUE_DARK:              r = static_cast<char>(0x40); g = static_cast<char>(0x40); b = static_cast<char>(0xFF); return; //4040ff
    case Color::PURPLE:                 r = static_cast<char>(0x90); g = static_cast<char>(0x40); b = static_cast<char>(0xF0); return; //9040f0
    case Color::MAGENTA:                r = static_cast<char>(0xD8); g = static_cast<char>(0x40); b = static_cast<char>(0xC0); return; //d840c0
    case Color::PINK_DARK:              r = static_cast<char>(0xD8); g = static_cast<char>(0x40); b = static_cast<char>(0x60); return; //d84060
    case Color::RED:                    r = static_cast<char>(0xE0); g = static_cast<char>(0x50); b = static_cast<char>(0x00); return; //e05000
    case Color::BROWN_LIGHT:            r = static_cast<char>(0xC0); g = static_cast<char>(0x70); b = static_cast<char>(0x00); return; //c07000
    case Color::OLIVE:                  r = static_cast<char>(0x88); g = static_cast<char>(0x88); b = static_cast<char>(0x00); return; //888800
    case Color::GREEN_DARK:             r = static_cast<char>(0x50); g = static_cast<char>(0xA0); b = static_cast<char>(0x00); return; //50a000
    case Color::GREEN:                  r = static_cast<char>(0x48); g = static_cast<char>(0xA8); b = static_cast<char>(0x10); return; //48a810
    case Color::TURQUOISE:              r = static_cast<char>(0x48); g = static_cast<char>(0xA0); b = static_cast<char>(0x68); return; //48a068
    case Color::AQUAMARINE:             r = static_cast<char>(0x40); g = static_cast<char>(0x90); b = static_cast<char>(0xC0); return; //4090c0
    case Color::WHITE:                  r = static_cast<char>(0xFF); g = static_cast<char>(0xFF); b = static_cast<char>(0xFF); return; //ffffff
    case Color::BLUE_LIGHTER:           r = static_cast<char>(0x60); g = static_cast<char>(0xA0); b = static_cast<char>(0xFF); return; //60a0ff
    case Color::BLUE_LIGHT:             r = static_cast<char>(0x50); g = static_cast<char>(0x80); b = static_cast<char>(0xFF); return; //5080ff
    case Color::PURPLE_LIGHT:           r = static_cast<char>(0xA0); g = static_cast<char>(0x70); b = static_cast<char>(0xFF); return; //a070ff
    case Color::MAGENTA_LIGHT:          r = static_cast<char>(0xF0); g = static_cast<char>(0x60); b = static_cast<char>(0xFF); return; //f060ff
    case Color::PINK:                   r = static_cast<char>(0xFF); g = static_cast<char>(0x60); b = static_cast<char>(0xB0); return; //ff60b0
    case Color::RED_LIGHT:              r = static_cast<char>(0xFF); g = static_cast<char>(0x78); b = static_cast<char>(0x30); return; //ff7830
    case Color::ORANGE:                 r = static_cast<char>(0xFF); g = static_cast<char>(0xA0); b = static_cast<char>(0x00); return; //ffa000
    case Color::YELLOW:                 r = static_cast<char>(0xE8); g = static_cast<char>(0xD0); b = static_cast<char>(0x20); return; //e8d020
    case Color::GREEN_LIGHT:            r = static_cast<char>(0x98); g = static_cast<char>(0xE8); b = static_cast<char>(0x00); return; //98e800
    case Color::GREEN_LIGHTER:          r = static_cast<char>(0x70); g = static_cast<char>(0xF0); b = static_cast<char>(0x40); return; //70f040
    case Color::TURQUOISE_LIGHT:        r = static_cast<char>(0x70); g = static_cast<char>(0xE0); b = static_cast<char>(0x90); return; //70e090
    case Color::AQUAMARINE_LIGHT:       r = static_cast<char>(0x60); g = static_cast<char>(0xD0); b = static_cast<char>(0xE0); return; //60d0e0
    case Color::GRAY_DARK:              r = static_cast<char>(0x60); g = static_cast<char>(0x60); b = static_cast<char>(0x60); return; //606060
    case Color::BLUE_LIGHTEST:          r = static_cast<char>(0x90); g = static_cast<char>(0xD0); b = static_cast<char>(0xFF); return; //90d0ff
    case Color::BLUE_LIGHTEST_PURPLE:   r = static_cast<char>(0xA0); g = static_cast<char>(0xB8); b = static_cast<char>(0xFF); return; //a0b8ff
    case Color::PURPLE_LIGHTEST:        r = static_cast<char>(0xC0); g = static_cast<char>(0xB0); b = static_cast<char>(0xFF); return; //c0b0ff
    case Color::MAGENTA_LIGHTEST:       r = static_cast<char>(0xE0); g = static_cast<char>(0xB0); b = static_cast<char>(0xFF); return; //e0b0ff
    case Color::PINK_LIGHT:             r = static_cast<char>(0xFF); g = static_cast<char>(0xB8); b = static_cast<char>(0xE8); return; //ffb8e8
    case Color::RED_LIGHTEST:           r = static_cast<char>(0xFF); g = static_cast<char>(0xC8); b = static_cast<char>(0xB8); return; //ffc8b8
    case Color::ORANGE_LIGHT:           r = static_cast<char>(0xFF); g = static_cast<char>(0xD8); b = static_cast<char>(0xA0); return; //ffd8a0
    case Color::YELLOW_LIGHT:           r = static_cast<char>(0xFF); g = static_cast<char>(0xF0); b = static_cast<char>(0x90); return; //fff090
    case Color::GREEN_LIGHTEST_YELLOW:  r = static_cast<char>(0xC8); g = static_cast<char>(0xF0); b = static_cast<char>(0x80); return; //c8f080
    case Color::GREEN_LIGHTEST:         r = static_cast<char>(0xA0); g = static_cast<char>(0xF0); b = static_cast<char>(0xA0); return; //a0f0a0
    case Color::TURQUOISE_LIGHTEST:     r = static_cast<char>(0xA0); g = static_cast<char>(0xFF); b = static_cast<char>(0xC8); return; //a0ffc8
    case Color::AQUAMARINE_LIGHTEST:    r = static_cast<char>(0xA0); g = static_cast<char>(0xFF); b = static_cast<char>(0xF0); return; //a0fff0
    case Color::GRAY_LIGHT:             r = static_cast<char>(0xA0); g = static_cast<char>(0xA0); b = static_cast<char>(0xA0); return; //a0a0a0
    }
}

bool Colors::Is_Blue_Color(Color::Color color) {
    switch (color) {
    default:                            return false;
    case Color::BLUE:                   return true;
    case Color::BLUE_DARK:              return true;
    case Color::BLUE_DARKER:            return true;
    case Color::BLUE_DARKEST:           return true;
    case Color::BLUE_LIGHT:             return true;
    case Color::BLUE_LIGHTER:           return true;
    case Color::BLUE_LIGHTEST:          return true;
    case Color::BLUE_LIGHTEST_PURPLE:   return true;
    case Color::AQUAMARINE:             return true;
    case Color::AQUAMARINE_DARK:        return true;
    case Color::AQUAMARINE_LIGHT:       return true;
    case Color::AQUAMARINE_LIGHTEST:    return true;
    }
}

bool Colors::Is_Brown_Color(Color::Color color) {
    switch (color) {
    default:                    return false;
    case Color::BROWN:          return true;
    case Color::BROWN_LIGHT:    return true;
    }
}

bool Colors::Is_Gray_Color(Color::Color color) {
    switch (color) {
    default:                    return false;
    case Color::GRAY:           return true;
    case Color::GRAY_DARK:      return true;
    case Color::GRAY_LIGHT:     return true;
    case Color::GRAY_LIGHTEST:  return true;
    case Color::BLACK:          return true;
    case Color::WHITE:          return true;
    }
}

bool Colors::Is_Green_Color(Color::Color color) {
    switch (color) {
    default:                            return false;
    case Color::GREEN:                  return true;
    case Color::GREEN_DARK:             return true;
    case Color::GREEN_DARKER:           return true;
    case Color::GREEN_DARKEST:          return true;
    case Color::GREEN_LIGHT:            return true;
    case Color::GREEN_LIGHTER:          return true;
    case Color::GREEN_LIGHTEST:         return true;
    case Color::GREEN_LIGHTEST_YELLOW:  return true;
    case Color::TURQUOISE:              return true;
    case Color::TURQUOISE_DARK:         return true;
    case Color::TURQUOISE_LIGHT:        return true;
    case Color::TURQUOISE_LIGHTEST:     return true;
    }
}

bool Colors::Is_Orange_Color(Color::Color color) {
    switch (color) {
    default:                    return false;
    case Color::RED_LIGHT:      return true;
    case Color::RED:            return true;
    case Color::RED_DARK:       return true;
    case Color::ORANGE:         return true;
    case Color::ORANGE_LIGHT:   return true;
    }
}

bool Colors::Is_Pink_Color(Color::Color color) {
    switch (color) {
    default:                        return false;
    case Color::PINK:               return true;
    case Color::RED_LIGHTEST:       return true;
    case Color::MAGENTA:            return true;
    case Color::MAGENTA_LIGHT:      return true;
    case Color::MAGENTA_LIGHTEST:   return true;
    }
}

bool Colors::Is_Purple_Color(Color::Color color) {
    switch (color) {
    default:                        return false;
    case Color::PURPLE:             return true;
    case Color::PURPLE_DARK:        return true;
    case Color::PURPLE_DARKEST:     return true;
    case Color::PURPLE_LIGHT:       return true;
    case Color::PURPLE_LIGHTEST:    return true;
    }
}

bool Colors::Is_Red_Color(Color::Color color) {
    switch (color) {
    default:                        return false;
    case Color::PINK_DARK:          return true;
    case Color::PINK_DARKEST:       return true;
    case Color::PINK_LIGHT:         return true;
    }
}

bool Colors::Is_Yellow_Color(Color::Color color) {
    switch (color) {
    default:                    return false;
    case Color::OLIVE:          return true;
    case Color::YELLOW:         return true;
    case Color::YELLOW_LIGHT:   return true;
    }
}
