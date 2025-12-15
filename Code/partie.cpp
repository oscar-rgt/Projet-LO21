#include <algorithm>
#include <cstdio>
#include <random>
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
    for (auto p : piles) delete p;
    piles.clear();
    chantier.vider();
}

void Partie::initialiser(int nb, const vector<string>& noms, TuileCite mode, const array<bool, 5>& vars, unsigned int nivIC) {
    // Reset
    for (auto j : joueurs) delete j;
    joueurs.clear();
    for (auto p : piles) delete p;
    piles.clear();
    chantier.vider();

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
    piles.emplace_back(new Pile(0, static_cast<int>(joueurs.size()) + 2));

    // Autres piles : nbJoueurs + 1 tuiles
    for (int i = 1; i <= nbPiles; i++) {
        piles.emplace_back(new Pile(i, static_cast<int>(joueurs.size()) + 1));
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
        if (indexPileActuelle >= getNbPiles()) return;

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

bool Partie::actionPlacerTuile(int index, int x, int y, int z, int rotation) {
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
    if (j->getPierres() < coutPierre) throw PartieException("Pas assez de pierres !");

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
    catch (const CiteException&) {
        // Annuler la rotation pour remettre la tuile dans l'état initial visuel
        for (int r = 0; r < (3 - rotation) % 3; ++r) t->tourner();
        throw;
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
    return (indexPileActuelle == getNbPiles() && chantier.getNbTuiles() == 1);
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


/*bool Partie::sauvegarder(const string& nomFichier) const {
    ofstream f(nomFichier);
    if (!f.is_open()) return false;

    // A. ETAT GLOBAL
    f << joueurs.size() << endl;
    f << indexJoueurActuel << endl;
    f << indexPileActuelle << endl;

    // B. LE CHANTIER
    f << chantier.getNbTuiles() << endl;
    for (size_t i = 0; i < chantier.getNbTuiles(); i++) {
        Tuile* t = chantier.getTuile(i);
        // On sauvegarde l'ID, l'INVERSION
        f << t->getId() << " " << t->getInversion() << endl;

        // On sauvegarde le contenu visuel exact des 3 hexagones
        for(int k=0; k<3; k++) {
            f << (int)t->getHexagone(k)->getType() << " " << t->getHexagone(k)->getEtoiles() << " ";
        }
        f << endl;
    }

    // C. LES JOUEURS ET LEURS CITÉS
    for (Joueur* j : joueurs) {
        f << j->getNom() << endl;
        f << j->getPoints() << endl;
        f << j->getPierres() << endl;

        // On écrit l'historique des coups pour rejouer la partie
        const auto& hist = j->getCite()->getHistorique();
        f << hist.size() << endl;

        for (const auto& a : hist) {
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
    if (!f.is_open()) throw SauvegardeException("Fichier introuvable ou impossible a ouvrir.");

    // A. NETTOYAGE COMPLET (Reset)
    for (auto j : joueurs) delete j;
    joueurs.clear();
    chantier.vider();
    for(auto p : piles) delete p;
    piles.clear();

    // B. LECTURE GLOBAL
    int nbJ, idxJ, idxP;
    if (!(f >> nbJ >> idxJ >> idxP)) throw SauvegardeException("Format de fichier invalide (En-tete corrompu).");

    joueurs.size() = nbJ;
    indexJoueurActuel = idxJ;
    indexPileActuelle = idxP;

    // C. GÉNÉRATION DU FUTUR (Nouvelles piles aléatoires)
    initialiserPiles();
    chantier.vider();
    indexPileActuelle = idxP;

    // D. RESTAURATION DU CHANTIER (Passé immédiat)
    int nbTuilesChantier;
    f >> nbTuilesChantier;
    for (int i = 0; i < nbTuilesChantier; i++) {
        int id, prix;
        bool inv;
        f >> id >> prix >> inv;

        // Nouvelle tuile (générée aléatoirement via new Tuile)
        Tuile* t = new Tuile(id);
        t->setPrix(prix);
        if (inv) t->inverser();

        // On force son apparence pour qu'elle soit identique à la sauvegarde
        for(int k=0; k<3; k++) {
            int type, etoiles;
            f >> type >> etoiles;
            t->reconstruireHexagone(k, type, etoiles);
        }

        // On l'ajoute au chantier
        chantier.ajouterTuileSpecifique(t);
    }

    // E. RESTAURATION DES JOUEURS (Passé lointain)
    for (int i = 0; i < joueurs.size(); i++) {
        string nom;
        int pts, pierres;
        f >> nom >> pts >> pierres;

        // Note : ici on crée un Joueur humain par défaut.
        // Si tu as des IA, il faudrait sauvegarder le type ("IA" ou "Humain") et faire un if/else.
        Joueur* j = new Joueur(nom);
        j->setPoints(pts);
        j->utiliserPierres(j->getPierres()); // Reset pierres par défaut
        j->ajouterPierres(pierres);

        int nbActions;
        f >> nbActions;

        // REPLAY : On rejoue tous les coups
        for (int k = 0; k < nbActions; k++) {
            Action a;
            f >> a.tuileId >> a.x >> a.y >> a.z >> a.inversion;

            Tuile* t = new Tuile(a.tuileId);
            if (a.inversion) t->inverser();

            for(int h=0; h<3; h++) {
                int type, etoiles;
                f >> type >> etoiles;
                t->reconstruireHexagone(h, type, etoiles);
            }

            try {
                // Le moteur Cite refait les calculs de voisins et remplit la map
                j->getCite()->placer(t, {a.x, a.y, a.z});
            } catch (...) {
                // Ne devrait pas arriver sur une save valide
            }
        }
        joueurs.push_back(j);
    }

    return true;
}

bool Partie::supprimerSauvegarde(const string& nomFichier) {
    return std::remove(nomFichier.c_str()) == 0;
}
// À la fin du fichier Code/partie.cpp

bool Partie::sauvegardeExiste(const string& nomFichier) {
    ifstream f(nomFichier);
    return f.good();
}
*/
