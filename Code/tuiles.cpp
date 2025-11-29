#include <iostream>
#include <cstring>
#include "tuiles.h"
#include "hexagone.h"
#include <random>

using namespace std;


int randomIndexAkropolis() {
    static random_device rd;
    static mt19937 gen(rd());
    static discrete_distribution<> dist({12, 10, 10, 8, 10, 15});
    return dist(gen);
}

int randomStarValue() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dist(1, 3);
    return dist(gen);
}

int randomPlaceValue() {
    static random_device rd;
    static mt19937 gen(rd());
    static discrete_distribution<> dist({80,20});
    return dist(gen);
}


Tuile::Tuile(unsigned int i, unsigned int p, unsigned int n) : id(i), inversion(0), prix(p), niveau(n) {
    for (int i = 0; i < 3; i++) {
        int t = randomIndexAkropolis();
        if (Type(t) == Carriere) {
            hexagones[i] = Hexagone(Type(t), n, this);
        }
        else {
            bool place = randomPlaceValue();
            if (place == true) {
                int etoiles = randomStarValue();
                hexagones[i] = Hexagone(Type(t), n, this, etoiles, place);
            }
            else {
                hexagones[i] = Hexagone(Type(t), n, this);
            }

        }
        
    }
}

void Tuile::tourner() {
    Hexagone temp = hexagones[0];
    Hexagone temp1 = hexagones[1];
    hexagones[0] = hexagones[2];
    hexagones[1] = temp;
    hexagones[2] = temp1;
}




