#include <iostream>
#include <cstring>
#include "tuiles.h"
#include "hexagone.h"
#include <random>
#include <vector> 

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
    static discrete_distribution<> dist({ 80,20 });
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
    
    hexagones.resize(3); // Initialize vector with 3 elements
    
    int positions[3] = { 50, 83, 130 };
    for (int k = 0; k < 3; k++) {
        int t = randomIndexAkropolis();
        if (Type(t) == Carriere) {
            hexagones[k] = new Hexagone(Type(t), n, this);
        }
        else {
            bool place = randomPlaceValue();
            if (place == true) {
                int etoiles = randomStarValue();
                hexagones[k] = new Hexagone(Type(t), n, this, etoiles, place);
            }
            else {
                hexagones[k] = new Hexagone(Type(t), n, this);
            }

        }
        design.replace(positions[k], 3, hexagones[k]->affiche());
    }
}

TuileDepart::TuileDepart() : Tuile() {
    id = 0; // ID specifique pour la tuile de depart
    niveau = 0;
    inversion = false;
    prix = 0;

    hexagones.resize(4);
    
    // 0: Habitation (Bleu) avec 1 étoile (Centre)
    hexagones[0] = new Hexagone(Habitation, 1, this, 1, true); 
    
    // 1, 2, 3: Carrières (Autour)
    hexagones[1] = new Hexagone(Carriere, 1, this);
    hexagones[2] = new Hexagone(Carriere, 1, this);
    hexagones[3] = new Hexagone(Carriere, 1, this);
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
    if (hexagones.size() != 3) return; // Rotation interdite à la tuile de départ
    Hexagone* temp = hexagones[0];
    Hexagone* temp1 = hexagones[1];
    hexagones[0] = hexagones[2];
    hexagones[1] = temp;
    hexagones[2] = temp1;
}


Tuile::Tuile(const Tuile& t)
    : id(t.id), niveau(t.niveau), inversion(t.inversion), design(t.design), prix(t.prix)
{
    // On ne fait PAS hexagones = t.hexagones (sinon deux tuiles pointent vers les mêmes hexagones -> crash au delete)
    hexagones.reserve(t.hexagones.size());
    for (size_t i = 0; i < t.hexagones.size(); ++i) {
        // On crée un NOUVEL hexagone en copiant celui de l'autre tuile
        // Cela appelle le constructeur de copie par défaut de Hexagone
        hexagones.push_back(new Hexagone(*t.hexagones[i]));

        // On met à jour le pointeur 'tuile' pour qu'il pointe vers CETTE nouvelle tuile
        hexagones.back()->tuile = this;
    }
}

Tuile& Tuile::operator=(const Tuile& t) {
    if (this != &t) {
        // 1. Nettoyer l'existant
        for (size_t i = 0; i < hexagones.size(); ++i) {
            delete hexagones[i];
        }
        hexagones.clear();

        // 2. Copier les données scalaires
        id = t.id;
        niveau = t.niveau;
        inversion = t.inversion;
        design = t.design;
        prix = t.prix;

        // 3. Copie profonde des hexagones
        hexagones.reserve(t.hexagones.size());
        for (size_t i = 0; i < t.hexagones.size(); ++i) {
            hexagones.push_back(new Hexagone(*t.hexagones[i]));
            hexagones.back()->tuile = this;
        }
    }
    return *this;
}

Tuile::~Tuile() {
    for (size_t i = 0; i < hexagones.size(); ++i) {
        delete hexagones[i]; // On libère la mémoire de chaque hexagone
    }
    hexagones.clear();
}