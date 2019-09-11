#ifndef PALETTE_H
#define PALETTE_H

#include "Byte_Writer.h"
#include "Color.h"

class Colors;

class Palettes : public Byte_Writer {
public:
    Palettes(QFile *file, Level_Offset *levelOffset);
    ~Palettes();
    bool Randomize_Palettes(int paletteMode);

private:
    bool Coin_Palette_Random();
    bool Sky_Palette_Random();

    bool Castle_Random();
    bool Overworld_Random();
    bool Underground_Random();
    bool Underwater_Random();

    bool Apply_Enemy_Color_Glow(qint64 greenColorOffset, qint64 brownColorOffset, Color::Color baseColor, Color::Color darkColor, Color::Color lightColor);
    bool Get_Overworld_Green_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3);
    bool Get_Overworld_Red_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3);
    bool Get_Overworld_Brown_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3);
    bool Get_Random_Pipe_Colors(qint64 offset);
    bool Get_Random_Pipe_Colors(qint64 offset, Color::Color darkColor);

    Colors *colors;
    int paletteMode;
};

#endif // PALETTE_H
