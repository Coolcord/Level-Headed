#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Byte_Writer.h"
#include <QByteArray>
#include <QMap>

class Graphics_Combiner;
class Text;
class Sequential_Archive_Handler;

class Graphics : public Byte_Writer {
public:
    Graphics(QFile *file, Level_Offset *levelOffset, Sequential_Archive_Handler *sequentialArchiveHandler, Text *text);
    ~Graphics();
    bool Apply_Bone_Caster_Fix();
    bool Apply_Coin_Flinger_Fix();
    bool Apply_Cutter_Flower_Fix();
    bool Apply_Double_Jump_Star_Fix();
    bool Apply_Fire_Star_Fix();
    bool Apply_Hammer_Suit_Fix();
    bool Apply_Luigis_Fire_Flower_Fix();
    bool Apply_Pink_Fury_Fix();
    bool Apply_Poison_Bubbles_Fix();
    bool Apply_Power_Wand_Fix();
    bool Apply_Pyro_Spark_Wand_Fix();
    bool Apply_Slime_Flower_Fix();
    bool Apply_Snowballs_Fix();
    bool Apply_Spinball_Flower_Fix();
    bool Apply_Title_Screen_1P_Fix(qint64 &versionOffset);
    bool Apply_Title_Screen_2P_Fix(qint64 &versionOffset);
    bool Change_1UP_Palette(int palette);
    bool Combine_Graphics();
    bool Combine_Mario();
    bool Make_Sprite_Tiles_Transparent(const QByteArray &tiles);
    bool Read_Graphics_Bytes_From_Sprite_Tile_ID(char tileID, QByteArray &graphicsBytes);
    bool Read_Graphics_Bytes_From_Background_Tile_ID(char tileID, QByteArray &graphicsBytes);
    bool Perform_Horizontal_Flip(QByteArray &graphicsBytes);
    bool Perform_Vertical_Flip(QByteArray &graphicsBytes);
    bool Write_Graphics_Bytes_To_Sprite_Tile_ID(char tileID, const QByteArray &graphicsBytes);
    bool Write_Title_Screen_For_1_Player_Game();
    bool Write_Title_Screen_For_2_Player_Game();
    bool Write_Title_Screen_For_Partial_Game();

private:
    bool Write_Title_Screen_Core();
    QByteArray Get_Version_Bytes();
    void Get_Version_Offset_From_Title_Screen_Fix(const QByteArray &patchBytes, qint64 &versionOffset);
    void Initialize_Tile_Order_Map();
    void Deallocate_Tile_Order_Map();
    char Reverse_Bits(char byte);

    Graphics_Combiner *graphicsCombiner;
    Text *text;
    Sequential_Archive_Handler *sequentialArchiveHandler;
    qint64 versionOffset;
};

#endif // GRAPHICS_H
