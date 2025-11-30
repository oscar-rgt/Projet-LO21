#include "jeu_console.h"
#include "joueur.h"  
#include "tuiles.h" 
#include "placement.h" 
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

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
    cout << "Score : " << j->getPoints() << endl;
    cout << "##########################################" << endl;

    cout << "\n--- VOTRE CITE ---" << endl;
    j->getCite()->afficher();
    j->getCite()->afficherMap();
    afficherChantier();
}

void JeuConsole::jouerTour() {
    afficherEtatJeu();

    int maxChoix = Partie::getInstance().getChantier().getNbTuiles() - 1;
    
    cout << "\n--- ACTION ---" << endl;
    int index = saisieNombre("Quelle tuile choisir ?", 0, maxChoix);

    int rotation = 0;
    if (saisieOuiNon("Tourner la tuile ?")) {
        rotation = saisieNombre("Combien de rotations ?", 1, 3);
    }

    int x = saisieNombre("Coord X", -10, 10);
    int y = saisieNombre("Coord Y", -10, 10);
    int z = saisieNombre("Coord Z (Niveau)", 0, 10);

    try {
        Partie::getInstance().actionPlacerTuile(index, x, y, z, rotation);
    } catch (CiteException &e) {
        cout << " ECHEC : " << e.getInfos() << endl;
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
        cout << i + 1 << ". " << j->getNom() << " : " << j->getPoints() << " points" << endl;
    }
    cout << "\n--- GAGNANT ---" << endl;
    int maxPoints = 0;
    int gagnant = 0;
    for (int i = 0; i < Partie::getInstance().getNbJoueurs(); i++) {
        Joueur* j = Partie::getInstance().getJoueur(i);
        if (j->getPoints() > maxPoints) {
            maxPoints = j->getPoints();
            gagnant = i;
        }
    }
    cout << Partie::getInstance().getJoueur(gagnant)->getNom() << " a remporte la partie avec " << maxPoints << " points !" << endl;
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

    bool modeTuileCite = saisieOuiNon("Mode tuile cite standard ?");
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