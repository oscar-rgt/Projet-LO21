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

using namespace std;

<<<<<<< Updated upstream
class Joueur;
class Pile;
class Tuile;

class PartieException {
    string info;
=======
// Exception spécifique
class PartieException : public AkropolisException {
>>>>>>> Stashed changes
public:
    PartieException(const string& s) :info(s) {}
    const string& getInfo() const { return info; }
};

class Partie {
public:
    enum class TuileCite { STANDARD, AUGMENTE };

    // Singleton : Accès unique
    static Partie& getInstance();
<<<<<<< Updated upstream
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

=======

    // ==========================================
    // ACCESSEURS POUR SAVE.CPP (Lecture)
    // ==========================================
    int getIndexJoueurActuel() const { return indexJoueurActuel; }
    int getIndexPileActuelle() const { return indexPileActuelle; }
    unsigned int getNiveauIllustreConstructeur() const { return niveauIllustreConstructeur; }
    
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

    // Ajoute un joueur (utilisé lors de la reconstruction depuis le fichier)
    void ajouterJoueur(Joueur* j) { joueurs.push_back(j); }

    // Nettoie toute la mémoire avant de charger une sauvegarde
    void resetPourChargement();

    // ==========================================
    // MÉTHODES DE JEU (Logique coeur)
    // ==========================================
    void initialiser(int nbJoueurs, const std::vector<std::string>& nomsJoueurs,
        TuileCite mode, const std::array<bool, 5>& variantesActives,
        unsigned int niveauIA = 0);

    bool actionPlacerTuile(int indexTuileChantier, int x, int y, int z, int rotation, int inversion);
    int jouerTourIA();
    void passerAuJoueurSuivant();
    bool estFinDePartie() const;
    
    Joueur* getJoueurActuel() const;
    vector<Joueur*> determinerGagnants();
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
>>>>>>> Stashed changes

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
    // Singleton : Constructeurs privés
    Partie();
    ~Partie();
    Partie(const Partie&) = delete;
    Partie& operator=(const Partie&) = delete;

    // Méthodes internes
    void initialiserPiles();
    void designerArchitecteChef();
    void remplirChantier(); // Pour compléter le chantier à la fin du tour

    // Données membres
    array<bool, 5> variantes;
    int indexJoueurActuel;
<<<<<<< Updated upstream
    int nbJoueurs;
    int indexPileActuelle; // Pour savoir quelle pile on utilise

    vector<Joueur*> joueurs; //iterator ?
    vector<Pile*> piles; //iterator ?

    Chantier chantier;

    TuileCite modeTuileCite;
    unsigned int niveauIllustreConstructeur;

    vector<Tuile*> tuilesDepart; // Pour gérer la mémoire des tuiles de départ
=======
    int indexPileActuelle;
    unsigned int niveauIllustreConstructeur; // Correspond au getter/setter

    vector<Joueur*> joueurs;
    vector<Pile*> piles;
    Chantier chantier;

    TuileCite modeTuileCite;
>>>>>>> Stashed changes
};

#endif
