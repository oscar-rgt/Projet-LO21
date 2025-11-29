#include "partie.h"
#include <iostream>
#include <random>

using namespace std;

Partie& Partie::getInstance(int id_, Pile* pile, array<bool, 5> variantes)
{
    static bool dejaCree = false;
    static Partie instance(id_);
    if (dejaCree) {
        cerr << " getInstance() a déjà été appelé. ";
    }

    dejaCree = true;
    return instance;
}

Partie::Partie(int id_)
    : id(id_)
{
    bool reprise;
    //définition des attributs nbJoueurs, joueurs, variantes
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
    } while (reponse != "o" || reponse != "n");
    reprise = reponse == "o" ? true : false;

    if (reprise == false) {
        cout << "Combien de joueurs ?\n";
        do {
            cin >> reponseNombre;
        } while (reponseNombre < 0 || reponseNombre > 4);
        nbJoueurs = reponseNombre;
        for (unsigned int i = 0; i < nbJoueurs; i++) {
            cout << "Nom du joueur " << i + 1 << " : ";
            cin >> nom;
            joueurs[i] = &Joueur(nom);
        }
                //choix du niveau de difficulté de l'Illustre Constructeur si mode solo;
        if (nbJoueurs == 1) {
            cout << "Quel niveau de difficulté pour l'Illustre Constructeur ? (1 2 3)\n";
            do {
                cin >> reponseNombre;
            } while (reponseNombre < 0 || reponseNombre > 3);
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
         } while (reponse != "o" || reponse != "n");
         variante = reponse == "o" ? true : false;
         if (variante) {
             for (unsigned int i = 0; i < 5; i++) {
                 cout << "Voulez-vous activer la variante " << i << " ?\n";
                 do {
                     cin >> reponse;
                 } while (reponse != "o" || reponse != "n");
                 variantes[i] = reponse == "o" ? true : false;
             }
         }
    }
    return reprise;
}

void Partie::bouclePartieMulti() {
    unsigned int pileActuelle = 0;
    for (unsigned int i = 0; i < nbJoueurs; i++) {
        //if (piles[pileActuelle].vide()) pileActuelle++;
        //afficher pile
        joueurs[i]->getCite()->afficher();

        unsigned int choix;
        cout << "\nQuelle tuile voulez-vous placer ?\n";
        do {
            cin >> choix;
        } while (choix < 1 || choix > 4);

        string tourner;
        
        do {
            do {
                cout << "Voulez vous tourner la tuile choisie ? (o/n)\n";
                cin >> tourner;
            } while (tourner != "o" || tourner != "n");
            if (tourner == "o") piles[pileActuelle].getTuile(pileActuelle*10 + (choix-1)%3).tourner();
        } while (tourner != "n");


        int x, y, z;
        
        cout << "\Où voulez vous placer cette tuile ?\n";
        do {
            cout << "Coordonnees en x : ";
            cin >> x;
        } while (x < 0 || x > 14);
        do {
            cout << "Coordonnees en y : ";
            cin >> y;
        } while (y < 0 || y > 6);
        do {
            cout << "Coordonnees en z : ";
            cin >> z;
        } while (z < 0);

        joueurs[i]->getCite()->placer(&piles[pileActuelle].getTuile(pileActuelle * 10 + (choix - 1) % 3), { x, y, z });
    }
}

void Partie::initPartie(bool reprise) {
    //if(reprise){remettre comme état sauvegardé}


    //Creation des piles de la partie
    if (modeTuileCite == standard) {
        for (unsigned int i = 0; i < 11; i++) {
            switch (nbJoueurs) {
            case 2:
                piles[i] = Pile(i, 3);
                break;
            case 3:
                piles[i] = Pile(i, 4);
                break;
            case 4:
                piles[i] = Pile(i, 5);
                break;
            }
        }
    }
    else {
        switch (nbJoueurs) {
        case 2:
            for (unsigned int i = 0; i < 19; i++) {
                piles[i] = Pile(i, 3);
            }
            break;
        case 3:
            for (unsigned int i = 0; i < 15; i++) {
                piles[i] = Pile(i, 4);
            }
            break;
        case 4:
            for (unsigned int i = 0; i < 11; i++) {
                piles[i] = Pile(i, 5);
            }
            break;
        }

    }

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