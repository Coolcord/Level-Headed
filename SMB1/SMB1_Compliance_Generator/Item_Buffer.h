#ifndef ITEM_BUFFER_H
#define ITEM_BUFFER_H

#include <QFile>
#include <QString>

struct Object_Buffer_Data;
struct Enemy_Buffer_Data;
struct Extra_Enemy_Args;

class Item_Buffer {
public:
    Item_Buffer(int numBytesLeft);
    virtual ~Item_Buffer() {}
    int Get_Num_Bytes_Left();
    int Get_Num_Items();
    int Get_Level_Length();
    int Get_Current_Y();
    void Set_Current_Y(int value);
    int Get_Absolute_X(int x);
    int Get_Current_Page();
    void Set_Coordinate_Safety(bool value);
    bool Will_Page_Flag_Be_Tripped(int x);

protected:
    Item_Buffer(const Item_Buffer&);
    Item_Buffer& operator=(const Item_Buffer&);
    virtual bool Is_Coordinate_Valid(int coordinate)=0;
    bool Is_Byte_Valid(int byte);
    bool Is_Safe_To_Write_Item(int numBytes = 2);
    bool Handle_Level_Length_On_Page_Change(int page);
    Object_Buffer_Data Get_Empty_Object_Buffer_Data();
    Enemy_Buffer_Data Get_Empty_Enemy_Buffer_Data();
    Extra_Enemy_Args Get_Empty_Enemy_Args();
    Extra_Enemy_Args Get_Empty_Enemy_Args(bool onlyHardMode);
    void Update_Level_Stats(int x);

    int numBytesLeft;
    int numItems;
    int levelLength;
    int currentX;
    int currentY;
    int currentPage;
    bool coordinateSafety;
};

#endif // ITEM_BUFFER_H
