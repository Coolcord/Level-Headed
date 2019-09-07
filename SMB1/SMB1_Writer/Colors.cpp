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
    case 0x15:      color = Color::RED_LIGHT; return true;
    case 0x16:      color = Color::ORANGE_DARK; return true;
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
    case 0x26:      color = Color::ORANGE; return true;
    case 0x27:      color = Color::ORANGE_LIGHT; return true;
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
    case 0x36:      color = Color::PINK_LIGHTEST; return true;
    case 0x37:      color = Color::ORANGE_LIGHTEST; return true;
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
    switch (Random::Get_Instance().Get_Num(11)) {
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
    case 11:    return Color::BLACK;
    }
}

Color::Color Colors::Get_Random_Light_Color() {
    switch (Random::Get_Instance().Get_Num(11)) {
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
    case 11:    return Color::WHITE;
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
    case Color::PURPLE_DARK:            r = static_cast<char>(0x60); g = static_cast<char>(0x10); b = static_cast<char>(0xA0); return; //6010a0
    case Color::MAGENTA_DARK:           r = static_cast<char>(0x98); g = static_cast<char>(0x20); b = static_cast<char>(0x78); return; //982078
    case Color::RED:                    r = static_cast<char>(0xB0); g = static_cast<char>(0x10); b = static_cast<char>(0x30); return; //b01030
    case Color::ORANGE_DARKEST:         r = static_cast<char>(0xA0); g = static_cast<char>(0x30); b = static_cast<char>(0x00); return; //a03000
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
    case Color::RED_LIGHT:              r = static_cast<char>(0xD8); g = static_cast<char>(0x40); b = static_cast<char>(0x60); return; //d84060
    case Color::ORANGE_DARK:            r = static_cast<char>(0xE0); g = static_cast<char>(0x50); b = static_cast<char>(0x00); return; //e05000
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
    case Color::ORANGE:                 r = static_cast<char>(0xFF); g = static_cast<char>(0x78); b = static_cast<char>(0x30); return; //ff7830
    case Color::ORANGE_LIGHT:           r = static_cast<char>(0xFF); g = static_cast<char>(0xA0); b = static_cast<char>(0x00); return; //ffa000
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
    case Color::PINK_LIGHTEST:          r = static_cast<char>(0xFF); g = static_cast<char>(0xC8); b = static_cast<char>(0xB8); return; //ffc8b8
    case Color::ORANGE_LIGHTEST:        r = static_cast<char>(0xFF); g = static_cast<char>(0xD8); b = static_cast<char>(0xA0); return; //ffd8a0
    case Color::YELLOW_LIGHT:           r = static_cast<char>(0xFF); g = static_cast<char>(0xF0); b = static_cast<char>(0x90); return; //fff090
    case Color::GREEN_LIGHTEST_YELLOW:  r = static_cast<char>(0xC8); g = static_cast<char>(0xF0); b = static_cast<char>(0x80); return; //c8f080
    case Color::GREEN_LIGHTEST:         r = static_cast<char>(0xA0); g = static_cast<char>(0xF0); b = static_cast<char>(0xA0); return; //a0f0a0
    case Color::TURQUOISE_LIGHTEST:     r = static_cast<char>(0xA0); g = static_cast<char>(0xFF); b = static_cast<char>(0xC8); return; //a0ffc8
    case Color::AQUAMARINE_LIGHTEST:    r = static_cast<char>(0xA0); g = static_cast<char>(0xFF); b = static_cast<char>(0xF0); return; //a0fff0
    case Color::GRAY_LIGHT:             r = static_cast<char>(0xA0); g = static_cast<char>(0xA0); b = static_cast<char>(0xA0); return; //a0a0a0
    }
}
