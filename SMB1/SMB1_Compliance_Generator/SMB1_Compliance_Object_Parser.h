#ifndef SMB1_COMPLIANCE_OBJECT_PARSER_H
#define SMB1_COMPLIANCE_OBJECT_PARSER_H

#include "SMB1_Compliance_Map.h"

class SMB1_Compliance_Object_Parser : public SMB1_Compliance_Map {
public:
    SMB1_Compliance_Object_Parser() : SMB1_Compliance_Map() {}
    ~SMB1_Compliance_Object_Parser() {}
    bool Parse_Line(const QString &line, Object_Item::Object_Item &objectItem, int &x, int &y, int &length, QString &attributesMap);

private:
    bool Get_Number_From_String(const QString &string, int &num);
};

#endif // SMB1_COMPLIANCE_OBJECT_PARSER_H
