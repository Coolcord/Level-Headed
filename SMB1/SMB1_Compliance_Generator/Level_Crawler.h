#ifndef LEVEL_CRAWLER_H
#define LEVEL_CRAWLER_H

#include "SMB1_Compliance_Map.h"
#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Level_Attribute.h"
#include <QFile>

class Object_Writer;
class Text_Insertion_Buffer;

class Level_Crawler : public SMB1_Compliance_Map {
public:
    Level_Crawler(Text_Insertion_Buffer *objectsBuffer);
    ~Level_Crawler();
    bool Crawl_Level(Brick::Brick startingBrick);
    int Get_Safe_Size();
    bool Find_Safe_Coordinate(int &x, int &y, int lastX);
    bool Find_Safe_Coordinate(int size, int &x, int &y, int lastX, bool reverse = false);
    bool Find_Safe_Coordinate_At_Y(int &x, int y, int lastX);
    bool Find_Safe_Coordinate_At_Y(int size, int &x, int y, int lastX, bool reverse = false);
    bool Find_Safe_Coordinate_At_X(int x, int &y);
    bool Find_Safe_Green_Leaping_Paratroopa_Coordinate(int &x, int &y, int lastX, bool reverse = false);
    bool Find_Safe_Green_Flying_Paratroopa_Coordinate(int &x, int &y, int lastX, bool reverse = false);
    bool Find_Safe_Red_Paratroopa_Coordinate(int &x, int &y, int lastX, bool reverse = false);

private:
    void Populate_Brick_Map();
    bool Is_Coordinate_Safe(int x, int y);
    bool Scan_For_Safe_Green_Flying_Paratroopa_Spawn(int x, int &y);
    void Crawl_Forward(int x, int spaces);
    void Crawl_Forward_With_Hole(int x, int spaces, int &holeSteps);
    void Mark_Bad_Coordinate(int x, int y);
    void Mark_Bad_X(int x);
    void Clear_X(int x);
    QString Make_Key(int x, int y);
    bool Parse_Object(const QString &line, int &x, int &holeCrawlSteps);

    //Debug Functions
    bool Draw_Map();
    int Get_X_From_Key(const QString &key);
    int Get_Y_From_Key(const QString &key);

    Text_Insertion_Buffer *objectsBuffer;
    Brick::Brick brick;
    Brick::Brick nextBrick;
    QMap<QString, Brick::Brick> *bricks;
    bool endDetected;
    int safeSize;
    QMap<QString, bool> *badCoordinates;
};

#endif // LEVEL_CRAWLER_H
