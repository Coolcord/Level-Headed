#ifndef SMB1_COMPLIANCE_PARSER_H
#define SMB1_COMPLIANCE_PARSER_H

#include "../Common SMB1 Files/Object_Item.h"
#include "../Common SMB1 Files/Enemy_Item.h"
#include <QMap>
#include <QString>

class SMB1_Compliance_Parser
{
public:
    SMB1_Compliance_Parser();
    ~SMB1_Compliance_Parser();

private:
    QMap<QString, Object_Item::Object_Item> *objects;
    QMap<QString, Enemy_Item::Enemy_Item> *enemies;
};

#endif // SMB1_COMPLIANCE_PARSER_H
