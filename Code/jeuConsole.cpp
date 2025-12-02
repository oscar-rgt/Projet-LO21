#include "jeuConsole.h"
#include "joueur.h"
#include "ia.h"
#include "tuiles.h"
#include "cite.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <cstdlib>

using namespace std;

void JeuConsole::nettoyerEcran() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

int JeuConsole::saisieNombre(const string& prompt, int min, int max) {
    int reponse = -1;
    cout << prompt << " (" << min << " - " << max << ") : ";
    while (!(cin >> reponse) || reponse < min || reponse > max) {
        cout << "Erreur. Entrez un nombre entre " << min << " et " << max << " : ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return reponse;
}

bool JeuConsole::saisieOuiNon(const string& prompt) {
    string reponse;
    cout << prompt << " (o/n) : ";
    while (true) {
        cin >> reponse;
        for (auto& c : reponse) c = tolower(c);
        if (reponse == "o") return true;
        if (reponse == "n") return false;
        cout << "Repondre par 'o' ou 'n' : ";
    }
}

void JeuConsole::afficherChantier() {
    const auto& chantier = Partie::getInstance().getChantier();
    vector<string> lignes(9, "");
    const int hauteur = 9;
    const int largeur_reelle = 20;
    const int largeur_visuelle = 19;
    cout << "\n=== CHANTIER === " << Partie::getInstance().getNbPiles() - Partie::getInstance().getIndexPileActuelle() << " pile(s) restante(s)" << endl;
    for (int i = 0; i < chantier.getNbTuiles(); ++i) {
        Tuile* t = chantier.getTuile(i);
        string designTuile = t->getDesign();
        for (int j = 0; j < hauteur; j++) {
            string segment = designTuile.substr(j * largeur_reelle, largeur_visuelle);
            lignes[j] += segment;
            lignes[j] += "         ";
        }
    }
    for (int j = 0; j < hauteur; j++) {
        cout << lignes[j] << endl;
    }
    cout << "\n\n";
    for (int i = 0; i < chantier.getNbTuiles(); i++) {
        int pierres = chantier.getTuile(i)->getPrix();
        cout << "    ";
        cout << pierres << " pierres";
        cout << "               ";
    }
}

void JeuConsole::afficherEtatJeu() {
    Joueur* j = Partie::getInstance().getJoueurActuel();
    cout << "\n##########################################" << endl;
    cout << "TOUR DE : " << j->getNom() << endl;
    cout << "Pierres : " << j->getPierres() << endl;
    cout << "Score : " << j->getScore()->getTotal() << endl;
    cout << "##########################################" << endl;

    cout << "\n--- CITE ---" << endl;

    if (dynamic_cast<IA*>(j)) {
        cout << "(Cite de l'IA - Gestion virtuelle)" << endl;
    } else {
        j->getCite()->afficher();
        afficherChantier();
    }


}

void JeuConsole::jouerTour() {

    nettoyerEcran();
    afficherEtatJeu();

    Joueur* j = Partie::getInstance().getJoueurActuel();

    // Si c'est l'IA, on joue automatiquement
    if (dynamic_cast<IA*>(j)) {
        cout << "\n\n--- TOUR DE L'IA ---" << endl;
        cout << "\nL'Illustre Constructeur reflechit..." << endl;
		int indexChoisi = Partie::getInstance().jouerTourIA();
        afficherChantier();
        try {
            cout << "\n\nL'IA choisit la tuile " << indexChoisi << endl;
            cout << "Appuyez sur une touche pour continuer...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        } catch (const PartieException& e) {
            cout << "L'IA a rencontre une erreur en choisissant une tuile : " << e.getInfo() << endl;
		}
        return;
    }

    //Tour humain

    size_t maxChoix = Partie::getInstance().getChantier().getNbTuiles() - 1;

    cout << "\n--- ACTION ---" << endl;
    int index = saisieNombre("Quelle tuile choisir ?", 0, maxChoix);

    Tuile* tuileAffichee = Partie::getInstance().getChantier().getTuile(index);

    // --- MODE PREVISUALISATION ---

    bool placementValide = false;
    int rotationCompteur = 0;
    bool inversionEtat = false;

    while (!placementValide) {
        nettoyerEcran();
        afficherEtatJeu(); // On réaffiche le contexte


        cout << "\n\nCommandes : [R]otation | [I]nversion | [V]alider | [A]nnuler choix" << endl;
        cout << "Votre choix : ";

        string choix;
        cin >> choix;

        // Gestion des commandes
        if (choix == "R" || choix == "r") {
            tuileAffichee->tourner();
            rotationCompteur = (rotationCompteur + 1) % 3;
        }
        else if (choix == "I" || choix == "i") {
            tuileAffichee->inverser();
        }
        else if (choix == "A" || choix == "a") {
            // Annuler le choix de la tuile, on recommence le tour
            if (tuileAffichee->getInversion() != 0) tuileAffichee->inverser(); // Remettre l'état initial
            for(int i = 0; i < (3 - rotationCompteur) % 3; ++i) {
                tuileAffichee->tourner(); // Remettre l'orientation initiale
			}
            jouerTour();
            return;
        }
        else if (choix == "V" || choix == "v") {
            // On sort de la boucle de prévisualisation pour aller placer
            placementValide = true;
        }
    }

    //Placement
    int x = saisieNombre("Coord X", -10, 10);
    int y = saisieNombre("Coord Y", -10, 10);
    int z = saisieNombre("Coord Z (Niveau)", 0, 10);


	cout << "\n\nLa tuile\n\n" << tuileAffichee->getDesign() << "\n\nva etre placee en (" << x << ", " << y << ", " << z << ").\n" <<endl;
    if(saisieOuiNon("Valider ce choix ?")){

        try {
            bool succes = Partie::getInstance().actionPlacerTuile(index, x, y, z, rotationCompteur, inversionEtat);

            if (!succes) {
                cout << ">> ECHEC : Pas assez de pierres ou regle non respectee." << endl;
                cout << "Appuyez sur Entree pour reessayer...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                jouerTour();
            }
        }
        catch (CiteException& e) {
            cout << ">> ECHEC : " << e.getInfos() << endl;
            cout << "Appuyez sur Entree pour reessayer...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            jouerTour();
        }
    }
    else {
        if (tuileAffichee->getInversion() != 0) tuileAffichee->inverser(); // Remettre l'état initial
        for (int i = 0; i < (3 - rotationCompteur) % 3; ++i) {
            tuileAffichee->tourner(); // Remettre l'orientation initiale
        }
        jouerTour();
    }


}

void JeuConsole::lancer() {
    cout << "=== AKROPOLIS CONSOLE ===" << endl;
    demanderConfiguration();

    while (!Partie::getInstance().estFinDePartie()) {
        jouerTour();
    }

    cout << "=== FIN DE PARTIE ===" << endl;

    //Classement final
    cout << "\n--- CLASSEMENT ---" << endl;
    for (int i = 0; i < Partie::getInstance().getNbJoueurs(); i++) {
        Joueur* j = Partie::getInstance().getJoueur(i);

        // Calcul du score IA si n�cessaire
        IA* ia = dynamic_cast<IA*>(j);
        if (ia) {
            //j->setPoints(ia->calculerScoreIA());
            int scoreIA = ia->calculerScoreIA();
        }
    cout << i + 1 << ". " << j->getNom()
		<< " : " << j->getScore()->getTotal()
     << " points" << endl;


    }
    cout << "\n--- GAGNANT ---" << endl;
    int maxPoints = 0;
    int gagnant = 0;
    for (int i = 0; i < Partie::getInstance().getNbJoueurs(); i++) {
        Joueur* j = Partie::getInstance().getJoueur(i);
        int scoreJoueur = j->getScore()->getTotal();
        if (scoreJoueur > maxPoints) {
            maxPoints = scoreJoueur;
            gagnant = i;
        }

    }
    cout << Partie::getInstance().getJoueur(gagnant)->getNom()<< " a remporte la partie avec " << maxPoints << " points !" << endl;

}

void JeuConsole::demanderConfiguration() {
    cout << "\n--- CONFIGURATION DE LA PARTIE ---" << endl;
    int nbJoueurs = saisieNombre("Combien de joueurs ?", 1, 4);
    nomsJoueurs.clear();
    for (int i = 0; i < nbJoueurs; ++i) {
        string nom;
        cout << "Nom du joueur " << i + 1 << " : ";
        cin >> nom;
        nomsJoueurs.push_back(nom);
    }
    int niveauIllustreConstructeur = 0;
    if(nbJoueurs == 1){
        cout << "Mode solo active" << endl;
        niveauIllustreConstructeur = saisieNombre("Niveau Illustre Constructeur", 1, 3);
    }

    bool modeTuileCite = saisieOuiNon("Mode tuile cite augmente ?");
    Partie::TuileCite mode = modeTuileCite ? Partie::TuileCite::AUGMENTE : Partie::TuileCite::STANDARD;

    if(saisieOuiNon("Activer les variantes ?")){
        cout << "Selection des variantes :\n" << endl;
        variantesActives[0] = saisieOuiNon("Variante habitations active ?");
        variantesActives[1] = saisieOuiNon("Variante marches active ?");
        variantesActives[2] = saisieOuiNon("Variante casernes active ?");
        variantesActives[3] = saisieOuiNon("Variante temples active ?");
        variantesActives[4] = saisieOuiNon("Variante jardins active ?");
	}


    Partie::getInstance().initialiser(nbJoueurs, nomsJoueurs, mode, variantesActives, niveauIllustreConstructeur);
}
