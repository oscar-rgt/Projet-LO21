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
#include "score.h"
#include "except.h"

using namespace std;

class Joueur;
class Pile;
class Tuile;




class PartieException : public AkropolisException {
public:
    PartieException(const string& i) : AkropolisException(i) {}
};

class Partie {
public:
    int getIndexJoueurActuel() const { return indexJoueurActuel; }

    enum class TuileCite { STANDARD, AUGMENTE };
    static Partie& getInstance();

    // Configuration
    void initialiser(int nbJoueurs, const std::vector<std::string>& nomsJoueurs,
        TuileCite mode, const std::array<bool, 5>& variantesActives,
        unsigned int niveauIA = 0);

    // ACTION PRINCIPALE
    bool actionPlacerTuile(int indexTuileChantier, int x, int y, int z, int rotation, int inversion);

    int jouerTourIA();

    void passerAuJoueurSuivant();
    bool estFinDePartie() const;

    int getNbJoueurs() const { return static_cast<int>(joueurs.size()); }
    Joueur* getJoueurActuel() const;


    vector<Joueur*> determinerGagnants(); // Retourne maintenant des Joueur*

    // Accès au chantier
    const Chantier& getChantier() const { return chantier; }

    int getNbPiles() const { return static_cast<int>(piles.size()); }
    int getIndexPileActuelle() const { return indexPileActuelle; }

    const std::array<bool, 5>& getVariantes() const { return variantes; }

    // Sauvegarde (méthodes inchangées)
    bool sauvegarder(const string& nomFichier = "akropolis_save.txt") const;
    bool charger(const string& nomFichier = "akropolis_save.txt");
    static bool supprimerSauvegarde(const string& nomFichier = "akropolis_save.txt");
    bool sauvegardeExiste(const string& nomFichier);

    // ==========================================
    // ITERATORS
    // ==========================================
    class JoueurIterator {
        friend class Partie;
        vector<Joueur*>::iterator current;
        JoueurIterator(vector<Joueur*>::iterator it) : current(it) {}
    public:
        JoueurIterator() {}
        JoueurIterator& operator++() { ++current; return *this; }
        bool operator!=(const JoueurIterator& other) const { return current != other.current; }
        Joueur* operator*() const { return *current; }
    };
    JoueurIterator debutJoueurs() { return JoueurIterator(joueurs.begin()); }
    JoueurIterator finJoueurs() { return JoueurIterator(joueurs.end()); }

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
    void designerArchitecteChef();
    void remplirChantier();

    array<bool, 5> variantes;
    int indexJoueurActuel;
    int indexPileActuelle;

    vector<Joueur*> joueurs;
    vector<Pile*> piles;

    Chantier chantier;

    TuileCite modeTuileCite;
    unsigned int niveauIA;

    vector<Tuile*> tuilesDepart;
};

#endif