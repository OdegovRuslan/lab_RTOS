#include <iostream>
#include <cstdlib>
#include <pthread.h>

#include "LKG.hpp"

void* P(void* arg){
    LKG* lkg = reinterpret_cast<LKG*>(arg);
    lkg->Print();
}

int main(int argc, char * argv[]) {

    LKG lkg;
    if (argc != 1){
        if (argc < 5) return -1;
        else lkg.Introduce(atoi(argv[1]), atoi(argv[2]),  atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    }

    pthread_t p;
    pthread_create(&p, NULL, P, &lkg);
    pthread_join(p, NULL);

    return 0;
}