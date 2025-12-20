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

    void initialiser(const std::vector<std::string>& noms, bool modeSolo, unsigned int niveauIA);
    void vider();

    void passerAuSuivant();
    Joueur* getJoueurActuel() const;

    size_t getNbJoueurs() const { return joueurs.size(); }
    int getIndexJoueurActuel() const { return static_cast<int>(indexJoueurActuel); }
    void setIndexJoueurActuel(int idx) { indexJoueurActuel = idx; }

    void designerArchitecteChefAleatoire();

    class Iterator {
        friend class GestionnaireJoueurs;
        std::vector<Joueur*>::const_iterator current;
    public:
        Iterator(std::vector<Joueur*>::const_iterator it) : current(it) {}

        Iterator& operator++() { ++current; return *this; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
        bool operator==(const Iterator& other) const { return current == other.current; }

        Joueur* operator*() const { return *current; }
    };

    Iterator begin() const { return Iterator(joueurs.begin()); }
    Iterator end() const { return Iterator(joueurs.end()); }

    std::vector<Joueur*> determinerGagnants();

    //pour sauvegarde
    void ajouterJoueurExistant(Joueur* j);
    const std::vector<Joueur*>& getJoueurs() const { return joueurs; }
};

#endif 
