#ifndef GESTIONNAIRE_JOUEURS_H
#define GESTIONNAIRE_JOUEURS_H

#include <vector>
#include <string>
#include "joueur.h"
#include "ia.h"


class GestionnaireJoueurs {
private:
    std::vector<Joueur*> joueurs;
    size_t indexJoueurActuel;

public:
    GestionnaireJoueurs();
    ~GestionnaireJoueurs();

    // Gestion de la liste
    void initialiser(const std::vector<std::string>& noms, bool modeSolo, unsigned int niveauIA);
    void vider();

    // Navigation
    void passerAuSuivant();
    Joueur* getJoueurActuel() const;
    Joueur* getJoueur(int index) const;

    // Accesseurs
    size_t getNbJoueurs() const { return joueurs.size(); }
    int getIndexJoueurActuel() const { return static_cast<int>(indexJoueurActuel); }
    void setIndexJoueurActuel(int idx) { indexJoueurActuel = idx; }

    // Logique Architecte Chef (Premier Joueur)
    void designerArchitecteChefAleatoire();

    // Itérateurs (pour compatibilité avec votre code existant)
    auto begin() { return joueurs.begin(); }
    auto end() { return joueurs.end(); }
    auto begin() const { return joueurs.cbegin(); }
    auto end() const { return joueurs.cend(); }

    // Gestion Fin de partie
    std::vector<Joueur*> determinerGagnants();

    // Pour la sauvegarde
    void ajouterJoueurExistant(Joueur* j);
    const std::vector<Joueur*>& getJoueurs() const { return joueurs; }
};

#endif // GESTIONNAIRE_JOUEURS_H
