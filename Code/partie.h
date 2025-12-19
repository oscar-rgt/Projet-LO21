#ifndef PARTIE_H
#define PARTIE_H

#include <vector>
#include <string>
#include <array>

// Nouveaux modules
#include "configjeu.h"
#include "gestionnairejoueurs.h"

// Modules existants conservés
#include "chantier.h"
#include "pile.h"
#include "tuiles.h"
#include "except.h"

using namespace std;

// Exception spécifique (inchangée)
class PartieException : public AkropolisException {
public:
    PartieException(const string& i) : AkropolisException(i) {}
};

class Partie {
public:
    // Singleton
    static Partie& getInstance();

    // ==========================================
    // INITIALISATION & CONFIGURATION
    // ==========================================
    void initialiser(int nbJoueurs, const std::vector<std::string>& nomsJoueurs,
                     TuileCite mode, const std::array<bool, 5>& variantesActives,
                     unsigned int niveauIA = 0);

    // Pour SaveManager
    void resetPourChargement();

    // ==========================================
    // MÉTHODES DE JEU
    // ==========================================

    // L'action principale : délègue les règles et le placement
    void actionPlacerTuile(int indexTuileChantier, int x, int y, int z, int rotation);

    int jouerTourIA();
    void passerAuJoueurSuivant();
    bool estFinDePartie() const;

    // ==========================================
    // ACCESSEURS
    // ==========================================

    // Vers GestionnaireJoueurs
    Joueur* getJoueurActuel() const { return gestionnaireJoueurs.getJoueurActuel(); }
    Joueur* getJoueur(int index) const { return gestionnaireJoueurs.getJoueur(index); }
    int getIndexJoueurActuel() const { return gestionnaireJoueurs.getIndexJoueurActuel(); }
    int getNbJoueurs() const { return (int)gestionnaireJoueurs.getNbJoueurs(); }
    const vector<Joueur*>& getJoueurs() const { return gestionnaireJoueurs.getJoueurs(); }

    // Vers ConfigJeu
    unsigned int getNiveauIllustreConstructeur() const { return config.niveauIllustreConstructeur; }
    TuileCite getModeTuileCite() const { return config.modeTuileCite; }
    const std::array<bool, 5>& getVariantes() const { return config.variantes; }

    // Vers Piles et Chantier
    const vector<Pile*>& getPiles() const { return piles; }
    const Chantier& getChantier() const { return chantier; }
    Chantier& getChantier() { return chantier; }
    int getNbPiles() const { return (int)piles.size(); }
    int getIndexPileActuelle() const { return indexPileActuelle; }

    // Fin de partie
    vector<Joueur*> determinerGagnants() { return gestionnaireJoueurs.determinerGagnants(); }

    // ==========================================
    // MUTATEURS (Pour save.cpp)
    // ==========================================
    void setIndexJoueurActuel(int idx) { gestionnaireJoueurs.setIndexJoueurActuel(idx); }
    void setIndexPileActuelle(int idx) { indexPileActuelle = idx; }
    void setNiveauIllustreConstructeur(unsigned int lvl) { config.niveauIllustreConstructeur = lvl; }
    void setModeTuileCite(TuileCite mode) { config.modeTuileCite = mode; }
    void setVariantes(const std::array<bool, 5>& vars) { config.variantes = vars; }

    void ajouterJoueur(Joueur* j) { gestionnaireJoueurs.ajouterJoueurExistant(j); }
    void ajouterPile(Pile* p) { piles.push_back(p); }

    // ==========================================
    // ITERATEURS
    // ==========================================
    auto debutJoueurs() { return gestionnaireJoueurs.begin(); }
    auto finJoueurs() { return gestionnaireJoueurs.end(); }

    // Itérateur Piles
    auto debutPiles() const { return piles.begin(); }
    auto finPiles() const { return piles.end(); }

private:
    Partie();
    ~Partie();
    Partie(const Partie&) = delete;
    Partie& operator=(const Partie&) = delete;

    // Méthodes internes
    void initialiserPiles();
    void remplirChantier();

    // 1. Configuration
    ConfigJeu config;

    // 2. Logique Joueurs
    GestionnaireJoueurs gestionnaireJoueurs;

    // 3. Logique Matériel
    vector<Pile*> piles;
    Chantier chantier;
    int indexPileActuelle;
};

#endif // PARTIE_H
