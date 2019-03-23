#ifndef RANDOM_H
#define RANDOM_H

#include <QRandomGenerator>
#include <QString>

class Random {
public:
    static Random& Get_Instance() { static Random instance; return instance; }
    void Seed(const QString &seed, unsigned int instance); //instance should be > 0
    int Get_Num(int max); //Max is inclusive (e.g. max of 10 will return numbers 0-10)
    int Get_Num(int min, int max);

private:
    Random() { this->randomGenerator = new QRandomGenerator(); this->instance = 0; }
    ~Random() { delete this->randomGenerator; this->randomGenerator = nullptr; }
    void Seed(unsigned int seed);

    QRandomGenerator *randomGenerator;
    unsigned int instance;

public:
    Random(Random const&) = delete;
    void operator=(Random const&) = delete;
};

#endif // RANDOM_H
