#include "jeu_console.h"
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

// --- Utilitaires de Saisie ---

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

// --- Logique du Jeu Console ---

void JeuConsole::demanderConfiguration() {
    cout << "\n--- CONFIGURATION DE LA PARTIE ---" << endl;

    // 1. Nombre de joueurs
    int nbJoueurs = saisieNombre("Combien de joueurs ?", 1, 4);

    // 2. Noms
    nomsJoueurs.clear();
    for (int i = 0; i < nbJoueurs; ++i) {
        string nom;
        cout << "Nom du joueur " << i + 1 << " : ";
        cin >> nom;
        nomsJoueurs.push_back(nom);
    }

    // 3. Difficulte A (Si solo)
    unsigned int niveauIC = 0;
    if (nbJoueurs == 1) {
        niveauIC = saisieNombre("Niveau IA (1-3)", 1, 3);
    }

    // 4. Mode de jeu (Tuiles)
    if (saisieOuiNon("Mode 'Augmente' (o) ou 'Standard' (n) ?")) {
        modeTuileCite = Partie::TuileCite::AUGMENTE;
    }
    else {
        modeTuileCite = Partie::TuileCite::STANDARD;
    }

    // 5. Variantes
    if (saisieOuiNon("Activer des variantes ?")) {
        for (int i = 0; i < 5; ++i) {
            variantesActives[i] = saisieOuiNon("Activer variante " + to_string(i + 1) + " ?");
        }
    }

    // ENVOI AU MOTEUR
    Partie::getInstance().initialiser(nbJoueurs, nomsJoueurs, modeTuileCite, variantesActives, niveauIC);
}

void JeuConsole::afficherEtatJeu() {
    Partie& partie = Partie::getInstance();
    Joueur* joueur = partie.getJoueurActuel();

    if (!joueur) return;

    cout << "\n------------------------------------------" << endl;
    cout << "TOUR DE : " << joueur->getNom() << endl;
    cout << "------------------------------------------" << endl;


    joueur->getCite()->afficher();
}

void JeuConsole::jouerTour() {
    afficherEtatJeu();


    cout << "1. Choisir une tuile" << endl;
    int indexTuile = saisieNombre("Quel index de tuile ?", 1, 3);

    int rotation = 0;
    if (saisieOuiNon("Tourner la tuile ?")) {
        rotation = saisieNombre("Combien de rotations (1-3) ?", 1, 3);
    }

    cout << "2. Placer la tuile" << endl;
    int x = saisieNombre("X", -10, 10);
    int y = saisieNombre("Y", -10, 10);
    int z = saisieNombre("Z (Etage)", 0, 5);


    // APPEL AU MOTEUR
    bool succes = Partie::getInstance().actionPlacerTuile(indexTuile, x, y, z, rotation);

    if (succes) {
        cout << ">> Succes ! Tuile posee." << endl;
        Partie::getInstance().passerAuJoueurSuivant();
    }
    else {
        cout << ">> Erreur ! Placement invalide." << endl;
        jouerTour();
    }
    afficherScore();
}

void JeuConsole::afficherScore() {
    Partie& partie = Partie::getInstance();
    Joueur* joueur = partie.getJoueurActuel();

    if (!joueur) return;

    cout << "\n------------------------------------------" << endl;
    cout << "SCORE : " << joueur->getScore() << endl;
    cout << "------------------------------------------" << endl;

}

void JeuConsole::lancer() {
    cout << "==========================================" << endl;
    cout << "          AKROPOLIS (MODE CONSOLE)        " << endl;
    cout << "==========================================" << endl;

    // etape 1 : Configuration et Initialisation du Moteur
    demanderConfiguration();

    // etape 2 : Boucle de jeu principale
    while (!Partie::getInstance().estFinDePartie()) {
        jouerTour();
    }

    cout << "\n--- FIN DE PARTIE ---" << endl;
}