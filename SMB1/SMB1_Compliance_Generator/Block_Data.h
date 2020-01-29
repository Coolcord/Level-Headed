#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

struct Block_Data {
    int x; //absolute
    int y;
    bool partOfGroup; //only ever set to true when using horizontal/vertical bricks/question blocks
    bool verticalGroup;
    bool hittable;
    bool safeForMushroom;
    bool safeForStar;
};

#endif // BLOCK_DATA_H
