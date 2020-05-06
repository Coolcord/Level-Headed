#ifndef LEVEL_SCRIPT_PARSER_H
#define LEVEL_SCRIPT_PARSER_H

#include "SMB1_Compliance_Parser_Arguments.h"
#include "SMB1_Compliance_Map.h"
#include <QTextStream>

class Level_Script_Parser : public SMB1_Compliance_Map {
public:
    Level_Script_Parser(SMB1_Compliance_Parser_Arguments *args);
    ~Level_Script_Parser();
    bool Parse_Level_Script();

private:
    bool Parse_Header();
    bool Parse_Items();
    bool Parse_Object(const QString &line);
    bool Parse_Enemy(const QString &line);
    bool Parse_Difficulty(const QString &value, bool &onlyHardMode);
    QString Parse_Through_Comments_Until_First_Word(const QString &word);
    QString Parse_Through_Comments_Until_First_Word_Or_Seperator(const QString &word);
    QString Parse_Through_Comments_Until_Seperator();

    SMB1_Compliance_Parser_Arguments *args;
    QTextStream *stream;
};

#endif // LEVEL_SCRIPT_PARSER_H
