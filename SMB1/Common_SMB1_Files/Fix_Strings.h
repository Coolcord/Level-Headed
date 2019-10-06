#ifndef FIX_STRINGS_H
#define FIX_STRINGS_H

#include "../../../Hexagon/Hexagon/Patch_Strings.h"
#include <QString>

namespace Fix_Strings {
    const static QString STRING_FIXES = "Fixes";
    const static QString STRING_GRAPHICS_PACK = "Graphics Pack";
    const static QString STRING_MARIO_SPRITE = "Mario Sprite";
    const static QString STRING_ALLOW_PALETTES = Patch_Strings::STRING_COMMENT+" Palette: None";
    const static QString STRING_ALLOW_ONLY_COIN_PALETTES = Patch_Strings::STRING_COMMENT+" Palette: Coins";
    const static QString STRING_INVALID_TONES = Patch_Strings::STRING_COMMENT+" Invalid Tones: ";
    const static QString STRING_COMPATIBLE_SECTION = Patch_Strings::STRING_COMMENT+" Compatible:";
}

#endif // FIX_STRINGS_H
