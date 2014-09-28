#ifndef SMB1_COMPLIANCE_MAP_H
#define SMB1_COMPLIANCE_MAP_H

#include "../Common SMB1 Files/Object_Item.h"
#include "../Common SMB1 Files/Enemy_Item.h"
#include <QMap>
#include <QString>

class SMB1_Compliance_Map
{
public:
    SMB1_Compliance_Map();
    ~SMB1_Compliance_Map();

protected:
    QMap<QString, Object_Item::Object_Item> *objects;
    QMap<QString, Enemy_Item::Enemy_Item> *enemies;

private:
    void Populate_Object_Map();
    void Populate_Enemy_Map();
};

#endif // SMB1_COMPLIANCE_MAP_H
