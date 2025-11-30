#include "partie.h"
#include "joueur.h"
#include "pile.h"
#include "tuiles.h"
#include "placement.h" // Pour CiteException
#include <algorithm>
#include <random>
#include <iostream>

using namespace std;

Partie& Partie::getInstance() {
    static Partie instance;
    return instance;
}

Partie::Partie() : indexJoueurActuel(0), nbJoueurs(0), indexPileActuelle(0) {}

Partie::~Partie() {
    for (auto j : joueurs) delete j;
    joueurs.clear();
    // Les tuiles dans piles sont gérées par Pile, mais celles du chantier doivent être nettoyées si la partie s'arrête brutalement
    for (auto t : chantier) delete t; 
    chantier.clear();
}

void Partie::initialiser(int nb, const vector<string>& noms, TuileCite mode, const array<bool, 5>& vars, unsigned int nivIC) {
    // Reset
    for (auto j : joueurs) delete j;
    joueurs.clear();
    for (auto t : chantier) delete t;
    chantier.clear();
    pierresChantier.clear();
    piles.clear();

    nbJoueurs = nb;
    modeTuileCite = mode;
    variantes = vars;
    niveauIllustreConstructeur = nivIC;

    for (const auto& nom : noms) joueurs.push_back(new Joueur(nom));

    initialiserPiles();
    designerArchitecteChef();
}

void Partie::initialiserPiles() {
    // Création des piles selon les règles
    int nbPiles = 11; // Standard
    if (modeTuileCite == TuileCite::AUGMENTE) {
        if (nbJoueurs == 2) nbPiles = 19;
        else if (nbJoueurs == 3) nbPiles = 15;
        else if (nbJoueurs == 4) nbPiles = 11;
    }

    piles.reserve(nbPiles);
    for (int i = 0; i < nbPiles; ++i) {
        piles.emplace_back(i, nbJoueurs + 1); 
    }
    indexPileActuelle = 0;
    
    // Remplissage initial du chantier
    remplirChantier();
}

void Partie::remplirChantier() {
    int tailleCible = nbJoueurs + 2;
    
    while (chantier.size() < tailleCible) {
        if (indexPileActuelle >= piles.size()) break; //plus de pile

        Pile& p = piles[indexPileActuelle];
        if (p.estVide()) {
            indexPileActuelle++;
            continue;
        }

        // On récupère une tuile de la pile
        try {
			Tuile t = *(p.piocher());
            
            chantier.push_back(new Tuile(t)); // Copie sur le tas
            pierresChantier.push_back(0); // 0 pierre dessus au début
        } catch (PileException& e) {
            indexPileActuelle++; // Pile vide ou erreur
        }
    }
}

void Partie::designerArchitecteChef() {
    if (joueurs.empty()) return;
    random_device rd;
    mt19937 gen(rd());
    shuffle(joueurs.begin(), joueurs.end(), gen);
    indexJoueurActuel = 0;
}

bool Partie::actionPlacerTuile(int index, int x, int y, int z, int rotation) {
    if (index < 0 || index >= chantier.size()) return false;
    Joueur* j = getJoueurActuel();
    if (!j) return false;

    // 1. Coût en pierres
    // Coût = index (0 pour la 1ère, 1 pour la 2ème, etc.)
    int coutPierre = index;
    if (j->getPierres() < coutPierre) {
        cout << "Pas assez de pierres !" << endl;
        return false;
    }

    // 2. Vérifier le placement 
    Tuile* t = chantier[index];
    
    // Appliquer la rotation sur la tuile du chantier (temporairement)
    for(int r=0; r<rotation; ++r) t->tourner();

    try {
        j->getCite()->placer(t, {x, y, z}); 
        
        
        // 3. Paiement des pierres
        j->utiliserPierres(coutPierre);
        // On dépose 1 pierre sur chaque tuile qu'on a sauté (indices 0 à index-1)
        for(int i=0; i<index; ++i) {
            pierresChantier[i]++;
        }

        // 4. Récupération des pierres sur la tuile choisie
        int pierresGagnees = pierresChantier[index];
        j->ajouterPierres(pierresGagnees);

        // 6. Mise à jour du chantier
        chantier.erase(chantier.begin() + index);
        pierresChantier.erase(pierresChantier.begin() + index);

        // 7. Remplir le chantier (glissement automatique + pioche)
        remplirChantier();

        return true;

    } catch (const CiteException& e) {
        // Annuler la rotation pour remettre la tuile dans l'état initial visuel
        for(int r=0; r < (3-rotation)%3; ++r) t->tourner();
        cout << "Erreur placement : " << e.getInfos() << endl;
        return false;
    }
}

void Partie::passerAuJoueurSuivant() {
    indexJoueurActuel = (indexJoueurActuel + 1) % nbJoueurs;
}

bool Partie::estFinDePartie() const {
    // Fin si plus de piles et chantier vide (ou insuffisant)
    return (indexPileActuelle >= piles.size() && chantier.empty());
}

Joueur* Partie::getJoueurActuel() const {
    if (joueurs.empty()) return nullptr;
    return joueurs[indexJoueurActuel];
}