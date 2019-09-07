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
    if (!this->Random_Coin_Palette()) return false;

    return true;
}

bool Palettes::Random_Coin_Palette() {
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
            if (Random::Get_Instance().Get_Num(1)) {
                colorLight = this->colors->Get_Darker_Shade_From_Color(colorLightest);
                colorDark = this->colors->Get_Darker_Shade_From_Color(colorLight);
                colorDarkest = this->colors->Get_Darker_Shade_From_Color(colorDark);
            } else {
                colorLight = this->colors->Get_Slightly_Darker_Color_From_Color(colorLightest);
                colorDark = this->colors->Get_Slightly_Darker_Color_From_Color(colorLight);
                colorDarkest = this->colors->Get_Slightly_Darker_Color_From_Color(colorDark);
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
    if (!this->Write_Bytes_To_Offset(0x09E3, this->colors->Get_QByteArray_From_Color(nonShinyColor))) return false; //underwater
    if (!this->Write_Bytes_To_Offset(0x09E7, this->colors->Get_QByteArray_From_Color(nonShinyColor))) return false; //overworld
    if (!this->Write_Bytes_To_Offset(0x09EB, this->colors->Get_QByteArray_From_Color(nonShinyColor))) return false; //underground
    return this->Write_Bytes_To_Offset(0x09EF, this->colors->Get_QByteArray_From_Color(nonShinyColor)); //castle
}
