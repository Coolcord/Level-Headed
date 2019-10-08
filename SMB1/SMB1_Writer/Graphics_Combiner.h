#ifndef GRAPHICS_COMBINER_H
#define GRAPHICS_COMBINER_H

#include <QByteArray>
#include <QMap>

class Graphics_Combiner {
public:
    Graphics_Combiner();
    ~Graphics_Combiner();
    bool Combine_All_Except_Mario();
    bool Combine_Mario();
    QString Get_Brick_Patch_Name();
    QString Get_Mario_Patch_Name();

private:
    void Initialize_Tile_Order_Map();
    void Deallocate_Tile_Order_Map();

    QMap<qint64, QByteArray*> *tileOrderMap;
    QString marioPatchName;
    QString brickPatchName;
};

#endif // GRAPHICS_COMBINER_H
