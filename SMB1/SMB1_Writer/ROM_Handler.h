#ifndef ROM_HANDLER_H
#define ROM_HANDLER_H

#include "ROM_Type.h"

class ROM_Handler
{
    public:
        ROM_Handler();
    private:
        enum ROM_Type {
            DEFAULT,
            TRACK,
            FAMICOM,
            INVALID
        };
        ROM_Type romType;
        ROM_Type Get_ROM_Type();
};

#endif // ROM_HANDLER_H
