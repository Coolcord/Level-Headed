#include "Spawner.h"
#include <assert.h>
#include <QTime>

int Spawner::Get_Random_X() {
    //Aim for a lower value... but allow higher values to be possible
    switch (qrand() % 4) {
    case 0:     return (qrand() % 0x08);
    case 1:     return (qrand() % 0x0A);
    case 2:     return (qrand() % 0x0D);
    case 3:     return (qrand() % 0x10);
    default:    assert(false); return 0;
    }
}

