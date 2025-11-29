#ifndef PARTIE_H
#define PARTIE_H

#include <vector>
#include <string>
#include <array>
#include <memory>

// Forward declarations (pour �viter d'inclure les headers ici)

class Joueur;
class Pile;

class Partie {
public:
    enum class TuileCite { STANDARD, AUGMENTE };

    // --- Singleton : Acc�s unique � l'instance ---
    static Partie& getInstance();

    // Suppression des constructeurs par copie/d�placement
    Partie(const Partie&) = delete;
    Partie& operator=(const Partie&) = delete;

    // --- Configuration (Initialisation des param�tres) ---
    // Cette m�thode est appel�e par le contr�leur (Console ou Qt) pour d�marrer
    void initialiser(int nbJoueurs, const std::vector<std::string>& nomsJoueurs,
        TuileCite mode, const std::array<bool, 5>& variantesActives,
        unsigned int niveauIC = 0);

    // --- Actions de Jeu (API du Moteur) ---

    // Le c�ur du jeu : Tente de placer une tuile pour le joueur actuel
    // Renvoie true si l'action est valide et effectu�e, false sinon.
    bool actionPlacerTuile(int indexTuileChoisie, int x, int y, int z, int rotation);

    // Passe au joueur suivant
    void passerAuJoueurSuivant();

    // V�rifie si la partie est termin�e
    bool estFinDePartie() const;

    // --- Getters (Pour l'affichage) ---
    int getNbJoueurs() const { return nbJoueurs; }
    Joueur* getJoueurActuel() const;
    int getIndexJoueurActuel() const { return indexJoueurActuel; }

    // Acc�s aux piles (const pour lecture seule par l'interface)
    const std::vector<Pile>& getPiles() const { return piles; }
    void ajouterTuileChantier(Tuile& tuile) { chantier.push_back(tuile); }
    void retirerTuileChantier(Tuile& tuile) { chantier.remove(tuile); }

private:
    // Constructeur priv� pour le Singleton
    Partie();
    ~Partie();

    void initialiserPiles();
    void designerArchitecteChef();

    // Donn�es de la partie
    std::array<bool, 5> variantes;
    int indexJoueurActuel;
    int nbJoueurs;
    unsigned int pileActuelle;
    unsigned int tuileActuelle;

    // Conteneurs
    std::vector<Joueur*> joueurs;
    std::vector<Pile> piles;
    std::vector<Tuile> chantier;

    TuileCite modeTuileCite;
    unsigned int niveauIllustreConstructeur;
};

#endif // PARTIE_H