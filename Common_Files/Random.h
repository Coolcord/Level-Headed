#ifndef RANDOM_H
#define RANDOM_H

#include <QTime>

namespace Random {
    int Get_Num(int max); //Max is inclusive (e.g. max of 10 will return numbers 0-10)
    int Get_Num(int min, int max);
    uint Get_Seed_From_QString(const QString &string);
}

#endif // RANDOM_H
