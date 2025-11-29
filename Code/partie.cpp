#include "partie.h"
#include <random>
#include <algorithm>
#include <iostream> 
#include "joueur.h" 
#include "pile.h" 
#include "tuiles.h"

using namespace std;

Partie& Partie::getInstance() {
    static Partie instance;
    return instance;
}

Partie::Partie() : indexJoueurActuel(0), nbJoueurs(0), modeTuileCite(TuileCite::STANDARD), niveauIllustreConstructeur(0) {}

Partie::~Partie() {
    // Nettoyage de la mémoire (car on utilise new Joueur)
    for (Joueur* j : joueurs) {
        delete j;
    }
    joueurs.clear();
}

void Partie::initialiser(int nb, const vector<string>& nomsJoueurs,
    TuileCite mode, const array<bool, 5>& variantesActives,
    unsigned int niveauIC)
{
    // 1. Reset de la partie précédente
    for (Joueur* j : joueurs) delete j;
    joueurs.clear();
    piles.clear();

    // 2. Configuration
    nbJoueurs = nb;
    modeTuileCite = mode;
    variantes = variantesActives;
    niveauIllustreConstructeur = niveauIC;

    // 3. Création des joueurs
    for (const string& nom : nomsJoueurs) {
        joueurs.push_back(new Joueur(nom)); 
    }

    // 4. Initialisation du plateau et des piles
    initialiserPiles();
    designerArchitecteChef();
}

void Partie::initialiserPiles() {
    int nbPiles = 0;

    
    if (modeTuileCite == TuileCite::STANDARD) {
        nbPiles = 11;
    }
    else {
        nbPiles = 19;
    }

    piles.reserve(nbPiles);
    for (int i = 0; i < nbPiles; ++i) {
        piles.emplace_back(i, 3); // Taille 3 par défaut 
    }
    pileActuelle = 0;
    tuileActuelle = 0;
    ajouterTuileChantier(piles[pileActuelle].getTuile(tuileActuelle));
    ajouterTuileChantier(piles[pileActuelle].getTuile(++tuileActuelle));
    ajouterTuileChantier(piles[pileActuelle].getTuile(++tuileActuelle));
}

void Partie::designerArchitecteChef() {
    if (joueurs.empty()) return;

    random_device rd;
    mt19937 gen(rd());
    shuffle(joueurs.begin(), joueurs.end(), gen);
    indexJoueurActuel = 0;
}

bool Partie::actionPlacerTuile(int indexTuileChoisie, int x, int y, int z, int rotation) {

    Joueur* joueur = getJoueurActuel();
    if (!joueur) return false;
    
    Tuile& tuile = chantier[indexTuileChoisie]; 
    retirerTuileChantier(tuile);
    if (tuileActuelle == 2) {
        tuileActuelle = 0;
        pileActuelle++;
    }
    ajouterTuileChantier(piles[pileActuelle].getTuile(tuileActuelle++));
    if (joueur->getCite()->estLibre({ x, y, z })) {
        joueur->getCite()->placer(tuile, { x, y, z });
        return true;
    }

    return true;
}

void Partie::passerAuJoueurSuivant() {
    indexJoueurActuel = (indexJoueurActuel + 1) % nbJoueurs;
}

bool Partie::estFinDePartie() const {
    return piles.empty();
}

Joueur* Partie::getJoueurActuel() const {
    if (joueurs.empty()) return nullptr;
    return joueurs[indexJoueurActuel];
}