#include <algorithm>
#include <fstream>
#include <cstdio>
#include <random>
#include <iostream>
#include <map> 
#include "partie.h"
#include "ia.h"

using namespace std;

Partie& Partie::getInstance() {
    static Partie instance;
    return instance;
}

// CORRECTION 1 : Suppression de nbJoueurs(0)
Partie::Partie() : 
    indexJoueurActuel(0), 
    indexPileActuelle(0),
    modeTuileCite(TuileCite::STANDARD), 
    niveauIllustreConstructeur(0)       
{
    variantes.fill(false);
}

Partie::~Partie() {
    for (auto j : joueurs) delete j;
    joueurs.clear();
    for (auto p : piles) delete p; 
    piles.clear();
    chantier.vider();
}

// Nouvelle méthode indispensable pour le chargement
void Partie::resetPourChargement() {
<<<<<<< Updated upstream
    // 1. SUPPRIMER LES PILES EXISTANTES
    for (size_t i = 0; i < piles.size(); ++i) {
        Pile::detruire(piles[i]); // ou delete piles[i];
    }
    piles.clear(); // Le vecteur redevient vide ! (Taille 0)

    // 2. VIDER LE CHANTIER
    chantier.vider();
=======
    // 1. Nettoyer les joueurs
    for (auto j : joueurs) delete j;
    joueurs.clear();
    
    // 2. Vider le chantier (sans détruire les tuiles, elles sont dans les piles)
    chantier.vider();
    
    // 3. IMPORTANT : Détruire ET vider les piles existantes
    for (auto p : piles) delete p;
    piles.clear();
    
    // 4. Réinitialiser les index
    indexJoueurActuel = 0;
    indexPileActuelle = 0;
    niveauIllustreConstructeur = 0;
    
    // NE PAS appeler initialiserPiles() ici !
    // Les piles seront reconstruites manuellement dans charger()
}

void Partie::initialiser(int nb, const vector<string>& noms, TuileCite mode, const array<bool, 5>& vars, unsigned int nivIC) {
    // Nettoyage complet avant nouvelle partie
    resetPourChargement();
>>>>>>> Stashed changes

    // 3. SUPPRIMER LES JOUEURS
    for (size_t i = 0; i < joueurs.size(); ++i) delete joueurs[i];
    joueurs.clear();

    // 4. RESET INDEX
    indexJoueurActuel = 0;
    indexPileActuelle = 0;
}

void Partie::initialiser(int nb, const vector<string>& noms, TuileCite mode, const array<bool, 5>& vars, unsigned int nivIC) {
    // Nettoyage complet avant nouvelle partie
    resetPourChargement();

    modeTuileCite = mode;
    variantes = vars;
    
    // CORRECTION 2 : Renommage de niveauIA -> niveauIllustreConstructeur
    niveauIllustreConstructeur = nivIC;

    // Création des joueurs
    for (const auto& nom : noms) {
        joueurs.push_back(new Joueur(nom));
    }

    // Gestion Mode Solo
    if (nb == 1) {
        // Le joueur humain commence avec 1 pierre (il en a 2 par défaut, on retire 1)
        if (!joueurs.empty()) joueurs[0]->utiliserPierres(1);

        // L'IA est le 2ème joueur
        joueurs.push_back(new IA("Illustre Architecte", niveauIllustreConstructeur));
    }

    initialiserPiles();

    if (niveauIllustreConstructeur > 0) {
        indexJoueurActuel = 0; // Humain commence toujours en solo
    }
    else {
        designerArchitecteChef();
    }
}

void Partie::initialiserPiles() {
<<<<<<< Updated upstream
    int nbPiles = 11; // Standard
    if (modeTuileCite == TuileCite::AUGMENTE) {
        // Note : on utilise joueurs.size() car nbJoueurs n'existe plus en variable
=======
    int nbPiles = 11; // Standard par défaut
    
    if (modeTuileCite == TuileCite::AUGMENTE) {
>>>>>>> Stashed changes
        size_t n = joueurs.size();
        if (n == 2) nbPiles = 19;
        else if (n == 3) nbPiles = 15;
        else if (n == 4) nbPiles = 11;
    }

    piles.reserve(nbPiles); 

    // ========================================
    // CORRECTION : Pile de départ = nbJoueurs + 2 tuiles
    // (Pas +1 comme les autres !)
    // ========================================
    int nbJoueursReel = joueurs.size();
    piles.emplace_back(new Pile(0, nbJoueursReel + 2));

    // Autres piles : nbJoueurs + 1 tuiles
    for (int i = 1; i < nbPiles; i++) {
        piles.emplace_back(new Pile(i, nbJoueursReel + 1));
    }
    
    // ========================================
    // CORRECTION : On commence à la pile 0 (pas 1 !)
    // ========================================
    indexPileActuelle = 0;

    remplirChantier();

}
void Partie::remplirChantier() {
    if (estFinDePartie()) return;

    // Le chantier est rempli uniquement quand il est vide OU qu'il ne reste qu'1 tuile
    if (chantier.estVide() || chantier.getNbTuiles() == 1) {
        if (indexPileActuelle >= piles.size()) return; // Plus de piles

        // ========================================
        // CORRECTION : On utilise indexPileActuelle directement
        // (pas indexPileActuelle - 1)
        // ========================================
        Pile* p = piles[indexPileActuelle];
        chantier.ajouterPile(*p);
        indexPileActuelle++; // On passe à la pile suivante
    }

<<<<<<< Updated upstream
    // Mise à jour des prix : 0, 1, 2...
=======
    // Mise à jour des prix : 0, 1, 2, 3...
>>>>>>> Stashed changes
    int prix = 0;
    for (auto it = chantier.begin(); it != chantier.end(); ++it) {
        (*it)->setPrix(prix++);
    }
}

void Partie::designerArchitecteChef() {
    if (joueurs.empty()) return;
    random_device rd;
    mt19937 gen(rd());
    shuffle(joueurs.begin(), joueurs.end(), gen);
    indexJoueurActuel = 0;
}

bool Partie::actionPlacerTuile(int index, int x, int y, int z, int rotation, int inversion) {
    // 1. Récupérer la tuile depuis le chantier
    auto itTuile = chantier.begin();
    for (int i = 0; i < index; ++i) {
        ++itTuile;
        if (itTuile == chantier.end()) return false;
    }
    Tuile* t = *itTuile;

    Joueur* j = getJoueurActuel();
    if (!j) return false;

    // 2. Vérifier le coût (Pierres)
    int coutPierre = index; // Le coût dépend de la position dans le chantier (0, 1, 2...)
    if (j->getPierres() < coutPierre) {
        return false; 
    }

    // 3. Appliquer les transformations temporaires sur la tuile
    if (inversion) t->inverser();
    for(int r = 0; r < rotation; r++) t->tourner();

    try {
        // 4. Tenter de placer (On passe la rotation !)
        j->getCite()->placer(t, { x, y, z }, j, rotation); 
        
        // Si aucune exception n'est levée, le placement est valide :

        // Mise à jour visuelle (Quadrillage)
        j->getCite()->agrandirQ('S'); 

        // Paiement des pierres
        j->utiliserPierres(coutPierre);

        // Règle : Si on paie, les pierres vont sur les tuiles sautées (sauf si Variante Architecte/Solo)
        if (getNiveauIllustreConstructeur() > 0 && dynamic_cast<IA*>(j) == nullptr) {
            // En solo, les pierres payées vont à l'IA (simplification courante ou règle spécifique)
            for (auto* joueur : joueurs) {
                if (dynamic_cast<IA*>(joueur)) {
                    joueur->ajouterPierres(coutPierre);
                    break;
                }
            }
        }
        else {
            // En multi, on pose les pierres sur les tuiles qu'on n'a pas prises
            auto itPaiement = chantier.begin();
            for (int k = 0; k < index; ++k) {
                (*itPaiement)->setPrix((*itPaiement)->getPrix() + 1);
                ++itPaiement;
            }
        }

        // Calcul du score (mise à jour temps réel)
        j->getScore()->calculerScore();

        // 5. Nettoyage et fin de tour
        chantier.retirerTuile(t); 

        if (!estFinDePartie()) {
            remplirChantier();
        }
        passerAuJoueurSuivant();
        
        return true;
    }
    catch (const CiteException&) {
        // Si le placement échoue, on remet la tuile dans son état d'origine
        // C'est important pour l'affichage suivant dans le chantier
        for (int r = 0; r < (3 - rotation) % 3; ++r) t->tourner();
        if (inversion) t->inverser();
        return false;
    }
}

int Partie::jouerTourIA() {
    Joueur* j = getJoueurActuel();
    IA* ia = dynamic_cast<IA*>(j);
    if (!ia) return -1;

    int indexChoisi = ia->choisirTuile(chantier);

    auto it = chantier.begin();
    for (int k = 0; k < indexChoisi; ++k) ++it;
    Tuile* t = *it;

    if (indexChoisi > 0) {
        ia->utiliserPierres(indexChoisi);
        auto itP = chantier.begin();
        for (int i = 0; i < indexChoisi; i++) {
            (*itP)->setPrix((*itP)->getPrix() + 1);
            ++itP;
        }
    }

    ia->ajouterPierres(t->getPrix());
    ia->ajouterTuile(t); // L'IA stocke ses tuiles
    chantier.retirerTuile(t);

    if (!estFinDePartie()) {
        remplirChantier();
    }
    passerAuJoueurSuivant();

    return indexChoisi;
}

void Partie::passerAuJoueurSuivant() {
    if (joueurs.empty()) return;
    indexJoueurActuel = (indexJoueurActuel + 1) % joueurs.size();
}

bool Partie::estFinDePartie() const {
    return (indexPileActuelle == piles.size() && chantier.getNbTuiles() == 1);
}

Joueur* Partie::getJoueurActuel() const {
    if (joueurs.empty()) throw PartieException("Aucun joueur.");
    return joueurs[indexJoueurActuel];
}

// CORRECTION : getJoueur(i) n'existe plus dans .h, mais on peut utiliser getJoueurs()[i]
// Cette méthode helper peut être gardée pour simplifier l'accès
Joueur* Partie::getJoueur(int index) const {
    if (index < 0 || index >= static_cast<int>(joueurs.size())) return nullptr;
    return joueurs[index];
}

vector<Joueur*> Partie::determinerGagnants() {
    int maxScore = -1;
    map<Joueur*, int> memoScores;

    for (Joueur* j : joueurs) {
        int s = 0;
        if (IA* ia = dynamic_cast<IA*>(j)) {
            s = ia->calculerScoreIA();
        }
        else {
            j->getScore()->calculerScore(); 
            s = j->getScore()->getTotal();
        }
        memoScores[j] = s;
        if (s > maxScore) maxScore = s;
    }

    vector<Joueur*> candidats;
    for (Joueur* j : joueurs) {
        if (memoScores[j] == maxScore) {
            candidats.push_back(j);
        }
    }

    if (candidats.size() > 1) {
        int maxPierres = -1;
        for (Joueur* j : candidats) {
            if (j->getPierres() > maxPierres) maxPierres = j->getPierres();
        }
        vector<Joueur*> gagnantsFinaux;
        for (Joueur* j : candidats) {
            if (j->getPierres() == maxPierres) gagnantsFinaux.push_back(j);
        }
        return gagnantsFinaux;
    }

    return candidats;
}
