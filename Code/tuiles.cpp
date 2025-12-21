#include "tuiles.h"
#include "hexagone.h"
#include <random>
#include <vector> 

using namespace std;

// =========================================================
// GÉNÉRATEURS ALÉATOIRES 
// =========================================================

// Retourne un type de quartier aléatoire pondéré, en essayant de respecter la distribution du jeu Akropolis
int randomIndexAkropolis() {

    // Initialisation statique : le générateur n'est créé qu'une seule fois
    static random_device rd;

    static mt19937 gen(rd());

    // Distribution discrète : Définit les poids pour chaque type (Carrière, Habitation, etc.)
    // Exemple : 12 chances pour Carrière, 10 pour Habitation...
    static discrete_distribution<> dist({ 12, 10, 10, 8, 10, 15 });
    return dist(gen);
}

// Génère le nombre d'étoiles (1, 2 ou 3) pour une Place
int randomStarValue() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dist(1, 3); // Équiprobable entre 1 et 3
    return dist(gen);
}

// Détermine si un quartier est une place ou un quartier simple
int randomPlaceValue() {
    static random_device rd;
    static mt19937 gen(rd());
    static discrete_distribution<> dist({ 80,20 }); // 80% de chance d'être normal (0), 20% de chance d'être une Place (1)
    return dist(gen);
}

// =========================================================
// CLASSE TUILE
// =========================================================

Tuile::Tuile(unsigned int i, unsigned int p) : id(i), inversion(0), prix(p), design()
{
    // On prépare le vecteur pour contenir exactement 3 hexagones
    hexagones.resize(3); 
    auto itHex = hexagones.begin();

    // Boucle de création des 3 hexagones
    for (int k = 0; k < 3; k++, ++itHex) {
        int t = randomIndexAkropolis();

        if (TypeQuartier(t) == Carriere) { // Cas 1 : C'est une Carrière (jamais d'étoiles)
            *itHex = new Hexagone(TypeQuartier(t), 0, this);
        } else { // Cas 2 : Autre quartier (peut être une Place)
			bool place = randomPlaceValue(); // place ou pas
            if (place) {
				int etoiles = randomStarValue(); // si oui, combien d'étoiles
                *itHex = new Hexagone(TypeQuartier(t), 0, this, etoiles, place);;
            } else {
                *itHex = new Hexagone(TypeQuartier(t), 0, this);
            }
        }
    }
}

Tuile::Tuile(const Tuile& t)
    : id(t.id), inversion(t.inversion), prix(t.prix), design(t.design)
{
    // On réserve la mémoire pour éviter les réallocations inutiles
    hexagones.reserve(t.hexagones.size());
    for (auto it = t.hexagones.cbegin(); it != t.hexagones.cend(); ++it) {

        // 'new Hexagone(**it)' appelle le constructeur de copie de Hexagone
        // Cela crée un NOUVEL objet en mémoire, identique à l'original.
        hexagones.push_back(new Hexagone(**it));

        // Le nouvel hexagone doit pointer vers CETTE nouvelle tuile, pas vers l'ancienne
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

        // 2. Copier les données 
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
        // Nettoyage manuel de la mémoire allouée dynamiquement
        delete* it;
    }
    hexagones.clear();
}

void Tuile::setPrix(unsigned int p) {
    // On plafonne le prix à 3 pierres maximum
    if (p > 3) prix = 3;
    else prix = p;
}

void Tuile::tourner() {
    // Modifie l'ordre interne des hexagones dans le vecteur pour simuler une rotation.

    if (hexagones.size() != 3) return; // Rotation interdite à la tuile de départ

    // On récupère des itérateurs pour manipuler directement les éléments du vecteur
    auto it0 = hexagones.begin();
    auto it1 = hexagones.begin() + 1;
    auto it2 = hexagones.begin() + 2;
    if (inversion) {
        // ROTATION SENS 1 (si inversée)
        // Permutation : 0->2, 1->0, 2->1
        Hexagone* temp = *it0;
        Hexagone* temp1 = *it2;
        *it0 = *it1;
        *it1 = temp1;
        *it2 = temp;
    }
    else {
        // ROTATION SENS 2 (Standard)
        // Permutation : 0->1, 1->2, 2->0
        Hexagone* temp = *it0;
        Hexagone* temp1 = *it1;
        *it0 = *it2;
        *it1 = temp;
        *it2 = temp1;
    }

    
}

string& Tuile::getDesign() {
    // La forme de la tuile change si elle est inversée
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

        // Positions exactes (index char) dans la string où insérer le code du quartier
        int positions[3] = { 44, 124, 91 };
        auto it = hexagones.begin();
        for (int k = 0; k < 3; k++, ++it) {
            // On remplace les 2 espaces vides du gabarit par les 2 lettres du type de quartier (sans le niveau z car c'est pour le chantier
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

        // Positions exactes (index char) dans la string où insérer le code du quartier
        int positions[3] = { 50, 130, 83 };
        auto it = hexagones.begin();
        for (int k = 0; k < 3; k++, ++it) {
            // On remplace les 2 espaces vides du gabarit par les 2 lettres du type de quartier (sans le niveau z car c'est pour le chantier
            design.replace(positions[k], 2, (*it)->affiche().substr(0, 2));
        }
    }
    return design;
}

void Tuile::reconstruireHexagone(int index, int typeInt, int etoiles) {

    if (index < 0 || index >= (int)hexagones.size()) return;

    // Au lieu de supprimer et recréer l'objet , on met à jour ses attributs.
    Hexagone* h = hexagones[index];
    
    // On écrase les valeurs aléatoires par les valeurs sauvegardées
    h->type = (TypeQuartier)typeInt;
    h->etoiles = etoiles;
    h->place = (etoiles > 0);

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

bool Tuile::contientPlace() const{
    if (!this) return false;
    for (int i = 0; i < getNbHexagones(); ++i) {
        Hexagone* h = getHexagone((int)i);
        if (h->estPlace()) {
            return true;
        }
    }
    return false;
}
