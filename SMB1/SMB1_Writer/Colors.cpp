#include "Colors.h"
#include "../../Common_Files/Random.h"
#include <assert.h>

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
    case 0x03:      color = Color::PURPLE_DARK; return true;
    case 0x04:      color = Color::MAGENTA_DARK; return true;
    case 0x05:      color = Color::RED; return true;
    case 0x06:      color = Color::ORANGE_DARKEST; return true;
    case 0x07:      color = Color::BROWN; return true;
    case 0x08:      color = Color::OLIVE_DARK; return true;
    case 0x09:      color = Color::GREEN_DARKEST; return true;
    case 0x0A:      color = Color::GREEN_DARKER; return true;
    case 0x0B:      color = Color::PARSLAY; return true;
    case 0x0C:      color = Color::TURQUOISE_DARKEST; return true;
    case 0x0D:      color = Color::BLACK; return true;
    case 0x0E:      color = Color::BLACK; return true;
    case 0x0F:      color = Color::BLACK; return true;
    case 0x10:      color = Color::GRAY_LIGHTEST; return true;
    case 0x11:      color = Color::BLUE_LIGHT; return true;
    case 0x12:      color = Color::BLUE_DARK; return true;
    case 0x13:      color = Color::PURPLE; return true;
    case 0x14:      color = Color::MAGENTA; return true;
    case 0x15:      color = Color::RED_LIGHT; return true;
    case 0x16:      color = Color::ORANGE_DARK; return true;
    case 0x17:      color = Color::BROWN_LIGHT; return true;
    case 0x18:      color = Color::OLIVE; return true;
    case 0x19:      color = Color::GREEN_DARK; return true;
    case 0x1A:      color = Color::GREEN; return true;
    case 0x1B:      color = Color::PARSLAY_LIGHT; return true;
    case 0x1C:      color = Color::TURQUOISE_DARK; return true;
    case 0x1D:      color = Color::BLACK; return true;
    case 0x1E:      color = Color::BLACK; return true;
    case 0x1F:      color = Color::BLACK; return true;
    case 0x20:      color = Color::WHITE; return true;
    case 0x21:      color = Color::BLUE; return true;
    case 0x22:      color = Color::BLUE_LIGHTER; return true;
    case 0x23:      color = Color::PURPLE_LIGHT; return true;
    case 0x24:      color = Color::MAGENTA_LIGHT; return true;
    case 0x25:      color = Color::PINK; return true;
    case 0x26:      color = Color::ORANGE; return true;
    case 0x27:      color = Color::ORANGE_LIGHT; return true;
    case 0x28:      color = Color::YELLOW; return true;
    case 0x29:      color = Color::GREEN_LIGHT; return true;
    case 0x2A:      color = Color::GREEN_LIGHTER; return true;
    case 0x2B:      color = Color::AQUAMARINE; return true;
    case 0x2C:      color = Color::TURQUOISE; return true;
    case 0x2D:      color = Color::GRAY_DARK; return true;
    case 0x2E:      color = Color::BLACK; return true;
    case 0x2F:      color = Color::BLACK; return true;
    case 0x30:      color = Color::WHITE; return true;
    case 0x31:      color = Color::BLUE_LIGHTEST; return true;
    case 0x32:      color = Color::BLUE_LIGHTEST_PURPLE; return true;
    case 0x33:      color = Color::PURPLE_LIGHTEST; return true;
    case 0x34:      color = Color::MAGENTA_LIGHTEST; return true;
    case 0x35:      color = Color::PINK_LIGHT; return true;
    case 0x36:      color = Color::PINK_LIGHTEST; return true;
    case 0x37:      color = Color::ORANGE_LIGHTEST; return true;
    case 0x38:      color = Color::YELLOW_LIGHT; return true;
    case 0x39:      color = Color::GREEN_LIGHTEST_YELLOW; return true;
    case 0x3A:      color = Color::GREEN_LIGHTEST; return true;
    case 0x3B:      color = Color::AQUAMARINE_LIGHT; return true;
    case 0x3C:      color = Color::TURQUOISE_LIGHT; return true;
    case 0x3D:      color = Color::GRAY_LIGHT; return true;
    case 0x3E:      color = Color::BLACK; return true;
    case 0x3F:      color = Color::BLACK; return true;
    }
}

Color::Color Colors::Which_Color_Is_Darker(Color::Color color1, Color::Color color2) {
    if (color1 == color2) return color1;
    int color1Weight = this->Get_Color_Darkness_Weight(color1);
    int color2Weight = this->Get_Color_Darkness_Weight(color2);
    assert(color1Weight != color2Weight);
    if (color1Weight > color2Weight) return color1;
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

Color::Color Colors::Get_Random_Color() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Base_Color();
    case 1:     return this->Get_Random_Dark_Color();
    case 2:     return this->Get_Random_Light_Color();
    }
}

Color::Color Colors::Get_Random_Base_Color() {
    switch (Random::Get_Instance().Get_Num(10)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Blue_Base_Color();
    case 1:     return this->Get_Random_Brown_Base_Color();
    case 2:     return this->Get_Random_Gray_Base_Color();
    case 3:     return this->Get_Random_Green_Base_Color();
    case 4:     return this->Get_Random_Magenta_Base_Color();
    case 5:     return this->Get_Random_Orange_Base_Color();
    case 6:     return this->Get_Random_Pink_Base_Color();
    case 7:     return this->Get_Random_Purple_Base_Color();
    case 8:     return this->Get_Random_Red_Base_Color();
    case 9:     return this->Get_Random_Turquoise_Base_Color();
    case 10:    return this->Get_Random_Yellow_Base_Color();
    }
}

Color::Color Colors::Get_Random_Dark_Color() {
    switch (Random::Get_Instance().Get_Num(10)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Blue_Dark_Color();
    case 1:     return this->Get_Random_Brown_Dark_Color();
    case 2:     return this->Get_Random_Gray_Dark_Color();
    case 3:     return this->Get_Random_Green_Dark_Color();
    case 4:     return this->Get_Random_Magenta_Dark_Color();
    case 5:     return this->Get_Random_Orange_Dark_Color();
    case 6:     return this->Get_Random_Pink_Dark_Color();
    case 7:     return this->Get_Random_Purple_Dark_Color();
    case 8:     return this->Get_Random_Red_Dark_Color();
    case 9:     return this->Get_Random_Turquoise_Dark_Color();
    case 10:    return this->Get_Random_Yellow_Dark_Color();
    }
}

Color::Color Colors::Get_Random_Light_Color() {
    switch (Random::Get_Instance().Get_Num(10)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Blue_Light_Color();
    case 1:     return this->Get_Random_Brown_Light_Color();
    case 2:     return this->Get_Random_Gray_Light_Color();
    case 3:     return this->Get_Random_Green_Light_Color();
    case 4:     return this->Get_Random_Magenta_Light_Color();
    case 5:     return this->Get_Random_Orange_Light_Color();
    case 6:     return this->Get_Random_Pink_Light_Color();
    case 7:     return this->Get_Random_Purple_Light_Color();
    case 8:     return this->Get_Random_Red_Light_Color();
    case 9:     return this->Get_Random_Turquoise_Light_Color();
    case 10:    return this->Get_Random_Yellow_Light_Color();
    }
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
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Brown_Base_Color();
    case 1:     return this->Get_Random_Brown_Dark_Color();
    case 2:     return this->Get_Random_Brown_Light_Color();
    }
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

Color::Color Colors::Get_Random_Magenta_Color() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Magenta_Base_Color();
    case 1:     return this->Get_Random_Magenta_Dark_Color();
    case 2:     return this->Get_Random_Magenta_Light_Color();
    }
}

Color::Color Colors::Get_Random_Orange_Color() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Orange_Base_Color();
    case 1:     return this->Get_Random_Orange_Dark_Color();
    case 2:     return this->Get_Random_Orange_Light_Color();
    }
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

Color::Color Colors::Get_Random_Turquoise_Color() {
    switch (Random::Get_Instance().Get_Num(2)) {
    default:    assert(false); return Color::BLACK;
    case 0:     return this->Get_Random_Turquoise_Base_Color();
    case 1:     return this->Get_Random_Turquoise_Dark_Color();
    case 2:     return this->Get_Random_Turquoise_Light_Color();
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

}

Color::Color Colors::Get_Random_Blue_Dark_Color() {

}

Color::Color Colors::Get_Random_Blue_Light_Color() {

}

Color::Color Colors::Get_Random_Brown_Base_Color() {

}

Color::Color Colors::Get_Random_Brown_Dark_Color() {

}

Color::Color Colors::Get_Random_Brown_Light_Color() {

}

Color::Color Colors::Get_Random_Gray_Base_Color() {

}

Color::Color Colors::Get_Random_Gray_Dark_Color() {

}

Color::Color Colors::Get_Random_Gray_Light_Color() {

}

Color::Color Colors::Get_Random_Green_Base_Color() {

}

Color::Color Colors::Get_Random_Green_Dark_Color() {

}

Color::Color Colors::Get_Random_Green_Light_Color() {

}

Color::Color Colors::Get_Random_Magenta_Base_Color() {

}

Color::Color Colors::Get_Random_Magenta_Dark_Color() {

}

Color::Color Colors::Get_Random_Magenta_Light_Color() {

}

Color::Color Colors::Get_Random_Orange_Base_Color() {

}

Color::Color Colors::Get_Random_Orange_Dark_Color() {

}

Color::Color Colors::Get_Random_Orange_Light_Color() {

}

Color::Color Colors::Get_Random_Pink_Base_Color() {

}

Color::Color Colors::Get_Random_Pink_Dark_Color() {

}

Color::Color Colors::Get_Random_Pink_Light_Color() {

}

Color::Color Colors::Get_Random_Purple_Base_Color() {

}

Color::Color Colors::Get_Random_Purple_Dark_Color() {

}

Color::Color Colors::Get_Random_Purple_Light_Color() {

}

Color::Color Colors::Get_Random_Red_Base_Color() {

}

Color::Color Colors::Get_Random_Red_Dark_Color() {

}

Color::Color Colors::Get_Random_Red_Light_Color() {

}

Color::Color Colors::Get_Random_Turquoise_Base_Color() {

}

Color::Color Colors::Get_Random_Turquoise_Dark_Color() {

}

Color::Color Colors::Get_Random_Turquoise_Light_Color() {

}

Color::Color Colors::Get_Random_Yellow_Base_Color() {

}

Color::Color Colors::Get_Random_Yellow_Dark_Color() {

}

Color::Color Colors::Get_Random_Yellow_Light_Color() {

}

int Colors::Get_Color_Darkness_Weight(Color::Color color) {

}
