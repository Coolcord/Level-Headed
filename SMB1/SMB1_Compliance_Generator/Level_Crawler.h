#ifndef LEVEL_CRAWLER_H
#define LEVEL_CRAWLER_H

#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Level_Attribute.h"
#include <QFile>

class Buffer_Data;
class Object_Buffer;

class Level_Crawler {
public:
    Level_Crawler(Object_Buffer *objects);
    ~Level_Crawler();
    bool Crawl_Level();
    bool Recrawl_Level();
    Brick::Brick Get_Starting_Brick();
    void Set_Starting_Brick(Brick::Brick startingBrick);
    int Get_Safe_Size();
    bool Is_Coordinate_Empty(int x, int y);
    bool Is_Coordinate_Used(int x, int y);

private:
    void Crawl_Forward(int x, int spaces);
    void Crawl_Forward_With_Hole(int x, int spaces, int &holeSteps);
    void Mark_Bad_Coordinate(int x, int y);
    void Mark_Bad_X(int x);
    void Clear_X(int x);
    QString Make_Key(int x, int y);
    bool Parse_Object(const Buffer_Data &data, int &x, int &holeCrawlSteps);

    //Debug Functions
    bool Draw_Map();
    int Get_X_From_Key(const QString &key);
    int Get_Y_From_Key(const QString &key);

    Object_Buffer *objects;
    Brick::Brick brick;
    Brick::Brick nextBrick;
    Brick::Brick startingBrick;
    bool endDetected;
    int safeSize;
    bool levelCrawled;
    QMap<QString, bool> *badCoordinates;
};

#endif // LEVEL_CRAWLER_H
