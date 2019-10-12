#ifndef PALETTE_H
#define PALETTE_H

#include "Byte_Writer.h"
#include "Color.h"

class Colors;
class Sequential_Archive_Handler;

class Palettes : public Byte_Writer {
public:
    Palettes(QFile *file, Level_Offset *levelOffset, Sequential_Archive_Handler *sequentialArchiveHandler);
    ~Palettes();
    bool Randomize_Palettes(int paletteMode);
    bool Randomize_Mario_Sprite_Palette();

private:
    bool Coin_Palette_Random();
    bool Sky_Palette_Random();

    bool Castle_Random();
    bool Overworld_Random();
    bool Underground_Random();
    bool Underwater_Random();

    bool Randomize_Color_Groups();

    bool Apply_Color_Glow(qint64 greenColorOffset, qint64 brownColorOffset, qint64 pipeColorOffset, Color::Color baseColor, Color::Color darkColor, Color::Color lightColor);
    bool Get_Overworld_Green_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3);
    bool Get_Overworld_Red_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3);
    bool Get_Overworld_Brown_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3);
    bool Get_Overworld_Pipe_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3);
    bool Get_Overworld_Brick_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3);
    bool Get_Overworld_Score_Group_Colors(Color::Color &color1, Color::Color &color2, Color::Color &color3);
    bool Get_Random_Pipe_Colors(qint64 offset);
    bool Get_Random_Pipe_Colors(qint64 offset, Color::Color darkColor);
    bool Get_Random_Red_Green_Colors(qint64 offset);
    bool Get_Random_Brown_Colors(qint64 offset);

    Colors *colors;
    Sequential_Archive_Handler *sequentialArchiveHandler;
    int paletteMode;
};

#endif // PALETTE_H
