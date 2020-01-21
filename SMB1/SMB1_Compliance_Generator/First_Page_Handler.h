#ifndef FIRST_PAGE_HANDLER_H
#define FIRST_PAGE_HANDLER_H

#include "../Common_SMB1_Files/Castle.h"
#include "../Common_SMB1_Files/Background.h"

class Object_Buffer;

class First_Page_Handler {
public:
    First_Page_Handler(Object_Buffer *object, Background::Background headerBackground, Castle::Castle castle, bool useAutoScroll);
    void Handle_First_Page(int &x);
    Background::Background Get_Header_Background();

private:
    Object_Buffer *object;
    Castle::Castle castle;
    Background::Background headerBackground;
    bool firstPageWritten;
    bool useAutoScroll;
};

#endif // FIRST_PAGE_HANDLER_H
