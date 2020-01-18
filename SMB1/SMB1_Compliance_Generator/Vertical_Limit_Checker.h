#ifndef VERTICAL_LIMIT_CHECKER_H
#define VERTICAL_LIMIT_CHECKER_H

#include "SMB1_Compliance_Object_Parser.h"
#include <QString>

class Object_Writer;

class Vertical_Limit_Checker : public SMB1_Compliance_Object_Parser {
public:
    Vertical_Limit_Checker() : SMB1_Compliance_Object_Parser() {}
    ~Vertical_Limit_Checker() {}
    bool Does_Level_Exceed_Vertical_Object_Limit(Object_Writer *objects);
};

#endif // VERTICAL_LIMIT_CHECKER_H
