#include "LKG.hpp"

LKG::LKG(){
    a = 106;
    c = 1283;
    m = 6075;
    x0 = 1;
    n = 5;
}

LKG::Introduce(int _a, int _c, int _m, int _x0, int _n){
    a = _a;
    c = _c;
    m = _m;
    x0 = _x0;
    n = _n;
}

LKG::Print(){
    if (n != 0)
    {
        x0 = (a*x0 + c) % m;
        n--;
        std::cout << x0 << std::endl;
        Print();
    }

    return 0;
}