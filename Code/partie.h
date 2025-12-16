#ifndef PARTIE_H
#define PARTIE_H

#include <vector>
#include <string>
#include <array>
#include "chantier.h"
#include "joueur.h"
#include "pile.h"
#include "tuiles.h"
#include "cite.h"
#include "score.h"
#include "except.h"

using namespace std;

// Exception spécifique
class PartieException : public AkropolisException {
public:
    PartieException(const string& i) : AkropolisException(i) {}
};

class Partie {
public:
    enum class TuileCite { STANDARD, AUGMENTE };

    // Singleton : Accès unique
    static Partie& getInstance();

    // ==========================================
    // ACCESSEURS POUR SAVE.CPP (Lecture)
    // ==========================================
    int getIndexJoueurActuel() const { return indexJoueurActuel; }
    int getIndexPileActuelle() const { return indexPileActuelle; }
    unsigned int getNiveauIllustreConstructeur() const { return niveauIllustreConstructeur; }
    TuileCite getModeTuileCite() const { return modeTuileCite; }
    
    int getNbJoueurs() const { return static_cast<int>(joueurs.size()); }

    // Renvoie le vecteur brut (nécessaire pour SaveManager)
    const vector<Pile*>& getPiles() const { return piles; }
    const vector<Joueur*>& getJoueurs() const { return joueurs; }
    const std::array<bool, 5>& getVariantes() const { return variantes; }

    // Accès au chantier :
    // Version const pour la sauvegarde
    const Chantier& getChantier() const { return chantier; }
    // Version non-const pour le chargement (permet d'appeler vider())
    Chantier& getChantier() { return chantier; }

    // ==========================================
    // MUTATEURS POUR SAVE.CPP (Chargement)
    // ==========================================
    void setIndexJoueurActuel(int idx) { indexJoueurActuel = idx; }
    void setIndexPileActuelle(int idx) { indexPileActuelle = idx; }
    void setNiveauIllustreConstructeur(unsigned int lvl) { niveauIllustreConstructeur = lvl; }
    
    // Setter pour le nombre de joueurs (logique simplifiée pour compatibilité)
    void setNbJoueurs(int nb) { /* Géré dynamiquement par le vecteur */ }


    void setModeTuileCite(TuileCite mode) { modeTuileCite = mode; }
    void setVariantes(const std::array<bool, 5>& vars) { variantes = vars; }

    // Ajoute un joueur (utilisé lors de la reconstruction depuis le fichier)
    void ajouterJoueur(Joueur* j) { joueurs.push_back(j); }
    //Ajoute une pile (pour la reconstruction)
    void ajouterPile(Pile* p) { piles.push_back(p); }

    // Nettoie toute la mémoire avant de charger une sauvegarde
    void resetPourChargement();

    // ==========================================
    // MÉTHODES DE JEU (Logique coeur)
    // ==========================================
    void initialiser(int nbJoueurs, const std::vector<std::string>& nomsJoueurs,
        TuileCite mode, const std::array<bool, 5>& variantesActives,
        unsigned int niveauIA = 0);

    // ACTION PRINCIPALE
    bool actionPlacerTuile(int indexTuileChantier, int x, int y, int z, int rotation);

    int jouerTourIA();
    void passerAuJoueurSuivant();
    bool estFinDePartie() const;
    
    Joueur* getJoueurActuel() const;


    vector<Joueur*> determinerGagnants(); // Retourne maintenant des Joueur*

    int getNbPiles() const { return static_cast<int>(piles.size()); }

    // Sauvegarde (méthodes inchangées)
    /*bool sauvegarder(const string& nomFichier = "akropolis_save.txt") const;
    bool charger(const string& nomFichier = "akropolis_save.txt");
    static bool supprimerSauvegarde(const string& nomFichier = "akropolis_save.txt");
    bool sauvegardeExiste(const string& nomFichier);*/
    Joueur* getJoueur(int index) const;

    // ==========================================
    // ITERATEURS (Pour parcourir proprement)
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
    // Singleton : Constructeurs privés
    Partie();
    ~Partie();
    Partie(const Partie&) = delete;
    Partie& operator=(const Partie&) = delete;

    // Méthodes internes
    void initialiserPiles();
    void designerArchitecteChef();
    void remplirChantier();

    // Données membres
    array<bool, 5> variantes;
    int indexJoueurActuel;
    int indexPileActuelle;
    unsigned int niveauIllustreConstructeur; // Correspond au getter/setter

    vector<Joueur*> joueurs;
    vector<Pile*> piles;
    Chantier chantier;

    TuileCite modeTuileCite;
};

#endif
