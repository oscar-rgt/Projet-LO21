#include "partie.h"

Partie::Partie(int id_, Mode m, Pile* pile)
    : id(id_), mode(m), nbJoueurs(0), pileTuiles(pile)
{
    joueurs.fill(nullptr);
}

bool Partie::ajouterJoueur(Joueur* j) {
    if(nbJoueurs < 4) {
        joueurs[nbJoueurs++] = j;
        return true;
    } else {
        std::cout << "Partie complète, impossible d'ajouter " << j->getNom() << std::endl;
        return false;
    }
}

//accesseurs
int Partie::getId() const { return id; }
Partie::Mode Partie::getMode() const { return mode; }
int Partie::getNbJoueurs() const { return nbJoueurs; }
Joueur* Partie::getJoueur(int index) const {
    if(index >= 0 && index < nbJoueurs) return joueurs[index];
    return nullptr;
}
Pile* Partie::getPile() const { return pileTuiles; }

//joue automatiquement jusque la fin de la partie
void Partie::jouerTours() {
    if(nbJoueurs == 0 || !pileTuiles) {
        std::cout << "Erreur : pas de joueurs ou pas de pile de tuiles.\n";
        return;
    }

    int tour = 1;
    int currentJoueur = 0;

    std::cout << "=== Début de la partie ===\n";

    while(pileTuiles->nbTuiles > 0) {
        Joueur* j = joueurs[currentJoueur];

        std::cout << "\nTour " << tour << " : " << j->getNom() << std::endl;

        // Action automatique : poser une tuile fictive
        std::cout << j->getNom() << " pose une tuile et gagne 1 point.\n";
        j->ajouterPoints(1);

        // Retirer une tuile de la pile
        pileTuiles->nbTuiles--;
        std::cout << "Tuiles restantes : " << pileTuiles->nbTuiles << std::endl;

        // Afficher score actuel
        std::cout << "Score de " << j->getNom() << " : " << j->getPoints() << std::endl;

        // Joueur suivant
        currentJoueur = (currentJoueur + 1) % nbJoueurs;
        tour++;
    }

    // Fin de partie
    std::cout << "\n=== Fin de la partie ===\n";
    for(int i = 0; i < nbJoueurs; ++i) {
        std::cout << joueurs[i]->getNom() << " : " << joueurs[i]->getPoints() << " points.\n";
    }
}
