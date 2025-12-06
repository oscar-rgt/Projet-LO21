#include <algorithm>
#include <fstream>
#include <cstdio>
#include <random>
#include <iostream>
#include "partie.h"
#include "ia.h"

using namespace std;


Partie& Partie::getInstance() {
    static Partie instance;
    return instance;
}

Partie::Partie() : indexJoueurActuel(0), indexPileActuelle(0) {}

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

    modeTuileCite = mode;
    variantes = vars;
    niveauIA = nivIC;

    if (nb > 1) for (const auto& nom : noms) joueurs.push_back(new Joueur(nom));


    // Si mode solo, on ajoute l'IA
    if (nb == 1) {

        joueurs.push_back(new Joueur(noms[0], 1)); // Il commence avec 2 par défaut, on en enlève 1 -> reste 1

        // L'IA est toujours le 2ème "joueur" dans la liste
        joueurs.push_back(new IA("Illustre Architecte", niveauIA));

        // Configuration initiale spécifique Solo
        // Joueur : 1 tuile départ, 1 pierre, Pion Architecte
        // IA : 1 tuile départ, 2 pierres

        // Le joueur humain est à l'index 0

        // L'IA (index 1) a déjà 2 pierres par défaut.
    }

    initialiserPiles();

    if (niveauIA > 0) {
        // En mode solo, le joueur humain est toujours premier joueur
        indexJoueurActuel = 0;
    }
    else {
        designerArchitecteChef();
    }
}

void Partie::initialiserPiles() {
    // Création des piles selon les règles
    int nbPiles = 11; // Standard
    if (modeTuileCite == TuileCite::AUGMENTE) {
        if (joueurs.size() == 2) nbPiles = 19;
        else if (joueurs.size() == 3) nbPiles = 15;
        else if (joueurs.size() == 4) nbPiles = 11;
    }

    piles.reserve(nbPiles); 

    // Pile de départ : nbJoueurs + 2 tuiles
    piles.emplace_back(new Pile(0, joueurs.size() + 2));

    // Autres piles : nbJoueurs + 1 tuiles
    for (int i = 1; i <= nbPiles; i++) {
        piles.emplace_back(new Pile(i, joueurs.size() + 1));
    }
    indexPileActuelle = 1;

    // Remplissage initial du chantier avec la première pile
    remplirChantier();
}

void Partie::remplirChantier() {
    // Règle : Le chantier est rempli avec une pile entière uniquement lorsqu'il ne reste plus qu'une tuile dedans.
    // Exception : Au début, le chantier est vide, donc on le remplit.

    if (estFinDePartie()) return;

    if (chantier.estVide() || chantier.getNbTuiles() == 1) {
        if (indexPileActuelle >= piles.size()) return;

        Pile* p = piles[indexPileActuelle - 1];
        chantier.ajouterPile(*p);
        indexPileActuelle++;
    }

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
    // Récupération via itérateur
    auto itTuile = chantier.begin();
    for (int i = 0; i < index; ++i) {
        ++itTuile;
        if (itTuile == chantier.end()) return false;
    }
    Tuile* t = *itTuile;

    Joueur* j = getJoueurActuel();
    if (!j) return false;

    // 1. Coût en pierres
    // Coût = index (0 pour la 1ère, 1 pour la 2ème, etc.)
    int coutPierre = index;
    if (j->getPierres() < coutPierre) return false;

    try {
        j->getCite()->placer(t, { x, y, z }, j);

        // Paiement des pierres
        j->utiliserPierres(coutPierre);

        // --- LOGIQUE SPÉCIFIQUE SOLO ---
        // Si c'est un humain qui joue en mode solo (contre l'IA), les pierres vont à l'IA.
        if (niveauIA > 0 && dynamic_cast<IA*>(j) == nullptr) {
            // On cherche l'IA parmi les joueurs avec notre nouvel ITÉRATEUR
            for (auto itJ = debutJoueurs(); itJ != finJoueurs(); ++itJ) {
                if (dynamic_cast<IA*>(*itJ)) {
                    (*itJ)->ajouterPierres(coutPierre);
                    break;
                }
            }
        }
        else {
            // Mode normal ou tour de l'IA : on dépose les pierres sur les tuiles sautées
            auto itPaiement = chantier.begin();
            for (int k = 0; k < index; ++k) {
                (*itPaiement)->setPrix((*itPaiement)->getPrix() + 1);
                ++itPaiement;
            }
        }

        // Calcul du score
        j->getScore()->calculerScore();

        // Retrait de la tuile du chantier
        chantier.retirerTuile(t);

        if (!estFinDePartie()) remplirChantier();
        passerAuJoueurSuivant();
        return true;

    }
    catch (const CiteException& e) {
        // Annuler la rotation pour remettre la tuile dans l'état initial visuel
        for (int r = 0; r < (3 - rotation) % 3; ++r) t->tourner();
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
    ia->ajouterTuile(t);
    chantier.retirerTuile(t);

    if (!estFinDePartie()) remplirChantier();
    passerAuJoueurSuivant();

    return indexChoisi;
}

void Partie::passerAuJoueurSuivant() {
    indexJoueurActuel = (indexJoueurActuel + 1) % joueurs.size();
}

bool Partie::estFinDePartie() const {
    return (indexPileActuelle == piles.size() && chantier.getNbTuiles() == 1);
}

Joueur* Partie::getJoueurActuel() const {
    if (joueurs.empty() || !joueurs[indexJoueurActuel]) throw PartieException("Joueur inexistant.");
    return joueurs[indexJoueurActuel];
}



vector<Joueur*> Partie::determinerGagnants() {
    int maxScore = -1;
    // On utilise une map pour stocker les scores calculés
    map<Joueur*, int> memoScores;

    // 1. Calcul des scores
    for (auto it = debutJoueurs(); it != finJoueurs(); ++it) {
        Joueur* j = *it;
        int s = 0;

        if (IA* ia = dynamic_cast<IA*>(j)) {
            s = ia->calculerScoreIA();
        }
        else {
            j->getScore()->calculerScore(); 
            s = j->getScore()->getTotal();
        }

        memoScores[j] = s; // Sauvegarde du score pour l'étape suivante

        if (s > maxScore) {
            maxScore = s;
        }
    }

    // 2. Chercher égalités (Candidats)
    vector<Joueur*> candidats;
    for (auto it = debutJoueurs(); it != finJoueurs(); ++it) {
        Joueur* j = *it;
        if (memoScores[j] == maxScore) {
            candidats.push_back(j);
        }
    }

    // 3. Départage en fonction des pierres cf regles
    if (candidats.size() > 1) {
        int maxPierres = -1;
        // Trouver le max de pierres parmi les candidats
        for (Joueur* j : candidats) {
            if (j->getPierres() > maxPierres) {
                maxPierres = j->getPierres();
            }
        }

        // Garder ceux qui ont ce max de pierres
        vector<Joueur*> gagnantsFinaux;
        for (Joueur* j : candidats) {
            if (j->getPierres() == maxPierres) {
                gagnantsFinaux.push_back(j);
            }
        }
        return gagnantsFinaux;
    }

    return candidats;
}


bool Partie::sauvegarder(const string& nomFichier) const {
    ofstream f(nomFichier);
    if (!f.is_open()) return false;

    // A. ETAT GLOBAL
    f << nbJoueurs << endl;
    f << indexJoueurActuel << endl;
    f << indexPileActuelle << endl;

    // B. LE CHANTIER
    f << chantier.getNbTuiles() << endl;
    for (size_t i = 0; i < chantier.getNbTuiles(); i++) {
        Tuile* t = chantier.getTuile(i);
        // On sauvegarde l'ID, le prix actuel et l'INVERSION
        f << t->getId() << " " << t->getPrix() << " " << t->getInversion() << endl;

        // On sauvegarde le contenu visuel exact des 3 hexagones
        for(int k=0; k<3; k++) {
            f << (int)t->getHexagone(k)->getType() << " " << t->getHexagone(k)->getEtoiles() << " ";
        }
        f << endl;
    }

    // C. LES JOUEURS ET LEURS CITÉS
    for (Joueur* j : joueurs) {
        // CORRECTION CRITIQUE : Remplacer les espaces par des '_'
        // Sinon "Illustre Architecte" fait planter la lecture au chargement
        string nomSauve = j->getNom();
        std::replace(nomSauve.begin(), nomSauve.end(), ' ', '_');

        f << nomSauve << endl;
        // On sauvegarde le score total actuel (juste pour info, il sera recalculé)
        f << j->getScore()->getTotal() << endl; 
        f << j->getPierres() << endl;

        // On écrit l'historique des coups pour rejouer la partie
        const auto& hist = j->getCite()->getHistorique();
        f << hist.size() << endl;

        for (const auto& a : hist) {
            // On sauvegarde l'action complète + l'état visuel de la tuile jouée
            f << a.tuileId << " " << a.x << " " << a.y << " " << a.z << " " << a.inversion << " ";
            for(int k=0; k<3; k++) {
                f << a.hexas[k].type << " " << a.hexas[k].etoiles << " ";
            }
            f << endl;
        }
    }

    return true;
}


bool Partie::charger(const string& nomFichier) {

    ifstream f(nomFichier);
    if (!f.is_open()) {
        return false;
    }

    for (auto j : joueurs) {
        if (j) delete j; // Verification de securité
    }
    joueurs.clear();
    chantier.vider();

    for(auto p : piles) {
        if (p) delete p;
    }
    piles.clear();


    int nbJ, idxJ, idxP;
    // On lit les 3 premières valeurs
    if (!(f >> nbJ >> idxJ >> idxP)) {
        return false;
    }
    
    nbJoueurs = nbJ;
    indexJoueurActuel = idxJ;
    indexPileActuelle = idxP;

    initialiserPiles(); 
    chantier.vider(); 
    
    // On remet le curseur de pile au bon endroit
    indexPileActuelle = idxP;

    // --- RESTAURATION DU CHANTIER ---
    int nbTuilesChantier;
    if (!(f >> nbTuilesChantier)) {
        return false;
    }

    for (int i = 0; i < nbTuilesChantier; i++) {
        int id, prix;
        bool inv;
        
        if (!(f >> id >> prix >> inv)) {
            return false;
        }

        Tuile* t = new Tuile(id, prix); // <--- CA PEUT PLANTER ICI SI LE CONSTRUCTEUR EST MAUVAIS
        if (inv) t->inverser();

        for(int k=0; k<3; k++) {
            int type, etoiles;
            if (!(f >> type >> etoiles)) return false;

            // <--- CA VA SUREMENT PLANTER ICI :
            t->reconstruireHexagone(k, type, etoiles); 
        }
        chantier.ajouterTuileSpecifique(t);
    }

    // --- RESTAURATION DES JOUEURS ---
    for (int i = 0; i < nbJoueurs; i++) {
        string nom;
        int pts, pierres;
        if (!(f >> nom >> pts >> pierres)) {
            return false;
        }

        // On remet les espaces à la place des '_'
        std::replace(nom.begin(), nom.end(), '_', ' ');

        Joueur* j;
        // Detection IA
        if (nbJoueurs == 2 && i == 1) {
             j = new IA(nom, niveauIllustreConstructeur);
        } else {
             j = new Joueur(nom);
        }

        j->ajouterPierres(pierres - j->getPierres()); 

        int nbActions;
        f >> nbActions;

        for (int k = 0; k < nbActions; k++) {
            Action a;
            if (!(f >> a.tuileId >> a.x >> a.y >> a.z >> a.inversion)) {
                 return false;
            }

            Tuile* t = new Tuile(a.tuileId, 0);
            if (a.inversion) t->inverser();

            for(int h=0; h<3; h++) {
                int type, etoiles;
                f >> type >> etoiles;
                t->reconstruireHexagone(h, type, etoiles);
            }

            try {
                j->getCite()->placer(t, {a.x, a.y, a.z}, j);
            } catch (...) {
                // On ignore les erreurs de placement au chargement
            }
        }
        
        j->getScore()->calculerScore();
        joueurs.push_back(j);
    }

    return true;
}

bool Partie::supprimerSauvegarde(const string& nomFichier) {
    return std::remove(nomFichier.c_str()) == 0;
}


bool Partie::sauvegardeExiste(const string& nomFichier) {
    ifstream f(nomFichier);
    return f.good();
}
