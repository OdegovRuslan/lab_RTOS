#ifndef _LKG_
#define _LKG_

#include <cstdlib>
#include <iostream>


class LKG {

public:
    LKG();
    Introduce(int _a, int _c, int _m, int _x0, int _n);
    Print();

private:
    int a, c, m, x0, n;
};

#endif