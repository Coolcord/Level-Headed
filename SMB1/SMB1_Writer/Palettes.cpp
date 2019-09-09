#include "Palettes.h"
#include "../../Common_Files/Random.h"
#include "Colors.h"

Palettes::Palettes(QFile *file, Level_Offset *levelOffset) : Byte_Writer(file, levelOffset) {
    this->colors = new Colors();
}

Palettes::~Palettes() {
    delete this->colors;
}

bool Palettes::Randomize_Palettes() {
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

    //Write the Non-shiny Block Colors
    //TODO: ONLY PATCH THIS IF A LESS RESTRICTIVE MODE IS SPECIFIED!!!
    if (!this->Write_Bytes_To_Offset(0x09E3, this->colors->Get_QByteArray_From_Color(nonShinyColor))) return false; //underwater
    if (!this->Write_Bytes_To_Offset(0x09E7, this->colors->Get_QByteArray_From_Color(nonShinyColor))) return false; //overworld
    if (!this->Write_Bytes_To_Offset(0x09EB, this->colors->Get_QByteArray_From_Color(nonShinyColor))) return false; //underground
    return this->Write_Bytes_To_Offset(0x09EF, this->colors->Get_QByteArray_From_Color(nonShinyColor)); //castle
}

bool Palettes::Sky_Palette_Random() {
    if (!this->Write_Bytes_To_Offset(0x05DF, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Sky_Color()))) return false; //underwater sky color
    if (!this->Write_Bytes_To_Offset(0x05E0, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Sky_Color()))) return false; //overworld day
    if (!this->Write_Bytes_To_Offset(0x05E1, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false; //underground (this also affects the lives screen)
    if (!this->Write_Bytes_To_Offset(0x05E2, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false; //castle
    if (!this->Write_Bytes_To_Offset(0x05E3, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Sky_Color()))) return false; //Night
    if (!this->Write_Bytes_To_Offset(0x05E4, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Sky_Color()))) return false; //Snow
    if (!this->Write_Bytes_To_Offset(0x05E5, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false; //Night and Snow (also used underground when the snow palette is used)
    return this->Write_Bytes_To_Offset(0x05E6, this->colors->Get_QByteArray_From_Color(Color::BLACK)); //Night and Freeze
}

bool Palettes::Castle_Random() {
    Color::Color baseColor = this->colors->Get_Random_Underground_Color();
    Color::Color darkColor = this->colors->Get_Darkest_Shade_From_Color(baseColor);
    Color::Color lightColor = this->colors->Get_Lightest_Shade_From_Color(baseColor);
    if (!this->Write_Bytes_To_Offset(0x0D28, this->colors->Get_QByteArray_From_Color(lightColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0D29, this->colors->Get_QByteArray_From_Color(baseColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0D2A, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false;

    //Get a Random Lava Color
    //TODO: ONLY PATCH THIS IF A LESS RESTRICTIVE MODE IS SPECIFIED!!!
    if (!this->Write_Bytes_To_Offset(0x0D2D, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Color()))) return false;

    return this->Apply_Color_Glow(0x0D38, 0x0D40, baseColor, darkColor, lightColor);
}

bool Palettes::Overworld_Random() {
    //Random Tree Colors
    //TODO: ALLOW MORE EXTREME COLORS IF A LESS RESTRICTIVE MODE IS SPECIFIED!!!
    Color::Color darkColor = this->colors->Get_Random_Tree_Dark_Color();
    Color::Color lightColor = this->colors->Get_Random_Tree_Light_Color_From_Dark_Color(darkColor);
    if (!this->Write_Bytes_To_Offset(0x0CDC, this->colors->Get_QByteArray_From_Color(lightColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0CDD, this->colors->Get_QByteArray_From_Color(darkColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0CDE, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false;
    if (!this->Write_Bytes_To_Offset(0x0CE5, this->colors->Get_QByteArray_From_Color(this->colors->Get_Random_Water_Color()))) return false; //water color
    return true;
}

bool Palettes::Underground_Random() {
    //Random Brick Colors
    Color::Color baseColor = this->colors->Get_Random_Underground_Color();
    Color::Color darkColor = this->colors->Get_Darkest_Shade_From_Color(baseColor);
    Color::Color lightColor = this->colors->Get_Lightest_Shade_From_Color(baseColor);
    if (!this->Write_Bytes_To_Offset(0x0D04, this->colors->Get_QByteArray_From_Color(lightColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0D05, this->colors->Get_QByteArray_From_Color(baseColor))) return false;
    if (!this->Write_Bytes_To_Offset(0x0D06, this->colors->Get_QByteArray_From_Color(Color::BLACK))) return false;
    return this->Apply_Color_Glow(0x0D14, 0x0D1C, baseColor, darkColor, lightColor);
}

bool Palettes::Underwater_Random() {
    Color::Color waterColor = this->colors->Get_Random_Water_Color();
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

    return true;
}

bool Palettes::Apply_Color_Glow(qint64 greenColorOffset, qint64 brownColorOffset, Color::Color baseColor, Color::Color darkColor, Color::Color lightColor) {
    //TODO: ONLY CONSIDER APPLYING GLOW IF A LESS RESTRICTIVE MODE IS SPECIFIED!!!
    //TODO: ADD A NEW MODE THAT USES A SAFER GLOW COLOR INSTEAD OF THE baseColor!!!
    if (Random::Get_Instance().Get_Num(1)) {
        //Green Group
        if (!this->Write_Bytes_To_Offset(greenColorOffset, this->colors->Get_QByteArray_From_Color(baseColor))) return false;

        //Brown Group
        if (!this->Write_Bytes_To_Offset(brownColorOffset, this->colors->Get_QByteArray_From_Color(darkColor))) return false;
        if (!this->Write_Bytes_To_Offset(brownColorOffset+1, this->colors->Get_QByteArray_From_Color(lightColor))) return false;
        return this->Write_Bytes_To_Offset(brownColorOffset+2, this->colors->Get_QByteArray_From_Color(baseColor));
    } else { //no glow
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
