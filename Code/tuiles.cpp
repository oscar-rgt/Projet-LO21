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
    static discrete_distribution<> dist({ 12, 10, 10, 8, 10, 15 });
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


Tuile::Tuile(unsigned int i, unsigned int p) : id(i), prix(p), inversion(0), design() {
    cout << "[DEBUG] Création de la tuile ID=" << id << ", Prix=" << prix << endl;
    hexagones.resize(3);
    auto itHex = hexagones.begin();

    for (int k = 0; k < 3; k++, ++itHex) {
        int t = randomIndexAkropolis();
        cout << "[DEBUG]   Hexagone " << k << " : Type=" << t << endl;

        if (TypeQuartier(t) == Carriere) {
            *itHex = new Hexagone(TypeQuartier(t), 0, this);
            cout << "[DEBUG]     Hexagone " << k << " est une carrière." << endl;
        } else {
            bool place = randomPlaceValue();
            if (place) {
                int etoiles = randomStarValue();
                *itHex = new Hexagone(TypeQuartier(t), 0, this, etoiles, place);
                cout << "[DEBUG]     Hexagone " << k << " est une place avec " << etoiles << " étoiles." << endl;
            } else {
                *itHex = new Hexagone(TypeQuartier(t), 0, this);
                cout << "[DEBUG]     Hexagone " << k << " est un quartier standard." << endl;
            }
        }
    }
}
TuileDepart::TuileDepart() : Tuile() {
    id = 0; // ID specifique pour la tuile de depart
    inversion = false;
    prix = 0;

    hexagones.resize(4);
    auto it = hexagones.begin();
    // 0: Habitation (Bleu) avec 1 étoile (Centre)
    *it = new Hexagone(Habitation, 0, this, 1, true);

    // 1, 2, 3: Carrières (Autour)
    *(it + 1) = new Hexagone(Carriere, 0, this);
    *(it + 2) = new Hexagone(Carriere, 0, this);
    *(it + 3) = new Hexagone(Carriere, 0, this);
}

void Tuile::setPrix(unsigned int p) {
    if (p > 3) prix = 3;
    else prix = p;
}

void Tuile::tourner() {
    if (hexagones.size() != 3) return; // Rotation interdite à la tuile de départ
    auto it0 = hexagones.begin();
    auto it1 = hexagones.begin() + 1;
    auto it2 = hexagones.begin() + 2;
    if (inversion) {
        // Echange des pointeurs via les itérateurs
        Hexagone* temp = *it0;
        Hexagone* temp1 = *it2;
        *it0 = *it1;
        *it1 = temp1;
        *it2 = temp;
    }
    else {
        Hexagone* temp = *it0;
        Hexagone* temp1 = *it1;
        *it0 = *it2;
        *it1 = temp;
        *it2 = temp1;
    }

    
}


Tuile::Tuile(const Tuile& t)
    : id(t.id), inversion(t.inversion), design(t.design), prix(t.prix)
{
    // On ne fait PAS hexagones = t.hexagones (sinon deux tuiles pointent vers les mêmes hexagones -> crash au delete)
    hexagones.reserve(t.hexagones.size());
    for (auto it = t.hexagones.cbegin(); it != t.hexagones.cend(); ++it) {
        hexagones.push_back(new Hexagone(**it));
        hexagones.back()->tuile = this;
    }
}

Tuile& Tuile::operator=(const Tuile& t) {
    if (this != &t) {
        // 1. Nettoyer l'existant avec itérateur
        for (auto it = hexagones.begin(); it != hexagones.end(); ++it) {
            delete* it;
        }
        hexagones.clear();

        // 2. Copier les données scalaires
        id = t.id;
        inversion = t.inversion;
        design = t.design;
        prix = t.prix;

        // 3. Copie profonde des hexagones
        hexagones.reserve(t.hexagones.size());
        for (auto it = t.hexagones.cbegin(); it != t.hexagones.cend(); ++it) {
            hexagones.push_back(new Hexagone(**it));
            hexagones.back()->tuile = this;
        }
    }
    return *this;
}

Tuile::~Tuile() {
    for (auto it = hexagones.begin(); it != hexagones.end(); ++it) {
        delete* it;
    }
    hexagones.clear();
}

string& Tuile::getDesign() {
    if (inversion) {
        design =
            "   _____           \n" // Ligne 0 15
            "  /     \\          \n" // Ligne 1
            " /       \\_____    \n" // Ligne 2
            " \\       /     \\   \n" // Ligne 3
            "  \\_____/       \\  \n" // Ligne 4
            "  /     \\       /  \n" // Ligne 5
            " /       \\_____/   \n" // Ligne 6
            " \\       /         \n" // Ligne 7
            "  \\_____/          \n";

        int positions[3] = { 44, 124, 91 };
        auto it = hexagones.begin();
        for (int k = 0; k < 3; k++, ++it) {
            design.replace(positions[k], 2, (*it)->affiche().substr(0,2));
        }
    }
    else {
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
        int positions[3] = { 50, 130, 83 };
        auto it = hexagones.begin();
        for (int k = 0; k < 3; k++, ++it) {
            design.replace(positions[k], 2, (*it)->affiche().substr(0, 2));
        }
    }
    return design;
}

void Tuile::reconstruireHexagone(int index, int typeInt, int etoiles) {
    cout << "[DEBUG] Reconstruction de l'hexagone " << index << " : Type=" << typeInt << ", Étoiles=" << etoiles << endl;

    if (index < 0 || index >= (int)hexagones.size()) {
        cout << "[DEBUG]   Index invalide !" << endl;
        return;
    }

    Hexagone* h = hexagones[index];
    
    // On écrase les valeurs aléatoires par les valeurs sauvegardées
    h->type = (TypeQuartier)typeInt;
    h->etoiles = etoiles;
    h->place = (etoiles > 0);

    cout << "[DEBUG]   Hexagone " << index << " reconstruit avec succès." << endl;
}
