#ifndef PARTIE_H
#define PARTIE_H

#include <vector>
#include <string>
#include <array>
#include <memory>

// Forward declarations (pour éviter d'inclure les headers ici)

class Joueur;
class Pile;

class Partie {
public:
    enum class TuileCite { STANDARD, AUGMENTE };

    // --- Singleton : Accès unique à l'instance ---
    static Partie& getInstance();

    // Suppression des constructeurs par copie/déplacement
    Partie(const Partie&) = delete;
    Partie& operator=(const Partie&) = delete;

    // --- Configuration (Initialisation des paramètres) ---
    // Cette méthode est appelée par le contrôleur (Console ou Qt) pour démarrer
    void initialiser(int nbJoueurs, const std::vector<std::string>& nomsJoueurs,
        TuileCite mode, const std::array<bool, 5>& variantesActives,
        unsigned int niveauIC = 0);

    // --- Actions de Jeu (API du Moteur) ---

    // Le cœur du jeu : Tente de placer une tuile pour le joueur actuel
    // Renvoie true si l'action est valide et effectuée, false sinon.
    bool actionPlacerTuile(int indexTuileChoisie, int x, int y, int z, int rotation);

    // Passe au joueur suivant
    void passerAuJoueurSuivant();

    // Vérifie si la partie est terminée
    bool estFinDePartie() const;

    // --- Getters (Pour l'affichage) ---
    int getNbJoueurs() const { return nbJoueurs; }
    Joueur* getJoueurActuel() const;
    int getIndexJoueurActuel() const { return indexJoueurActuel; }

    // Accès aux piles (const pour lecture seule par l'interface)
    const std::vector<Pile>& getPiles() const { return piles; }

private:
    // Constructeur privé pour le Singleton
    Partie();
    ~Partie();

    void initialiserPiles();
    void designerArchitecteChef();

    // Données de la partie
    std::array<bool, 5> variantes;
    int indexJoueurActuel;
    int nbJoueurs;

    // Conteneurs
    std::vector<Joueur*> joueurs;
    std::vector<Pile> piles;

    TuileCite modeTuileCite;
    unsigned int niveauIllustreConstructeur;
};

#endif // PARTIE_H