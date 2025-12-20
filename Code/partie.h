#ifndef PARTIE_H
#define PARTIE_H

#include <vector>
#include <string>
#include <array>


#include "configjeu.h"
#include "gestionnairejoueurs.h"
#include "chantier.h"
#include "pile.h"
#include "tuiles.h"
#include "except.h"

using namespace std;

class PartieException : public AkropolisException {
public:
    PartieException(const string& i) : AkropolisException(i) {}
};

class Partie {
public:
    
    static Partie& getInstance();

    
    void initialiser(int nbJoueurs, const std::vector<std::string>& nomsJoueurs,
                     TuileCite mode, const std::array<bool, 5>& variantesActives,
                     unsigned int niveauIA = 0);

    
    void resetPourChargement(); //pour sauvegarde

    void actionPlacerTuile(int indexTuileChantier, int x, int y, int z, int rotation);

    int jouerTourIA();
    void passerAuJoueurSuivant();
    bool estFinDePartie() const;

    //methodes liées à gestionnaire joueurs
    Joueur* getJoueurActuel() const { return gestionnaireJoueurs.getJoueurActuel(); }
    int getIndexJoueurActuel() const { return gestionnaireJoueurs.getIndexJoueurActuel(); }
    int getNbJoueurs() const { return (int)gestionnaireJoueurs.getNbJoueurs(); }
    const vector<Joueur*>& getJoueurs() const { return gestionnaireJoueurs.getJoueurs(); }
    vector<Joueur*> determinerGagnants() { return gestionnaireJoueurs.determinerGagnants(); }
    void ajouterJoueur(Joueur* j) { gestionnaireJoueurs.ajouterJoueurExistant(j); }
    GestionnaireJoueurs::Iterator debutJoueurs() const { return gestionnaireJoueurs.begin(); }
    GestionnaireJoueurs::Iterator finJoueurs() const { return gestionnaireJoueurs.end(); }

    //methodes liées à config
    unsigned int getNiveauIllustreConstructeur() const { return config.niveauIllustreConstructeur; }
    TuileCite getModeTuileCite() const { return config.modeTuileCite; }
    const std::array<bool, 5>& getVariantes() const { return config.variantes; }

    //méthodes liées au chantier
    const Chantier& getChantier() const { return chantier; }
    Chantier& getChantier() { return chantier; }

    //méthodes liées aux piles
    int getNbPiles() const { return static_cast<int>(piles.size()); }
    void ajouterPile(Pile* p) { piles.push_back(p); }
    int getIndexPileActuelle() const { return indexPileActuelle; }


    //méthodes liées à la sauvegarde
    void setIndexJoueurActuel(int idx) { gestionnaireJoueurs.setIndexJoueurActuel(idx); }
    void setIndexPileActuelle(int idx) { indexPileActuelle = idx; }
    void setNiveauIllustreConstructeur(unsigned int lvl) { config.niveauIllustreConstructeur = lvl; }
    void setModeTuileCite(TuileCite mode) { config.modeTuileCite = mode; }
    void setVariantes(const std::array<bool, 5>& vars) { config.variantes = vars; }

    
    class PileIterator {
        friend class Partie;
        vector<Pile*>::const_iterator current;
        PileIterator(vector<Pile*>::const_iterator it) : current(it) {}
    public:
        PileIterator() {}
        PileIterator& operator++() { ++current; return *this; }
        bool operator!=(const PileIterator& other) const { return current != other.current; }
        Pile* operator*() const { return *current; }
        Pile* operator->() const { return *current; }
    };

    PileIterator debutPiles() const { return PileIterator(piles.begin()); }
    PileIterator finPiles() const { return PileIterator(piles.end()); }

private:
    Partie();
    ~Partie();
    Partie(const Partie&) = delete;
    Partie& operator=(const Partie&) = delete;

    void initialiserPiles();
    void remplirChantier();
    
    ConfigJeu config;
    GestionnaireJoueurs gestionnaireJoueurs;
    vector<Pile*> piles;
    Chantier chantier;
    int indexPileActuelle;
};

#endif 
