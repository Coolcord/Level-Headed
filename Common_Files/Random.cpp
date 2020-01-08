#include "Random.h"
#include <assert.h>

void Random::Seed(const QString &seed, unsigned int instance) {
    assert(instance != 0);
    bool isNumber = false;
    unsigned int uSeed = 0;
    uSeed = seed.toUInt(&isNumber, 10);
    this->instance = instance;
    if (isNumber) return this->Seed(uSeed+instance);
    for (int i = 0; i < seed.size(); ++i) uSeed += static_cast<unsigned int>(seed.at(i).toLatin1());
    return this->Seed(uSeed+instance);
}

int Random::Get_Num(int max) {
    assert(this->instance != 0);
    assert(max >= 0);
    if (max == 0) return 0;
    return this->randomGenerator->bounded(max+1);
}

int Random::Get_Num(int min, int max) {
    assert(this->instance != 0);
    assert(min >= 0);
    assert(max >= 0);
    assert(min <= max);
    if (max == 0) return 0;
    return this->randomGenerator->bounded(min, max+1);
}

void Random::Seed(unsigned int seed) {
    srand(seed);
    qsrand(seed);
    this->randomGenerator->seed(seed);
}
