#include "partie.h"
#include "ia.h"
#include "cite.h"
#include "score.h"

using namespace std;

Partie& Partie::getInstance() {
    static Partie instance;
    return instance;
}

Partie::Partie() : indexPileActuelle(0) {}

Partie::~Partie() {
    for (auto p : piles) delete p;
    piles.clear();
    chantier.vider();
    //gestionnaire joueurs gère sa destruction
}

void Partie::resetPourChargement() {
    gestionnaireJoueurs.vider();
    chantier.vider();
    for (auto p : piles) delete p;
    piles.clear();
    indexPileActuelle = 0;
    config = ConfigJeu(); //config par défaut
}

void Partie::initialiser(int nb, const vector<string>& noms, TuileCite mode, const array<bool, 5>& vars, unsigned int nivIC) {
    resetPourChargement();

    //config du jeu
    config.nbJoueurs = nb;
    config.modeTuileCite = mode;
    config.variantes = vars;
    config.niveauIllustreConstructeur = nivIC;

    //init. joueurs
    gestionnaireJoueurs.initialiser(noms, config.estModeSolo(), nivIC);

    //init. piles
    initialiserPiles();

    //determiner qui commence à jouer
    if (!config.estModeSolo()) {
        gestionnaireJoueurs.designerArchitecteChefAleatoire();
    }
    //si le jeu est solo l'humain commence par défaut
}

void Partie::initialiserPiles() {
    int nbPiles = 11; //nb piles par défaut

    if (config.modeTuileCite == TuileCite::AUGMENTE) { //nb piles en mode augmenté
        size_t n = gestionnaireJoueurs.getNbJoueurs();
        if (n == 2) nbPiles = 19;
        else if (n == 3) nbPiles = 15;
        else if (n == 4) nbPiles = 11;
    }

    piles.reserve(nbPiles);
    int nbJoueursReel = (int)gestionnaireJoueurs.getNbJoueurs();

    piles.emplace_back(new Pile(0, nbJoueursReel + 2));
    for (int i = 1; i < nbPiles; i++) {
        piles.emplace_back(new Pile(i, nbJoueursReel + 1));
    }

    indexPileActuelle = 0;
    remplirChantier();
}

void Partie::remplirChantier() {
    if (estFinDePartie()) return;

    if (chantier.estVide() || chantier.getNbTuiles() == 1) {
        if (indexPileActuelle >= piles.size()) return;

        Pile* p = piles[indexPileActuelle];
        chantier.ajouterPile(*p);
        indexPileActuelle++;
    }

    int prix = 0;
    for (auto it = chantier.begin(); it != chantier.end(); ++it) {
        (*it)->setPrix(prix++);
    }
}

void Partie::passerAuJoueurSuivant() {
    gestionnaireJoueurs.passerAuSuivant();
}

bool Partie::estFinDePartie() const {
    return (indexPileActuelle == piles.size() && chantier.getNbTuiles() == 1);
}

void Partie::actionPlacerTuile(int index, int x, int y, int z, int rotation) {
    //on recupere la tuile choisie par le joueur
    auto itTuile = chantier.begin();
    for (int i = 0; i < index; ++i) {
        ++itTuile;
        if (itTuile == chantier.end()) throw PartieException("Tuile introuvable.");
    }
    Tuile* t = *itTuile;

    //on recupere le joueur qui est en train de jouer
    Joueur* j = gestionnaireJoueurs.getJoueurActuel();

    //on verifie qu'il ait assez de pierres
    int coutPierre = index;
    if (j->getPierres() < coutPierre) throw PartieException("Pas assez de pierres !");

    //on place la tuile si possible
    try {
        j->getCite()->placer(t, { x, y, z }, j, rotation);
    }
    //sinon on renvoie une erreur et on remet la rotation/inversion à leur état initial
    catch (const CiteException& e) {
        for (int r = 0; r < (3 - rotation) % 3; ++r) t->tourner();
        if (t->getInversion()) t->inverser();
        throw;
    }

    //le joueur depense les pierres que coutent la tuile choisie
    j->utiliserPierres(coutPierre);


    if (config.estModeSolo() && !j->estIA()) {
        //En mode solo les pierres vont à l'IA
        for (auto it = debutJoueurs(); it != finJoueurs(); ++it) {
            Joueur* joueur = *it;
            if (joueur->estIA()) {
                joueur->ajouterPierres(coutPierre);
                break;
            }
        }
    }
    else {
        //En mode multi, on redistribue le prix sur les tuiles restantes du chantier
        //En mode solo, la redistribution est gérée dans la methode jouerTourIA
        auto itPaiement = chantier.begin();
        for (int k = 0; k < index; ++k) {
            (*itPaiement)->setPrix((*itPaiement)->getPrix() + 1);
            ++itPaiement;
        }
    }

    //on calcule le nouveau score du joueur
    j->getScore()->calculerScore();

    //on enleve la tuile du chantier
    chantier.retirerTuile(t);

    //si la partie n'est pas fini, on essaie de remplir le chantier
    if (!estFinDePartie()) remplirChantier();

    passerAuJoueurSuivant();
}

int Partie::jouerTourIA() {
    //verification que l'ia joue bien 
    Joueur* j = gestionnaireJoueurs.getJoueurActuel();
    IA* ia = dynamic_cast<IA*>(j);
    if (!ia) return -1;

    //l'ia choisi sa tuile
    int indexChoisi = ia->choisirTuile(chantier);

    auto it = chantier.begin();
    for (int k = 0; k < indexChoisi; ++k) ++it;
    Tuile* t = *it;

    if (indexChoisi > 0) {
        //l'ia depense ses pierres
        ia->utiliserPierres(indexChoisi);
        auto itP = chantier.begin();
        for (int i = 0; i < indexChoisi; i++) {
            //redistribution des pierres sur les tuiles restantes du chantier
            (*itP)->setPrix((*itP)->getPrix() + 1);
            ++itP;
        }
    }

    ia->ajouterTuile(t);

    chantier.retirerTuile(t);

    if (!estFinDePartie()) remplirChantier();

    passerAuJoueurSuivant();

    return indexChoisi;
}