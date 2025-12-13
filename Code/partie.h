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
    int getIndexJoueurActuel() const { return indexJoueurActuel; }

    enum class TuileCite { STANDARD, AUGMENTE };
    static Partie& getInstance();
    void initialiser(int nbJoueurs, const std::vector<std::string>& nomsJoueurs,
    TuileCite mode, const std::array<bool, 5>& variantesActives,
    unsigned int niveauIC = 0);
    bool actionPlacerTuile(int indexTuileChantier, int x, int y, int z, int rotation, int inversion);

    int jouerTourIA(); 

    void passerAuJoueurSuivant();
    bool estFinDePartie() const;

    int getNbJoueurs() const { return nbJoueurs; }
    Joueur* getJoueurActuel() const;
    Joueur* getJoueur(int index) const;
    const Chantier& getChantier() const { return chantier; }
    size_t getNbPiles() const { return piles.size(); }
    int getIndexPileActuelle() const { return indexPileActuelle; }
    const std::array<bool, 5>& getVariantes() const { return variantes; }
    bool sauvegarder(const string& nomFichier = "akropolis_save.txt") const;
    bool charger(const string& nomFichier = "akropolis_save.txt");
    static bool supprimerSauvegarde(const string& nomFichier = "akropolis_save.txt");
    bool sauvegardeExiste(const string& nomFichier);


    // Accesseurs (Lecture pour la sauvegarde)
    unsigned int getNiveauIllustreConstructeur() const { return niveauIllustreConstructeur; }
    const std::vector<Joueur*>& getJoueurs() const { return joueurs; }
    const std::vector<Pile*>& getPiles() const { return piles; }
    
    // Surcharge non-const pour modifier le chantier au chargement
    Chantier& getChantier() { return chantier; } 

    // Mutateurs (Écriture pour le chargement)
    void setNbJoueurs(int n) { nbJoueurs = n; }
    void setIndexJoueurActuel(int i) { indexJoueurActuel = i; }
    void setIndexPileActuelle(int i) { indexPileActuelle = i; }
    void setNiveauIllustreConstructeur(unsigned int n) { niveauIllustreConstructeur = n; }

    // Gestion des vecteurs
    void viderJoueurs() { joueurs.clear(); }
    void ajouterJoueur(Joueur* j) { joueurs.push_back(j); }
    void viderPiles() { piles.clear(); }
    

    void resetPourChargement() {
        joueurs.clear();      // Vide les joueurs
        chantier.vider();     // Vide le chantier
        
        // Nettoyage des piles existantes
        for(auto p : piles) { delete p; }
        piles.clear();

        // Réinitialisation interne
        initialiserPiles();   // Appel de la méthode PRIVÉE
    }
    


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

    vector<Joueur*> joueurs; //iterator ?
    vector<Pile*> piles; //iterator ?

    Chantier chantier;

    TuileCite modeTuileCite;
    unsigned int niveauIllustreConstructeur;

    vector<Tuile*> tuilesDepart; // Pour gérer la mémoire des tuiles de départ
};

#endif
