#ifndef RANDOM_H
#define RANDOM_H

#include <QRandomGenerator>
#include <QString>

class Random {
public:
    static Random& Get_Instance() { static Random instance; return instance; }
    void Seed(unsigned int seed);
    void Seed(const QString &seed);
    int Get_Num(int max); //Max is inclusive (e.g. max of 10 will return numbers 0-10)
    int Get_Num(int min, int max);

private:
    Random() { this->randomGenerator = new QRandomGenerator(); this->value = 0; }
    ~Random() { delete this->randomGenerator; this->randomGenerator = nullptr; }
    QRandomGenerator *randomGenerator;

    int value;

public:
    Random(Random const&) = delete;
    void operator=(Random const&) = delete;
};

#endif // RANDOM_H
