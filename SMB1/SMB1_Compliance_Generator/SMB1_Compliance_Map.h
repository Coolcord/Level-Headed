#ifndef SMB1_COMPLIANCE_MAP_H
#define SMB1_COMPLIANCE_MAP_H

#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Enemy_Item.h"
#include "../Common_SMB1_Files/Level.h"
#include "../Common_SMB1_Files/Level_Attribute.h"
#include "../Common_SMB1_Files/Level_Compliment.h"
#include "../Common_SMB1_Files/Object_Item.h"
#include "../Common_SMB1_Files/Scenery.h"
#include <QMap>
#include <QString>

class SMB1_Compliance_Map {
public:
    SMB1_Compliance_Map();
    ~SMB1_Compliance_Map();

protected:
    QMap<QString, Object_Item::Object_Item> *objectsMap;
    QMap<QString, Enemy_Item::Enemy_Item> *enemiesMap;
    QMap<QString, Level_Attribute::Level_Attribute> *attributesMap;
    QMap<QString, Brick::Brick> *bricksMap;
    QMap<QString, Background::Background> *backgroundsMap;
    QMap<QString, Scenery::Scenery> *sceneriesMap;
    QMap<QString, Level_Compliment::Level_Compliment> *complimentsMap;
    QMap<QString, Level::Level> *levelsMap;

private:
    void Populate_Attributes();
    void Populate_Bricks();
    void Populate_Backgrounds();
    void Populate_Compliments();
    void Populate_Enemy_Map();
    void Populate_Object_Map();
    void Populate_Sceneries();
    void Populate_Levels();
};

#endif // SMB1_COMPLIANCE_MAP_H
