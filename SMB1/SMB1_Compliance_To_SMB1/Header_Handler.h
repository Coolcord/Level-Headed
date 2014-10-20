#ifndef HEADER_HANDLER_H
#define HEADER_HANDLER_H

#include "Item_Handler.h"
#include "../Common SMB1 Files/Level_Type.h"
#include "../Common SMB1 Files/Level_Attribute.h"
#include "../Common SMB1 Files/Level_Compliment.h"
#include <QFile>
#include <QMap>

class Header_Handler : public Item_Handler
{
public:
    Header_Handler(SMB1_Writer_Interface *writerPlugin, QFile *file);
    ~Header_Handler();
    bool Parse_Header(int &lineNum);

private:
    void Populate_Types();
    void Populate_Attributes();
    void Populate_Compliments();

    bool Parse_Ignored_Line(const QString &name, int &lineNum);

    QFile *file;
    QMap<QString, Level_Type::Level_Type> *types;
    QMap<QString, Level_Attribute::Level_Attribute> *attributes;
    QMap<QString, Level_Compliment::Level_Compliment> *compliments;
};

#endif // HEADER_HANDLER_H
