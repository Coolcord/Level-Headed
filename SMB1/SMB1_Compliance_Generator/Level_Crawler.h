#ifndef LEVEL_CRAWLER_H
#define LEVEL_CRAWLER_H

#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Level_Attribute.h"
#include "../Common_SMB1_Files/Object_Item.h"
#include <QFile>
#include <QHash>

class Buffer_Data;
class Object_Buffer;

class Level_Crawler {
public:
    Level_Crawler(Object_Buffer *objects);
    ~Level_Crawler();

    //Initiate Crawl
    bool Crawl_Level();
    bool Recrawl_Level();

    //Get and Set Attributes
    void Set_Level_Attribute(Level_Attribute::Level_Attribute levelAttribute);
    Level_Attribute::Level_Attribute Get_Level_Attribute();
    Brick::Brick Get_Starting_Brick();
    void Set_Starting_Brick(Brick::Brick startingBrick);
    int Get_Safe_Size();

    //Coordinate Checks
    bool Is_Coordinate_Empty(int x, int y);
    bool Is_Coordinate_Used(int x, int y);
    bool Is_Coordinate_Solid(int x, int y);
    bool Is_Coordinate_A_Platform(int x, int y);
    bool Is_Coordinate_Breakable_Or_Empty(int x, int y);
    Object_Item::Object_Item Get_Object_At_Coordinate(int x, int y);

    //Find Coordinates
    bool Find_Platform_Directly_Below(int x, int y, int &platformY);

private:
    void Crawl_Forward(int x, int spaces);
    void Crawl_Forward_With_Hole(int x, int spaces, int &holeSteps);
    void Mark_Used_Coordinate(Object_Item::Object_Item objectItem, int x, int y);
    void Mark_Used_X(Object_Item::Object_Item objectItem, int x);
    void Clear_X(int x);
    QString Make_Key(int x, int y);
    bool Parse_Object(const Buffer_Data &data, int &x, int &holeCrawlSteps);

    //Debug Functions
    bool Draw_Map();
    int Get_X_From_Key(const QString &key);
    int Get_Y_From_Key(const QString &key);

    Object_Buffer *objects;
    Level_Attribute::Level_Attribute levelAttribute;
    Brick::Brick brick;
    Brick::Brick nextBrick;
    Brick::Brick startingBrick;
    Object_Item::Object_Item brickItem;
    bool endDetected;
    int safeSize;
    bool levelCrawled;
    QHash<QString, Object_Item::Object_Item> *usedCoordinates;
};

#endif // LEVEL_CRAWLER_H
