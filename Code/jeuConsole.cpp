#include "jeuConsole.h"
#include "joueur.h"
#include "ia.h"
#include "tuiles.h"
#include "cite.h"
#include "partie.h"
#include "save.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <cstdlib>

using namespace std;

// ==========================================
// OUTILS ET AFFICHAGE (Parties conservées)
// ==========================================

void JeuConsole::nettoyerEcran() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

int JeuConsole::saisieNombre(const string& prompt, int min, int max) {
    int reponse = -1;
    cout << prompt << " ( de " << min << " a " << max << ") : ";
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
    
    // Petite sécurité : si le chantier est vide, on n'affiche rien pour éviter les bugs
    if (chantier.getNbTuiles() == 0) return;

    vector<string> lignes(9, "");
    const int hauteur = 9;
    const int largeur_reelle = 20;
    const int largeur_visuelle = 19;
    
    cout << "\n=== CHANTIER === " << Partie::getInstance().getNbPiles() - (Partie::getInstance().getIndexPileActuelle()) << " pile(s) restante(s)" << endl;
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
        // On affiche quand même le chantier pour voir ce qu'il se passe
        afficherChantier(); 
    }
    else {
        j->getCite()->afficher();
        afficherChantier();
    }
}

// ==========================================
// LOGIQUE DU TOUR
// ==========================================

void JeuConsole::jouerTour() {
    nettoyerEcran();
    afficherEtatJeu();

    Joueur* j = Partie::getInstance().getJoueurActuel();

    // --- TOUR DE L'IA ---
    if (dynamic_cast<IA*>(j)) {
        cout << "\n\n--- TOUR DE L'IA ---" << endl;
        cout << "\nL'Illustre Constructeur reflechit..." << endl;
        
        int indexChoisi = Partie::getInstance().jouerTourIA();
        try {
            cout << "\n\nL'IA choisit la tuile " << indexChoisi << endl;
            cout << "Appuyez sur une touche pour continuer...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
        catch (const PartieException& e) {
            cout << "Erreur IA : " << e.getInfo() << endl;
        }
        return;
    }

    // --- TOUR HUMAIN ---
    size_t maxChoix = Partie::getInstance().getChantier().getNbTuiles() - 1;

    cout << "\n--- ACTION ---" << endl;
    int index = saisieNombre("Quelle tuile choisir ? (-1 pour SAUVEGARDER et QUITTER)", -1, (int)maxChoix);
    
    if (index == -1) {
        cout << "\n>>> SAUVEGARDE EN COURS... <<<" << endl;
        
        if (SaveManager::sauvegarder(Partie::getInstance(), "save.txt")) {
            cout << ">> SUCCES : Partie sauvegardee dans 'akropolis_save.txt'" << endl;
        } else {
            cout << ">> ERREUR : Impossible de sauvegarder la partie." << endl;
        }
        
        cout << "Au revoir !" << endl;
        exit(0);
    }

    Tuile* tuileAffichee = Partie::getInstance().getChantier().getTuile(index);

    // --- MODE PREVISUALISATION ---
    bool placementValide = false;
    int rotationCompteur = 0;
    bool inversionEtat = false;
    bool invInitial = tuileAffichee->getInversion(); // Pour pouvoir annuler proprement

    while (!placementValide) {
        nettoyerEcran();
        afficherEtatJeu();

        cout << "\nTuile selectionnee :" << endl;
        cout << tuileAffichee->getDesign() << endl;

        cout << "\nCommandes : [R]otation | [I]nversion | [V]alider | [A]nnuler choix" << endl;
        cout << "Votre choix : ";

        string choix;
        cin >> choix;

        if (choix == "R" || choix == "r") {
            tuileAffichee->tourner();
            rotationCompteur = (rotationCompteur + 1) % 3;
        }
        else if (choix == "I" || choix == "i") {
            tuileAffichee->inverser();
            inversionEtat = !inversionEtat;
        }
        else if (choix == "A" || choix == "a") {
            // Annuler : on remet tout comme avant
            if (tuileAffichee->getInversion() != invInitial) tuileAffichee->inverser();
            for (int i = 0; i < (3 - (rotationCompteur % 3)) % 3; ++i) tuileAffichee->tourner();
            
            jouerTour(); // On recommence le choix
            return;
        }
        else if (choix == "V" || choix == "v") {
            placementValide = true;
        }
    }

    // --- PLACEMENT ---
    int x = saisieNombre("Coord X", -20, 20);
    int y = saisieNombre("Coord Y", -20, 20);
    int z = saisieNombre("Coord Z (Niveau)", 0, 10);

    // On passe 0 et false car la tuile a déjà été tournée visuellement sur le pointeur du chantier
    if (saisieOuiNon("Valider ce choix ?")) {
        try {
            bool succes = Partie::getInstance().actionPlacerTuile(index, x, y, z, 0, false);

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
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            jouerTour();
        }
    }
    else {
        // Annulation finale : on remet l'état initial
        if (tuileAffichee->getInversion() != invInitial) tuileAffichee->inverser();
        for (int i = 0; i < (3 - (rotationCompteur % 3)) % 3; ++i) tuileAffichee->tourner();
        jouerTour();
    }
}

// ==========================================
// MENU PRINCIPAL ET CONFIGURATION
// ==========================================

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
    bool partieChargeeSucces = false;

    // --- BOUCLE MENU ---
    do {
        nettoyerEcran();
        cout << "1. JOUER UNE PARTIE" << endl;
        cout << "2. CHARGER UNE PARTIE" << endl;
        cout << "3. REGLES DU JEU" << endl;
        cout << "4. QUITTER" << endl;
        cout << "\n";
        
        choixMenu = saisieNombre("Choisissez une option", 1, 4);

        switch (choixMenu)
        {
        case 1:
            // Nouvelle partie : on sort juste de la boucle
            // partieChargeeSucces reste à false
            break;

        case 2: {
            cout << "\nChargement..." << endl;
            if (SaveManager::charger(Partie::getInstance(), "save.txt")) {
                cout << ">> Succes !" << endl;
                partieChargeeSucces = true; // On retient qu'on a chargé
                choixMenu = 1; // On force la sortie du menu
                
                cout << "Appuyez sur Entree pour reprendre...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            } else {
                cerr << ">> Echec du chargement." << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            }
            break;
        }

        case 3:
            afficherRegles();
            break;

        case 4:
            exit(0);

        default:
            break;
        }
    } while (choixMenu != 1);

    // --- GESTION DE LA CONFIGURATION ---
    nettoyerEcran();

    // C'EST ICI LA CORRECTION PRINCIPALE :
    if (partieChargeeSucces) {
        // Si on a chargé une partie, ON NE FAIT RIEN DE PLUS.
        cout << ">> Reprise de la partie sauvegardee..." << endl;
        // On laisse une pause
        cout << "Appuyez sur Entree...";
    } 
    else {
        // Si c'est une nouvelle partie (pas chargée) : ON CONFIGURE
        demanderConfiguration();
    }

    // --- BOUCLE DE JEU ---
    while (!Partie::getInstance().estFinDePartie()) {
        jouerTour();
    }
    SaveManager::supprimerSauvegarde("save.txt");;
    // --- FIN DE PARTIE ---
    nettoyerEcran();
    cout << "=== FIN DE PARTIE ===" << endl;

    cout << "\n--- CLASSEMENT ---" << endl;
    for (int i = 0; i < Partie::getInstance().getNbJoueurs(); i++) {
        Joueur* j = Partie::getInstance().getJoueur(i);

        IA* ia = dynamic_cast<IA*>(j);
        if (ia) ia->calculerScoreIA(); // Mise à jour score IA
        else j->getScore()->calculerScore(); // Mise à jour score Humain

        cout << i + 1 << ". " << j->getNom() << " : " << j->getScore()->getTotal() << " points" << endl;
    }

    cout << "\n\nAppuyez sur Entree pour quitter.";
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
        
        // IMPORTANT : On remplace les espaces par des '_' pour éviter les bugs de sauvegarde
        std::replace(nom.begin(), nom.end(), ' ', '_');
        
        nomsJoueurs.push_back(nom);
    }
    
    int niveauIllustreConstructeur = 0;
    if (nbJoueurs == 1) {
        cout << "Mode solo active" << endl;
        niveauIllustreConstructeur = saisieNombre("Niveau Illustre Constructeur", 1, 3);
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
    } else {
        variantesActives.fill(false);
    }

    Partie::getInstance().initialiser(nbJoueurs, nomsJoueurs, mode, variantesActives, niveauIllustreConstructeur);
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
