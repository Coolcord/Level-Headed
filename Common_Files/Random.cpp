#ifndef RANDOM_H
#define RANDOM_H

#include "Random.h"

#include <QTime>

namespace Random {
    int Get_Num(int max) {
        return static_cast<int>(qrand()/(static_cast<double>(RAND_MAX)+1)*(max+1));
    }

    int Get_Num(int min, int max) {
        return static_cast<int>(((static_cast<double>(qrand())/(RAND_MAX+1))*(max-min+1)+min));
    }

    uint Get_Seed_From_QString(const QString &string) {
        bool isNumber = false;
        uint seed = 0;
        seed = string.toUInt(&isNumber, 10);
        if (isNumber) return seed;
        for (int i = 0; i < string.size(); ++i) seed += static_cast<uint>(string.at(i).toLatin1());
        return seed;
    }
}

#endif // RANDOM_H
