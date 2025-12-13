#include <algorithm>
#include <fstream>
#include <cstdio>
#include <random>
#include <iostream>
#include <map> 
#include "partie.h"
#include "ia.h"

using namespace std;

<<<<<<< Updated upstream




=======
>>>>>>> Stashed changes
Partie& Partie::getInstance() {
    static Partie instance;
    return instance;
}

<<<<<<< Updated upstream
Partie::Partie() : 
    indexJoueurActuel(0), 
    nbJoueurs(0), 
    indexPileActuelle(0),
    modeTuileCite(TuileCite::STANDARD), // <-- IMPORTANT : Initialisation par défaut
    niveauIllustreConstructeur(0)       // <-- IMPORTANT
{
    // On initialise les variantes à false par défaut
=======
// CORRECTION 1 : Suppression de nbJoueurs(0)
Partie::Partie() : 
    indexJoueurActuel(0), 
    indexPileActuelle(0),
    modeTuileCite(TuileCite::STANDARD), 
    niveauIllustreConstructeur(0)       
{
>>>>>>> Stashed changes
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
    for (auto j : joueurs) delete j;
    joueurs.clear();
    chantier.vider();
<<<<<<< Updated upstream
    piles.clear();
=======
    indexJoueurActuel = 0;
    indexPileActuelle = 0;
    niveauIllustreConstructeur = 0;
}

void Partie::initialiser(int nb, const vector<string>& noms, TuileCite mode, const array<bool, 5>& vars, unsigned int nivIC) {
    // Nettoyage complet avant nouvelle partie
    resetPourChargement();
>>>>>>> Stashed changes

    nbJoueurs = nb;
    modeTuileCite = mode;
    variantes = vars;
<<<<<<< Updated upstream
    niveauIllustreConstructeur = nivIC;

    for (const auto& nom : noms) joueurs.push_back(new Joueur(nom));


    // Si mode solo, on ajoute l'IA
    if (nbJoueurs == 1) {
        // L'IA est toujours le 2ème "joueur" dans la liste
        joueurs.push_back(new IA("Illustre Architecte", niveauIllustreConstructeur));
        nbJoueurs = 2; // On considère qu'il y a 2 joueurs pour la logique de tour

        // Configuration initiale spécifique Solo
        // Joueur : 1 tuile départ, 1 pierre, Pion Architecte
        // IA : 1 tuile départ, 2 pierres

        // Le joueur humain est à l'index 0
        joueurs[0]->utiliserPierres(1); // Il commence avec 2 par défaut, on en enlève 1 -> reste 1

        // L'IA (index 1) a déjà 2 pierres par défaut.
=======
    
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
>>>>>>> Stashed changes
    }

    initialiserPiles();

    if (niveauIllustreConstructeur > 0) {
<<<<<<< Updated upstream
        // En mode solo, le joueur humain est toujours premier joueur
        indexJoueurActuel = 0;
=======
        indexJoueurActuel = 0; // Humain commence toujours en solo
>>>>>>> Stashed changes
    }
    else {
        designerArchitecteChef();
    }
}

void Partie::initialiserPiles() {
    int nbPiles = 11; // Standard
    if (modeTuileCite == TuileCite::AUGMENTE) {
<<<<<<< Updated upstream
        if (nbJoueurs == 2) nbPiles = 19;
        else if (nbJoueurs == 3) nbPiles = 15;
        else if (nbJoueurs == 4) nbPiles = 11;
=======
        // Note : on utilise joueurs.size() car nbJoueurs n'existe plus en variable
        size_t n = joueurs.size();
        if (n == 2) nbPiles = 19;
        else if (n == 3) nbPiles = 15;
        else if (n == 4) nbPiles = 11;
>>>>>>> Stashed changes
    }

    piles.reserve(nbPiles); 

    // Pile de départ : nbJoueurs + 2 tuiles
    piles.emplace_back(new Pile(0, nbJoueurs + 2));

    // Autres piles : nbJoueurs + 1 tuiles
    for (int i = 1; i <= nbPiles; i++) {
        piles.emplace_back(new Pile(i, nbJoueurs + 1));
    }
    indexPileActuelle = 1;

    remplirChantier();
}

void Partie::remplirChantier() {
<<<<<<< Updated upstream
    // Règle : Le chantier est rempli avec une pile entière uniquement lorsqu'il ne reste plus qu'une tuile dedans.
    // Exception : Au début, le chantier est vide, donc on le remplit.

	if (estFinDePartie()) return;
=======
    if (estFinDePartie()) return;
>>>>>>> Stashed changes

    if (chantier.estVide() || chantier.getNbTuiles() == 1) {
        if (indexPileActuelle >= piles.size()) return; // Plus de piles

        Pile* p = piles[indexPileActuelle-1];
        chantier.ajouterPile(*p);
        indexPileActuelle++;
    }

<<<<<<< Updated upstream
    for (int i = 0; i < chantier.getNbTuiles(); i++) {
        chantier.getTuile(i)->setPrix(i);
=======
    // Mise à jour des prix : 0, 1, 2...
    int prix = 0;
    for (auto it = chantier.begin(); it != chantier.end(); ++it) {
        (*it)->setPrix(prix++);
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream
    if (index < 0 || index >= chantier.getNbTuiles()) return false;
=======
    // 1. Récupérer la tuile depuis le chantier
    auto itTuile = chantier.begin();
    for (int i = 0; i < index; ++i) {
        ++itTuile;
        if (itTuile == chantier.end()) return false;
    }
    Tuile* t = *itTuile;

>>>>>>> Stashed changes
    Joueur* j = getJoueurActuel();
    if (!j) return false;
    cout << "Est fin partie ?" << (indexPileActuelle >= piles.size()) << " et " << (chantier.getNbTuiles() <= 1) << endl;

<<<<<<< Updated upstream
    // 1. Coût en pierres
    // Coût = index (0 pour la 1ère, 1 pour la 2ème, etc.)
    int coutPierre = index;
    if (j->getPierres() < coutPierre) {
        return false;
    }

    // 2. Vérifier le placement
    Tuile* t = chantier.getTuile(index);
    if (!t) return false;

    //Appliquer l'inversion si demandée
    if (inversion) {
        t->inverser();
    }

    // Appliquer la rotation sur la tuile du chantier (temporairement)
    //for(int r=0; r<rotation; r++) t->tourner();

    try {
        j->getCite()->placer(t, { x, y, z }, j);
        ////////////////////////////////////// TEST POUR AGRANDIR LE Q/////////////////////////////////////////
       j->getCite()->agrandirQ('S');
        ////////////////////////////////////// TEST POUR AGRANDIR LE Q/////////////////////////////////////////
=======
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
>>>>>>> Stashed changes

        // 3. Paiement des pierres
        j->utiliserPierres(coutPierre);
        j->getScore()->calculerScore();

<<<<<<< Updated upstream
        // Gestion spécifique Solo : Si humain paie, pierres vont à l'IA
        if (niveauIllustreConstructeur > 0 && dynamic_cast<IA*>(j) == nullptr) {
            // Trouver l'IA (c'est l'autre joueur)
=======
        // Règle : Si on paie, les pierres vont sur les tuiles sautées (sauf si Variante Architecte/Solo)
        if (getNiveauIllustreConstructeur() > 0 && dynamic_cast<IA*>(j) == nullptr) {
            // En solo, les pierres payées vont à l'IA (simplification courante ou règle spécifique)
>>>>>>> Stashed changes
            for (auto* joueur : joueurs) {
                if (dynamic_cast<IA*>(joueur)) {
                    joueur->ajouterPierres(coutPierre);
                    break;
                }
            }
        }
        else {
<<<<<<< Updated upstream
            // Mode normal ou tour de l'IA

            // On dépose 1 pierre sur chaque tuile qu'on a sauté (indices 0 à index-1)
            for (int i = 0; i < index; i++) {
                chantier.getTuile(i)->setPrix(chantier.getTuile(i)->getPrix() + 1);
            }
        }


        // 6. Mise à jour du chantier
        chantier.retirerTuile(index);

        // 7. Remplir le chantier (si nécessaire)
        if (!estFinDePartie()) {
            remplirChantier();
        }

        // 8. Passer au joueur suivant
=======
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
>>>>>>> Stashed changes
        passerAuJoueurSuivant();
        
        return true;
    }
<<<<<<< Updated upstream
    catch (const CiteException& e) {
        // Annuler la rotation pour remettre la tuile dans l'état initial visuel
        for (int r = 0; r < (3 - rotation) % 3; ++r) t->tourner();
=======
    catch (const CiteException&) {
        // Si le placement échoue, on remet la tuile dans son état d'origine
        // C'est important pour l'affichage suivant dans le chantier
        for (int r = 0; r < (3 - rotation) % 3; ++r) t->tourner();
        if (inversion) t->inverser();
>>>>>>> Stashed changes
        return false;
    }
}

int Partie::jouerTourIA() {
    Joueur* j = getJoueurActuel();
    IA* ia = dynamic_cast<IA*>(j);
    if (!ia) return -1;



    int indexChoisi = ia->choisirTuile(chantier);
    Tuile* t = chantier.getTuile(indexChoisi);


    if (indexChoisi > 0) {
        ia->utiliserPierres(indexChoisi);
        for (int i = 0; i < indexChoisi; i++) {
            chantier.getTuile(i)->setPrix(chantier.getTuile(i)->getPrix() + 1);
        }
    }

<<<<<<< Updated upstream
    // Récupération des pierres sur la tuile choisie
    int pierresGagnees = t->getPrix();
    ia->ajouterPierres(pierresGagnees);

    ia->ajouterTuile(t);
    chantier.retirerTuile(indexChoisi);


=======
    ia->ajouterPierres(t->getPrix());
    ia->ajouterTuile(t); // L'IA stocke ses tuiles
    chantier.retirerTuile(t);

>>>>>>> Stashed changes
    if (!estFinDePartie()) {
        remplirChantier();
    }
    passerAuJoueurSuivant();

    return indexChoisi;
}

void Partie::passerAuJoueurSuivant() {
<<<<<<< Updated upstream
    indexJoueurActuel = (indexJoueurActuel + 1) % nbJoueurs;
=======
    if (joueurs.empty()) return;
    indexJoueurActuel = (indexJoueurActuel + 1) % joueurs.size();
>>>>>>> Stashed changes
}

bool Partie::estFinDePartie() const {
    return (indexPileActuelle == piles.size() && chantier.getNbTuiles() == 1);
}

Joueur* Partie::getJoueurActuel() const {
    if (joueurs.empty()) throw PartieException("Aucun joueur.");
    return joueurs[indexJoueurActuel];
}

<<<<<<< Updated upstream
Joueur* Partie::getJoueur(int index) const {
    if (index < 0 || index >= joueurs.size()) return nullptr;
    return joueurs[index];
}



=======
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
>>>>>>> Stashed changes
