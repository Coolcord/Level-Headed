#ifndef HEADER_HANDLER_H
#define HEADER_HANDLER_H

#include "Item_Handler.h"
#include "../Common_SMB1_Files/Level_Type.h"
#include "../Common_SMB1_Files/Level_Attribute.h"
#include "../Common_SMB1_Files/Brick.h"
#include "../Common_SMB1_Files/Background.h"
#include "../Common_SMB1_Files/Scenery.h"
#include "../Common_SMB1_Files/Level_Compliment.h"
#include <QTextStream>
#include <QMap>

class Header_Handler : public Item_Handler
{
public:
    Header_Handler(SMB1_Writer_Interface *writerPlugin, QTextStream *file);
    ~Header_Handler();
    bool Parse_Header(bool bonusLevel, int &lineNum, int &errorCode);

private:
    bool Parse_Through_Comments_Until_String(const QString &value, int &lineNum);
    QString Parse_Through_Comments_Until_First_Word(const QString &word, int &lineNum);
    void Populate_Attributes();
    void Populate_Bricks();
    void Populate_Backgrounds();
    void Populate_Sceneries();
    void Populate_Compliments();

    bool Parse_Ignored_Line(const QString &name, int &lineNum);

    QTextStream *file;
    QMap<QString, Level_Attribute::Level_Attribute> *attributes;
    QMap<QString, Brick::Brick> *bricks;
    QMap<QString, Background::Background> *backgrounds;
    QMap<QString, Scenery::Scenery> *sceneries;
    QMap<QString, Level_Compliment::Level_Compliment> *compliments;
};

#endif // HEADER_HANDLER_H
