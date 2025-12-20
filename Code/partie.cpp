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
    // GestionnaireJoueurs se nettoie tout seul
    for (auto p : piles) delete p;
    piles.clear();
    chantier.vider();
}

void Partie::resetPourChargement() {
    gestionnaireJoueurs.vider(); // Délégation
    chantier.vider();
    for (auto p : piles) delete p;
    piles.clear();
    indexPileActuelle = 0;
    config = ConfigJeu(); // Reset config par défaut
}

void Partie::initialiser(int nb, const vector<string>& noms, TuileCite mode, const array<bool, 5>& vars, unsigned int nivIC) {
    resetPourChargement();

    // 1. Configurer
    config.nbJoueurs = nb;
    config.modeTuileCite = mode;
    config.variantes = vars;
    config.niveauIllustreConstructeur = nivIC;

    // 2. Initialiser les joueurs (Délégation)
    gestionnaireJoueurs.initialiser(noms, config.estModeSolo(), nivIC);

    // 3. Préparer le matériel
    initialiserPiles();

    // 4. Déterminer qui commence
    if (!config.estModeSolo()) {
        gestionnaireJoueurs.designerArchitecteChefAleatoire();
    }
    // En solo, c'est toujours l'humain (index 0) qui commence, géré par défaut.
}

void Partie::initialiserPiles() {
    int nbPiles = 11;

    if (config.modeTuileCite == TuileCite::AUGMENTE) {
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

// =========================================================
// ACTION PRINCIPALE : L'ORCHESTRATION DU JEU
// =========================================================
void Partie::actionPlacerTuile(int index, int x, int y, int z, int rotation) {
    // 1. Récupération Tuile
    auto itTuile = chantier.begin();
    for (int i = 0; i < index; ++i) {
        ++itTuile;
        if (itTuile == chantier.end()) throw PartieException("Tuile introuvable.");
    }
    Tuile* t = *itTuile;

    // 2. Vérification Joueur
    Joueur* j = gestionnaireJoueurs.getJoueurActuel();

    // 3. Vérification Coût
    int coutPierre = index;
    if (j->getPierres() < coutPierre) throw PartieException("Pas assez de pierres !");

    // 4. Placement (Tentative)
    try {
        j->getCite()->placer(t, { x, y, z }, j, rotation);
    }
    catch (const CiteException& e) {
        for (int r = 0; r < (3 - rotation) % 3; ++r) t->tourner();
        if (t->getInversion()) t->inverser();
        throw;
    }

    // 5. Paiement et distribution des pierres
    j->utiliserPierres(coutPierre);

    if (config.estModeSolo() && !j->estIA()) {
        // En solo, les pierres vont à l'IA
        for (auto it = debutJoueurs(); it != finJoueurs(); ++it) {
            Joueur* joueur = *it;
            if (joueur->estIA()) {
                joueur->ajouterPierres(coutPierre);
                break;
            }
        }
    }
    else {
        // En multi, pierres sur les tuiles sautées
        auto itPaiement = chantier.begin();
        for (int k = 0; k < index; ++k) {
            (*itPaiement)->setPrix((*itPaiement)->getPrix() + 1);
            ++itPaiement;
        }
    }

    j->getScore()->calculerScore();
    chantier.retirerTuile(t);

    if (!estFinDePartie()) remplirChantier();

    passerAuJoueurSuivant();
}

int Partie::jouerTourIA() {
    Joueur* j = gestionnaireJoueurs.getJoueurActuel();
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
