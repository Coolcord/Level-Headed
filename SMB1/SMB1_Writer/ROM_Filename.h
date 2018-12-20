#ifndef ROM_FILENAME_H
#define ROM_FILENAME_H

#include <QString>

namespace ROM_Filename {
    const static QString STRING_FULL_SUPPORT = "(Full Support)";
    const static QString STRING_PARTIAL_SUPPORT = "(Partial Support)";
    const static QString STRING_USA0 = STRING_FULL_SUPPORT+" Super Mario Bros. (JU) (PRG0) [!].nes";
    const static QString STRING_USA1 = STRING_FULL_SUPPORT+" Super Mario Bros. (JU) (PRG1).nes";
    const static QString STRING_DUCK = STRING_PARTIAL_SUPPORT+" Super Mario Bros. + Duck Hunt (U).nes";
    const static QString STRING_TRACK = STRING_PARTIAL_SUPPORT+" Super Mario Bros. + Duck Hunt + World Class Track Meet (U).nes";
    const static QString STRING_COOP_CGTI_1 = STRING_PARTIAL_SUPPORT+" Simultaneous Co-op v1.0 (by Corpse Grinder and TI).nes";
}

#endif // ROM_FILENAME_H
