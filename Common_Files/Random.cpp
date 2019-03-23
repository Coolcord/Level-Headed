#include "Random.h"
#include <assert.h>

void Random::Seed(const QString &seed, unsigned int instance) {
    bool isNumber = false;
    unsigned int uSeed = 0;
    uSeed = seed.toUInt(&isNumber, 10);
    if (isNumber) return this->Seed(uSeed+instance);
    for (int i = 0; i < seed.size(); ++i) uSeed += static_cast<unsigned int>(seed.at(i).toLatin1());
    return this->Seed(uSeed+instance);
}

int Random::Get_Num(int max) {
    return this->randomGenerator->bounded(max+1);
}

int Random::Get_Num(int min, int max) {
    return this->randomGenerator->bounded(min, max+1);
}

void Random::Seed(unsigned int seed) {
    this->randomGenerator->seed(seed);
}
