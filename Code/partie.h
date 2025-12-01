#ifndef PARTIE_H
#define PARTIE_H

#include <vector>
#include <string>
#include <array>
#include "chantier.h"
#include "partie.h"
#include "joueur.h"
#include "pile.h"
#include "tuiles.h"
#include "cite.h"

using namespace std;

class Joueur;
class Pile;
class Tuile; 

class PartieException {
    string info;
public:
    PartieException(const string& s) :info(s) {}
    const string& getInfo() const { return info; }
};

class Partie {
public:
    enum class TuileCite { STANDARD, AUGMENTE };
    static Partie& getInstance();

    // Configuration
    void initialiser(int nbJoueurs, const std::vector<std::string>& nomsJoueurs,
        TuileCite mode, const std::array<bool, 5>& variantesActives,
        unsigned int niveauIC = 0);

    // ACTION PRINCIPALE
    // return true si succès, false si erreur (ressources, placement, index)
    bool actionPlacerTuile(int indexTuileChantier, int x, int y, int z, int rotation, int inversion);
    
	int jouerTourIA(); //retourne l'index de la tuile choisie par l'IA (-1 si erreur)

    void passerAuJoueurSuivant();
    bool estFinDePartie() const;

    int getNbJoueurs() const { return nbJoueurs; }
    Joueur* getJoueurActuel() const;
    Joueur* getJoueur(int index) const;
    
    // Accès au chantier pour l'affichage 
    const Chantier& getChantier() const { return chantier; }
    size_t getNbPiles() const { return piles.size(); }
    int getIndexPileActuelle() const { return indexPileActuelle; }

    const std::array<bool,5>& getVariantes() const { return variantes; }//getter public pour les variantes


private:
    Partie();
    ~Partie();
    Partie(const Partie&) = delete;
    Partie& operator=(const Partie&) = delete;

    void initialiserPiles();
    void designerArchitecteChef();
    void remplirChantier(); // Pour compléter le chantier à la fin du tour

    array<bool, 5> variantes;
    int indexJoueurActuel;
    int nbJoueurs;
    int indexPileActuelle; // Pour savoir quelle pile on utilise

    vector<Joueur*> joueurs;
    vector<Pile*> piles;

    Chantier chantier;

    TuileCite modeTuileCite;
    unsigned int niveauIllustreConstructeur;
    
    vector<Tuile*> tuilesDepart; // Pour gérer la mémoire des tuiles de départ
};

#endif
