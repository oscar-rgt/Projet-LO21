#include <algorithm>
#include <random>
#include <iostream>
#include "partie.h"

using namespace std;

Partie& Partie::getInstance() {
    static Partie instance;
    return instance;
}

Partie::Partie() : indexJoueurActuel(0), nbJoueurs(0), indexPileActuelle(0) {}

Partie::~Partie() {
    for (auto j : joueurs) delete j;
    joueurs.clear();
    chantier.vider();
}

void Partie::initialiser(int nb, const vector<string>& noms, TuileCite mode, const array<bool, 5>& vars, unsigned int nivIC) {
    // Reset
    for (auto j : joueurs) delete j;
    joueurs.clear();
    chantier.vider();
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

    piles.reserve(nbPiles + 1); // +1 pour la pile de départ

    // Pile de départ : nbJoueurs + 2 tuiles
    piles.emplace_back(new Pile(0, nbJoueurs + 2));

    // Autres piles : nbJoueurs + 1 tuiles
    for (int i = 1; i <= nbPiles; ++i) {
        piles.emplace_back(new Pile(i, nbJoueurs + 1)); 
    }
    indexPileActuelle = 0;
    
    // Remplissage initial du chantier avec la première pile
    remplirChantier();
}

void Partie::remplirChantier() {
    // Règle : Le chantier est rempli avec une pile entière uniquement lorsqu'il ne reste plus qu'une tuile dedans.
    // Exception : Au début, le chantier est vide, donc on le remplit.
    
    if (chantier.estVide() || chantier.getNbTuiles() == 1) {
        if (indexPileActuelle >= piles.size()) return; // Plus de piles

        Pile* p = piles[indexPileActuelle];
        chantier.ajouterPile(*p);
        indexPileActuelle++;
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
    if (index < 0 || index >= chantier.getNbTuiles()) return false;
    Joueur* j = getJoueurActuel();
    if (!j) return false;

    // 1. Coût en pierres
    // Coût = index (0 pour la 1ère, 1 pour la 2ème, etc.)
    int coutPierre = index;
    if (j->getPierres() < coutPierre) {
        return false;
    }

    // 2. Vérifier le placement 
    Tuile* t = chantier.getTuile(index);
    if (!t) return false;
    
    // Appliquer la rotation sur la tuile du chantier (temporairement)
    for(int r=0; r<rotation; ++r) t->tourner();

    try {
        j->getCite()->placer(t, {x, y, z}); 
        
        // 3. Paiement des pierres
        j->utiliserPierres(coutPierre);
        // On dépose 1 pierre sur chaque tuile qu'on a sauté (indices 0 à index-1)
        for(int i=0; i<index; ++i) {
            chantier.ajouterPierre(i);
        }

        // 4. Récupération des pierres sur la tuile choisie
        int pierresGagnees = chantier.ramasserPierres(index);
        j->ajouterPierres(pierresGagnees);

        // 6. Mise à jour du chantier
        chantier.retirerTuile(index);

        // 7. Remplir le chantier (si nécessaire)
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
    // Lorsque le chantier est vide, la partie s'arrête.
    return chantier.estVide();
}

Joueur* Partie::getJoueurActuel() const {
    if (joueurs.empty()) return nullptr;
    return joueurs[indexJoueurActuel];
}

Joueur* Partie::getJoueur(int index) const {
    if (index < 0 || index >= joueurs.size()) return nullptr;
    return joueurs[index];
}