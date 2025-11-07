#include "partie.h"
#include "joueur.h"
#include "pile.h"

int main() {
    // Créer une pile de tuiles
    Pile pile;
    pile.nbTuiles = 10;

    // Créer une partie
    Partie p(1, Partie::multi, &pile);

    // Créer quelques joueurs
    Joueur j1("Alice");
    Joueur j2("Bob");
    p.ajouterJoueur(&j1);
    p.ajouterJoueur(&j2);

    // Jouer la partie
    p.jouerTours();

    return 0;
}
