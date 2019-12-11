#include "Graphics_Combiner.h"
#include "Color.h"
#include "Graphics.h"
#include "Graphics_Offsets.h"
#include "Sequential_Archive_Handler.h"
#include <QMap>
#include <QSet>

Graphics_Combiner::Graphics_Combiner(QFile *f, Level_Offset *lo, Sequential_Archive_Handler *sequentialArchiveHandler, Graphics *graphics) : Byte_Writer(f, lo) {
    assert(sequentialArchiveHandler); assert(graphics);
    this->marioPatchName = QString();
    this->brickPatchName = QString();
    this->sequentialArchiveHandler = sequentialArchiveHandler;
    this->graphics = graphics;
    this->graphicsOffsets = new Graphics_Offsets();
}

Graphics_Combiner::~Graphics_Combiner() {
    delete this->graphicsOffsets;
}

QString Graphics_Combiner::Get_Brick_Patch_Name() {
    return this->brickPatchName;
}

QString Graphics_Combiner::Get_Mario_Patch_Name() {
    return this->marioPatchName;
}

bool Graphics_Combiner::Combine_All_Except_Mario() {
    if (!this->sequentialArchiveHandler->Are_Color_Palettes_Allowed() || this->sequentialArchiveHandler->Are_Only_Coin_Palettes_Allowed()) return true; //nothing to do
    if (!this->Combine_Air_Bubble()) return false;
    if (!this->Combine_Blooper()) return false;
    if (!this->Combine_Bowser()) return false;
    if (!this->Combine_Bowser_Fire()) return false;
    if (!this->Combine_Brick_Piece()) return false;
    if (!this->Combine_Bullet_Bill()) return false;
    if (!this->Combine_Buzzy_Beetle()) return false;
    if (!this->Combine_Castle_Flag()) return false;
    if (!this->Combine_Cheep_Cheep()) return false;
    if (!this->Combine_Coin_Animation()) return false;
    if (!this->Combine_Explosion()) return false;
    if (!this->Combine_Fireball()) return false;
    if (!this->Combine_Fire_Flower()) return false;
    if (!this->Combine_Flagpole_Flag()) return false;
    if (!this->Combine_Goomba()) return false;
    if (!this->Combine_Hammer()) return false;
    if (!this->Combine_Hammer_Bro()) return false;
    if (!this->Combine_Jump_Spring()) return false;
    if (!this->Combine_Koopa()) return false;
    if (!this->Combine_Lakitu()) return false;
    if (!this->Combine_Lift()) return false;
    if (!this->Combine_Mushroom_Powerup()) return false;
    if (!this->Combine_One_Up_Font()) return false;
    if (!this->Combine_Peach()) return false;
    if (!this->Combine_Piranha_Plant()) return false;
    if (!this->Combine_Podoboo()) return false;
    if (!this->Combine_Score_Font()) return false;
    if (!this->Combine_Sky_Lift()) return false;
    if (!this->Combine_Spiny()) return false;
    if (!this->Combine_Spiny_Egg()) return false;
    if (!this->Combine_Starman()) return false;
    if (!this->Combine_Toad()) return false;
    if (!this->Combine_Axe()) return false;
    if (!this->Combine_Brick_Block()) return false;
    if (!this->Combine_Bowser_Bridge()) return false;
    if (!this->Combine_Bridge()) return false;
    if (!this->Combine_Bullet_Bill_Cannon()) return false;
    if (!this->Combine_Castle_Block()) return false;
    if (!this->Combine_Chain()) return false;
    if (!this->Combine_Cloud_Block()) return false;
    if (!this->Combine_Coin()) return false;
    if (!this->Combine_Coin_Icon()) return false;
    if (!this->Combine_Coral()) return false;
    if (!this->Combine_Flagpole()) return false;
    if (!this->Combine_Font()) return false;
    if (!this->Combine_Mushroom_Platform()) return false;
    if (!this->Combine_Overworld_Block()) return false;
    if (!this->Combine_Pipe()) return false;
    if (!this->Combine_Question_Block()) return false;
    if (!this->Combine_Rope()) return false;
    if (!this->Combine_Selector_Icon()) return false;
    if (!this->Combine_Solid_Block()) return false;
    if (!this->Combine_Tree_Platform()) return false;
    if (!this->Combine_Underwater_Block()) return false;
    if (!this->Combine_Vine()) return false;
    if (!this->Combine_Water()) return false;
    return true;
}

bool Graphics_Combiner::Combine_Air_Bubble() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Air_Bubble_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Air Bubble");
}

bool Graphics_Combiner::Combine_Blooper() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Blooper_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Blooper");
}

bool Graphics_Combiner::Combine_Bowser() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Bowser_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Bowser");
}

bool Graphics_Combiner::Combine_Bowser_Fire() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Bowser_Fire_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Bowser Fire");
}

bool Graphics_Combiner::Combine_Brick_Piece() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Brick_Piece_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Brick Piece");
}

bool Graphics_Combiner::Combine_Bullet_Bill() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Bullet_Bill_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Bullet Bill");
}

bool Graphics_Combiner::Combine_Buzzy_Beetle() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Buzzy_Beetle_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Buzzy Beetle");
}

bool Graphics_Combiner::Combine_Castle_Flag() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Castle_Flag_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Castle Flag");
}

bool Graphics_Combiner::Combine_Cheep_Cheep() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Cheep_Cheep_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Cheep Cheep");
}

bool Graphics_Combiner::Combine_Coin_Animation() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Coin_Animation_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Coin Animation");
}

bool Graphics_Combiner::Combine_Explosion() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Explosion_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Explosion");
}

bool Graphics_Combiner::Combine_Fireball() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Fireball_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Fireball");
}

bool Graphics_Combiner::Combine_Fire_Flower() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Fire_Flower_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Fire Flower");
}

bool Graphics_Combiner::Combine_Flagpole_Flag() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Flagpole_Flag_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Flagpole Flag");
}

bool Graphics_Combiner::Combine_Goomba() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Goomba_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Goomba");
}

bool Graphics_Combiner::Combine_Hammer() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Hammer_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Hammer");
}

bool Graphics_Combiner::Combine_Hammer_Bro() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Hammer_Bro_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Hammer Bro");
}

bool Graphics_Combiner::Combine_Jump_Spring() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Jump_Spring_Animation_Offsets(), true)) return true;
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Jump_Spring_Base_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Jump Spring");
}

bool Graphics_Combiner::Combine_Koopa() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Koopa_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Koopa");
}

bool Graphics_Combiner::Combine_Lakitu() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Lakitu_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Lakitu");
}

bool Graphics_Combiner::Combine_Lift() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Lift_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Lift");
}

bool Graphics_Combiner::Combine_Mario() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Mario_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Mario", this->marioPatchName);
}

bool Graphics_Combiner::Combine_Mushroom_Powerup() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Mushroom_Powerup_Offsets(), true)) return true;

    //Make sure the red palette does not have black for color 3
    QByteArray bytes;
    if (!this->Read_Bytes_From_Offset(0x0CF6, 1, bytes)) return false;
    if (bytes.at(0) == Color::BLACK) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Powerup Mushroom");
}

bool Graphics_Combiner::Combine_One_Up_Font() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_One_Up_Font_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Font One Up");
}

bool Graphics_Combiner::Combine_Peach() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Peach_Offsets(), true)) return true;

    //Make sure the red palette does not have black for color 3
    QByteArray bytes;
    if (!this->Read_Bytes_From_Offset(0x0CF6, 1, bytes)) return false;
    if (bytes.at(0) == Color::BLACK) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Peach");
}

bool Graphics_Combiner::Combine_Piranha_Plant() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Piranha_Plant_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Piranha Plant");
}

bool Graphics_Combiner::Combine_Podoboo() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Podoboo_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Podoboo");
}

bool Graphics_Combiner::Combine_Score_Font() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Score_Font_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Font Score");
}

bool Graphics_Combiner::Combine_Sky_Lift() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Sky_Lift_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Lift Sky");
}

bool Graphics_Combiner::Combine_Spiny() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Spiny_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Spiny");
}

bool Graphics_Combiner::Combine_Spiny_Egg() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Spiny_Egg_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Spiny Egg");
}

bool Graphics_Combiner::Combine_Starman() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Starman_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Starman");
}

bool Graphics_Combiner::Combine_Toad() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Toad_Offsets(), true)) return true;

    //Make sure the red palette does not have black for color 3
    QByteArray bytes;
    if (!this->Read_Bytes_From_Offset(0x0CF6, 1, bytes)) return false;
    if (bytes.at(0) == Color::BLACK) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Toad");
}

bool Graphics_Combiner::Combine_Axe() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Axe_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Axe");
}

bool Graphics_Combiner::Combine_Brick_Block() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Brick_Block_Offsets(), false)) return true;
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Brick_Block_Animation_Offsets(), true)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Brick Block", this->brickPatchName);
}

bool Graphics_Combiner::Combine_Bowser_Bridge() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Bowser_Bridge_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Bowser Bridge");
}

bool Graphics_Combiner::Combine_Bridge() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Bridge_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Bridge");
}

bool Graphics_Combiner::Combine_Bullet_Bill_Cannon() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Bullet_Bill_Cannon_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Bullet Bill Cannon");
}

bool Graphics_Combiner::Combine_Castle_Block() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Castle_Block_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Block Castle");
}

bool Graphics_Combiner::Combine_Chain() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Chain_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Chain");
}

bool Graphics_Combiner::Combine_Cloud_Block() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Cloud_Block_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Block Cloud");
}

bool Graphics_Combiner::Combine_Coin() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Coin_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Coin");
}

bool Graphics_Combiner::Combine_Coin_Icon() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Coin_Icon_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Coin Icon");
}

bool Graphics_Combiner::Combine_Coral() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Coral_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Coral");
}

bool Graphics_Combiner::Combine_Flagpole() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Flagpole_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Flagpole");
}

bool Graphics_Combiner::Combine_Font() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Font_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Font");
}

bool Graphics_Combiner::Combine_Mushroom_Platform() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Mushroom_Platform_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Island Mushroom");
}

bool Graphics_Combiner::Combine_Overworld_Block() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Overworld_Block_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Block Overworld");
}

bool Graphics_Combiner::Combine_Pipe() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Pipe_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Pipe");
}

bool Graphics_Combiner::Combine_Question_Block() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Question_Block_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Question Block");
}

bool Graphics_Combiner::Combine_Rope() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Rope_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Rope");
}

bool Graphics_Combiner::Combine_Selector_Icon() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Selector_Icon_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Selector Icon");
}

bool Graphics_Combiner::Combine_Solid_Block() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Solid_Block_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Solid Block");
}

bool Graphics_Combiner::Combine_Tree_Platform() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Tree_Platform_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Island Tree");
}

bool Graphics_Combiner::Combine_Underwater_Block() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Underwater_Block_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Block Underwater");
}

bool Graphics_Combiner::Combine_Vine() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Vine_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Vine");
}

bool Graphics_Combiner::Combine_Water() {
    if (this->Does_Graphics_Pack_Use_New_Tiles(this->graphicsOffsets->Get_Water_Offsets(), false)) return true;
    return this->sequentialArchiveHandler->Apply_Random_Graphics_Sprite("Water");
}

bool Graphics_Combiner::Does_Graphics_Pack_Use_New_Tiles(QStack<qint64> offsets, bool sprite) {
    if (offsets.isEmpty()) return false; //no new tiles

    //Read the old tiles
    QMap<char, bool> tiles;
    QByteArray oldTiles;
    QStack<qint64> oldOffsets = offsets;
    if (sprite) tiles.insert(static_cast<char>(0xFC), true);
    else tiles.insert(static_cast<char>(0x24), true);
    int tileOrderSize = 0;

    //Read the original tiles
    while (!oldOffsets.isEmpty()) {
        qint64 offset = oldOffsets.pop();
        QByteArray *oldTiles = this->graphicsOffsets->Get_Values_At_Offset_And_Never_Fail(offset);
        if (tileOrderSize == 0) tileOrderSize = oldTiles->size();
        else assert(oldTiles->size() == tileOrderSize);
        for (char c : *oldTiles) tiles.insert(c, false);
    }

    //Compare against the new tiles
    while (!offsets.isEmpty()) {
        qint64 offset = offsets.pop();
        QByteArray newTiles;
        assert(this->Read_Bytes_From_Offset(offset, tileOrderSize, newTiles));
        assert(newTiles.size() == tileOrderSize);

        //Update blank tiles
        for (int i = 0; i < newTiles.size(); ++i) {
            if (this->Is_Tile_Blank(newTiles.at(i), sprite)) {
                tiles.insert(newTiles.at(i), true); //mark the old tile as safe to use
                if (sprite) newTiles.data()[i] = static_cast<char>(0xFC);
                else newTiles.data()[i] = static_cast<char>(0x24);
            }
        }

        //Check to see if any new tiles were used
        for (char c : newTiles) {
            if (tiles.contains(c)) tiles.insert(c, true);
            else return true;
        }

        //Make sure all tiles were used
        for (QMap<char, bool>::iterator iter = tiles.begin(); iter != tiles.end(); ++iter) {
            if (!iter.value()) return true;
        }
    }
    return false;
}

bool Graphics_Combiner::Is_Tile_Blank(char tileID, bool sprite) {
    QByteArray graphicsBytes;
    if (sprite) assert(this->graphics->Read_Graphics_Bytes_From_Sprite_Tile_ID(tileID, graphicsBytes));
    else assert(this->graphics->Read_Graphics_Bytes_From_Background_Tile_ID(tileID, graphicsBytes));
    for (int i = 0; i < graphicsBytes.size(); ++i) {
        if (graphicsBytes.at(i) != static_cast<char>(0x00)) return false;
    }
    return true;
}
