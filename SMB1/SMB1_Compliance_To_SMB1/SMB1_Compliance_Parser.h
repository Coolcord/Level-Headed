#ifndef SMB1_COMPLIANCE_PARSER_H
#define SMB1_COMPLIANCE_PARSER_H

#include "../Common SMB1 Files/Object_Item.h"
#include "../Common SMB1 Files/Enemy_Item.h"
#include "../SMB1_Writer/SMB1_Writer_Interface.h"
#include <QMap>
#include <QString>
#include <QFile>

class Object_Handler;
class Enemy_Handler;

class SMB1_Compliance_Parser
{
public:
    SMB1_Compliance_Parser(SMB1_Writer_Interface *writerPlugin);
    ~SMB1_Compliance_Parser();
    bool Parse_Level(QString fileLocation);

private:
    void Populate_Objects_Map();
    void Populate_Enemy_Map();
    void Populate_Enemy_Properties_Map();
    bool Parse_Header(QFile *file);
    bool Parse_Object(const QString &line);
    bool Parse_Enemy(const QString &line);

    QMap<QString, Object_Item::Object_Item> *objects;
    QMap<QString, Enemy_Item::Enemy_Item> *enemies;
    SMB1_Writer_Interface *writerPlugin;
    Object_Handler *objectHandler;
    Enemy_Handler *enemyHandler;
};

#endif // SMB1_COMPLIANCE_PARSER_H
