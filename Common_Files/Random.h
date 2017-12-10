#ifndef RANDOM_H
#define RANDOM_H

#include <QTime>

namespace Random {
    //Max is inclusive (e.g. max of 10 will return numbers 0-10)
    static int Get_Num(int max) {
        return static_cast<int>(qrand()/(static_cast<double>(RAND_MAX)+1)*(max+1));
	}
    /*
    static int Get_Num(int min, int max) {
		return static_cast<int>(((double)qrand()/(RAND_MAX+1))*(max-min+1)+min);
    }*/
}

#endif // RANDOM_H
