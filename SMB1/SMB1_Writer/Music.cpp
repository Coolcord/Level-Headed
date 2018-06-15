#include "Music.h"

bool Music::Disable_Music() {
    if (!this->Write_Bytes_To_Offset(0x1124, QByteArray(1, 0x49))) return false; //disables most music
    if (!this->Write_Bytes_To_Offset(0x582E, QByteArray(1, 0x82))) return false; //disables Starman music
    if (!this->Write_Bytes_To_Offset(0x7920, QByteArray(1, 0x4B))) return false; //disables castle complete theme
    if (!this->Write_Bytes_To_Offset(0x7922, QByteArray(1, 0x4B))) return false; //disables level complete theme
    return true;
}
