#ifndef SMB1_COMPLIANCE_PARSER_H
#define SMB1_COMPLIANCE_PARSER_H

#include "../SMB1_Compliance_Generator/SMB1_Compliance_Map.h"
#include "../SMB1_Writer/SMB1_Writer_Interface.h"
#include <QString>
#include <QFile>

class Object_Handler;
class Enemy_Handler;

class SMB1_Compliance_Parser : public SMB1_Compliance_Map
{
public:
    SMB1_Compliance_Parser(SMB1_Writer_Interface *writerPlugin);
    ~SMB1_Compliance_Parser();
    bool Parse_Level(const QString &fileLocation);

private:
    bool Parse_Header(QFile *file);
    bool Parse_Items(QFile *file);
    bool Parse_Object(const QString &line);
    bool Parse_Enemy(const QString &line);

    SMB1_Writer_Interface *writerPlugin;
    Object_Handler *objectHandler;
    Enemy_Handler *enemyHandler;
};

#endif // SMB1_COMPLIANCE_PARSER_H
