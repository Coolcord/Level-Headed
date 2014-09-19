#include "SMB1_Compliance_Parser.h"
#include "../Common SMB1 Files/Object_Item.h"
#include "../Common SMB1 Files/Object_Item_String.h"
#include "../Common SMB1 Files/Enemy_Item.h"
#include "../Common SMB1 Files/Enemy_Item_String.h"

SMB1_Compliance_Parser::SMB1_Compliance_Parser() {

}

SMB1_Compliance_Parser::~SMB1_Compliance_Parser() {
    delete this->objects;
    delete this->enemies;
}


