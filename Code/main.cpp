#include <iostream>
#include "joueur.h"
#include "pile.h"
#include "partie.h"

int main() {

    // créer une pile de 10 tuiles pour tester
    Pile pile(1, 10);

    // créer des joueurs
    Joueur j1("Alice");
    Joueur j2("Bob");

    // créer une partie, mode multi, reliée à ta pile
    Partie partie(42, Partie::multi, &pile);

    // ajouter les joueurs
    partie.ajouterJoueur(&j1);
    partie.ajouterJoueur(&j2);

    // lancer la partie
    partie.jouerTours();

    return 0;
}
