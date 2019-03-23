#ifndef ROM_FILENAME_H
#define ROM_FILENAME_H

#include <QString>

namespace ROM_Filename {
    const static QString STRING_FULL_SUPPORT = "(Full Support)";
    const static QString STRING_PARTIAL_SUPPORT = "(Partial Support)";
    const static QString STRING_USA0 = STRING_FULL_SUPPORT+" Super Mario Bros. (JU).nes";
    const static QString STRING_EUROPE = STRING_PARTIAL_SUPPORT+" Super Mario Bros. (E).nes";
    const static QString STRING_FDS = STRING_PARTIAL_SUPPORT+" Super Mario Bros. (J).fds";
    const static QString STRING_COOP_CGTI_1 = STRING_PARTIAL_SUPPORT+" Simultaneous Co-op v1.0 (by Corpse Grinder and TI).nes";
    const static QString STRING_BILL_KILL_2 = STRING_PARTIAL_SUPPORT+" Super Bill Kill v2.0 (Vs. Multiplayer) (by jeffXvX).nes";
}

#endif // ROM_FILENAME_H
