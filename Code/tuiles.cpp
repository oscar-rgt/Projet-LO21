#include <iostream>
#include <cstring>
#include "tuiles.h"
#include "hexagone.h"
#include <random>

using namespace std;


int randomIndexAkropolis() {
    static random_device rd;
    static mt19937 gen(rd());
    static discrete_distribution<> dist({12, 10, 10, 8, 10, 5, 15});
    return dist(gen);
}

int randomPlaceValue() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dist(1, 3);
    return dist(gen);
}


Tuile::Tuile(unsigned int i, unsigned int p, unsigned int n) : id(i), orientation(N), prix(p), niveau(n) {
    for (int i = 0; i < 3; i++) {
        int t = randomIndexAkropolis();
        if (Type(t) == Place) {
            int etoiles = randomPlaceValue();
            hexagones[i] = Hexagone(Type(t), n, this, etoiles);
        }
        else {
            hexagones[i] = Hexagone(Type(t),n, this);
        }
    }
}




