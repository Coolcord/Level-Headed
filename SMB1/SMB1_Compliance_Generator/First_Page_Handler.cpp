#include "First_Page_Handler.h"
#include "Object_Writer.h"
#include <assert.h>

First_Page_Handler::First_Page_Handler(Object_Writer *object, Background::Background headerBackground, Castle::Castle castle, bool useAutoScroll) {
    assert(object);
    this->object = object;
    this->headerBackground = headerBackground;
    this->castle = castle;
    this->firstPageWritten = false;
    this->useAutoScroll = useAutoScroll;
}

void First_Page_Handler::Handle_First_Page(int &x) {
    if (this->firstPageWritten) return;
    switch (castle) {
    case Castle::SMALL:
        assert(this->object->Castle(0));
        x += 16;
        break;
    case Castle::BIG:
        assert(this->object->Big_Castle(0));
        assert(this->object->Change_Background(6, this->headerBackground));
        x += 10;
        break;
    case Castle::NONE:
        x += 16;
        break;
    }
    this->object->Set_First_Page_Safety(true);
    this->firstPageWritten = true;
}

Background::Background First_Page_Handler::Get_Header_Background() {
    switch (this->castle) {
    case Castle::BIG:
        return Background::CASTLE_WALL;
    case Castle::SMALL:
    case Castle::NONE:
        return this->headerBackground;
    }
    assert(false); return Background::BLANK_BACKGROUND;
}
