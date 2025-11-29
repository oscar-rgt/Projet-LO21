#include "partie.h"
#include <iostream>
#include <random>

using namespace std;

Partie& Partie::getInstance(int id_, Mode m, Pile* pile, array<bool, 5> variantes)
{
    static bool dejaCree = false;
    static Partie instance(id_, m, pile, variantes);
    if (dejaCree) {
        cerr << " getInstance() a déjà été appelé. ";
    }

    dejaCree = true;
    return instance;
}

Partie::Partie(int id_, Mode m, Pile* pile, array<bool, 5> variantes_)
    : id(id_), mode(m), nbJoueurs(0), pileTuiles(pile), variantes(variantes_)
{
    bool reprise;
    reprise = menu();
    initPartie(reprise);

}


bool Partie::menu() {
    string reponse = "";
    string nom;
    unsigned int reponseNombre = 0;
    bool reprise;
    bool variante;

    //reprise ou abandon d'une eventuelle partie en cours
    //if(partieEnCours)
    cout << "Voulez vous reprendre la partie déjà commencée ou en recommencer une nouvelle ? (o/n)\n";
    do {
        cin >> reponse;
    } while (reponse != "o" && reponse != "n");
    reprise = reponse == "o" ? true : false;

    if (reprise == false) {
        cout << "Combien de joueurs ?\n";
        do {
            cin >> reponseNombre;
        } while (reponseNombre < 0 && reponseNombre > 4);
        nbJoueurs = reponseNombre;
        for (unsigned int i = 0; i < nbJoueurs; i++) {
            cout << "Nom du joueur " << i + 1 << " : ";
            cin >> nom;
            ajouterJoueur(&Joueur(nom));
        }
                //choix du niveau de difficulté de l'Illustre Constructeur si mode solo;
        if (nbJoueurs == 1) {
            cout << "Quel niveau de difficulté pour l'Illustre Constructeur ? (1 2 3)\n";
            do {
                cin >> reponseNombre;
            } while (reponseNombre < 0 && reponseNombre > 3);
            niveauIllustreConstructeur = reponseNombre;
        }
                //nombre de tuiles cité(standard ou augmenté);
        cout << "Nombre de tuiles cité : (standard / augmente)\n";
        do {
            cin >> reponse;
        } while (reponse != "standard" && reponse != "augmente");
         modeTuileCite = reponse=="standard"?standard:augmente;
                //activation d'une ou plusieurs variantes.
         cout << "Activation de variantes ? (o/n)\n";
         do {
             cin >> reponse;
         } while (reponse != "o" && reponse != "n");
         variante = reponse == "o" ? true : false;
         if (variante) {
             for (unsigned int i = 0; i < 5; i++) {
                 cout << "Voulez-vous activer la variante " << i << " ?\n";
                 do {
                     cin >> reponse;
                 } while (reponse != "o" && reponse != "n");
                 variantes[i] = reponse == "o" ? true : false;
             }
         }
    }
    return reprise;
}

void Partie::bouclePartieMulti() {
    for (unsigned int i = 0; i < nbJoueurs; i++) {

    }
}

void Partie::initPartie(bool reprise) {
    //if(reprise){remettre comme état sauvegardé}

    //désignation aléatoire de l'architecte en chef au début de partie
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dist(1, nbJoueurs);
    Joueur* temp = joueurs[dist(gen)];
    joueurs[dist(gen)] = joueurs[0];
    joueurs[0] = temp;

    if (nbJoueurs > 1) bouclePartieMulti();
    //else bouclePartieIA();

}