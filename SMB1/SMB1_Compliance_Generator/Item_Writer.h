#ifndef ITEM_WRITER_H
#define ITEM_WRITER_H

#include "../../../C_Common_Code/Qt/Text_Insertion_Buffer/Text_Insertion_Buffer.h"
#include <QFile>
#include <QString>

class Item_Writer {
public:
    Item_Writer(int numBytesLeft);
    virtual ~Item_Writer() { delete this->buffer; }
    int Get_Num_Bytes_Left();
    int Get_Num_Items();
    int Get_Level_Length();
    int Get_Current_Y();
    void Set_Current_Y(int value);
    int Get_Absolute_X(int x);
    int Get_Current_Page();
    void Set_Coordinate_Safety(bool value);
    bool Will_Page_Flag_Be_Tripped(int x);
    bool Write_Buffer_To_File(QFile *file);
    Text_Insertion_Buffer *Get_Buffer();

protected:
    Item_Writer(const Item_Writer&);
    Item_Writer& operator=(const Item_Writer&);
    bool Write_Item(int x, const QString &item);
    virtual bool Is_Coordinate_Valid(int coordinate)=0;
    bool Is_Byte_Valid(int byte);
    bool Is_Safe_To_Write_Item(int numBytes = 2);
    bool Handle_Level_Length_On_Page_Change(int page);

    Text_Insertion_Buffer *buffer;
    int numBytesLeft;
    int numItems;
    int levelLength;
    int currentX;
    int currentY;
    int currentPage;
    bool coordinateSafety;
};

#endif // ITEM_WRITER_H
