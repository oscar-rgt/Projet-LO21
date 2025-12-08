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


    // Calcul : Total piles - Index pile actuelle
    size_t pilesRestantes = Partie::getInstance().getNbPiles() - Partie::getInstance().getIndexPileActuelle();
    cout << "\n=== CHANTIER === " << pilesRestantes << " pile(s) restante(s)" << endl;

    // --- 2. AFFICHAGE DES TUILES VIA ITÉRATEUR ---
    for (auto it = chantier.begin(); it != chantier.end(); ++it) {
        Tuile* t = *it; // Accès à la tuile via l'itérateur
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

    // --- 3. AFFICHAGE DES PRIX VIA ITÉRATEUR ---
    for (auto it = chantier.begin(); it != chantier.end(); ++it) {
        // (*it) donne le pointeur Tuile*, donc (*it)->getPrix() donne le prix
        int pierres = (*it)->getPrix();
        cout << "    ";
        cout << pierres << " pierres";
        cout << "               ";
    }
    cout << endl;
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
    }
    else {
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
        afficherChantier();
        int indexChoisi = Partie::getInstance().jouerTourIA();
        try {
            cout << "\n\nL'IA choisit la tuile " << indexChoisi << endl;
            cout << "Appuyez sur une touche pour continuer...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        catch (const PartieException& e) {
            cout << "L'IA a rencontre une erreur en choisissant une tuile : " << e.getInfo() << endl;
        }
        return;
    }

    //Tour humain

    // 1. Calculer le nombre de tuiles disponibles avec les itérateurs
    const auto& chantier = Partie::getInstance().getChantier();

    size_t maxChoix = chantier.getNbTuiles() - 1;

    cout << "\n--- ACTION ---" << endl;
    int index = saisieNombre("Quelle tuile choisir ?", 0, (int)maxChoix);

    // 2. Récupérer la tuile choisie (On avance l'itérateur jusqu'à l'index)
    auto itTuile = chantier.begin();
    for (int k = 0; k < index; ++k) {
        ++itTuile;
    }
    Tuile* tuileAffichee = *itTuile;

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
            for (int i = 0; i < (3 - rotationCompteur) % 3; ++i) {
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

    // Placement
    int x = saisieNombre("Coord X", -999, 999);
    int y = saisieNombre("Coord Y", -99, 999);
    int z = saisieNombre("Coord Z (Niveau)", 0, 10);


    cout << "\n\nLa tuile\n\n" << tuileAffichee->getDesign() << "\n\nva etre placee en (" << x << ", " << y << ", " << z << ").\n" << endl;

    if (saisieOuiNon("Valider ce choix ?")) {
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
        // Annulation finale
        if (tuileAffichee->getInversion() != 0) tuileAffichee->inverser(); // Remettre l'état initial
        for (int i = 0; i < (3 - rotationCompteur) % 3; ++i) {
            tuileAffichee->tourner(); // Remettre l'orientation initiale
        }
        jouerTour();
    }
}

void JeuConsole::lancer() {

    // --- ECRAN TITRE --- (identique à avant)
    cout << "\n\n\n\n\n\n\n";
    cout << "                __       __   ___   _______     ______    _______    ______    ___        __      ________  " << endl;
    cout << "               /\"\"\\     |/\"| /  \") /\"      \\   /    \" \\  |   __ \"\\  /    \" \\  |\"  |      |\" \\    /\"       ) " << endl;
    cout << "              /    \\    (: |/   / |:        | // ____  \\ (. |__) :)// ____  \\ ||  |      ||  |  (:   \\___/  " << endl;
    cout << "             /' /\\  \\   |    __/  |_____/   )/  /    ) :)|:  ____//  /    ) :)|:  |      |:  |   \\___  \\    " << endl;
    cout << "            //  __'  \\  (// _  \\   //      /(: (____/ // (|  /   (: (____/ //  \\  |___   |.  |    __/  \\\\   " << endl;
    cout << "           /   /  \\\\  \\ |: | \\  \\ |:  __   \\ \\        / /|__/ \\   \\        /  ( \\_|:  \\  /\\  |\\  /\" \\   :)  " << endl;
    cout << "          (___/    \\___)(__|  \\__)|__|  \\___) \\\"_____/ (_______)   \\\"_____/    \\_______)(__\\_|_)(_______/   " << endl;
    cout << "\n\n";




    cout << "                              ===========================================================" << endl;
    cout << "                                         B I E N V E N U E   D A N S   L A               " << endl;
    cout << "                                            C I T E   D E S   D I E U X                  " << endl;
    cout << "                              ===========================================================" << endl;
    cout << "\n\n";
    cout << "                                        >>> APPUYEZ SUR ENTREE POUR COMMENCER <<<        " << endl;
    cout << "\n\n\n\n\n";


    //Crédits
    cout << "-----------------------------------------------------------" << endl;
    cout << " LO21 - Programmation C++ | Semestre Automne 2025          " << endl;
    cout << " Oscar.R, Louane.R, Valentin.R, Noemie.M, Jeanne.D     " << endl;
    cout << "-----------------------------------------------------------" << endl;

    //nettoyage ecran et lancement jeu
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    nettoyerEcran();


    unsigned int choixMenu;
    do {
        nettoyerEcran();
        std::cout << "1. JOUER UNE PARTIE                     " << std::endl;
        std::cout << "2. REGLES DU JEU                        " << std::endl;
        std::cout << "3. QUITTER                              " << std::endl;
        std::cout << "\n\n";
        choixMenu = saisieNombre("Choisissez une option", 1, 3);

        switch (choixMenu)
        {
        case 1:
            break;
        case 2:
            afficherRegles();
            break;
        case 3:
            exit(0);
        default:
            break;
        }
    } while (choixMenu != 1);

    nettoyerEcran();
    demanderConfiguration();

    while (!Partie::getInstance().estFinDePartie()) {
        jouerTour();
    }

	nettoyerEcran();

    cout << "=== FIN DE PARTIE ===" << endl;

    cout << "\n--- SCORES ---" << endl;
	int score = 0;

    // Utilisation de debutJoueurs() et finJoueurs() au lieu de l'index
    for (auto it = Partie::getInstance().debutJoueurs(); it != Partie::getInstance().finJoueurs(); ++it) {
        Joueur* j = *it;

        // Calcul du score IA si nécessaire
        IA* ia = dynamic_cast<IA*>(j);
        if (ia) {
            ia->calculerScoreIA(); // Mise à jour score interne
        }
        else {
            j->getScore()->calculerScore();
            score = j->getScore()->getTotal();
        }

        cout << j->getNom() << " : " << score << " points (" << j->getPierres() << " pierres)" << endl;
    }

    cout << "\n--- RESULTAT ---" << endl;

    // determinerGagnants renvoie maintenant un vector<Joueur*> directement
    vector<Joueur*> gagnants = Partie::getInstance().determinerGagnants();

    if (gagnants.size() == 1) {
        cout << ">>> LE VAINQUEUR EST : " << gagnants[0]->getNom() << " !!! <<<" << endl;
    }
    else if (gagnants.size() > 1) {
        cout << ">>> EGALITE PARFAITE ENTRE : ";
        for (size_t k = 0; k < gagnants.size(); ++k) {
            cout << gagnants[k]->getNom();
            if (k < gagnants.size() - 1) cout << " ET ";
        }
        cout << " ! <<<" << endl;
    }


    cout << "\n\n\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
	exit(0);
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
    int niveauIA = 0;
    if (nbJoueurs == 1) {
        cout << "Mode solo active" << endl;
        niveauIA = saisieNombre("Niveau Illustre Constructeur", 1, 3);
    }

    bool modeTuileCite = saisieOuiNon("Mode tuile cite augmente ?");
    Partie::TuileCite mode = modeTuileCite ? Partie::TuileCite::AUGMENTE : Partie::TuileCite::STANDARD;

    if (saisieOuiNon("Activer les variantes ?")) {
        cout << "Selection des variantes :\n" << endl;
        variantesActives[0] = saisieOuiNon("Variante habitations active ?");
        variantesActives[1] = saisieOuiNon("Variante marches active ?");
        variantesActives[2] = saisieOuiNon("Variante casernes active ?");
        variantesActives[3] = saisieOuiNon("Variante temples active ?");
        variantesActives[4] = saisieOuiNon("Variante jardins active ?");
    }


    Partie::getInstance().initialiser(nbJoueurs, nomsJoueurs, mode, variantesActives, niveauIA);
}

void JeuConsole::afficherRegles() {
    nettoyerEcran();
    cout << "===========================================================" << endl;
    cout << "                  REGLES D'AKROPOLIS                       " << endl;
    cout << "===========================================================" << endl;
    cout << "1. LE BUT :\n Construisez la cite la plus prestigieuse." << endl;
    cout << "   Les points sont marques grace aux Quartiers" << endl;
    cout << "   multiplies par les Places correspondantes." << endl;
    cout << "\n";
    cout << "2. LES QUARTIERS :" << endl;
    cout << "   - Habitation (H) : 1 point pour chaque quartier habitation adjacent a un autre." << endl;
    cout << "   (Ne compte que pour votre plus grand groupe d'habitations)" << endl;
    cout << "   - Marche (M)    : 1 point si votre marche n'est adjacent a aucun autre." << endl;
    cout << "   - Caserne (C)   : 1 point si votre caserne n'est pas completement entouree par d'autres hexagones." << endl;
    cout << "   - Temple (T)   : 1 point si votre temple est completement entoure par d'autres hexagones." << endl;
    cout << "   - Jardin (J)     : 1 point pour chaque jardin pose sans condition." << endl;
	cout << "   - Carriere (X)  : Permet d'agrandir votre cite mais ne donne pas de points." << endl;
    cout << "\n";
    cout << "3. LES PLACES :" << endl;
	cout << "   Les places de chaque type vous permettent de multiplier vos points" << endl;
	cout << "   en fonction du chiffre qui est ecrit dessus." << endl;
    cout << "   Par exemple, un hexagone 2H est une place Habitation a 2 etoiles." << endl;
    cout << "   Elle multiplie donc par 2 les points gagnes par vos quartiers Habitation." << endl;
    cout << "   /!\\ ATTENTION : Si vous n'avez aucune place d'un certain type," << endl;
	cout << "       vous ne marquez aucun point pour ses quartiers correspondants." << endl;
    cout << "\n";
    cout << "4. LA PIERRE :" << endl;
    cout << "   Vous commencez avec un nombre de 2 pierres." << endl;
    cout << "   Ces dernieres vous permettront d'acheter des tuiles." << endl;
	cout << "   Les pierres influent aussi sur votre score." << endl;
    cout << "   En effet, chaque pierre vous rapporte un point." << endl;
    cout << "   De plus, en cas d'egalite en fin de partie," << endl;
	cout << "   le joueur avec le plus de pierres l'emporte." << endl;
    cout << "   Les pierres s'obtiennent en construisant au dessus d'une carriere." << endl;
    cout << "   Chaque carriere recouverte donne une pierre." << endl;
    cout << "\n";
	cout << "5. PLACEMENT :" << endl;
	cout << "   Vous l'aurez compris, votre cite peut s'etendre aussi bien en surface qu'en hauteur." << endl;
    cout << "   Lorsqu'un hexagone est place en hauteur, son nombre de points est multiplie par son niveau d'elevation." << endl;
	cout << "   Par exemple, un quartier Jardin place au niveau 3 rapporte 3 points." << endl;
    cout << "   A vous de trouver le bon equilibre pour devenir le plus prestigieux des architectes !" << endl;
    cout << "===========================================================" << endl;
    cout << "Appuyez sur Entree pour revenir au menu.";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}