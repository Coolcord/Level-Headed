#include "Item_Handler.h"
#include <assert.h>

Item_Handler::Item_Handler(SMB1_Writer_Interface *writerPlugin) {
    assert(writerPlugin);
    this->writerPlugin = writerPlugin;
}

bool Item_Handler::Parse_Num(const QString &value, int &num) {
    if (value == nullptr || value.isEmpty()) return false;
    bool success = false;
    num = value.toInt(&success);
    return success;
}

