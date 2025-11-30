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


Tuile::Tuile(unsigned int i, unsigned int p, unsigned int n) : id(i), inversion(0), prix(p), niveau(n) 
{
    design =
        "         _____     \n" // Ligne 0
        "        /     \\    \n" // Ligne 1
        "  _____/       \\   \n" // Ligne 2
        " /     \\       /   \n" // Ligne 3
        "/       \\_____/    \n" // Ligne 4
        "\\       /     \\    \n" // Ligne 5
        " \\_____/       \\   \n" // Ligne 6
        "       \\       /   \n" // Ligne 7
        "        \\_____/    \n";
    int positions[3] = { 50, 83, 130 };
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
        design.replace(positions[i], 3, hexagones[i].affiche());
    }
}
void Tuile::setPrix(unsigned int p) {
    if (p >= 0 && p <= 3) {
        prix = p;
    }
    else {
        throw TuileException("Le prix n'est pas correct");
    }
}

void Tuile::tourner() {
    Hexagone temp = hexagones[0];
    Hexagone temp1 = hexagones[1];
    hexagones[0] = hexagones[2];
    hexagones[1] = temp;
    hexagones[2] = temp1;
}


Tuile::Tuile(const Tuile& t) 
    : id(t.id), niveau(t.niveau), inversion(t.inversion), design(t.design), prix(t.prix) 
{
    for(int i=0; i<3; ++i) {
        hexagones[i] = t.hexagones[i];*
        // Comme Tuile est friend de Hexagone, on peut accéder au pointeur privé 'tuile' de l'hexagone
        hexagones[i].tuile = this; 
    }
}

Tuile& Tuile::operator=(const Tuile& t) {
    if (this != &t) {
        id = t.id;
        niveau = t.niveau;
        inversion = t.inversion;
        design = t.design;
        prix = t.prix;
        for(int i=0; i<3; ++i) {
            hexagones[i] = t.hexagones[i];
            hexagones[i].tuile = this;
        }
    }
    return *this;
}