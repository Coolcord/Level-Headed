#ifndef LEVEL_CRAWLER_H
#define LEVEL_CRAWLER_H

#include "SMB1_Compliance_Map.h"
#include "../Common SMB1 Files/Brick.h"
#include "../Common SMB1 Files/Level_Attribute.h"
#include <QFile>

class Level_Crawler : public SMB1_Compliance_Map
{
public:
    Level_Crawler(QFile *file);
    ~Level_Crawler();
    bool Crawl_Level(Brick::Brick startingBrick);
    bool Find_Safe_Coordinate(int &x, int &y, int lastX);
    bool Find_Safe_Coordinate(int size, int &x, int &y, int lastX);
    bool Find_Safe_Coordinate_At_X(int x, int &y);

private:
    bool Is_Coordinate_Safe(int x, int y);
    void Crawl_Forward(int x, int spaces);
    void Crawl_Forward_With_Hole(int x, int spaces, int &holeSteps);
    void Mark_Bad_Coordinate(int x, int y);
    void Mark_Bad_X(int x);
    void Clear_X(int x);
    QString Make_Key(int x, int y);
    bool Parse_Object(const QString &line, int &x, int &holeCrawlSteps);

    Brick::Brick brick;
    QFile *file;
    QMap<QString, bool> *badCoordinates;
};

#endif // LEVEL_CRAWLER_H
