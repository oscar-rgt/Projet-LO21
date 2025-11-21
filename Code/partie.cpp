#include "partie.h"

using namespace std;

Partie& Partie::getInstance(int id_, Mode m, Pile* pile) {
    static Partie instance(id_, m, pile);
    return instance;
}

Partie::Partie(int id_, Mode m, Pile* pile)
    : id(id_), mode(m), nbJoueurs(0), pileTuiles(pile)
{
    joueurs.fill(nullptr);
}

bool Partie::ajouterJoueur(Joueur* j) {
    if (nbJoueurs < 4) {
        joueurs[nbJoueurs++] = j;
        return true;
    } else {
        cout << "Partie complète, impossible d'ajouter " << j->getNom() << endl;
        return false;
    }
}

int Partie::getId() const { return id; }
Partie::Mode Partie::getMode() const { return mode; }
int Partie::getNbJoueurs() const { return nbJoueurs; }

Joueur* Partie::getJoueur(int index) const {
    if (index >= 0 && index < nbJoueurs) return joueurs[index];
    return nullptr;
}

Pile* Partie::getPile() const { return pileTuiles; }

void Partie::jouerTours() {
    if (nbJoueurs == 0 || !pileTuiles) {
        cout << "Erreur : pas de joueurs ou pas de pile de tuiles.\n";
        return;
    }

    int tour = 1;
    int currentJoueur = 0;

    cout << "=== Début de la partie ===\n";

    while (pileTuiles->nbTuiles > 0) {
        Joueur* j = joueurs[currentJoueur];

        cout << "\nTour " << tour << " : " << j->getNom() << endl;

        cout << j->getNom() << " pose une tuile et gagne 1 point.\n";
        j->ajouterPoints(1);

        pileTuiles->nbTuiles--;
        cout << "Tuiles restantes : " << pileTuiles->nbTuiles << endl;

        cout << "Score de " << j->getNom() << " : " << j->getPoints() << endl;

        currentJoueur = (currentJoueur + 1) % nbJoueurs;
        tour++;
    }

    cout << "\n=== Fin de la partie ===\n";
    for (int i = 0; i < nbJoueurs; ++i) {
        cout << joueurs[i]->getNom() << " : " << joueurs[i]->getPoints() << " points.\n";
    }
}
