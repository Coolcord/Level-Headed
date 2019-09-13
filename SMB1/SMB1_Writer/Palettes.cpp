#include "Palettes.h"
#include "../../Common_Files/Random.h"
#include "Colors.h"

Palettes::Palettes(QFile *f, Level_Offset *lo) : Byte_Writer(f, lo) {
    this->colors = new Colors();
    this->paletteMode = 1;
}

Palettes::~Palettes() {
    delete this->colors;
}

bool Palettes::Randomize_Palettes(int paletteMode) {
    if (paletteMode < 1 || paletteMode > 10) return false;
    this->paletteMode = paletteMode;
    if (this->paletteMode == 1) return true; //original palette specified. Nothing more to do

    if (!this->Randomize_Color_Groups()) return false;
    if (!this->Coin_Palette_Random()) return false;
    if (!this->Sky_Palette_Random()) return false;
    if (!this->Overworld_Random()) return false;
    if (!this->Underground_Random()) return false;
    if (!this->Underwater_Random()) return false;
    if (!this->Castle_Random()) return false;

    return true;
}

bool Palettes::Coin_Palette_Random() {
    Color::Color nonShinyColor = Color::BLACK;

    //Determine how many colors to use in the flash animation
    if (Random::Get_Instance().Get_Num(1)) { //4 color flash
        Color::Color colorLightest = this->colors->Get_Random_Lightest_Shade_Color();
        Color::Color colorLight = Color::BLACK, colorDark = Color::BLACK, colorDarkest = Color::BLACK;
        if (colorLightest == Color::WHITE) {
            colorLight = Color::GRAY_LIGHTEST; colorDark = Color::GRAY_LIGHT; colorDarkest = Color::GRAY;
        } else if (colorLightest == Color::GRAY_LIGHT) {
            colorLightest = Color::GRAY_LIGHTEST; colorLight = Color::GRAY_LIGHT; colorDark = Color::GRAY; colorDarkest = Color::GRAY_DARK;
        } else {
            //Possibly increment by a slight step up instead of a shade increase
            bool colorsValid = false;
            if (Random::Get_Instance().Get_Num(1)) {
                colorLight = this->colors->Get_Slightly_Darker_Color_From_Color(colorLightest);
                colorDark = this->colors->Get_Slightly_Darker_Color_From_Color(colorLight);
                colorDarkest = this->colors->Get_Slightly_Darker_Color_From_Color(colorDark);
                if (colorDark != Color::BLACK) colorsValid = true;
            }

            //Increment via Shade
            if (!colorsValid) {
                colorLight = this->colors->Get_Darker_Shade_From_Color(colorLightest);
                colorDark = this->colors->Get_Darker_Shade_From_Color(colorLight);
                colorDarkest = this->colors->Get_Darker_Shade_From_Color(colorDark);
            }
        }
        nonShinyColor = colorDark;

        //Write the flash animation
        if (!this->Write_Bytes_To_Offset(0x09D3, this->colors->Get_QByteArray_From_Color(colorDarkest))) return false;
        if (!this->Write_Bytes_To_Offset(0x09D4, this->colors->Get_QByteArray_From_Color(colorDark))) return false;
        if (!this->Write_Bytes_To_Offset(0x09D5, this->colors->Get_QByteArray_From_Color(colorLight))) return false;
        if (!this->Write_Bytes_To_Offset(0x09D6, this->colors->Get_QByteArray_From_Color(colorLightest))) return false;
        if (!this->Write_Bytes_To_Offset(0x09D7, this->colors->Get_QByteArray_From_Color(colorLight))) return false;
        if (!this->Write_Bytes_To_Offset(0x09D8, this->colors->Get_QByteArray_From_Color(colorDark))) return false;
    } else { //3 color flash
        bool invertColors = false;
        Color::Color colorDark = this->colors->Get_Random_Darkest_Shade_Color();
        if (colorDark != Color::GRAY && Random::Get_Instance().Get_Num(1)) {
            colorDark = this->colors->Get_Lighter_Shade_From_Color(colorDark);
            invertColors = true;
        }
        Color::Color colorBase = this->colors->Get_Lighter_Shade_From_Color(colorDark);
        Color::Color colorLight = this->colors->Get_Lighter_Shade_From_Color(colorBase);

        //Swap the dark and light color if a lighter shade is being used
        if (invertColors) {
            Color::Color tmpColor = colorDark;
            colorDark = colorLight;
            colorLight = tmpColor;
        }
        nonShinyColor = colorBase;

        //Determine Flash Style
        if (Random::Get_Instance().Get_Num(1)) { //original
            if (!this->Write_Bytes_To_Offset(0x09D3, QByteArray(3, static_cast<char>(this->colors->Get_Hex_From_Color(colorLight))))) return false;
            if (!this->Write_Bytes_To_Offset(0x09D6, this->colors->Get_QByteArray_From_Color(colorBase))) return false;
            if (!this->Write_Bytes_To_Offset(0x09D7, this->colors->Get_QByteArray_From_Color(colorDark))) return false;
            if (!this->Write_Bytes_To_Offset(0x09D8, this->colors->Get_QByteArray_From_Color(colorBase))) return false;
        } else { //equal time as light and dark
            if (!this->Write_Bytes_To_Offset(0x09D3, this->colors->Get_QByteArray_From_Color(colorLight))) return false;
            if (!this->Write_Bytes_To_Offset(0x09D4, this->colors->Get_QByteArray_From_Color(colorBase))) return false;
            if (!this->Write_Bytes_To_Offset(0x09D5, QByteArray(2, static_cast<char>(this->colors->Get_Hex_From_Color(colorDark))))) return false;
            if (!this->Write_Bytes_To_Offset(0x09D7, this->colors->Get_QByteArray_From_Color(colorBase))) return false;
            if (!this->Write_Bytes_To_Offset(0x09D8, this->colors->Get_QByteArray_From_Color(colorLight))) return false;
        }
    }

    //Write the Non-shiny Block Base Color
    if (this->paletteMode >= 5) {
        if (!this->Write_Bytes_To_Offset(0x09E7, this->colors->Get_QByteArray_From_Color(nonShinyColor))) return false; //overworld
        if (!this->Write_Bytes_To_Offset(0x09EB, this->colors->Get_QByteArray_From_Color(nonShinyColor))) return false; //underground
        if (!this->Write_Bytes_To_Offset(0x09EF, this->colors->Get_QByteArray_From_Color(nonShinyColor))) return false; //castle
    }
    return true;
}

bool Palettes::Sky_Palette_Random() {
    if (this->paletteMode >= 10) {
        if (!this->Write_Bytes_To_Offset(0x05DF, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Base_Or_Dark_Color()))) return false; //underwater sky color
        if (!this->Write_Bytes_To_Offset(0x05E0, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Base_Or_Dark_Color()))) return false; //overworld day
        if (!this->Write_Bytes_To_Offset(0x05E1, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Base_Or_Dark_Color()))) return false; //underground (this also affects the lives screen)
        if (!this->Write_Bytes_To_Offset(0x05E2, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Base_Or_Dark_Color()))) return false; //castle
        if (!this->Write_Bytes_To_Offset(0x05E3, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Base_Or_Dark_Color()))) return false; //Night
        if (!this->Write_Bytes_To_Offset(0x05E4, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Base_Or_Dark_Color()))) return false; //Snow
        if (!this->Write_Bytes_To_Offset(0x05E5, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Base_Or_Dark_Color()))) return false; //Night and Snow (also used underground when the snow palette is used)
        return this->Write_Bytes_To_Offset(0x05E6, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Base_Or_Dark_Color())); //Night and Freeze
    } else {
        if (this->paletteMode >= 6) {
            if (!this->Write_Bytes_To_Offset(0x05DF, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Sky_Color()))) return false; //underwater sky color
            if (!this->Write_Bytes_To_Offset(0x05E0, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Sky_Day_Color()))) return false; //overworld day
            if (!this->Write_Bytes_To_Offset(0x05E1, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false; //underground (this also affects the lives screen)
            if (!this->Write_Bytes_To_Offset(0x05E2, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false; //castle
            if (!this->Write_Bytes_To_Offset(0x05E3, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Sky_Night_Color()))) return false; //Night
            if (!this->Write_Bytes_To_Offset(0x05E4, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Sky_Day_Color()))) return false; //Snow
            if (!this->Write_Bytes_To_Offset(0x05E5, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false; //Night and Snow (also used underground when the snow palette is used)
            return this->Write_Bytes_To_Offset(0x05E6, this->colors->Get_QByteArray_From_Color(Color::BLACK)); //Night and Freeze
        } else {
            Color::Color skyColor = Color::BLACK;
            if (Random::Get_Instance().Get_Num(1)) skyColor = this->colors->Get_Random_Sky_Blue_Color();
            if (!this->Write_Bytes_To_Offset(0x05DF, this->colors->Get_QByteArray_From_Color(skyColor))) return false; //underwater sky color
            if (!this->Write_Bytes_To_Offset(0x05E0, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Sky_Blue_Color()))) return false; //overworld day
            if (!this->Write_Bytes_To_Offset(0x05E1, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false; //underground (this also affects the lives screen)
            if (!this->Write_Bytes_To_Offset(0x05E2, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false; //castle
            if (!this->Write_Bytes_To_Offset(0x05E3, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false; //Night
            if (!this->Write_Bytes_To_Offset(0x05E4, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Sky_Blue_Color()))) return false; //Snow
            if (!this->Write_Bytes_To_Offset(0x05E5, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false; //Night and Snow (also used underground when the snow palette is used)
            return this->Write_Bytes_To_Offset(0x05E6, this->colors->Get_QByteArray_From_Color(Color::BLACK)); //Night and Freeze
        }
    }
}

bool Palettes::Castle_Random() {
    Color::Color baseColor = Color::BLACK, darkColor = Color::BLACK, lightColor = Color::BLACK, outlineColor = Color::BLACK;
    if (this->paletteMode < 4) {
        baseColor = this->colors->Get_Random_Tree_Snow_Dark_Color();
        darkColor = this->colors->Get_Darkest_Shade_From_Color(baseColor);
        lightColor = this->colors->Get_Random_Tree_Light_Color_From_Dark_Color(baseColor);
        outlineColor = this->colors->Get_Random_Tree_Snow_Outline_Color();
    } else {
        baseColor = this->colors->Get_Random_Base_Color();
        darkColor = this->colors->Get_Darkest_Shade_From_Color(baseColor);
        lightColor = this->colors->Get_Lightest_Shade_From_Color(baseColor);
    }
    if (!this->Write_Bytes_To_Offset(0x09F0, this->colors->Get_QByteArray_From_Color(outlineColor))) return false; //nonshiny border color
    if (!this->Write_Bytes_To_Offset(0x0D28, this->colors->Get_QByteArray_From_Color(lightColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0D29, this->colors->Get_QByteArray_From_Color(baseColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0D2A, this->colors->Get_QByteArray_From_Color(outlineColor))) return false;

    //Get a Random Lava Color
    if (this->paletteMode >= 5) {
        if (!this->Write_Bytes_To_Offset(0x0D2D, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Color()))) return false;
    }

    //Handle Pipe Colors
    if (this->paletteMode >= 5 && !this->Get_Random_Pipe_Colors(0x0D24)) return false;
    return this->Apply_Color_Glow(0x0D38, 0x0D40, 0x0D24, baseColor, darkColor, lightColor);
}

bool Palettes::Overworld_Random() {
    //Random Water Color
    if (this->paletteMode >= 8) {
        if (!this->Write_Bytes_To_Offset(0x0CE5, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Color()))) return false;
    } else {
        if (!this->Write_Bytes_To_Offset(0x0CE5, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Water_Color()))) return false;
    }

    //Random Brick Color
    if (this->paletteMode >= 8) {
        Color::Color baseBrickColor = this->colors->Get_Random_Base_Color();
        Color::Color lightBrickColor = this->colors->Get_Lightest_Shade_From_Color(baseBrickColor);
        if (!this->Write_Bytes_To_Offset(0x09E8, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false; //nonshiny border color
        if (!this->Write_Bytes_To_Offset(0x0CE0, this->colors->Get_QByteArray_From_Color(lightBrickColor))) return false;
        if (!this->Write_Bytes_To_Offset(0x0CE1, this->colors->Get_QByteArray_From_Color(baseBrickColor))) return false;
        if (!this->Write_Bytes_To_Offset(0x0CE2, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false;
    }

    //Random Tree Colors
    Color::Color darkColor = Color::BLACK, lightColor = Color::BLACK, outlineColor = Color::BLACK;
    if (this->paletteMode < 7) {
        darkColor = this->colors->Get_Random_Tree_Green_Dark_Color();
        lightColor = this->colors->Get_Random_Tree_Light_Color_From_Dark_Color(darkColor);
        outlineColor = Color::BLACK;
    } else {
        lightColor = this->colors->Get_Random_Pipe_Light_Color();
        darkColor = this->colors->Get_Random_Pipe_Dark_Color_From_Light_Color(lightColor);
        outlineColor = Color::BLACK;
    }
    if (!this->Write_Bytes_To_Offset(0x0CDC, this->colors->Get_QByteArray_From_Color(lightColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0CDD, this->colors->Get_QByteArray_From_Color(darkColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0CDE, this->colors->Get_QByteArray_From_Color(outlineColor))) return false;

    //Random Mushroom Colors (also affects trees)
    if (this->paletteMode < 7) {
        darkColor = this->colors->Get_Random_Tree_Orange_Dark_Color();
        lightColor = this->colors->Get_Random_Tree_Light_Color_From_Dark_Color(darkColor);
        outlineColor = Color::BLACK;
    } else {
        lightColor = this->colors->Get_Random_Pipe_Light_Color();
        darkColor = this->colors->Get_Random_Pipe_Dark_Color_From_Light_Color(lightColor);
        outlineColor = Color::BLACK;
    }
    if (!this->Write_Bytes_To_Offset(0x0D58, this->colors->Get_QByteArray_From_Color(lightColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0D59, this->colors->Get_QByteArray_From_Color(darkColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0D5A, this->colors->Get_QByteArray_From_Color(outlineColor))) return false;


    //Random Snow Day Colors
    if (this->paletteMode < 7) {
        darkColor = this->colors->Get_Random_Tree_Snow_Dark_Color();
        lightColor = this->colors->Get_Random_Tree_Light_Color_From_Dark_Color(darkColor);
        outlineColor = this->colors->Get_Random_Tree_Snow_Outline_Color();
    } else {
        lightColor = this->colors->Get_Random_Pipe_Light_Color();
        darkColor = this->colors->Get_Random_Pipe_Dark_Color_From_Light_Color(lightColor);
        outlineColor = Color::BLACK;
    }
    if (!this->Write_Bytes_To_Offset(0x0D48, this->colors->Get_QByteArray_From_Color(lightColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0D49, this->colors->Get_QByteArray_From_Color(darkColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0D4A, this->colors->Get_QByteArray_From_Color(outlineColor))) return false;

    //Random Snow Night Colors
    if (this->paletteMode < 7) {
        darkColor = this->colors->Get_Random_Tree_Snow_Dark_Color();
        lightColor = this->colors->Get_Random_Tree_Light_Color_From_Dark_Color(darkColor);
        outlineColor = this->colors->Get_Random_Tree_Snow_Outline_Color();
    } else {
        lightColor = this->colors->Get_Random_Pipe_Light_Color();
        darkColor = this->colors->Get_Random_Pipe_Dark_Color_From_Light_Color(lightColor);
        outlineColor = Color::BLACK;
    }
    if (!this->Write_Bytes_To_Offset(0x0D50, this->colors->Get_QByteArray_From_Color(lightColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0D51, this->colors->Get_QByteArray_From_Color(darkColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0D52, this->colors->Get_QByteArray_From_Color(outlineColor))) return false;
    if (this->paletteMode >= 9 && !this->Get_Random_Red_Green_Colors(0x0CF0)) return false; //green palette
    if (this->paletteMode >= 10 && !this->Get_Random_Red_Green_Colors(0x0CF4)) return false; //red palette
    if (this->paletteMode >= 9 && !this->Get_Random_Brown_Colors(0x0CFA)) return false; //brown palette
    return true;
}

bool Palettes::Underground_Random() {
    //Random Brick Colors
    Color::Color baseColor = this->colors->Get_Random_Base_Color();
    Color::Color darkColor = this->colors->Get_Darkest_Shade_From_Color(baseColor);
    Color::Color lightColor = this->colors->Get_Lightest_Shade_From_Color(baseColor);
    if (this->paletteMode > 2) {
        if (!this->Write_Bytes_To_Offset(0x09EC, this->colors->Get_QByteArray_From_Color(baseColor))) return false; //nonshiny block border
        if (!this->Write_Bytes_To_Offset(0x0D04, this->colors->Get_QByteArray_From_Color(lightColor))) return false;
        if (!this->Write_Bytes_To_Offset(0x0D05, this->colors->Get_QByteArray_From_Color(baseColor))) return false;
        if (!this->Write_Bytes_To_Offset(0x0D06, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false;
    }
    if (this->paletteMode >= 5 && !this->Get_Random_Pipe_Colors(0x0D00)) return false;
    return this->Apply_Color_Glow(0x0D14, 0x0D1C, 0x0D00, baseColor, darkColor, lightColor);
}

bool Palettes::Underwater_Random() {
    Color::Color waterColor = this->colors->Get_Random_Water_Color();
    if (this->paletteMode >= 10) waterColor = this->colors->Get_Random_Base_Or_Dark_Color();
    if (!this->Write_Bytes_To_Offset(0x09E3, this->colors->Get_QByteArray_From_Color(waterColor))) return false; //underwater water color (behind coins)
    if (!this->Write_Bytes_To_Offset(0x09E4, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false; //nonshiny block border
    if (!this->Write_Bytes_To_Offset(0x0CC1, this->colors->Get_QByteArray_From_Color(waterColor))) return false; //underwater water color

    //Random Coral Colors
    Color::Color color1 = this->colors->Get_Random_Coral_Color();
    Color::Color color2 = this->colors->Get_Lighter_Shade_From_Color(color1);
    if (!this->Write_Bytes_To_Offset(0x0CB8, this->colors->Get_QByteArray_From_Color(color1))) return false;
    if (!this->Write_Bytes_To_Offset(0x0CB9, this->colors->Get_QByteArray_From_Color(waterColor))) return false; //underwater water color (background behind coral)
    if (!this->Write_Bytes_To_Offset(0x0CBA, this->colors->Get_QByteArray_From_Color(color2))) return false;

    //Random Brick Colors
    color1 = this->colors->Get_Random_Base_Color();
    color2 = this->colors->Get_Lightest_Shade_From_Color(color1);
    if (!this->Write_Bytes_To_Offset(0x0CBC, this->colors->Get_QByteArray_From_Color(color2))) return false;
    if (!this->Write_Bytes_To_Offset(0x0CBD, this->colors->Get_QByteArray_From_Color(color1))) return false;
    if (!this->Write_Bytes_To_Offset(0x0CBE, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false;

    if (this->paletteMode >= 6 && !this->Get_Random_Red_Green_Colors(0x0CCC)) return false; //green palette
    if (this->paletteMode >= 10 && !this->Get_Random_Red_Green_Colors(0x0CD0)) return false; //red palette
    if (this->paletteMode >= 6 && !this->Get_Random_Brown_Colors(0x0CD6)) return false; //brown palette
    return true;
}

bool Palettes::Randomize_Color_Groups() {
    if (!this->Write_Bytes_To_Offset(0x538E, QByteArray(1, static_cast<char>(Random::Get_Instance().Get_Num(0x21, 0x23))))) return false; //castle flag color
    if (!this->Write_Bytes_To_Offset(0x6614, QByteArray(1, static_cast<char>(Random::Get_Instance().Get_Num(0x21, 0x22))))) return false; //vine color

    //Lift Color
    int color = Random::Get_Instance().Get_Num(1, 3);
    if (!this->Write_Bytes_To_Offset(0x6614, QByteArray(1, static_cast<char>(color)))) return false;
    if (!this->Write_Bytes_To_Offset(0x6D81, QByteArray(1, static_cast<char>(color)))) return false;

    //Coin Color
    color = Random::Get_Instance().Get_Num(1, 3);
    if (!this->Write_Bytes_To_Offset(0x66C2, QByteArray(1, static_cast<char>(color)))) return false;
    if (!this->Write_Bytes_To_Offset(0x66C7, QByteArray(1, static_cast<char>(color+0x80)))) return false;

    if (!this->Write_Bytes_To_Offset(0x687D, QByteArray(1, static_cast<char>(Random::Get_Instance().Get_Num(1, 3))))) return false; //spinies and spiny eggs
    if (!this->Write_Bytes_To_Offset(0x687F, QByteArray(1, static_cast<char>(Random::Get_Instance().Get_Num(1, 3))))) return false; //flying cheep cheeps
    return true;
}

bool Palettes::Apply_Color_Glow(qint64 greenColorOffset, qint64 brownColorOffset, qint64 pipeColorOffset, Color::Color baseColor, Color::Color darkColor, Color::Color lightColor) {
    if (baseColor == Color::BLACK) { //fix black glow colors
        baseColor = Color::GRAY_LIGHT; darkColor = Color::GRAY_DARK; lightColor = Color::WHITE;
    }
    bool useSafeGlow = this->paletteMode <= 5 && Random::Get_Instance().Get_Num(1);
    if (pipeColorOffset == 0x0D24 && this->paletteMode < 4) useSafeGlow = true; //force safe glow on Castle levels
    bool useFullGlow = false;
    if (this->paletteMode > 5) useFullGlow = Random::Get_Instance().Get_Num(1);
    if (useFullGlow || useSafeGlow) {
        //Pipe Colors
        if (this->paletteMode == 4 || (this->paletteMode >= 5 && Random::Get_Instance().Get_Num(1))) {
            if (!this->Get_Random_Pipe_Colors(pipeColorOffset, lightColor)) return false;
        }

        //Brown Group
        if (!this->Write_Bytes_To_Offset(brownColorOffset, this->colors->Get_QByteArray_From_Color(darkColor))) return false;
        if (!this->Write_Bytes_To_Offset(brownColorOffset+1, this->colors->Get_QByteArray_From_Color(lightColor))) return false;
        if (!this->Write_Bytes_To_Offset(brownColorOffset+2, this->colors->Get_QByteArray_From_Color(baseColor))) return false;

        if (useFullGlow) {
            return this->Write_Bytes_To_Offset(greenColorOffset, this->colors->Get_QByteArray_From_Color(baseColor)); //green group
        } else if (useSafeGlow) {
            return this->Write_Bytes_To_Offset(greenColorOffset, this->colors->Get_QByteArray_From_Color(Color::AQUAMARINE)); //green group
        } else {
            assert(false); return false;
        }
    } else { //no glow
        //Pipe Colors
        if (this->paletteMode <= 3 && pipeColorOffset == 0x0D00) { //underground
            if (!this->Write_Bytes_To_Offset(0x09EC, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false; //nonshiny block border
            Color::Color color1 = Color::BLACK, color2 = Color::BLACK, color3 = Color::BLACK;
            assert(this->Get_Overworld_Pipe_Group_Colors(color1, color2, color3));
            if (!this->Write_Bytes_To_Offset(pipeColorOffset, this->colors->Get_QByteArray_From_Color(color1))) return false;
            if (!this->Write_Bytes_To_Offset(pipeColorOffset+1, this->colors->Get_QByteArray_From_Color(color2))) return false;
            if (!this->Write_Bytes_To_Offset(pipeColorOffset+2, this->colors->Get_QByteArray_From_Color(color3))) return false;
            assert(this->Get_Overworld_Brick_Group_Colors(color1, color2, color3));
            if (!this->Write_Bytes_To_Offset(0x0D04, this->colors->Get_QByteArray_From_Color(color1))) return false;
            if (!this->Write_Bytes_To_Offset(0x0D05, this->colors->Get_QByteArray_From_Color(color2))) return false;
            if (!this->Write_Bytes_To_Offset(0x0D06, this->colors->Get_QByteArray_From_Color(color3))) return false;
            assert(this->Get_Overworld_Score_Group_Colors(color1, color2, color3));
            if (!this->Write_Bytes_To_Offset(0x0D08, this->colors->Get_QByteArray_From_Color(color1))) return false;
            if (!this->Write_Bytes_To_Offset(0x0D09, this->colors->Get_QByteArray_From_Color(color2))) return false;
            if (!this->Write_Bytes_To_Offset(0x0D0A, this->colors->Get_QByteArray_From_Color(color3))) return false;
        }

        Color::Color greenColor1 = Color::BLACK, greenColor2 = Color::BLACK, greenColor3 = Color::BLACK;
        Color::Color brownColor1 = Color::BLACK, brownColor2 = Color::BLACK, brownColor3 = Color::BLACK;
        assert(this->Get_Overworld_Green_Group_Colors(greenColor1, greenColor2, greenColor3));
        assert(this->Get_Overworld_Brown_Group_Colors(brownColor1, brownColor2, brownColor3));

        //Green Group
        if (!this->Write_Bytes_To_Offset(greenColorOffset, this->colors->Get_QByteArray_From_Color(greenColor1))) return false;
        if (!this->Write_Bytes_To_Offset(greenColorOffset+1, this->colors->Get_QByteArray_From_Color(greenColor2))) return false;
        if (!this->Write_Bytes_To_Offset(greenColorOffset+2, this->colors->Get_QByteArray_From_Color(greenColor3))) return false;

        //Brown Group
        if (!this->Write_Bytes_To_Offset(brownColorOffset, this->colors->Get_QByteArray_From_Color(brownColor1))) return false;
        if (!this->Write_Bytes_To_Offset(brownColorOffset+1, this->colors->Get_QByteArray_From_Color(brownColor2))) return false;
        return this->Write_Bytes_To_Offset(brownColorOffset+2, this->colors->Get_QByteArray_From_Color(brownColor3));
    }
}

bool Palettes::Get_Overworld_Green_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3) {
    QByteArray bytes;
    if (!this->Read_Bytes_From_Offset(0x0CF0, 3, bytes)) return false;
    if (!this->colors->Get_Color_From_Hex(bytes.at(0), color1)) return false;
    if (!this->colors->Get_Color_From_Hex(bytes.at(1), color2)) return false;
    return this->colors->Get_Color_From_Hex(bytes.at(2), color3);
}

bool Palettes::Get_Overworld_Red_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3) {
    QByteArray bytes;
    if (!this->Read_Bytes_From_Offset(0x0CF4, 3, bytes)) return false;
    if (!this->colors->Get_Color_From_Hex(bytes.at(0), color1)) return false;
    if (!this->colors->Get_Color_From_Hex(bytes.at(1), color2)) return false;
    return this->colors->Get_Color_From_Hex(bytes.at(2), color3);
}

bool Palettes::Get_Overworld_Brown_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3) {
    QByteArray bytes;
    if (!this->Read_Bytes_From_Offset(0x0CF8, 3, bytes)) return false;
    if (!this->colors->Get_Color_From_Hex(bytes.at(0), color1)) return false;
    if (!this->colors->Get_Color_From_Hex(bytes.at(1), color2)) return false;
    return this->colors->Get_Color_From_Hex(bytes.at(2), color3);
}

bool Palettes::Get_Overworld_Pipe_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3) {
    QByteArray bytes;
    if (!this->Read_Bytes_From_Offset(0x0CDC, 3, bytes)) return false;
    if (!this->colors->Get_Color_From_Hex(bytes.at(0), color1)) return false;
    if (!this->colors->Get_Color_From_Hex(bytes.at(1), color2)) return false;
    return this->colors->Get_Color_From_Hex(bytes.at(2), color3);
}

bool Palettes::Get_Overworld_Brick_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3) {
    QByteArray bytes;
    if (!this->Read_Bytes_From_Offset(0x0CE0, 3, bytes)) return false;
    if (!this->colors->Get_Color_From_Hex(bytes.at(0), color1)) return false;
    if (!this->colors->Get_Color_From_Hex(bytes.at(1), color2)) return false;
    return this->colors->Get_Color_From_Hex(bytes.at(2), color3);
}

bool Palettes::Get_Overworld_Score_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3) {
    QByteArray bytes;
    if (!this->Read_Bytes_From_Offset(0x0CE4, 3, bytes)) return false;
    if (!this->colors->Get_Color_From_Hex(bytes.at(0), color1)) return false;
    if (!this->colors->Get_Color_From_Hex(bytes.at(1), color2)) return false;
    return this->colors->Get_Color_From_Hex(bytes.at(2), color3);
}

bool Palettes::Get_Random_Pipe_Colors(qint64 offset) {
    return this->Get_Random_Pipe_Colors(offset, this->colors->Get_Random_Pipe_Dark_Color());
}

bool Palettes::Get_Random_Pipe_Colors(qint64 offset, Color::Color lightColor) {
    lightColor = this->colors->Get_Darker_Shade_From_Color(this->colors->Get_Lightest_Shade_From_Color(lightColor));
    Color::Color borderColor = Color::BLACK;
    if (Random::Get_Instance().Get_Num(1)) borderColor = this->colors->Get_Darkest_Shade_From_Color(lightColor);
    Color::Color darkColor = this->colors->Get_Random_Pipe_Dark_Color_From_Light_Color(lightColor);
    if (!this->Write_Bytes_To_Offset(offset, this->colors->Get_QByteArray_From_Color(lightColor))) return false;
    if (!this->Write_Bytes_To_Offset(offset+1, this->colors->Get_QByteArray_From_Color(darkColor))) return false;
    if (!this->Write_Bytes_To_Offset(offset+2, this->colors->Get_QByteArray_From_Color(borderColor))) return false;
    return true;
}

bool Palettes::Get_Random_Red_Green_Colors(qint64 offset) {
    QByteArray bytes; QVector<Color::Color> excludedColors; excludedColors.append(Color::BLACK);
    if (offset == 0x0CCC || offset == 0x0CD0) { //underwater
        if (!this->Read_Bytes_From_Offset(0x0CC1, 1, bytes)) return false;
        excludedColors.append(static_cast<Color::Color>(bytes.at(0)));
    } else { //overworld
        if (!this->Read_Bytes_From_Offset(0x05E0, 1, bytes)) return false;
        excludedColors.append(static_cast<Color::Color>(bytes.at(0)));
    }
    Color::Color baseColor = this->colors->Get_Random_Color_Excluding_Colors(excludedColors);
    if (!this->Write_Bytes_To_Offset(offset, this->colors->Get_QByteArray_From_Color(baseColor))) return false;

    if (this->paletteMode >= 10) {
        if (!this->Write_Bytes_To_Offset(offset+1, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Lightest_Shade_Color()))) return false;
        if (!this->Write_Bytes_To_Offset(offset+2, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Light_Shade_Color(0x1, 0xE)))) return false;
    }
    return true;
}

bool Palettes::Get_Random_Brown_Colors(qint64 offset) {
    if (offset == 0x0CD6) { //underwater
        Color::Color baseColor = this->colors->Get_Random_Light_Color();
        if (!this->Write_Bytes_To_Offset(offset-1, this->colors->Get_QByteArray_From_Color(baseColor))) return false;

        if (this->paletteMode >= 10) {
            if (!this->Write_Bytes_To_Offset(offset-2, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Light_Shade_Color(0x1, 0xE)))) return false;
            if (!this->Write_Bytes_To_Offset(offset, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Lightest_Shade_Color()))) return false;
        }
    } else {
        Color::Color baseColor = this->colors->Get_Random_Base_Or_Dark_Color();
        if (!this->Write_Bytes_To_Offset(offset, this->colors->Get_QByteArray_From_Color(baseColor))) return false;

        if (this->paletteMode >= 10) {
            if (!this->Write_Bytes_To_Offset(offset-2, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Light_Shade_Color(0x1, 0xE)))) return false;
            if (!this->Write_Bytes_To_Offset(offset-1, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Lightest_Shade_Color()))) return false;
        }
    }
    return true;
}
