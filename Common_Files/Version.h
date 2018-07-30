#ifndef VERSION_H
#define VERSION_H

#include <QString>

namespace Version {
    const static QString VERSION_NUMBER = "0.3.0";
    const static QString VERSION = "v"+VERSION_NUMBER;
}

/*
 * Level-Headed Version Rules
 *
 * Version numbers will follow symantic versioning with some modifications.
 * There are four numbers. Significant.Major.Minor-Patch
 * Significant and Patch can go up to 999.
 * Major and Minor can go up to 99.
 */

#endif // VERSION_H
