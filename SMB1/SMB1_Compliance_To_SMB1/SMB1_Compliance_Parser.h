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
    int Parse_Level(const QString &fileLocation);
    int Parse_Level(const QString &fileLocation, int &lineNum);

private:
    bool Parse_Header(QFile *file, int &lineNum, int &errorCode);
    bool Parse_Items(QFile *file, int &lineNum, int &errorCode);
    bool Parse_Object(const QString &line, int &errorCode);
    bool Parse_Enemy(const QString &line, int &errorCode);

    SMB1_Writer_Interface *writerPlugin;
    Object_Handler *objectHandler;
    Enemy_Handler *enemyHandler;
};

#endif // SMB1_COMPLIANCE_PARSER_H
