#ifndef SMB1_COMPLIANCE_MAP_H
#define SMB1_COMPLIANCE_MAP_H

#include "../Common_SMB1_Files/Object_Item.h"
#include "../Common_SMB1_Files/Enemy_Item.h"
#include <QMap>
#include <QString>

class SMB1_Compliance_Map {
public:
    SMB1_Compliance_Map();
    ~SMB1_Compliance_Map();

protected:
    QMap<QString, Object_Item::Object_Item> *objectsMap;
    QMap<QString, Enemy_Item::Enemy_Item> *enemiesMap;

private:
    void Populate_Object_Map();
    void Populate_Enemy_Map();
};

#endif // SMB1_COMPLIANCE_MAP_H
