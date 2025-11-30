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
    const auto& pierres = Partie::getInstance().getPierresChantier();

    cout << "\n=== CHANTIER (Tuiles Disponibles) ===" << endl;
    for (size_t i = 0; i < chantier.size(); ++i) {
        cout << "Choix " << i << " (Cout: " << i << " pierres) : " << endl;
        if (pierres[i] > 0) cout << "  [Contient " << pierres[i] << " pierres bonus !]" << endl;
        
        // Affichage ASCII de la tuile
        Tuile* t = chantier[i];
        chantier[i].getDesign();
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
    
    afficherChantier();
}

void JeuConsole::jouerTour() {
    afficherEtatJeu();

    int maxChoix = Partie::getInstance().getChantier().size() - 1;
    
    cout << "\n--- ACTION ---" << endl;
    int index = saisieNombre("Quelle tuile choisir ?", 0, maxChoix);

    int rotation = 0;
    if (saisieOuiNon("Tourner la tuile ?")) {
        rotation = saisieNombre("Combien de rotations (1-3) ?", 1, 3);
    }

    int x = saisieNombre("Coord X", -10, 10);
    int y = saisieNombre("Coord Y", -10, 10);
    int z = saisieNombre("Coord Z (Niveau)", 0, 10);

    bool succes = Partie::getInstance().actionPlacerTuile(index, x, y, z, rotation);

    if (succes) {
        cout << ">> Succes ! Tuile placee." << endl;
        Partie::getInstance().passerAuJoueurSuivant();
    } else {
        cout << ">> ECHEC : Placement impossible ou pas assez de pierres." << endl;
        // On ne passe PAS au joueur suivant, on relance le tour
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
        Partie::getInstance().joueurActuel = i;
        cout << i + 1 << ". " << Partie::getInstance().getJoueurActuel()->getNom() << " : " << Partie::getInstance().getJoueurActuel()->getPoints() << " points" << endl;
    }
    cout << "\n--- GAGNANT ---" << endl;
    Partie::getInstance().joueurActuel = 0;
    int maxPoints = 0;
    int gagnant = 0;
    for (int i = 0; i < Partie::getInstance().getNbJoueurs(); i++) {
        if (Partie::getInstance().getJoueurActuel()->getPoints() > maxPoints) {
            maxPoints = Partie::getInstance().getJoueurActuel()->getPoints();
            gagnant = i;
        }
    }
    cout << Partie::getInstance().getJoueurActuel()->getNom() << " a remporte la partie avec " << Partie::getInstance().getJoueurActuel()->getPoints() << " points !" << endl;
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
        cout << "Mode solo activé" << endl;
        niveauIllustreConstructeur = saisieNombre("Niveau Illustre Constructeur", 1, 3);
    }

    bool modeTuileCite = saisieOuiNon("Mode tuile cite ?");
    Partie::TuileCite mode = modeTuileCite ? Partie::TuileCite::AUGMENTE : Partie::TuileCite::STANDARD;
    
    vector<bool> variantesActives(5, false);
    variantesActives[0] = saisieOuiNon("Variante habitations active ?");
    variantesActives[1] = saisieOuiNon("Variante marchés active ?");
    variantesActives[2] = saisieOuiNon("Variante casernes active ?");
    variantesActives[3] = saisieOuiNon("Variante temples active ?");
    variantesActives[4] = saisieOuiNon("Variante jardins active ?");
    
    Partie::getInstance().initialiser(nbJoueurs, nomsJoueurs, mode, variantesActives, niveauIllustreConstructeur);
}