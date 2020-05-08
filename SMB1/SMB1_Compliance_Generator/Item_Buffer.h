#ifndef ITEM_BUFFER_H
#define ITEM_BUFFER_H

#include "Buffer_Data.h"
#include <QFile>
#include <QLinkedList>
#include <QString>

class Item_Buffer {
public:
    Item_Buffer(int numBytesLeft);
    virtual ~Item_Buffer() { delete this->itemBuffer; }
    virtual void Clear_Buffer()=0;
    int Get_Num_Bytes_Left();
    int Get_Num_Items();
    int Get_Level_Length();
    int Get_Current_Y();
    void Set_Current_Y(int value);
    void Set_Num_Bytes_Left_And_Total_Bytes(int value);
    int Get_Absolute_X();
    int Get_Page_Relative_Absolute_X(int x);
    int Get_Current_Page();
    void Set_Coordinate_Safety(bool value);
    bool Will_Page_Flag_Be_Tripped(int x);
    void Insert_Into_Buffer(const Buffer_Data &data);

    //Buffer Peeking
    bool Is_Last_Item_A_Page_Change();

    //Buffer Navigation
    bool Is_Empty();
    bool At_Beginning();
    bool At_End();
    bool Seek_To_Object_Item(int absoluteX, int y, Object_Item::Object_Item objectItem);
    bool Seek_To_Enemy_Item(int absoluteX, int y, Enemy_Item::Enemy_Item enemyItem);
    bool Seek_To_Absolute_X(int absoluteX); //seeks to the last item at absolute X
    void Seek_To_First_Item();
    void Seek_To_Next();
    void Seek_To_Previous();
    void Seek_To_End();
    Buffer_Data Get_Current();
    Buffer_Data *Get_Current_For_Modification();

protected:
    Item_Buffer(const Item_Buffer&);
    Item_Buffer& operator=(const Item_Buffer&);
    void Initialize_Buffer();
    virtual bool Is_Coordinate_Valid(int &coordinate)=0;
    virtual void Update_Last_Page_Change_For_Next_X(int &x)=0;
    bool Is_Byte_Valid(int byte);
    bool Is_Safe_To_Write_Item(int numBytes = 2);
    bool Handle_Level_Length_On_Page_Change(int page);
    void Update_Level_Stats(int x);

    QLinkedList<Buffer_Data> *itemBuffer;
    QLinkedList<Buffer_Data>::iterator itemBufferIter;
    int numBytesLeft;
    int totalBytes;
    int numItems;
    int levelLength;
    int currentX;
    int currentAbsoluteX;
    int currentY;
    int currentPage;
    bool coordinateSafety;
};

#endif // ITEM_BUFFER_H
